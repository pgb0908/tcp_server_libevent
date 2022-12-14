//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_TIMERIMPL_H
#define LIBEVENT_TEST1_TIMERIMPL_H


#include <atomic>
#include "Timer.h"
#include "EventImplBase.h"
#include "Libevent.h"


/**
 * Utility helper functions for Timer implementation.
 */
class TimerUtils {
public:
    /**
     * Intended for consumption by enable(HR)Timer, this method is templated method to avoid implicit
     * duration conversions for its input arguments. This lets us have an opportunity to check bounds
     * before doing any conversions. When the passed in duration exceeds INT32_MAX max seconds, the
     * output will be clipped to yield INT32_MAX seconds and 0 microseconds for the
     * output argument. We clip to INT32_MAX to guard against overflowing the timeval structure.
     * Throws an EnvoyException on negative duration input.
     * @tparam Duration std::chrono duration type, e.g. seconds, milliseconds, ...
     * @param d duration value
     * @param tv output parameter that will be updated
     */
    template <typename Duration> static void durationToTimeval(const Duration& d, timeval& tv) {
        if (d.count() < 0) {
/*            ExceptionUtil::throwEnvoyException(
                    fmt::format("Negative duration passed to durationToTimeval(): {}", d.count()));*/

            throw std::runtime_error("Negative duration passed to durationToTimeval()");
        };
        constexpr int64_t clip_to = INT32_MAX; // 136.102208 years
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(d);
        if (secs.count() > clip_to) {
            tv.tv_sec = clip_to;
            tv.tv_usec = 0;
            return;
        }

        auto usecs = std::chrono::duration_cast<std::chrono::microseconds>(d - secs);
        tv.tv_sec = secs.count();
        tv.tv_usec = usecs.count();
    }
};

class TimerImpl : public Timer, EventImplBase {
public:
    TimerImpl(BasePtr& libevent, TimerCb cb, Dispatcher& dispatcher);

    // Timer
    void disableTimer() override;

    void enableTimer(std::chrono::milliseconds d, const ScopeTrackedObject* scope) override;
    void enableHRTimer(std::chrono::microseconds us, const ScopeTrackedObject* object) override;

    bool enabled() override;

private:
    void internalEnableTimer(const timeval& tv, const ScopeTrackedObject* scope);
    TimerCb cb_;
    Dispatcher& dispatcher_;
    // This has to be atomic for alarms which are handled out of thread, for
    // example if the DispatcherImpl::post is called by two threads, they race to
    // both set this to null.
    std::atomic<const ScopeTrackedObject*> object_{};
};


#endif //LIBEVENT_TEST1_TIMERIMPL_H
