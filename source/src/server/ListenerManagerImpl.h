//
// Created by bong on 22. 12. 29.
//

#ifndef EVENTLOOP_BASE_SERVER_LISTENERMANAGERIMPL_H
#define EVENTLOOP_BASE_SERVER_LISTENERMANAGERIMPL_H

#include "include/server/ListenerManager.h"
#include "include/server/Worker.h"


namespace Server {

    namespace Configuration {
        class TransportSocketFactoryContextImpl;
    }

    class Server;

/**
 * Prod implementation of ListenerComponentFactory that creates real sockets and attempts to fetch
 * sockets from the parent process via the hot restarter. The filter factory list is created from
 * statically registered filters.
 */
    class ProdListenerComponentFactory : public ListenerComponentFactory {
    public:
        ProdListenerComponentFactory(Server& server) : server_(server) {}
        /**
         * Static worker for createNetworkFilterFactoryList() that can be used directly in tests.
         */
/*        static std::vector<Network::FilterFactoryCb> createNetworkFilterFactoryListImpl(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::Filter>& filters,
                Configuration::FilterChainFactoryContext& filter_chain_factory_context);*/

        /**
         * Static worker for createListenerFilterFactoryList() that can be used directly in tests.
         */
/*        static Filter::ListenerFilterFactoriesList createListenerFilterFactoryListImpl(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::ListenerFilter>& filters,
                Configuration::ListenerFactoryContext& context,
                Filter::TcpListenerFilterConfigProviderManagerImpl& config_provider_manager);*/

        /**
         * Static worker for createUdpListenerFilterFactoryList() that can be used directly in tests.
         */
/*        static std::vector<Network::UdpListenerFilterFactoryCb> createUdpListenerFilterFactoryListImpl(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::ListenerFilter>& filters,
                Configuration::ListenerFactoryContext& context);*/

        //static Network::ListenerFilterMatcherSharedPtr createListenerFilterMatcher(const envoy::config::listener::v3::ListenerFilter& listener_filter);

        // Server::ListenerComponentFactory
/*        LdsApiPtr createLdsApi(const envoy::config::core::v3::ConfigSource& lds_config,
                               const xds::core::v3::ResourceLocator* lds_resources_locator) override {
            return std::make_unique<LdsApiImpl>(
                    lds_config, lds_resources_locator, server_.clusterManager(), server_.initManager(),
                    server_.stats(), server_.listenerManager(),
                    server_.messageValidationContext().dynamicValidationVisitor());
        }*/
/*        std::vector<Network::FilterFactoryCb> createNetworkFilterFactoryList(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::Filter>& filters,
                Server::Configuration::FilterChainFactoryContext& filter_chain_factory_context) override {
            return createNetworkFilterFactoryListImpl(filters, filter_chain_factory_context);
        }*/
/*        Filter::ListenerFilterFactoriesList createListenerFilterFactoryList(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::ListenerFilter>& filters,
                Configuration::ListenerFactoryContext& context) override {
            return createListenerFilterFactoryListImpl(filters, context,
                                                       tcp_listener_config_provider_manager_);
        }*/
/*        std::vector<Network::UdpListenerFilterFactoryCb> createUdpListenerFilterFactoryList(
                const Protobuf::RepeatedPtrField<envoy::config::listener::v3::ListenerFilter>& filters,
                Configuration::ListenerFactoryContext& context) override {
            return createUdpListenerFilterFactoryListImpl(filters, context);
        }*/

/*        Network::SocketSharedPtr createListenSocket(
                Network::Address::InstanceConstSharedPtr address, Network::Socket::Type socket_type,
                const Network::Socket::OptionsSharedPtr& options, BindType bind_type,
                const Network::SocketCreationOptions& creation_options, uint32_t worker_index) override;*/

        //DrainManagerPtr createDrainManager(envoy::config::listener::v3::Listener::DrainType drain_type) override;
        uint64_t nextListenerTag() override { return next_listener_tag_++; }

/*        Filter::TcpListenerFilterConfigProviderManagerImpl* getTcpListenerConfigProviderManager() override {
            return &tcp_listener_config_provider_manager_;
        }*/



    private:
        Server& server_;
        uint64_t next_listener_tag_{1};
        //Filter::TcpListenerFilterConfigProviderManagerImpl tcp_listener_config_provider_manager_;
    };

    class ListenerImpl;
    using ListenerImplPtr = std::unique_ptr<ListenerImpl>;

    /**
     * Provides the draining filter chains and the functionality to schedule listener destroy.
     */
/*    class DrainingFilterChainsManager {
    public:
        DrainingFilterChainsManager(ListenerImplPtr&& draining_listener,
                                    uint64_t workers_pending_removal);
        uint64_t getDrainingListenerTag() const { return draining_listener_->listenerTag(); }
        const std::list<const Network::FilterChain*>& getDrainingFilterChains() const {
            return draining_filter_chains_;
        }
        ListenerImpl& getDrainingListener() const { return *draining_listener_; }
        uint64_t decWorkersPendingRemoval() { return --workers_pending_removal_; }

        // Schedule listener destroy.
        void startDrainSequence(std::chrono::seconds drain_time, Event::Dispatcher& dispatcher,
                                std::function<void()> completion) {
            drain_sequence_completion_ = completion;
            ASSERT(!drain_timer_);

            drain_timer_ = dispatcher.createTimer([this]() -> void { drain_sequence_completion_(); });
            drain_timer_->enableTimer(drain_time);
        }

        void addFilterChainToDrain(const Network::FilterChain& filter_chain) {
            draining_filter_chains_.push_back(&filter_chain);
        }

        uint32_t numDrainingFilterChains() const { return draining_filter_chains_.size(); }

    private:
        ListenerImplPtr draining_listener_;
        std::list<const Network::FilterChain*> draining_filter_chains_;

        uint64_t workers_pending_removal_;
        Event::TimerPtr drain_timer_;
        std::function<void()> drain_sequence_completion_;
    };*/

/**
 * Implementation of ListenerManager.
 */
    class ListenerManagerImpl : public ListenerManager{
    public:
        ListenerManagerImpl(Server& server, ListenerComponentFactory& listener_factory,
                            WorkerFactory& worker_factory);

        //void onListenerWarmed(ListenerImpl& listener);
        //void inPlaceFilterChainUpdate(ListenerImpl& listener);

        // Server::ListenerManager
        bool addOrUpdateListener(const std::string& version_info, bool added_via_api) override;
/*        void createLdsApi(const envoy::config::core::v3::ConfigSource& lds_config,
                          const xds::core::v3::ResourceLocator* lds_resources_locator) override {
            ASSERT(lds_api_ == nullptr);
            lds_api_ = factory_.createLdsApi(lds_config, lds_resources_locator);
        }*/
        //std::vector<std::reference_wrapper<Network::ListenerConfig>> listeners(ListenerState state = ListenerState::ACTIVE) override;
        uint64_t numConnections() const override;
        bool removeListener(const std::string& listener_name) override;
        void startWorkers(std::function<void()> callback) override;
        void stopListeners(StopListenersType stop_listeners_type) override;
        void stopWorkers() override;
        //void beginListenerUpdate() override { error_state_tracker_.clear(); }
        //void endListenerUpdate(FailureStates&& failure_state) override;
        bool isWorkerStarted() override { return workers_started_; }
        //Http::Context& httpContext() { return server_.httpContext(); }
        //ApiListenerOptRef apiListener() override;

        //Quic::QuicStatNames& quicStatNames() { return quic_stat_names_; }

        Server& server_;
        ListenerComponentFactory& factory_;



    private:
        //using ListenerList = std::list<ListenerImplPtr>;
        /**
         * Callback invoked when a listener initialization is completed on worker.
         */
        using ListenerCompletionCallback = std::function<void()>;

        bool addOrUpdateListenerInternal(const std::string& version_info, bool added_via_api,
                                         const std::string& name);
       // bool removeListenerInternal(const std::string& listener_name, bool dynamic_listeners_only);

/*        struct DrainingListener {
            DrainingListener(ListenerImplPtr&& listener, uint64_t workers_pending_removal)
                    : listener_(std::move(listener)), workers_pending_removal_(workers_pending_removal) {}

            ListenerImplPtr listener_;
            uint64_t workers_pending_removal_;
        };*/

        //bool doFinalPreWorkerListenerInit(ListenerImpl& listener);
        //void addListenerToWorker(Worker& worker, absl::optional<uint64_t> overridden_listener, ListenerImpl& listener, ListenerCompletionCallback completion_callback);

        //ProtobufTypes::MessagePtr dumpListenerConfigs(const Matchers::StringMatcher& name_matcher);
        //static ListenerManagerStats generateStats(Stats::Scope& scope);
/*        static bool hasListenerWithDuplicatedAddress(const ListenerList& list,
                                                     const ListenerImpl& listener);*/
/*        void updateWarmingActiveGauges() {
            // Using set() avoids a multiple modifiers problem during the multiple processes phase of hot
            // restart.
            stats_.total_listeners_warming_.set(warming_listeners_.size());
            stats_.total_listeners_active_.set(active_listeners_.size());
        }*/
/*        bool listenersStopped(const envoy::config::listener::v3::Listener& config) {
            // Currently all listeners in a given direction are stopped because of the way admin
            // drain_listener functionality is implemented. This needs to be revisited, if that changes - if
            // we support drain by listener name,for example.
            return stop_listeners_type_ == StopListenersType::All ||
                   (stop_listeners_type_ == StopListenersType::InboundOnly &&
                    config.traffic_direction() == envoy::config::core::v3::INBOUND);
        }*/
/*        void incListenerCreateFailureStat() {
            // TODO(mattklein123): Now that we no longer create sockets and apply options on workers,
            // this stat is confusingly named and should be more specific, however it's unclear if it's
            // worth changing.
            stats_.listener_create_failure_.inc();
        }*/

        /**
         * Mark a listener for draining. The listener will no longer be considered active but will remain
         * present to allow connection draining.
         * @param listener supplies the listener to drain.
         */
        //void drainListener(ListenerImplPtr&& listener);

        /**
         * Start to draining filter chains that are owned by draining listener but not owned by
         * new_listener. The new listener should have taken over the listener socket and partial of the
         * filter chains from listener. This method is used by in place filter chain update.
         * @param draining_listener supplies the listener to be replaced.
         * @param new_listener supplies the new listener config which is going to replace the draining
         * listener.
         */
        //void drainFilterChains(ListenerImplPtr&& draining_listener, ListenerImpl& new_listener);

        /**
         * Stop a listener. The listener will stop accepting new connections and its socket will be
         * closed.
         * @param listener supplies the listener to stop.
         * @param completion supplies the completion to be called when all workers are stopped accepting
         * new connections. This completion is called on the main thread.
         */
        //void stopListener(Network::ListenerConfig& listener, std::function<void()> completion);

        /**
         * Get a listener by name. This routine is used because listeners have inherent order in static
         * configuration and especially for tests. Thus, we can't use a map.
         * @param listeners supplies the listener list to look in.
         * @param name supplies the name to search for.
         */
        //ListenerList::iterator getListenerByName(ListenerList& listeners, const std::string& name);

        //void setNewOrDrainingSocketFactory(const std::string& name, ListenerImpl& listener);
        //void createListenSocketFactory(ListenerImpl& listener);

        //void maybeCloseSocketsForListener(ListenerImpl& listener);
        //void setupSocketFactoryForListener(ListenerImpl& new_listener,const ListenerImpl& existing_listener);

        //ApiListenerPtr api_listener_;
        // Active listeners are listeners that are currently accepting new connections on the workers.
        //ListenerList active_listeners_;
        // Warming listeners are listeners that may need further initialization via the listener's init
        // manager. For example, RDS, or in the future KDS. Once a listener is done warming it will
        // be transitioned to active.
        //ListenerList warming_listeners_;
        // Draining listeners are listeners that are in the process of being drained and removed. They
        // go through two phases where first the workers stop accepting new connections and existing
        // connections are drained. Then after that time period the listener is removed from all workers
        // and any remaining connections are closed.
        //std::list<DrainingListener> draining_listeners_;
        //std::list<DrainingFilterChainsManager> draining_filter_chains_manager_;

        std::vector<WorkerPtr> workers_;
        bool workers_started_{};
        absl::optional<StopListenersType> stop_listeners_type_;
        //Stats::ScopeSharedPtr scope_;
        //ListenerManagerStats stats_;
        //ConfigTracker::EntryOwnerPtr config_tracker_entry_;
        //LdsApiPtr lds_api_;
        //const bool enable_dispatcher_stats_{};
        //using UpdateFailureState = envoy::admin::v3::UpdateFailureState;
        //absl::flat_hash_map<std::string, std::unique_ptr<UpdateFailureState>> error_state_tracker_;
        //FailureStates overall_error_state_;
        //Quic::QuicStatNames& quic_stat_names_;
    };
/*

    class ListenerFilterChainFactoryBuilder : public FilterChainFactoryBuilder {
    public:
        ListenerFilterChainFactoryBuilder(
                ListenerImpl& listener, Configuration::TransportSocketFactoryContextImpl& factory_context);

        Network::DrainableFilterChainSharedPtr
        buildFilterChain(const envoy::config::listener::v3::FilterChain& filter_chain,
                         FilterChainFactoryContextCreator& context_creator) const override;

    private:
        Network::DrainableFilterChainSharedPtr buildFilterChainInternal(
                const envoy::config::listener::v3::FilterChain& filter_chain,
                Configuration::FilterChainFactoryContextPtr&& filter_chain_factory_context) const;

        ListenerImpl& listener_;
        ProtobufMessage::ValidationVisitor& validator_;
        ListenerComponentFactory& listener_component_factory_;
        Configuration::TransportSocketFactoryContextImpl& factory_context_;
    };
*/

} // namespace Server

#endif //EVENTLOOP_BASE_SERVER_LISTENERMANAGERIMPL_H
