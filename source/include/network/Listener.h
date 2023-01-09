//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_LISTENER_H
#define LIBEVENT_TEST1_LISTENER_H

#include <cstdint>
#include <memory>
#include <string>

#include "include/api/IoError.h"
#include "include/buffer/Buffer.h"
#include "Address.h"
#include "include/common/Optref.h"
#include "include/common/Time.h"
#include "include/network/Socket.h"
#include "src/common/common/Interval_value.h"

namespace Network {

    class ActiveUdpListenerFactory;
    class UdpListenerWorkerRouter;

    class ListenSocketFactory;
    using ListenSocketFactoryPtr = std::unique_ptr<ListenSocketFactory>;

/**
 * ListenSocketFactory is a member of ListenConfig to provide listen socket.
 * Listeners created from the same ListenConfig instance have listening sockets
 * provided by the same ListenSocketFactory instance.
 */
    class ListenSocketFactory {
    public:
        virtual ~ListenSocketFactory() = default;

        /**
         * Called during actual listener creation.
         * @param worker_index supplies the worker index to get the socket for. All sockets are created
         *        ahead of time.
         * @return the socket to be used for a certain listener, which might be shared
         * with other listeners of the same config on other worker threads.
         */
        virtual SocketSharedPtr getListenSocket(uint32_t worker_index) = 0;

        /**
         * @return the type of the socket getListenSocket() returns.
         */
        virtual Socket::Type socketType() const = 0;

        /**
         * @return the listening address of the socket getListenSocket() returns. Before getListenSocket()
         * is called, the return value might has 0 as port number if the config doesn't specify it.
         */
        virtual const Address::InstanceConstSharedPtr& localAddress() const = 0;

        /**
         * Clone this socket factory so it can be used by a new listener (e.g., if the address is shared).
         */
        virtual ListenSocketFactoryPtr clone() const = 0;

        /**
         * Close all sockets. This is used during draining scenarios.
         */
        virtual void closeAllSockets() = 0;

        /**
         * Perform any initialization that must occur immediately prior to using the listen socket on
         * workers. For example, the actual listen() call, post listen socket options, etc. This is done
         * so that all error handling can occur on the main thread and the gap between performing these
         * actions and using the socket is minimized.
         */
        virtual void doFinalPreWorkerInit() = 0;
    };

/**
 * Configuration for a UDP listener.
 */
    class UdpListenerConfig {
    public:
        virtual ~UdpListenerConfig() = default;

        /**
         * @return factory for creating a listener.
         */
        //virtual ActiveUdpListenerFactory& listenerFactory() = 0;

        /**
         * @return factory for writing to a UDP socket.
         */
        //virtual UdpPacketWriterFactory& packetWriterFactory() = 0;

        /**
         * @param address is used to query the address specific router.
         * @return the UdpListenerWorkerRouter for this listener.
         */
        virtual UdpListenerWorkerRouter&
        listenerWorkerRouter(const Network::Address::Instance& address) = 0;

        /**
         * @return the configuration for the listener.
         */
        //virtual const envoy::config::listener::v3::UdpListenerConfig& config() = 0;
    };

    using UdpListenerConfigOptRef = OptRef<UdpListenerConfig>;

// Forward declare.
    class InternalListenerRegistry;

/**
 * Configuration for an internal listener.
 */
    class InternalListenerConfig {
    public:
        virtual ~InternalListenerConfig() = default;

        /**
         * @return InternalListenerRegistry& The internal registry of this internal listener config. The
         *         registry outlives this listener config.
         */
        virtual InternalListenerRegistry& internalListenerRegistry() = 0;
    };

    using InternalListenerConfigOptRef = OptRef<InternalListenerConfig>;

/**
 * A configuration for an individual listener.
 */
    class ListenerConfig {
    public:
        virtual ~ListenerConfig() = default;

        /**
         * @return FilterChainManager& the factory for adding and searching through configured
         *         filter chains.
         */
        //virtual FilterChainManager& filterChainManager() = 0;

        /**
         * @return FilterChainFactory& the factory for setting up the filter chain on a new
         *         connection.
         */
        //virtual FilterChainFactory& filterChainFactory() = 0;

        /**
         * @return std::vector<ListenSocketFactoryPtr>& the factories to create listen sockets.
         */
        virtual std::vector<ListenSocketFactoryPtr>& listenSocketFactories() = 0;

        /**
         * @return bool specifies whether the listener should actually listen on the port.
         *         A listener that doesn't listen on a port can only receive connections
         *         redirected from other listeners.
         */
        virtual bool bindToPort() const = 0;

        /**
         * @return bool if a connection should be handed off to another Listener after the original
         *         destination address has been restored. 'true' when 'use_original_dst' flag in listener
         *         configuration is set, false otherwise.
         */
        //virtual bool handOffRestoredDestinationConnections() const = 0;

        /**
         * @return uint32_t providing a soft limit on size of the listener's new connection read and
         * write buffers.
         */
        //virtual uint32_t perConnectionBufferLimitBytes() const = 0;

        /**
         * @return std::chrono::milliseconds the time to wait for all listener filters to complete
         *         operation. If the timeout is reached, the accepted socket is closed without a
         *         connection being created unless continueOnListenerFiltersTimeout() returns true.
         *         0 specifies a disabled timeout.
         */
        //virtual std::chrono::milliseconds listenerFiltersTimeout() const = 0;

        /**
         * @return bool whether the listener should try to create a connection when listener filters
         *         time out.
         */
        //virtual bool continueOnListenerFiltersTimeout() const = 0;

        /**
         * @return Stats::Scope& the stats scope to use for all listener specific stats.
         */
        //virtual Stats::Scope& listenerScope() = 0;

        /**
         * @return uint64_t the tag the listener should use for connection handler tracking.
         */
        //virtual uint64_t listenerTag() const = 0;

        /**
         * @return const std::string& the listener's name.
         */
        virtual const std::string& name() const = 0;

        /**
         * @return the UDP configuration for the listener IFF it is a UDP listener.
         */
        //virtual UdpListenerConfigOptRef udpListenerConfig() = 0;

        /**
         * @return the internal configuration for the listener IFF it is an internal listener.
         */
        //virtual InternalListenerConfigOptRef internalListenerConfig() = 0;

        /**
         * @return traffic direction of the listener.
         */
        //virtual envoy::config::core::v3::TrafficDirection direction() const = 0;

        /**
         * @param address is used for query the address specific connection balancer.
         * @return the connection balancer for this listener. All listeners have a connection balancer,
         *         though the implementation may be a NOP balancer.
         */
        //virtual ConnectionBalancer& connectionBalancer(const Network::Address::Instance& address) = 0;

        /**
         * Open connection resources for this listener.
         */
        //virtual ResourceLimit& openConnections() = 0;

        /**
         * @return std::vector<AccessLog::InstanceSharedPtr> access logs emitted by the listener.
         */
        //virtual const std::vector<AccessLog::InstanceSharedPtr>& accessLogs() const = 0;

        /**
         * @return pending connection backlog for TCP listeners.
         */
        //virtual uint32_t tcpBacklogSize() const = 0;

        /**
         * @return init manager of the listener.
         */
        //virtual Init::Manager& initManager() = 0;

        /**
         * @return bool whether the listener should avoid blocking connections based on the globally set
         * limit.
         */
        //virtual bool ignoreGlobalConnLimit() const = 0;
    };

/**
 * Callbacks invoked by a listener.
 */
    class TcpListenerCallbacks {
    public:
        virtual ~TcpListenerCallbacks() = default;

        /**
         * Called when a new connection is accepted.
         * @param socket supplies the socket that is moved into the callee.
         */
        //virtual void onAccept(ConnectionSocketPtr&& socket) = 0;

        enum class RejectCause {
            GlobalCxLimit,
            OverloadAction,
        };
        /**
         * Called when a new connection is rejected.
         */
        //virtual void onReject(RejectCause cause) = 0;
    };

/**
 * Utility struct that encapsulates the information from a udp socket's recvmmsg call.
 */
    struct UdpRecvData {
        struct LocalPeerAddresses {
            bool operator==(const LocalPeerAddresses& rhs) const {
                // TODO(mattklein123): Implement a hash directly on Address that does not use strings.
                return local_->asStringView() == rhs.local_->asStringView() &&
                       peer_->asStringView() == rhs.peer_->asStringView();
            }

            template <typename H> friend H AbslHashValue(H h, const LocalPeerAddresses& addresses) {
                // TODO(mattklein123): Implement a hash directly on Address that does not use strings.
                return H::combine(std::move(h), addresses.local_->asStringView(),
                                  addresses.peer_->asStringView());
            }

            Address::InstanceConstSharedPtr local_;
            Address::InstanceConstSharedPtr peer_;
        };

        LocalPeerAddresses addresses_;
        Buffer::InstancePtr buffer_;
        MonotonicTime receive_time_;
    };

/**
 * Encapsulates the information needed to send a udp packet to a target
 */
    struct UdpSendData {
        const Address::Ip* local_ip_;
        const Address::Instance& peer_address_;

        // The buffer is a reference so that it can be reused by the sender to send different
        // messages
        Buffer::Instance& buffer_;
    };

/**
 * UDP listener callbacks.
 */
    class UdpListenerCallbacks {
    public:
        virtual ~UdpListenerCallbacks() = default;

        /**
         * Called whenever data is received by the underlying udp socket.
         * TODO(danzh2010): Consider returning a value to indicate if more work is to
         * be done in the next event loop due to a limit on how much processing is
         * allowed in each event loop.
         *
         * @param data UdpRecvData from the underlying socket.
         */
        virtual void onData(UdpRecvData&& data) = 0;

        /**
         * Called whenever datagrams are dropped due to overflow or truncation.
         * @param dropped supplies the number of dropped datagrams.
         */
        virtual void onDatagramsDropped(uint32_t dropped) = 0;

        /**
         * Called when the underlying socket is ready for read, before onData() is
         * called. Called only once per event loop, even if followed by multiple
         * onData() calls.
         *
         */
        virtual void onReadReady() = 0;

        /**
         * Called when the underlying socket is ready for write.
         *
         * @param socket Underlying server socket for the listener.
         *
         * TODO(conqerAtapple): Maybe we need a UdpWriter here instead of Socket.
         */
        virtual void onWriteReady(const Socket& socket) = 0;

        /**
         * Called when there is an error event in the receive data path.
         * The send side error is a return type on the send method.
         *
         * @param error_code supplies the received error on the listener.
         */
        virtual void onReceiveError(Api::IoError::IoErrorCode error_code) = 0;

        /**
         * Returns the pointer to the udp_packet_writer associated with the
         * UdpListenerCallback
         */
        //virtual UdpPacketWriter& udpPacketWriter() = 0;

        /**
         * Returns the index of this worker, in the range of [0, concurrency).
         */
        virtual uint32_t workerIndex() const = 0;

        /**
         * Called whenever data is received on the underlying udp socket, on
         * the destination worker for the datagram according to ``destination()``.
         */
        virtual void onDataWorker(Network::UdpRecvData&& data) = 0;

        /**
         * Posts ``data`` to be delivered on this worker.
         */
        virtual void post(Network::UdpRecvData&& data) = 0;

        /**
         * An estimated number of UDP packets this callback expects to process in current read event.
         */
        virtual size_t numPacketsExpectedPerEventLoop() const = 0;
    };

    using UdpListenerCallbacksOptRef = absl::optional<std::reference_wrapper<UdpListenerCallbacks>>;

/**
 * An abstract socket listener. Free the listener to stop listening on the socket.
 */
    class Listener {
    public:
        virtual ~Listener() = default;

        /**
         * Temporarily disable accepting new connections.
         */
        virtual void disable() = 0;

        /**
         * Enable accepting new connections.
         */
        virtual void enable() = 0;

        /**
         * Set the fraction of incoming connections that will be closed immediately
         * after being opened.
         */
        virtual void setRejectFraction(UnitFloat reject_fraction) = 0;
    };

    using ListenerPtr = std::unique_ptr<Listener>;

/**
 * A UDP listener interface.
 */
    class UdpListener : public virtual Listener {
    public:
        ~UdpListener() override = default;

        /**
         * @return Event::Dispatcher& the dispatcher backing this listener.
         */
        virtual Event::Dispatcher& dispatcher() = 0;

        /**
         * @return the local address of the socket.
         */
        virtual const Network::Address::InstanceConstSharedPtr& localAddress() const = 0;

        /**
         * Send data through the underlying udp socket. If the send buffer of the socket FD is full, an
         * error code is returned.
         * TODO(sumukhs): We do not currently handle max MTU size of the datagram. Determine if we could
         * expose the path MTU information to the caller.
         *
         * @param data Supplies the data to send to a target using udp.
         * @return the error code of the underlying send api. On successfully sending 'n' bytes, the
         * underlying buffers in the data  are drained by 'n' bytes. The remaining can be retried by the
         * sender.
         */
        virtual Api::IoCallUint64Result send(const UdpSendData& data) = 0;

        /**
         * Flushes out remaining buffered data since last call of send().
         * This is a no-op if the implementation doesn't buffer data while sending.
         *
         * @return the error code of the underlying flush api.
         */
        virtual Api::IoCallUint64Result flush() = 0;

        /**
         * Make this listener readable at the beginning of the next event loop.
         */
        virtual void activateRead() = 0;
    };

    using UdpListenerPtr = std::unique_ptr<UdpListener>;

/**
 * Handles delivering datagrams to the correct worker.
 */
    class UdpListenerWorkerRouter {
    public:
        virtual ~UdpListenerWorkerRouter() = default;

        /**
         * Registers a worker's callbacks for this listener. This worker must accept
         * packets until it calls ``unregisterWorker``.
         */
        virtual void registerWorkerForListener(UdpListenerCallbacks& listener) = 0;

        /**
         * Unregisters a worker's callbacks for this listener.
         */
        virtual void unregisterWorkerForListener(UdpListenerCallbacks& listener) = 0;

        /**
         * Deliver ``data`` to the correct worker by calling ``onDataWorker()``
         * or ``post()`` on one of the registered workers.
         */
        virtual void deliver(uint32_t dest_worker_index, UdpRecvData&& data) = 0;
    };

    using UdpListenerWorkerRouterPtr = std::unique_ptr<UdpListenerWorkerRouter>;

/**
 * Base class for all listener typed metadata factories.
 */
    //class ListenerTypedMetadataFactory : public Envoy::Config::TypedMetadataFactory {};

} // namespace Network

#endif //LIBEVENT_TEST1_LISTENER_H
