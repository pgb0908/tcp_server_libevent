//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_WORKERIMPL_H
#define EVENTLOOP_BASE_SERVER_WORKERIMPL_H

#include <functional>
#include <memory>

#include "include/server/Worker.h"
#include "include/api/Api.h"
#include "include/network/ConnectionHandler.h"

namespace Server {

// Captures a set of stat names for the workers.
/*    struct WorkerStatNames {
        explicit WorkerStatNames(Stats::SymbolTable& symbol_table)
                : pool_(symbol_table),
                  reset_high_memory_stream_(pool_.add(OverloadActionStatsNames::get().ResetStreamsCount)) {}

        Stats::StatNamePool pool_;
        Stats::StatName reset_high_memory_stream_;
    };*/

    class ProdWorkerFactory : public WorkerFactory {
    public:
        explicit ProdWorkerFactory(Api::Api& api)
                :  api_(api){}

/*        ProdWorkerFactory(ThreadLocal::Instance& tls, Api::Api& api, ListenerHooks& hooks)
                : tls_(tls), api_(api), stat_names_(api.rootScope().symbolTable()), hooks_(hooks) {}*/

        // Server::WorkerFactory
        WorkerPtr createWorker(uint32_t index, const std::string& worker_name) override;

    private:
        //ThreadLocal::Instance& tls_;
        Api::Api& api_;
        //WorkerStatNames stat_names_;
        //ListenerHooks& hooks_;
    };

/**
 * A server threaded worker that wraps up a worker thread, event loop, etc.
 */
    class WorkerImpl : public Worker {
    public:
        WorkerImpl( Event::DispatcherPtr&& dispatcher, Network::ConnectionHandlerPtr handler, Api::Api& api);

/*        WorkerImpl(ThreadLocal::Instance& tls, ListenerHooks& hooks, Event::DispatcherPtr&& dispatcher,
                   Network::ConnectionHandlerPtr handler, OverloadManager& overload_manager,
                   Api::Api& api, WorkerStatNames& stat_names);*/

        // Server::Worker
  /*      void addListener(absl::optional<uint64_t> overridden_listener, Network::ListenerConfig& listener,
                         AddListenerCompletion completion, Runtime::Loader& loader) override;*/

        uint64_t numConnections() const override;

        //void removeListener(Network::ListenerConfig& listener, std::function<void()> completion) override;
/*        void removeFilterChains(uint64_t listener_tag,
                                const std::list<const Network::FilterChain*>& filter_chains,
                                std::function<void()> completion) override;*/
        void start(const Event::PostCb& cb) override;
        //void initializeStats(Stats::Scope& scope) override;
        void stop() override;
        //void stopListener(Network::ListenerConfig& listener, std::function<void()> completion) override;

    private:
        void threadRoutine(const Event::PostCb& cb);
/*        void stopAcceptingConnectionsCb(OverloadActionState state);
        void rejectIncomingConnectionsCb(OverloadActionState state);
        void resetStreamsUsingExcessiveMemory(OverloadActionState state);*/

        //ThreadLocal::Instance& tls_;
        //ListenerHooks& hooks_;
        Event::DispatcherPtr dispatcher_;
        Network::ConnectionHandlerPtr handler_;
        Api::Api& api_;
        //Stats::Counter& reset_streams_counter_;
        Thread::ThreadPtr thread_;
        //WatchDogSharedPtr watch_dog_;
    };

} // namespace Server


#endif //EVENTLOOP_BASE_SERVER_WORKERIMPL_H
