//
// Created by bong on 22. 12. 14.
//

#include "TimerImpl.h"
#include "event2/event.h"

TimerImpl::TimerImpl(BasePtr &libevent, TimerCb cb, Dispatcher &dispatcher)
    : cb_(cb), dispatcher_(dispatcher){
    evtimer_assign(
            &raw_event_, libevent.get(),
            [](evutil_socket_t, short, void* arg) -> void {
                TimerImpl* timer = static_cast<TimerImpl*>(arg);
                if (timer->object_ == nullptr) {
                    timer->cb_();
                    return;
                }
                //ScopeTrackerScopeState scope(timer->object_, timer->dispatcher_);
                timer->object_ = nullptr;
                timer->cb_();
            },
            this);
}

void TimerImpl::disableTimer() {
    event_del(&raw_event_);
}

void TimerImpl::enableTimer(std::chrono::milliseconds d, const ScopeTrackedObject *object) {
    timeval tv;
    TimerUtils::durationToTimeval(d, tv);
    internalEnableTimer(tv, object);
}

void TimerImpl::enableHRTimer(std::chrono::microseconds d, const ScopeTrackedObject *object = nullptr) {
    timeval tv;
    TimerUtils::durationToTimeval(d, tv);
    internalEnableTimer(tv, object);
}

bool TimerImpl::enabled() {
    return 0 != evtimer_pending(&raw_event_, nullptr);
}

void TimerImpl::internalEnableTimer(const timeval &tv, const ScopeTrackedObject *object) {
    object_ = object;

    event_add(&raw_event_, &tv);
}


