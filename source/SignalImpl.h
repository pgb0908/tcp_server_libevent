//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_SIGNALIMPL_H
#define LIBEVENT_TEST1_SIGNALIMPL_H


#include "Signal.h"
#include "EventImplBase.h"
#include "DispatcherImpl.h"

class SignalImpl : public SignalEvent, EventImplBase{
public:
    SignalImpl(DispatcherImpl& dispatcher, signal_t signal_num, SignalCb cb);

private:
    SignalCb cb_;
};


#endif //LIBEVENT_TEST1_SIGNALIMPL_H
