//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_SCALEDRANGETIMERMANAGER_H
#define LIBEVENT_TEST1_SCALEDRANGETIMERMANAGER_H


#include "ScaledTimer.h"
#include "Timer.h"

class ScaledRangeTimerManager {
public:
    virtual ~ScaledRangeTimerManager() = default;

    /**
     * Creates a new timer backed by the manager. Calling enableTimer on the returned object sets the
     * maximum duration, while the first argument here controls the minimum. Passing a value of
     * ScaleFactor(x) sets the min to (x * max) when the timer is enabled, while AbsoluteValue(y) sets
     * the min to the duration y.
     */
    virtual TimerPtr createTimer(ScaledTimerMinimum minimum, TimerCb callback) = 0;

    /**
     * Creates a new timer backed by the manager using the provided timer type to
     * determine the minimum.
     */
    virtual TimerPtr createTimer(ScaledTimerType timer_type, TimerCb callback) = 0;

    /**
     * Sets the scale factor for all timers created through this manager. The value should be between
     * 0 and 1, inclusive. The scale factor affects the amount of time timers spend in their target
     * range. The timers returned by createTimer will fire after (min + (max - min) * scale_factor).
     * This means that a scale factor of 0 causes timers to fire immediately at the min duration, a
     * factor of 0.5 causes firing halfway between min and max, and a factor of 1 causes firing at
     * max.
     */
    virtual void setScaleFactor(UnitFloat scale_factor) = 0;
};

using ScaledRangeTimerManagerPtr = std::unique_ptr<ScaledRangeTimerManager>;

class Dispatcher;
using ScaledRangeTimerManagerFactory = std::function<ScaledRangeTimerManagerPtr(Dispatcher&)>;

#endif //LIBEVENT_TEST1_SCALEDRANGETIMERMANAGER_H
