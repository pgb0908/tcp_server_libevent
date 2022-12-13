//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_TIMER_H
#define LIBEVENT_TEST1_TIMER_H

#include <functional>
#include <chrono>
#include <memory>
#include "ScopeTrackedObject.h"
#include "Dispatcher.h"
#include "TimeSource.h"
#include "SchedulableCallback.h"

/**
 * Callback invoked when a timer event fires.
 */
using TimerCb = std::function<void()>;

class Timer {
public:
    virtual ~Timer() = default;

    /**
     * Disable a pending timeout without destroying the underlying timer.
     */
    virtual void disableTimer() =0;

    /**
     * Enable a pending timeout. If a timeout is already pending, it will be reset to the new timeout.
     *
     * @param ms supplies the duration of the alarm in milliseconds.
     * @param object supplies an optional scope for the duration of the alarm.
     */
    virtual void enableTimer(std::chrono::milliseconds ms,
                             const ScopeTrackedObject* object = nullptr) = 0;

    /**
     * Enable a pending high resolution timeout. If a timeout is already pending, it will be reset to
     * the new timeout.
     *
     * @param us supplies the duration of the alarm in microseconds.
     * @param object supplies an optional scope for the duration of the alarm.
     */
    virtual void enableHRTimer(std::chrono::microseconds us,
                               const ScopeTrackedObject* object = nullptr) = 0;
    /**
     * Return whether the timer is currently armed.
     */
    virtual bool enabled() = 0;
};

using TimerPtr = std::unique_ptr<Timer>;

class Scheduler {
public:
    virtual ~Scheduler() = default;

    /**
     * Creates a timer.
     */
    virtual TimerPtr createTimer(const TimerCb& cb, Dispatcher& dispatcher) = 0;
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

/**
 * Interface providing a mechanism to measure time and set timers that run callbacks
 * when the timer fires.
 */
class TimeSystem : public TimeSource {
public:
    ~TimeSystem() override = default;

    using Duration = MonotonicTime::duration;
    using Nanoseconds = std::chrono::nanoseconds;
    using Microseconds = std::chrono::microseconds;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;

    /**
     * Creates a timer factory. This indirection enables thread-local timer-queue management,
     * so servers can have a separate timer-factory in each thread.
     */
    virtual SchedulerPtr createScheduler(Scheduler& base_scheduler,
                                         CallbackScheduler& cb_scheduler) = 0;
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

#endif //LIBEVENT_TEST1_TIMER_H
