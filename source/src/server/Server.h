//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_SERVER_H
#define LIBEVENT_TEST1_SERVER_H

#include "include/event/Dispatcher.h"
#include "include/thread/Thread.h"
#include "include/common/RandomGenerator.h"
#include "include/api/Api.h"
#include "include/server/LifecycleNotifier.h"

#include "absl/container/node_hash_map.h"

namespace Server {

    class myTime : public Event::TimeSystem {
    public:
        SystemTime systemTime() override;

        MonotonicTime monotonicTime() override;

        Event::SchedulerPtr
        createScheduler(Event::Scheduler &base_scheduler, Event::CallbackScheduler &cb_scheduler) override;
    };


class Server : public ServerLifecycleNotifier{
public:
    Server(Api::Api& api, Event::TimeSystem& timeSystem);

    void run();
    void shutdown();

    Event::Dispatcher& dispatcher() { return *dispatcher_; }

    HandlePtr registerCallback(Stage stage, StageCallback callback) override;

    HandlePtr registerCallback(Stage stage, StageCallbackWithCompletion callback) override;

    void notifyCallbacksForStage(
            Stage stage, Event::PostCb completion_cb = [] {});

    void initialize();

private:
    Api::Api& api_;
    Event::DispatcherPtr dispatcher_;
    bool workers_started_;

    using LifecycleNotifierCallbacks = std::list<StageCallback>;
    using LifecycleNotifierCompletionCallbacks = std::list<StageCallbackWithCompletion>;
    absl::node_hash_map<Stage, LifecycleNotifierCallbacks> stage_callbacks_;
    absl::node_hash_map<Stage, LifecycleNotifierCompletionCallbacks> stage_completable_callbacks_;

    //void addListenerToWorker(Worker& worker, ListenerImpl& listener);
    // std::unique_ptr<ListenerImpl> listener_;
    //std::unique_ptr<ProdWorkerFactory> workerFactory_;

    //std::vector<WorkerPtr> workers_;

};
}

#endif //LIBEVENT_TEST1_SERVER_H
