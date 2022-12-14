//
// Created by bong on 22. 12. 15.
//

#include "TimerImpl.h"

#include "event2/event.h"
#include "src/common/common/ScopTracker.h"
#include <iostream>

namespace Event {

    TimerImpl::TimerImpl(Libevent::BasePtr& libevent, TimerCb cb, Dispatcher& dispatcher)
            : cb_(cb), dispatcher_(dispatcher) {
        //ASSERT(cb_);
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
        //ASSERT(dispatcher_.isThreadSafe());
        event_del(&raw_event_);
    }

    void TimerImpl::enableTimer(const std::chrono::milliseconds d, const ScopeTrackedObject* object) {
        timeval tv;
        TimerUtils::durationToTimeval(d, tv);
        internalEnableTimer(tv, object);
    }

    void TimerImpl::enableHRTimer(const std::chrono::microseconds d,
                                  const ScopeTrackedObject* object = nullptr) {
        timeval tv;
        TimerUtils::durationToTimeval(d, tv);
        internalEnableTimer(tv, object);
    }

    void TimerImpl::internalEnableTimer(const timeval& tv, const ScopeTrackedObject* object) {
        //ASSERT(dispatcher_.isThreadSafe());
        object_ = object;

        auto i = event_add(&raw_event_, &tv);
        std::cout << "timer event return : "<< i << std::endl;
    }

    bool TimerImpl::enabled() {
        //ASSERT(dispatcher_.isThreadSafe());
        return 0 != evtimer_pending(&raw_event_, nullptr);
    }

} // namespace Event