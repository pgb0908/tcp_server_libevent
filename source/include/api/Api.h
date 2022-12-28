//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_API_H
#define EVENTLOOP_BASE_SERVER_API_H

#include <memory>
#include <string>

#include "include/event/Dispatcher.h"
#include "include/event/ScaledRangeTimerManager.h"
#include "include/thread/Thread.h"
#include "include/common/RandomGenerator.h"

namespace Api {

/**
 * "Public" API that different components use to interact with the various system abstractions.
 */
    class Api {
    public:
        virtual ~Api() = default;

        /**
         * Allocate a dispatcher.
         * @param name the identity name for a dispatcher, e.g. "worker_2" or "main_thread".
         *             This name will appear in per-handler/worker statistics, such as
         *             "server.worker_2.watchdog_miss".
         * @return Event::DispatcherPtr which is owned by the caller.
         */
        virtual Event::DispatcherPtr allocateDispatcher(const std::string& name) = 0;

        /**
         * Allocate a dispatcher.
         * @param name the identity name for a dispatcher, e.g. "worker_2" or "main_thread".
         *             This name will appear in per-handler/worker statistics, such as
         *             "server.worker_2.watchdog_miss".
         * @param scaled_timer_factory the factory to use when creating the scaled timer manager.
         * @return Event::DispatcherPtr which is owned by the caller.
         */
/*        virtual Event::DispatcherPtr
        allocateDispatcher(const std::string& name,
                           const Event::ScaledRangeTimerManagerFactory& scaled_timer_factory) = 0;*/

        /**
         * Allocate a dispatcher.
         * @param name the identity name for a dispatcher, e.g. "worker_2" or "main_thread".
         *             This name will appear in per-handler/worker statistics, such as
         *             "server.worker_2.watchdog_miss".
         * @param watermark_factory the watermark factory, ownership is transferred to the dispatcher.
         * @return Event::DispatcherPtr which is owned by the caller.
         */
/*        virtual Event::DispatcherPtr
        allocateDispatcher(const std::string& name, Buffer::WatermarkFactoryPtr&& watermark_factory) = 0;*/

        /**
         * @return a reference to the ThreadFactory
         */
        virtual Thread::ThreadFactory& threadFactory() = 0;

        /**
         * @return a reference to the Filesystem::Instance
         */
        //virtual Filesystem::Instance& fileSystem() = 0;

        /**
         * @return a reference to the TimeSource
         */
        virtual TimeSource& timeSource() = 0;

        /**
         * @return a reference to the root Stats::Scope
         */
        //virtual Stats::Scope& rootScope() = 0;

        /**
         * @return a reference to the RandomGenerator.
         */
        virtual Random::RandomGenerator& randomGenerator() = 0;

        /**
         * @return an optional reference to the ProcessContext
         */
        //virtual ProcessContextOptRef processContext() = 0;

        /**
         * @return the bootstrap Envoy started with.
         */
        //virtual const envoy::config::bootstrap::v3::Bootstrap& bootstrap() const = 0;

        /**
         * @return a reference to the Stats::CustomStatNamespaces.
         */
        //virtual Stats::CustomStatNamespaces& customStatNamespaces() = 0;
    };

    using ApiPtr = std::unique_ptr<Api>;

} // namespace Api

#endif //EVENTLOOP_BASE_SERVER_API_H
