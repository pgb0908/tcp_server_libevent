//
// Created by bong on 22. 12. 15.
//

#include "Server.h"

#include <memory>
#include "src/common/event/DispatcherImpl.h"
#include "src/common/api/ApiImpl.h"
#include "src/common/common/RandomGeneratorImpl.h"
#include "src/common/common/posix/ThreadImpl.h"

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


Server::Server(Api::Api& api) :
               api_(api),
               dispatcher_(std::make_unique<Event::DispatcherImpl>("main", api_)){

}

SystemTime myTime::systemTime() {
    return std::chrono::system_clock::now();
}

MonotonicTime myTime::monotonicTime() {
    return std::chrono::steady_clock::now();
}

Event::SchedulerPtr myTime::createScheduler(Event::Scheduler &base_scheduler, Event::CallbackScheduler &cb_scheduler) {

}
