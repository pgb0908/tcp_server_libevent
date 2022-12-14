//
// Created by bong on 22. 12. 14.
//

#include "ConnectionHandlerImpl.h"

void ConnectionHandlerImpl::removeListeners(uint64_t listener_tag) {

}

void ConnectionHandlerImpl::stopListeners(uint64_t listener_tag) {

}

void ConnectionHandlerImpl::stopListeners() {

}

void ConnectionHandlerImpl::disableListeners() {

}

void ConnectionHandlerImpl::enableListeners() {

}



Dispatcher &ConnectionHandlerImpl::dispatcher() {
    return dispatcher_;
}

BalancedConnectionHandlerOptRef
ConnectionHandlerImpl::getBalancedHandlerByTag(uint64_t listener_tag, const NetAddrInstance &address) {
    auto active_listener = findPerAddressActiveListenerDetails(findActiveListenerByTag(listener_tag), address);
    if (active_listener.has_value()) {
        // If the tag matches this must be a TCP listener.
        assert(active_listener->get().tcpListener().has_value());
        return active_listener->get().tcpListener().value().get();
    }
    return std::nullopt;
}

BalancedConnectionHandlerOptRef ConnectionHandlerImpl::getBalancedHandlerByAddress(const NetAddrInstance &address) {
    // Only Ip address can be restored to original address and redirect.
    //assert(address.type() == Network::Address::Type::Ip);

    // We do not return stopped listeners.
    // If there is exact address match, return the corresponding listener.
    if (auto listener_it = tcp_listener_map_by_address_.find(address.asStringView());
            listener_it != tcp_listener_map_by_address_.end() &&
            listener_it->second->listener_->listener() != nullptr) {
        return BalancedConnectionHandlerOptRef(listener_it->second->tcpListener().value().get());
    }

    OptRef<ConnectionHandlerImpl::PerAddressActiveListenerDetails> details;
    // Otherwise, we need to look for the wild card match, i.e., 0.0.0.0:[address_port].
    // We do not return stopped listeners.
    // TODO(wattli): consolidate with previous search for more efficiency.

    std::string addr_str = address.ip()->version() == Network::Address::IpVersion::v4
                           ? Network::Address::Ipv4Instance(address.ip()->port()).asString()
                           : Network::Address::Ipv6Instance(address.ip()->port()).asString();

    auto iter = tcp_listener_map_by_address_.find(addr_str);
    if (iter != tcp_listener_map_by_address_.end() &&
        iter->second->listener_->listener() != nullptr) {
        details = *iter->second;
    }

    return (details.has_value())
           ? BalancedConnectionHandlerOptRef(
                    ActiveTcpListenerOptRef(std::get<std::reference_wrapper<ActiveTcpListener>>(
                            details->typed_listener_))
                            .value()
                            .get())
           : std::nullopt;

    return std::nullopt;
}

ConnectionHandlerImpl::ConnectionHandlerImpl(Dispatcher &dispatcher, absl::optional<uint32_t> worker_index) :
        worker_index_(worker_index), dispatcher_(dispatcher),disable_listeners_(false){

}

void ConnectionHandlerImpl::addListener(absl::optional<uint64_t> overridden_listener, ListenerConfig &config,
                                        Loader &runtime) {

}


ConnectionHandlerImpl::ActiveTcpListenerOptRef ConnectionHandlerImpl::PerAddressActiveListenerDetails::tcpListener() {
    auto* val = absl::get_if<std::reference_wrapper<ActiveTcpListener>>(&typed_listener_);
    return (val != nullptr) ? absl::make_optional(*val) : absl::nullopt;
}

UdpListenerCallbacksOptRef ConnectionHandlerImpl::PerAddressActiveListenerDetails::udpListener() {
    auto* val = absl::get_if<std::reference_wrapper<UdpListenerCallbacks>>(&typed_listener_);
    return (val != nullptr) ? absl::make_optional(*val) : absl::nullopt;
}

InternalListenerOptRef ConnectionHandlerImpl::PerAddressActiveListenerDetails::internalListener() {
    auto* val = absl::get_if<std::reference_wrapper<InternalListener>>(&typed_listener_);
    return (val != nullptr) ? makeOptRef(val->get()) : absl::nullopt;
}
