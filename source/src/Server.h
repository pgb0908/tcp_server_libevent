//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_SERVER_H
#define LIBEVENT_TEST1_SERVER_H

#include "include/event/Dispatcher.h"

class Server {
public:
    Server();

    void run();
    void shutdown();

    Event::Dispatcher& dispatcher() { return *dispatcher_; }

private:
    //void addListenerToWorker(Worker& worker, ListenerImpl& listener);
    Event::DispatcherPtr dispatcher_;

    //std::unique_ptr<ListenerImpl> listener_;
    //std::unique_ptr<ProdWorkerFactory> workerFactory_;

    //std::vector<WorkerPtr> workers_;

};


#endif //LIBEVENT_TEST1_SERVER_H
