//
// Created by bong on 22. 12. 15.
//

#include "SignalImpl.h"
#include "event2/event.h"


namespace Event {

    SignalEventImpl::SignalEventImpl(DispatcherImpl& dispatcher, signal_t signal_num, SignalCb cb)
            : cb_(cb) {
        evsignal_assign(
                &raw_event_, &dispatcher.base(), signal_num,
                [](evutil_socket_t, short, void* arg) -> void { static_cast<SignalEventImpl*>(arg)->cb_(); },
                this);
        evsignal_add(&raw_event_, nullptr);
    }

} // namespace Event