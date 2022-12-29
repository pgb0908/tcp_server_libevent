//
// Created by bong on 22. 12. 15.
//

#include "Server.h"

#include <memory>
#include "src/common/event/DispatcherImpl.h"
#include "src/common/api/ApiImpl.h"
#include "src/common/common/RandomGeneratorImpl.h"
#include "src/common/common/posix/ThreadImpl.h"
#include "src/common/common/Cleanup.h"
#include "src/server/ListenerManagerImpl.h"

namespace Server {
    void Server::run() {
        std::cout << "starting main dispatch loop" << std::endl;
        dispatcher_->post([](){std::cout << "hello" << std::endl;});
        dispatcher_->post([this] { notifyCallbacksForStage(Stage::Startup); });
        dispatcher_->run(Event::Dispatcher::RunType::Block);
        std::cout << "main dispatch loop exited" << std::endl;
    }

    void Server::shutdown() {

    }


    Server::Server(Api::Api &api, Event::TimeSystem& timeSystem) :
            api_(api),
            dispatcher_(std::make_unique<Event::DispatcherImpl>("main", api_, timeSystem)),workers_started_(false),
            listener_component_factory_(*this),
            worker_factory_(api_)
    {
    }

    ServerLifecycleNotifier::HandlePtr
    Server::registerCallback(ServerLifecycleNotifier::Stage stage, ServerLifecycleNotifier::StageCallback callback) {
/*        auto& callbacks = stage_callbacks_[stage];
        return std::make_unique<LifecycleCallbackHandle<StageCallback>>(callbacks, callback);*/

        return nullptr;
    }

    ServerLifecycleNotifier::HandlePtr Server::registerCallback(ServerLifecycleNotifier::Stage stage,
                                                                ServerLifecycleNotifier::StageCallbackWithCompletion callback) {
/*        auto& callbacks = stage_completable_callbacks_[stage];
        return std::make_unique<LifecycleCallbackHandle<StageCallbackWithCompletion>>(callbacks,
                callback);*/

        return nullptr;
    }

    void Server::notifyCallbacksForStage(ServerLifecycleNotifier::Stage stage, Event::PostCb completion_cb) {
        const auto it = stage_callbacks_.find(stage);
        if (it != stage_callbacks_.end()) {
            for (const StageCallback& callback : it->second) {
                callback();
            }
        }

        // Wrap completion_cb so that it only gets invoked when all callbacks for this stage
        // have finished their work.
        std::shared_ptr<void> cb_guard(
                new Cleanup([this, completion_cb]() { dispatcher_->post(completion_cb); }));

        // Registrations which take a completion callback are typically implemented by executing a
        // callback on all worker threads using Slot::runOnAllThreads which will hang indefinitely if
        // worker threads have not been started so we need to skip notifications if envoy is shutdown
        // early before workers have started.
        if (workers_started_) {
            const auto it2 = stage_completable_callbacks_.find(stage);
            if (it2 != stage_completable_callbacks_.end()) {
                //ENVOY_LOG(info, "Notifying {} callback(s) with completion.", it2->second.size());
                for (const StageCallbackWithCompletion& callback : it2->second) {
                    callback([cb_guard] {});
                }
            }
        }
    }

    void Server::initialize() {
        // Workers get created first so they register for thread local updates.
        //listener_manager_ = std::make_unique<ListenerManagerImpl>(*this, listener_component_factory_, worker_factory_);


    }


    SystemTime myTime::systemTime() {
        return SystemTime();
    }

    MonotonicTime myTime::monotonicTime() {
        return MonotonicTime();
    }

    Event::SchedulerPtr
    myTime::createScheduler(Event::Scheduler &base_scheduler, Event::CallbackScheduler &cb_scheduler) {
        return Event::SchedulerPtr();
    }
}