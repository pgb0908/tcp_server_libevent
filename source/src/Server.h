//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_SERVER_H
#define LIBEVENT_TEST1_SERVER_H

#include "include/event/Dispatcher.h"
#include "include/thread/Thread.h"
#include "include/common/RandomGenerator.h"
#include "include/api/Api.h"

class myTime : public Event::TimeSystem {
public:
    SystemTime systemTime() override;

    MonotonicTime monotonicTime() override;

    Event::SchedulerPtr
    createScheduler(Event::Scheduler &base_scheduler, Event::CallbackScheduler &cb_scheduler) override;
};

class Server {
public:
    Server(Api::Api& api);

    void run();
    void shutdown();

    Event::Dispatcher& dispatcher() { return *dispatcher_; }

private:
    Api::Api& api_;
    //void addListenerToWorker(Worker& worker, ListenerImpl& listener);
    Event::DispatcherPtr dispatcher_;

    // std::unique_ptr<ListenerImpl> listener_;
    //std::unique_ptr<ProdWorkerFactory> workerFactory_;

    //std::vector<WorkerPtr> workers_;

};


#endif //LIBEVENT_TEST1_SERVER_H
