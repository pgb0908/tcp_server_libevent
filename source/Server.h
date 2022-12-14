//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_SERVER_H
#define LIBEVENT_TEST1_SERVER_H


#include "Dispatcher.h"
#include "WorkerImpl.h"
#include "ListenerImpl.h"

class Server {
public:
    Server();

    void run();
    void shutdown();
    Dispatcher& dispatcher() { return *dispatcher_; }

private:
    void addListenerToWorker(Worker& worker, ListenerImpl& listener);
    DispatcherPtr dispatcher_;

    std::unique_ptr<ListenerImpl> listener_;
    std::unique_ptr<ProdWorkerFactory> workerFactory_;

    std::vector<WorkerPtr> workers_;

};


#endif //LIBEVENT_TEST1_SERVER_H
