//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_CONNECTIONBALANCER_H
#define LIBEVENT_TEST1_CONNECTIONBALANCER_H


#include <memory>
#include <optional>
#include "ListenSocket.h"

/**
 * A connection handler that is balanced. Typically implemented by individual listeners depending
 * on their balancing configuration.
 */
class BalancedConnectionHandler {
public:
    virtual ~BalancedConnectionHandler() = default;

    /**
     * @return the number of active connections within the handler.
     */
    virtual uint64_t numConnections() const = 0;

    /**
     * Increment the number of connections within the handler. This must be called by a connection
     * balancer implementation prior to a connection being picked via pickTargetHandler(). This makes
     * sure that connection counts are accurate during connection transfer (i.e., that the target
     * balancer accounts for the incoming connection). This is done by the balancer vs. the
     * connection handler to account for different locking needs inside the balancer.
     */
    virtual void incNumConnections() = 0;

    /**
     * Post a connected socket to this connection handler. This is used for cross-thread connection
     * transfer during the balancing process.
     */
    virtual void post(ConnectionSocketPtr&& socket) = 0;

    virtual void onAcceptWorker(ConnectionSocketPtr&& socket,
                                bool hand_off_restored_destination_connections, bool rebalanced) = 0;
};

/**
 * An implementation of a connection balancer. This abstracts the underlying policy (e.g., exact,
 * fuzzy, etc.).
 */
class ConnectionBalancer {
public:
    virtual ~ConnectionBalancer() = default;

    /**
     * Register a new handler with the balancer that is available for balancing.
     */
    virtual void registerHandler(BalancedConnectionHandler& handler) = 0;

    /**
     * Unregister a handler with the balancer that is no longer available for balancing.
     */
    virtual void unregisterHandler(BalancedConnectionHandler& handler) = 0;

    /**
     * Pick a target handler to send a connection to.
     * @param current_handler supplies the currently executing connection handler.
     * @return current_handler if the connection should stay bound to the current handler, or a
     *         different handler if the connection should be rebalanced.
     *
     * NOTE: It is the responsibility of the balancer to call incNumConnections() on the returned
     *       balancer. See the comments above for more explanation.
     */
    virtual BalancedConnectionHandler&
    pickTargetHandler(BalancedConnectionHandler& current_handler) = 0;
};

using ConnectionBalancerSharedPtr = std::shared_ptr<ConnectionBalancer>;
using BalancedConnectionHandlerOptRef =
        std::optional<std::reference_wrapper<BalancedConnectionHandler>>;


#endif //LIBEVENT_TEST1_CONNECTIONBALANCER_H
