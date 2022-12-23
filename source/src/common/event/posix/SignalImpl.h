//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_SIGNALIMPL_H
#define EVENTLOOP_BASE_SERVER_SIGNALIMPL_H

#include "src/common/event/EventImplBase.h"

#include "include/event/Signal.h"
#include "src/common/event/DispatcherImpl.h"

namespace Event {

/**
 * libevent implementation of Event::SignalEvent.
 */
    class SignalEventImpl : public SignalEvent, ImplBase {
    public:
        SignalEventImpl(DispatcherImpl& dispatcher, signal_t signal_num, SignalCb cb);

    private:
        SignalCb cb_;
    };
} // namespace Event

#endif //EVENTLOOP_BASE_SERVER_SIGNALIMPL_H
