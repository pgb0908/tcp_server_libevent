//
// Created by bong on 22. 12. 14.
//

#include "SignalImpl.h"

SignalImpl::SignalImpl(DispatcherImpl &dispatcher, signal_t signal_num, SignalCb cb)
        : cb_(cb)
        {
            evsignal_assign(
                    &raw_event_, &dispatcher.base(), signal_num,
                    [](evutil_socket_t, short, void* arg) -> void { static_cast<SignalImpl*>(arg)->cb_(); },
                    this);
            evsignal_add(&raw_event_, nullptr);
}
