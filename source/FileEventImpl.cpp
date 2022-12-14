//
// Created by bong on 22. 12. 14.
//

#include <cassert>
#include "FileEventImpl.h"
#include <iostream>


FileEventImpl::FileEventImpl(DispatcherImpl& dispatcher, os_fd_t fd, FileReadyCb cb, FileTriggerType trigger, uint32_t events)
       : dispatcher_(dispatcher), cb_(cb), fd_(fd),
       trigger_(trigger), enabled_events_(events),
       activation_cb_(dispatcher.createSchedulableCallback([this]()  {
            mergeInjectedEventsAndRunCb(0);
        }))
    {
    if constexpr (PlatformDefaultTriggerType != FileTriggerType::EmulatedEdge) {
        assert(trigger_ != FileTriggerType::EmulatedEdge);
        std::cout << "Cannot use EmulatedEdge events if they are not the default platform type" << std::endl;
    }

    assignEvents(events, &dispatcher.base());
    event_add(&raw_event_, nullptr);
}

void FileEventImpl::activate(uint32_t events) {
    //assert(dispatcher_.isThreadSafe());

    // events is not empty.
    assert(events != 0);
    // Only supported event types are set.
    assert((events & (FileReadyType::Read | FileReadyType::Write | FileReadyType::Closed)) == events);

    // Schedule the activation callback so it runs as part of the next loop iteration if it is not
    // already scheduled.
    if (injected_activation_events_ == 0) {
        assert(!activation_cb_->enabled());
        activation_cb_->scheduleCallbackNextIteration();
    }
    assert(activation_cb_->enabled());

    // Merge new events with pending injected events.
    injected_activation_events_ |= events;
}

void FileEventImpl::setEnabled(uint32_t events) {
    //assert(dispatcher_.isThreadSafe());
    if (injected_activation_events_ != 0) {
        // Clear pending events on updates to the fd event mask to avoid delivering events that are no
        // longer relevant. Updating the event mask will reset the fd edge trigger state so the proxy
        // will be able to determine the fd read/write state without need for the injected activation
        // events.
        injected_activation_events_ = 0;
        activation_cb_->cancel();
    }
    updateEvents(events);
}

void FileEventImpl::unregisterEventIfEmulatedEdge(uint32_t event) {
    //assert(dispatcher_.isThreadSafe());
    // This constexpr if allows the compiler to optimize away the function on POSIX
    if constexpr (PlatformDefaultTriggerType == FileTriggerType::EmulatedEdge) {
        if (trigger_ == FileTriggerType::EmulatedEdge) {
            auto new_event_mask = enabled_events_ & ~event;
            updateEvents(new_event_mask);
        }
    }
}

void FileEventImpl::registerEventIfEmulatedEdge(uint32_t event) {
    //assert(dispatcher_.isThreadSafe());
    // This constexpr if allows the compiler to optimize away the function on POSIX
    if constexpr (PlatformDefaultTriggerType == FileTriggerType::EmulatedEdge) {
        assert((event & (FileReadyType::Read | FileReadyType::Write)) == event);
        if (trigger_ == FileTriggerType::EmulatedEdge) {
            auto new_event_mask = enabled_events_ | event;
            updateEvents(new_event_mask);
        }
    }
}

void FileEventImpl::assignEvents(uint32_t events, event_base *base) {
    //assert(dispatcher_.isThreadSafe());
    assert(base != nullptr);

    enabled_events_ = events;
    event_assign(
            &raw_event_, base, fd_,
            EV_PERSIST | (trigger_ == FileTriggerType::Edge ? EV_ET : 0) |
            (events & FileReadyType::Read ? EV_READ : 0) |
            (events & FileReadyType::Write ? EV_WRITE : 0) |
            (events & FileReadyType::Closed ? EV_CLOSED : 0),
            [](evutil_socket_t, short what, void* arg) -> void {
                auto* event = static_cast<FileEventImpl*>(arg);
                uint32_t events = 0;
                if (what & EV_READ) {
                    events |= FileReadyType::Read;
                }

                if (what & EV_WRITE) {
                    events |= FileReadyType::Write;
                }

                if (what & EV_CLOSED) {
                    events |= FileReadyType::Closed;
                }

                assert(events != 0);
                event->mergeInjectedEventsAndRunCb(events);
            },
            this);
}

void FileEventImpl::mergeInjectedEventsAndRunCb(uint32_t events) {
    //assert(dispatcher_.isThreadSafe());
    if (injected_activation_events_ != 0) {
        events |= injected_activation_events_;
        injected_activation_events_ = 0;
        activation_cb_->cancel();
    }

    // TODO(davinci26): This can be optimized further in (w)epoll backends using the `EPOLLONESHOT`
    // flag. With this flag `EPOLLIN`/`EPOLLOUT` are automatically disabled when the event is
    // activated.
    if constexpr (PlatformDefaultTriggerType == FileTriggerType::EmulatedEdge) {
        if (trigger_ == FileTriggerType::EmulatedEdge) {
            unregisterEventIfEmulatedEdge(events &
                                          (FileReadyType::Write | FileReadyType::Read));
        }
    }

    cb_(events);
}

void FileEventImpl::updateEvents(uint32_t events) {
    //assert(dispatcher_.isThreadSafe());
    // The update can be skipped in cases where the old and new event mask are the same if the fd is
    // using Level or EmulatedEdge trigger modes, but not Edge trigger mode. When the fd is registered
    // in edge trigger mode, re-registering the fd will force re-computation of the readable/writable
    // state even in cases where the event mask is not changing. See
    // https://github.com/envoyproxy/envoy/pull/16389 for more details.
    // TODO(antoniovicente) Consider ways to optimize away event registration updates in edge trigger
    // mode once setEnabled stops clearing injected_activation_events_ before calling updateEvents
    // and/or implement optimizations at the Network::ConnectionImpl level to reduce the number of
    // calls to setEnabled.
    if (events == enabled_events_ && trigger_ != FileTriggerType::Edge) {
        return;
    }
    auto* base = event_get_base(&raw_event_);
    event_del(&raw_event_);
    assignEvents(events, base);
    event_add(&raw_event_, nullptr);
}
