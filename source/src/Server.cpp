//
// Created by bong on 22. 12. 15.
//

#include "Server.h"
#include "src/common/event/DispatcherImpl.h"

void Server::run() {

/*    for (const auto& worker : workers_) {
        worker->start([](){
            std::cout << "hello" << std::endl;
            return; });
    }

    //dispatcher_->post([this] { notifyCallbacksForStage(Stage::Startup); }));
    dispatcher_->run(Dispatcher::RunType::Block);*/
}

void Server::shutdown() {

}

/*void Server::addListenerToWorker(Worker &worker, ListenerImpl &listener) {
    worker.addListener(
            overridden_listener, listener,
            [this, completion_callback]() -> void {
                // The add listener completion runs on the worker thread. Post back to the main thread to
                // avoid locking.
                server_.dispatcher().post([this, completion_callback]() -> void {
                    // stats_.listener_create_success_.inc();
                    if (completion_callback) {
                        completion_callback();
                    }
                });
            },
            server_.runtime());
}*/

Server::Server(){
    myTime time;
    //dispatcher_ = std::make_unique<Event::DispatcherImpl>("dispahter1", , time);
/*    listener_ = std::make_unique<ListenerImpl>();
    workerFactory_ = std::make_unique<ProdWorkerFactory>();

    // worker에게 listener 할당
    for (int i = 0; i < 2; i++) {
        workers_.emplace_back(
                workerFactory_->createWorker(i, std::string("worker_").append(std::to_string(i))));
    }

    for (const auto& worker : workers_) {
        addListenerToWorker(*worker, *listener_);
    }*/
}

SystemTime myTime::systemTime() {
    return std::chrono::system_clock::now();
}

MonotonicTime myTime::monotonicTime() {
    return std::chrono::steady_clock::now();
}

Event::SchedulerPtr myTime::createScheduler(Event::Scheduler &base_scheduler, Event::CallbackScheduler &cb_scheduler) {

}
