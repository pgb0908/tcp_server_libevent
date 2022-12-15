//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_FILEEVENTIMPL_H
#define EVENTLOOP_BASE_SERVER_FILEEVENTIMPL_H

#include <cstdint>

#include "include/event/FileEvent.h"

#include "src/common/event/DispatcherImpl.h"
#include "src/common/event/EventImplBase.h"

namespace Event {

/**
 * Implementation of FileEvent for libevent that uses persistent events and
 * assumes the user will read/write until EAGAIN is returned from the file.
 */
    class FileEventImpl : public FileEvent, ImplBase {
    public:
        FileEventImpl(DispatcherImpl& dispatcher, os_fd_t fd, FileReadyCb cb, FileTriggerType trigger,
                      uint32_t events);

        // Event::FileEvent
        void activate(uint32_t events) override;
        void setEnabled(uint32_t events) override;
        void unregisterEventIfEmulatedEdge(uint32_t event) override;
        void registerEventIfEmulatedEdge(uint32_t event) override;

    private:
        void assignEvents(uint32_t events, event_base* base);
        void mergeInjectedEventsAndRunCb(uint32_t events);
        void updateEvents(uint32_t events);

        Dispatcher& dispatcher_;
        FileReadyCb cb_;
        os_fd_t fd_;
        FileTriggerType trigger_;
        // Enabled events for this fd.
        uint32_t enabled_events_;

        // Injected FileReadyType events that were scheduled by recent calls to activate() and are pending
        // delivery.
        uint32_t injected_activation_events_{};
        // Used to schedule delayed event activation. Armed iff pending_activation_events_ != 0.
        SchedulableCallbackPtr activation_cb_;
    };
} // namespace Event

#endif //EVENTLOOP_BASE_SERVER_FILEEVENTIMPL_H
