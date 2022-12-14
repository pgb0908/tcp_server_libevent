//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_CONNECTIONHANDLERIMPL_H
#define LIBEVENT_TEST1_CONNECTIONHANDLERIMPL_H


#include <functional>
#include <atomic>
#include <cassert>
#include "ActiveTcpListener.h"
#include "ConnectionHandler.h"

class ConnectionHandlerImpl : public TcpConnectionHandler {
public:
    using ActiveTcpListenerOptRef = std::optional<std::reference_wrapper<ActiveTcpListener>>;

    ConnectionHandlerImpl(Dispatcher& dispatcher, std::optional<uint32_t> worker_index);

    uint64_t numConnections() const override {
        return num_handler_connections_;
    }

    void incNumConnections() override {
        ++num_handler_connections_;
    };

    void decNumConnections() override {
        assert(num_handler_connections_ > 0);
        --num_handler_connections_;
    };

    void addListener(std::optional<uint64_t> overridden_listener) override;

    void removeListeners(uint64_t listener_tag) override;

    void stopListeners(uint64_t listener_tag) override;

    void stopListeners() override;

    void disableListeners() override;

    void enableListeners() override;

    Dispatcher &dispatcher() override;

    BalancedConnectionHandlerOptRef
    getBalancedHandlerByTag(uint64_t listener_tag, const NetAddrInstance &address) override;

    BalancedConnectionHandlerOptRef getBalancedHandlerByAddress(const NetAddrInstance &address) override;

private:
/*    struct PerAddressActiveListenerDetails {
        // Strong pointer to the listener, whether TCP, UDP, QUIC, etc.
        ConnectionHandler::ActiveListenerPtr listener_;
        InstanceConstSharedPtr address_;
        uint64_t listener_tag_;

        std::variant<std::reference_wrapper<ActiveTcpListener>,
        std::reference_wrapper<InternalListener>>
                typed_listener_;

        // Helpers for accessing the data in the variant for cleaner code.
        ActiveTcpListenerOptRef tcpListener();
        //UdpListenerCallbacksOptRef udpListener();
        //InternalListenerOptRef internalListener();
    };

    struct ActiveListenerDetails {
        std::vector<std::shared_ptr<PerAddressActiveListenerDetails>> per_address_details_list_;

        using ListenerMethodFn = std::function<void(ConnectionHandler::ActiveListener&)>;

        *//**
         * A helper to execute specific method on each PerAddressActiveListenerDetails item.
         *//*
        void invokeListenerMethod(ListenerMethodFn fn) {
            std::for_each(per_address_details_list_.begin(), per_address_details_list_.end(),
                          [&fn](std::shared_ptr<PerAddressActiveListenerDetails>& details) {
                              fn(*details->listener_);
                          });
        }

        *//**
         * Add an ActiveListener into the list.
         *//*
        template <class ActiveListener>
        void addActiveListener(const InstanceConstSharedPtr& address,
                               UnitFloat& listener_reject_fraction, bool disable_listeners,
                               ActiveListener&& listener) {
            auto per_address_details = std::make_shared<PerAddressActiveListenerDetails>();
            per_address_details->typed_listener_ = *listener;
            per_address_details->listener_ = std::move(listener);
            per_address_details->address_ = address;
            if (disable_listeners) {
                per_address_details->listener_->pauseListening();
            }
            if (auto* listener = per_address_details->listener_->listener(); listener != nullptr) {
                listener->setRejectFraction(listener_reject_fraction);
            }
            per_address_details->listener_tag_ = config.listenerTag();
            per_address_details_list_.emplace_back(per_address_details);
        }
    };*/

/*    using ActiveListenerDetailsOptRef = std::optional<std::reference_wrapper<ActiveListenerDetails>>;
    ActiveListenerDetailsOptRef findActiveListenerByTag(uint64_t listener_tag);

    using PerAddressActiveListenerDetailsOptRef =
            std::optional<std::reference_wrapper<PerAddressActiveListenerDetails>>;
    PerAddressActiveListenerDetailsOptRef
    findPerAddressActiveListenerDetails(const ActiveListenerDetailsOptRef active_listener_details,
                                        const NetAddrInstance& address);*/



    // This has a value on worker threads, and no value on the main thread.
    const std::optional<uint32_t> worker_index_;
    Dispatcher& dispatcher_;
    const std::string per_handler_stat_prefix_;
    // Declare before its users ActiveListenerDetails.
    std::atomic<uint64_t> num_handler_connections_{};

    bool disable_listeners_;
    UnitFloat listener_reject_fraction_{UnitFloat::min()};
};


#endif //LIBEVENT_TEST1_CONNECTIONHANDLERIMPL_H
