//
// Created by bong on 22. 12. 28.
//

#include "ApiImpl.h"
#include "src/common/event/DispatcherImpl.h"


namespace Api {

    Impl::Impl(Thread::ThreadFactory& thread_factory,
               Event::TimeSystem& time_system,
               Random::RandomGenerator& random_generator)
            : thread_factory_(thread_factory), time_system_(time_system),
              random_generator_(random_generator) {}

/*    Impl::Impl(Thread::ThreadFactory& thread_factory, Stats::Store& store,
               Event::TimeSystem& time_system, Filesystem::Instance& file_system,
               Random::RandomGenerator& random_generator,
               const envoy::config::bootstrap::v3::Bootstrap& bootstrap,
               const ProcessContextOptRef& process_context,
               Buffer::WatermarkFactorySharedPtr watermark_factory)
            : thread_factory_(thread_factory), store_(store), time_system_(time_system),
              file_system_(file_system), random_generator_(random_generator), bootstrap_(bootstrap),
              process_context_(process_context), watermark_factory_(std::move(watermark_factory)) {}*/

    Event::DispatcherPtr Impl::allocateDispatcher(const std::string& name) {
        return std::make_unique<Event::DispatcherImpl>(name, *this, time_system_);
    }

/*    Event::DispatcherPtr
    Impl::allocateDispatcher(const std::string& name,
                             const Event::ScaledRangeTimerManagerFactory& scaled_timer_factory) {
        return std::make_unique<Event::DispatcherImpl>(name, *this, time_system_, scaled_timer_factory);
    }*/

/*    Event::DispatcherPtr Impl::allocateDispatcher(const std::string& name,
                                                  Buffer::WatermarkFactoryPtr&& factory) {
        return std::make_unique<Event::DispatcherImpl>(name, *this, time_system_, std::move(factory));
    }*/

} // namespace Api