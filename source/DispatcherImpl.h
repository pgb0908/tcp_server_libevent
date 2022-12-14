//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_DISPATCHERIMPL_H
#define LIBEVENT_TEST1_DISPATCHERIMPL_H


#include "Dispatcher.h"
#include "Libevent.h"
#include "LibeventScheduler.h"
#include "ListenSocket.h"
#include "Thread.h"
#include "ScaledRangeTimerManager.h"

class DispatcherImpl : public Dispatcher {
public:
    explicit DispatcherImpl(std::string name);

    /**
     * @return event_base& the libevent base.
     */
    event_base& base() { return base_scheduler_.base(); }

    // Event::Dispatcher
    const std::string& name() override { return name_; }
    //void registerWatchdog(const Server::WatchDogSharedPtr& watchdog, std::chrono::milliseconds min_touch_interval) override;
    //TimeSource& timeSource() override { return time_source_; }
    //void initializeStats(Stats::Scope& scope, const absl::optional<std::string>& prefix) override;
    //void clearDeferredDeleteList() override;
/*    ServerConnectionPtr createServerConnection(ConnectionSocketPtr&& socket,
                           TransportSocketPtr&& transport_socket,
                           StreamInfo& stream_info) override;*/

/*    ClientConnectionPtr createClientConnection(
            Network::Address::InstanceConstSharedPtr address,
            Network::Address::InstanceConstSharedPtr source_address,
            Network::TransportSocketPtr&& transport_socket,
            const Network::ConnectionSocket::OptionsSharedPtr& options,
            const Network::TransportSocketOptionsConstSharedPtr& transport_options) override;*/

    FileEventPtr createFileEvent(os_fd_t fd, FileReadyCb cb, FileTriggerType trigger,
                                 uint32_t events) override;

    //Filesystem::WatcherPtr createFilesystemWatcher() override;

/*    ListenerPtr createListener(Network::SocketSharedPtr&& socket,
                                        Network::TcpListenerCallbacks& cb, Runtime::Loader& runtime,
                                        bool bind_to_port, bool ignore_global_conn_limit) override;*/
/*    UdpListenerPtr createUdpListener(Network::SocketSharedPtr socket, Network::UdpListenerCallbacks& cb,
                      const envoy::config::core::v3::UdpSocketConfig& config) override;*/
    TimerPtr createTimer(TimerCb cb) override;
    TimerPtr createScaledTimer(ScaledTimerType timer_type, TimerCb cb) override;
    TimerPtr createScaledTimer(ScaledTimerMinimum minimum, TimerCb cb) override;

    SchedulableCallbackPtr createSchedulableCallback(std::function<void()> cb) override;
    void deferredDelete(DeferredDeletablePtr&& to_delete) override;
    void exit() override;
    SignalEventPtr listenForSignal(signal_t signal_num, SignalCb cb) override;
    void post(std::function<void()> callback) override;
    void deleteInDispatcherThread(DispatcherThreadDeletableConstPtr deletable) override;
    void run(RunType type) override;
    //Buffer::WatermarkFactory& getWatermarkFactory() override { return *buffer_factory_; }
    //void pushTrackedObject(const ScopeTrackedObject* object) override;
    //void popTrackedObject(const ScopeTrackedObject* expected_object) override;
    //bool trackedObjectStackIsEmpty() const override { return tracked_object_stack_.empty(); }
    MonotonicTime approximateMonotonicTime() const override;
    void updateApproximateMonotonicTime() override;
    void shutdown() override;

    // FatalErrorInterface
    //void onFatalError(std::ostream& os) const override;
    //void runFatalActionsOnTrackedObject(const FatalAction::FatalActionPtrList& actions) const override;

private:
    TimerPtr createTimerInternal(TimerCb cb);
    void updateApproximateMonotonicTimeInternal();
    void runPostCallbacks();
    void runThreadLocalDelete();

    // Validate that an operation is thread safe, i.e. it's invoked on the same thread that the
    // dispatcher run loop is executing on. We allow run_tid_ to be empty for tests where we don't
    // invoke run().
    bool isThreadSafe() const override {
        return run_tid_.isEmpty() || run_tid_ == thread_factory_->currentThreadId();
    }

    std::string &name_;
    std::unique_ptr<ThreadFactory> thread_factory_;
    //Random::RandomGenerator& random_generator_;
    //Filesystem::Instance& file_system_;
    std::string stats_prefix_;
    //DispatcherStatsPtr stats_;
    ThreadId run_tid_{};
    //Buffer::WatermarkFactorySharedPtr buffer_factory_;
    LibeventScheduler base_scheduler_;
    SchedulerPtr scheduler_;

    SchedulableCallbackPtr thread_local_delete_cb_;
    //Thread::MutexBasicLockable thread_local_deletable_lock_;
    // `deletables_in_dispatcher_thread` must be destroyed last to allow other callbacks populate.
    //std::list<DispatcherThreadDeletableConstPtr> deletables_in_dispatcher_thread_ ABSL_GUARDED_BY(thread_local_deletable_lock_);
    bool shutdown_called_{false};

    SchedulableCallbackPtr deferred_delete_cb_;

    SchedulableCallbackPtr post_cb_;
    //Thread::MutexBasicLockable post_lock_;
    //std::list<std::function<void()>> post_callbacks_ ABSL_GUARDED_BY(post_lock_);

    std::vector<DeferredDeletablePtr> to_delete_1_;
    std::vector<DeferredDeletablePtr> to_delete_2_;
    std::vector<DeferredDeletablePtr>* current_to_delete_{};

    //absl::InlinedVector<const ScopeTrackedObject*, ExpectedMaxTrackedObjectStackDepth> tracked_object_stack_;
    bool deferred_deleting_{};
    MonotonicTime approximate_monotonic_time_;
    //WatchdogRegistrationPtr watchdog_registration_;
    const ScaledRangeTimerManagerPtr scaled_timer_manager_;
};


#endif //LIBEVENT_TEST1_DISPATCHERIMPL_H
