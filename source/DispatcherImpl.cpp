//
// Created by bong on 22. 12. 13.
//

#include <cassert>
#include <list>
#include "DispatcherImpl.h"
#include "FileEventImpl.h"
#include "ThreadImpl.h"
#include "SignalImpl.h"

DispatcherImpl::DispatcherImpl( std::string name) :
    name_(name),
    thread_factory_(std::make_unique<ThreadFactoryImplPosix>()) {

}

MonotonicTime DispatcherImpl::approximateMonotonicTime() const {
    return MonotonicTime();
}

void DispatcherImpl::updateApproximateMonotonicTime() {

}

FileEventPtr DispatcherImpl::createFileEvent(os_fd_t fd, FileReadyCb cb, FileTriggerType trigger, uint32_t events) {
    return FileEventPtr{new FileEventImpl(
            *this, fd,
            [this, cb](uint32_t events) {
                //touchWatchdog();
                cb(events);
            },
            trigger, events)};
}

TimerPtr DispatcherImpl::createTimer(TimerCb cb) {
    assert(isThreadSafe());
    return createTimerInternal(cb);
}

TimerPtr DispatcherImpl::createScaledTimer(ScaledTimerType timer_type, TimerCb cb) {
    assert(isThreadSafe());
    return scaled_timer_manager_->createTimer(timer_type, std::move(cb));
}

TimerPtr DispatcherImpl::createScaledTimer(ScaledTimerMinimum minimum, TimerCb cb) {
    assert(isThreadSafe());
    return scaled_timer_manager_->createTimer(minimum, std::move(cb));
}

SchedulableCallbackPtr DispatcherImpl::createSchedulableCallback(std::function<void()> cb) {
    assert(isThreadSafe());
    return base_scheduler_.createSchedulableCallback([this, cb]() {
        //touchWatchdog();
        cb();
    });
}

void DispatcherImpl::deferredDelete(DeferredDeletablePtr &&to_delete) {
    assert(isThreadSafe());
    if (to_delete != nullptr) {
        to_delete->deleteIsPending();
        current_to_delete_->emplace_back(std::move(to_delete));
        //ENVOY_LOG(trace, "item added to deferred deletion list (size={})", current_to_delete_->size());
        if (current_to_delete_->size() == 1) {
            deferred_delete_cb_->scheduleCallbackCurrentIteration();
        }
    }
}

void DispatcherImpl::exit() {
    base_scheduler_.loopExit();
}

SignalEventPtr DispatcherImpl::listenForSignal(signal_t signal_num, SignalCb cb) {
    assert(isThreadSafe());
    return SignalEventPtr{new SignalImpl(*this, signal_num, cb)};
}

void DispatcherImpl::post(std::function<void()> callback) {
    bool do_post = true;
    {
/*        Thread::LockGuard lock(post_lock_);
        do_post = post_callbacks_.empty();
        post_callbacks_.push_back(callback);*/
    }

    if (do_post) {
        post_cb_->scheduleCallbackCurrentIteration();
    }
}

void DispatcherImpl::deleteInDispatcherThread(DispatcherThreadDeletableConstPtr deletable) {
    bool need_schedule = true;
    {
/*        Thread::LockGuard lock(thread_local_deletable_lock_);
        need_schedule = deletables_in_dispatcher_thread_.empty();
        deletables_in_dispatcher_thread_.emplace_back(std::move(deletable));*/
        // TODO(lambdai): Enable below after https://github.com/envoyproxy/envoy/issues/15072
        // ASSERT(!shutdown_called_, "inserted after shutdown");
    }

    if (need_schedule) {
        thread_local_delete_cb_->scheduleCallbackCurrentIteration();
    }
}

void DispatcherImpl::run(Dispatcher::RunType type) {
    run_tid_ = thread_factory_->currentThreadId();
    // Flush all post callbacks before we run the event loop. We do this because there are post
    // callbacks that have to get run before the initial event loop starts running. libevent does
    // not guarantee that events are run in any particular order. So even if we post() and call
    // event_base_once() before some other event, the other event might get called first.
    runPostCallbacks();
    base_scheduler_.run(type);
}

void DispatcherImpl::shutdown() {
    // TODO(lambdai): Resolve https://github.com/envoyproxy/envoy/issues/15072 and loop delete below
    // below 3 lists until all lists are empty. The 3 lists are list of deferred delete objects, post
    // callbacks and dispatcher thread deletable objects.
    assert(isThreadSafe());
    auto deferred_deletables_size = current_to_delete_->size();
    std::list<std::function<void()>>::size_type post_callbacks_size;
    {
/*        Thread::LockGuard lock(post_lock_);
        post_callbacks_size = post_callbacks_.size();*/
    }

    std::list<DispatcherThreadDeletableConstPtr> local_deletables;
    {
/*        Thread::LockGuard lock(thread_local_deletable_lock_);
        local_deletables = std::move(deletables_in_dispatcher_thread_);*/
    }
    auto thread_local_deletables_size = local_deletables.size();
    while (!local_deletables.empty()) {
        local_deletables.pop_front();
    }
    assert(!shutdown_called_);
    shutdown_called_ = true;
/*    ENVOY_LOG(
            trace,
            "{} destroyed {} thread local objects. Peek {} deferred deletables, {} post callbacks. ",
            __FUNCTION__, deferred_deletables_size, post_callbacks_size, thread_local_deletables_size);*/
}

void DispatcherImpl::updateApproximateMonotonicTimeInternal() {
    //approximate_monotonic_time_ = time_source_.monotonicTime();
}

void DispatcherImpl::runPostCallbacks() {
    // Clear the deferred delete list before running post callbacks to reduce non-determinism in
    // callback processing, and more easily detect if a scheduled post callback refers to one of the
    // objects that is being deferred deleted.
    //clearDeferredDeleteList();

    std::list<std::function<void()>> callbacks;
    {
        // Take ownership of the callbacks under the post_lock_. The lock must be released before
        // callbacks execute. Callbacks added after this transfer will re-arm post_cb_ and will execute
        // later in the event loop.
/*        Thread::LockGuard lock(post_lock_);
        callbacks = std::move(post_callbacks_);
        // post_callbacks_ should be empty after the move.
        assert(post_callbacks_.empty());*/
    }
    // It is important that the execution and deletion of the callback happen while post_lock_ is not
    // held. Either the invocation or destructor of the callback can call post() on this dispatcher.
    while (!callbacks.empty()) {
        // Touch the watchdog before executing the callback to avoid spurious watchdog miss events when
        // executing a long list of callbacks.
        //touchWatchdog();
        // Run the callback.
        callbacks.front()();
        // Pop the front so that the destructor of the callback that just executed runs before the next
        // callback executes.
        callbacks.pop_front();
    }
}

void DispatcherImpl::runThreadLocalDelete() {
    std::list<DispatcherThreadDeletableConstPtr> to_be_delete;
    {
/*        Thread::LockGuard lock(thread_local_deletable_lock_);
        to_be_delete = std::move(deletables_in_dispatcher_thread_);
        assert(deletables_in_dispatcher_thread_.empty());*/
    }
    while (!to_be_delete.empty()) {
        // Touch the watchdog before deleting the objects to avoid spurious watchdog miss events when
        // executing complicated destruction.
        //touchWatchdog();
        // Delete in FIFO order.
        to_be_delete.pop_front();
    }
}

TimerPtr DispatcherImpl::createTimerInternal(TimerCb cb) {
    return scheduler_->createTimer(
            [this, cb]() {
                //touchWatchdog();
                cb();
            },
            *this);
}
