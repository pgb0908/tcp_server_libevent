//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_WORKER_H
#define EVENTLOOP_BASE_SERVER_WORKER_H

#include <functional>

#include "include/event/Dispatcher.h"

namespace Server {

/**
 * Interface for a threaded connection handling worker. All routines are thread safe.
 */
    class Worker {
    public:
        virtual ~Worker() = default;

        /**
         * Completion called when a listener has been added on a worker and is listening for new
         * connections.
         */
        using AddListenerCompletion = std::function<void()>;

        /**
         * Add a listener to the worker and replace the previous listener if any. If the previous listener
         * doesn't exist, the behavior should be equivalent to add a new listener.
         * @param overridden_listener The previous listener tag to be replaced. nullopt if it's a new
         * listener.
         * @param listener supplies the listener to add.
         * @param completion supplies the completion to call when the listener has been added (or not) on
         *                   the worker.
         * @param runtime, supplies the runtime for the server
         */
/*        virtual void addListener(absl::optional<uint64_t> overridden_listener,
                                 Network::ListenerConfig& listener, AddListenerCompletion completion,
                                 Runtime::Loader& runtime) = 0;*/

        /**
         * @return uint64_t the number of connections across all listeners that the worker owns.
         */
        virtual uint64_t numConnections() const = 0;

        /**
         * Start the worker thread.
         * @param guard_dog supplies the guard dog to use for thread watching.
         * @param cb a callback to run when the worker thread starts running.
         */
        virtual void start(const Event::PostCb& cb) = 0;

        /**
         * Initialize stats for this worker's dispatcher, if available. The worker will output
         * thread-specific stats under the given scope.
         * @param scope the scope to contain the new per-dispatcher stats created here.
         */
        //virtual void initializeStats(Stats::Scope& scope) = 0;

        /**
         * Stop the worker thread.
         */
        virtual void stop() = 0;

        /**
         * Remove a listener from the worker.
         * @param listener supplies the listener to remove.
         * @param completion supplies the completion to be called when the listener has been removed.
         *        This completion is called on the worker thread. No locking is performed by the worker.
         */
  /*      virtual void removeListener(Network::ListenerConfig& listener,
                                    std::function<void()> completion) = 0;*/
        /**
         * Remove the stale filter chains of the given listener but leave the listener running.
         * @param listener_tag supplies the tag passed to addListener().
         * @param filter_chains supplies the filter chains to be removed.
         * @param completion supplies the completion to be called when the listener removed all the
         * untracked connections. This completion is called on the worker thread. No locking is performed
         * by the worker.
         */
/*        virtual void removeFilterChains(uint64_t listener_tag,
                                        const std::list<const Network::FilterChain*>& filter_chains,
                                        std::function<void()> completion) = 0;*/

        /**
         * Stop a listener from accepting new connections. This is used for server draining.
         * @param listener supplies the listener to stop.
         * @param completion supplies the completion to be called when the listener has stopped
         * accepting new connections. This completion is called on the worker thread. No locking is
         * performed by the worker.
         */
/*        virtual void stopListener(Network::ListenerConfig& listener,
                                  std::function<void()> completion) = 0;*/
    };

    using WorkerPtr = std::unique_ptr<Worker>;

/**
 * Factory for creating workers.
 */
    class WorkerFactory {
    public:
        virtual ~WorkerFactory() = default;

        /**
         * @param index supplies the index of the worker, in the range of [0, concurrency).
         * @param overload_manager supplies the server's overload manager.
         * @param worker_name supplies the name of the worker, used for per-worker stats.
         * @return WorkerPtr a new worker.
         */
        virtual WorkerPtr createWorker(uint32_t index, const std::string& worker_name) = 0;
    };

} // namespace Server

#endif //EVENTLOOP_BASE_SERVER_WORKER_H
