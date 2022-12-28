//
// Created by bong on 22. 12. 28.
//

#include "RealTimeSystem.h"

namespace Event {
    namespace {

        class RealScheduler : public Scheduler {
        public:
            RealScheduler(Scheduler& base_scheduler) : base_scheduler_(base_scheduler) {}
            TimerPtr createTimer(const TimerCb& cb, Dispatcher& d) override {
                return base_scheduler_.createTimer(cb, d);
            };

        private:
            Scheduler& base_scheduler_;
        };

    } // namespace

    SchedulerPtr RealTimeSystem::createScheduler(Scheduler& base_scheduler, CallbackScheduler&) {
        return std::make_unique<RealScheduler>(base_scheduler);
    }

} // namespace Event