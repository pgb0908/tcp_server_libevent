//
// Created by bong on 22. 12. 15.
//

#include "Server.h"

#include <memory>
#include "fmt/format.h"
#include "src/common/event/DispatcherImpl.h"
#include "src/common/api/ApiImpl.h"
#include "src/common/common/RandomGeneratorImpl.h"
#include "src/common/common/posix/ThreadImpl.h"
#include "src/common/common/Cleanup.h"
#include "src/server/ListenerManagerImpl.h"

namespace Server {
    void Server::run() {
        std::cout << "starting main dispatch loop" << std::endl;
        dispatcher_->post([this] { notifyCallbacksForStage(Stage::Startup); });

        dispatcher_->post([](){std::cout << "hello" << std::endl;});


        //Event::TimerPtr timer = dispatcher_->createTimer([]() ->void{ std::cout << "<<<<  timer called" << std::endl;});
        //timer->enableTimer(std::chrono::milliseconds(10000));
        //std::cout << "this is enabled timer : "  << timer->enabled()<< std::endl;

        infinit_loop();


        dispatcher_->run(Event::Dispatcher::RunType::Block);
        //dispatcher_->post([this] { notifyCallbacksForStage(Stage::PostInit); });

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
                //std::cout << "Notifying" << it2->second.size() << "callback(s) with completion." << std::endl;
                std::cout << fmt::format("Notifying {} callback(s) with completion.", it2->second.size()) << std::endl;
                //ENVOY_LOG(info, "Notifying {} callback(s) with completion.", it2->second.size());
                for (const StageCallbackWithCompletion& callback : it2->second) {
                    callback([cb_guard] {});
                }
            }
        }
    }

    void Server::initialize() {

        // Workers get created first so they register for thread local updates.
        listener_manager_ = std::make_unique<ListenerManagerImpl>(*this, listener_component_factory_, worker_factory_);

        // listener는 현재 하나
        listener_manager_->addOrUpdateListener("", false);

        listener_manager_->startWorkers([this]() {
/*            if (isShutdown()) {
                return;
            }*/

            //initialization_timer_->complete();
            // Update server stats as soon as initialization is done.
            //updateServerStats();
            workers_started_ = true;
            //hooks_.onWorkersStarted();
            // At this point we are ready to take traffic and all listening ports are up. Notify our
            // parent if applicable that they can stop listening and drain.
            //restarter_.drainParentListeners();
            //drain_manager_->startParentShutdownSequence();
        });


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