//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_WORKERIMPL_H
#define LIBEVENT_TEST1_WORKERIMPL_H


#include "Worker.h"
#include "ConnectionHandler.h"

class ProdWorkerFactory : public WorkerFactory{
public:
    ProdWorkerFactory() = default;

    // Server::WorkerFactory
    WorkerPtr createWorker(uint32_t index, const std::string& worker_name) override;

};


/**
 * A server threaded worker that wraps up a worker thread, event loop, etc.
 */
class WorkerImpl : public Worker{
public:
    WorkerImpl(DispatcherPtr&& dispatcher, ConnectionHandlerPtr handler);

    // Server::Worker
    void addListener(std::optional<uint64_t> overridden_listener) override;
    uint64_t numConnections() const override;

    //void removeListener(Network::ListenerConfig& listener, std::function<void()> completion) override;
/*    void removeFilterChains(uint64_t listener_tag,
                            const std::list<const Network::FilterChain*>& filter_chains,
                            std::function<void()> completion) override;*/
    void start(const PostCb& cb) override;
    void stop() override;
    //void stopListener(Network::ListenerConfig& listener, std::function<void()> completion) override;

private:
    void threadRoutine(const PostCb& cb);
    //void stopAcceptingConnectionsCb(OverloadActionState state);
    //void rejectIncomingConnectionsCb(OverloadActionState state);
    //void resetStreamsUsingExcessiveMemory(OverloadActionState state);

    //ThreadLocal::Instance& tls_;
    //ListenerHooks& hooks_;
    DispatcherPtr dispatcher_;
    ConnectionHandlerPtr handler_;
    //Api::Api& api_;
    //Stats::Counter& reset_streams_counter_;
    //ThreadPtr thread_;
};

#endif //LIBEVENT_TEST1_WORKERIMPL_H
