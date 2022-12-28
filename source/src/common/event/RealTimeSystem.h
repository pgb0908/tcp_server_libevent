//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_REALTIMESYSTEM_H
#define EVENTLOOP_BASE_SERVER_REALTIMESYSTEM_H

#include "include/event/Timer.h"
#include "src/common/common/Utility.h"

namespace Event {

/**
 * Real-world time implementation of TimeSystem.
 */
    class RealTimeSystem : public TimeSystem {
    public:
        // TimeSystem
        SchedulerPtr createScheduler(Scheduler&, CallbackScheduler&) override;

        // TimeSource
        SystemTime systemTime() override { return time_source_.systemTime(); }
        MonotonicTime monotonicTime() override { return time_source_.monotonicTime(); }

    private:
        RealTimeSource time_source_;
    };

} // namespace Event

#endif //EVENTLOOP_BASE_SERVER_REALTIMESYSTEM_H
