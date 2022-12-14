//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_CONNECTION_H
#define LIBEVENT_TEST1_CONNECTION_H


#include <memory>
#include <chrono>
#include <optional>
#include "Dispatcher.h"

/**
 * Events that occur on a connection.
 */
enum class ConnectionEvent {
    RemoteClose,
    LocalClose,
    Connected,
    ConnectedZeroRtt,
};

/**
 * Connections have both a read and write buffer.
 */
enum class ConnectionBufferType { Read, Write };

/**
 * Network level callbacks that happen on a connection.
 */
class ConnectionCallbacks {
public:
    virtual ~ConnectionCallbacks() = default;

    /**
     * Callback for connection events.
     * @param events supplies the ConnectionEvent that occurred.
     */
    virtual void onEvent(ConnectionEvent event) = 0;

    /**
     * Called when the write buffer for a connection goes over its high watermark.
     */
    virtual void onAboveWriteBufferHighWatermark() = 0;

    /**
     * Called when the write buffer for a connection goes from over its high
     * watermark to under its low watermark.
     */
    virtual void onBelowWriteBufferLowWatermark() = 0;
};

/**
 * Type of connection close to perform.
 */
enum class ConnectionCloseType {
    FlushWrite, // Flush pending write data before raising ConnectionEvent::LocalClose
    NoFlush,    // Do not flush any pending data and immediately raise ConnectionEvent::LocalClose
    FlushWriteAndDelay // Flush pending write data and delay raising a ConnectionEvent::LocalClose
    // until the delayed_close_timeout expires
};

/**
 * An abstract raw connection. Free the connection or call close() to disconnect.
 */
class Connection{
public:
    enum class State { Open, Closing, Closed };

    /**
     * Callback function for when bytes have been sent by a connection.
     * @param bytes_sent supplies the number of bytes written to the connection.
     * @return indicates if callback should be called in the future. If true is returned
     * the callback will be called again in the future. If false is returned, the callback
     * will be removed from callback list.
     */
    using BytesSentCb = std::function<bool(uint64_t bytes_sent)>;


    ~Connection()  = default;

    /**
     * Register callbacks that fire when connection events occur.
     */
    virtual void addConnectionCallbacks(ConnectionCallbacks& cb) = 0;

    /**
     * Unregister callbacks which previously fired when connection events occur.
     */
    virtual void removeConnectionCallbacks(ConnectionCallbacks& cb) = 0;

    /**
     * Register for callback every time bytes are written to the underlying TransportSocket.
     */
    virtual void addBytesSentCallback(BytesSentCb cb) = 0;

    /**
     * Enable half-close semantics on this connection. Reading a remote half-close
     * will not fully close the connection. This is off by default.
     * @param enabled Whether to set half-close semantics as enabled or disabled.
     */
    virtual void enableHalfClose(bool enabled) = 0;

    /**
     * @return true if half-close semantics are enabled, false otherwise.
     */
    virtual bool isHalfCloseEnabled() = 0;

    /**
     * Close the connection.
     */
    virtual void close(ConnectionCloseType type) = 0;

    /**
     * @return Event::Dispatcher& the dispatcher backing this connection.
     */
    virtual Dispatcher& dispatcher() = 0;

    /**
     * @return uint64_t the unique local ID of this connection.
     */
    virtual uint64_t id() const = 0;

    /**
     * @param vector of bytes to which the connection should append hash key data. Any data already in
     * the key vector must not be modified.
     */
    virtual void hashKey(std::vector<uint8_t>& hash) const = 0;

    /**
     * @return std::string the next protocol to use as selected by network level negotiation. (E.g.,
     *         ALPN). If network level negotiation is not supported by the connection or no protocol
     *         has been negotiated the empty string is returned.
     */
    virtual std::string nextProtocol() const = 0;

    /**
     * Enable/Disable TCP NO_DELAY on the connection.
     */
    virtual void noDelay(bool enable) = 0;

    /**
     * Disable socket reads on the connection, applying external back pressure. When reads are
     * enabled again if there is data still in the input buffer it will be re-dispatched through
     * the filter chain.
     * @param disable supplies TRUE is reads should be disabled, FALSE if they should be enabled.
     *
     * Note that this function reference counts calls. For example
     * readDisable(true);  // Disables data
     * readDisable(true);  // Notes the connection is blocked by two sources
     * readDisable(false);  // Notes the connection is blocked by one source
     * readDisable(false);  // Marks the connection as unblocked, so resumes reading.
     */
    virtual void readDisable(bool disable) = 0;

    /**
     * Set if Envoy should detect TCP connection close when readDisable(true) is called.
     * By default, this is true on newly created connections.
     *
     * @param should_detect supplies if disconnects should be detected when the connection has been
     * read disabled
     */
    virtual void detectEarlyCloseWhenReadDisabled(bool should_detect) = 0;

    /**
     * @return bool whether reading is enabled on the connection.
     */
    virtual bool readEnabled() const = 0;

    /**
     * @return the connection info provider backing this connection.
     */
/*    virtual ConnectionInfoSetter& connectionInfoSetter() = 0;
    virtual const ConnectionInfoProvider& connectionInfoProvider() const = 0;
    virtual ConnectionInfoProviderSharedPtr connectionInfoProviderSharedPtr() const = 0;*/

    /**
     * Credentials of the peer of a socket as decided by SO_PEERCRED.
     */
    struct UnixDomainSocketPeerCredentials {
        /**
         * The process id of the peer.
         */
        int32_t pid;
        /**
         * The user id of the peer.
         */
        uint32_t uid;
        /**
         * The group id of the peer.
         */
        uint32_t gid;
    };

    /**
     * @return The unix socket peer credentials of the remote client. Note that this is only
     * supported for unix socket connections.
     */
    virtual std::optional<UnixDomainSocketPeerCredentials> unixSocketPeerCredentials() const = 0;

    /**
     * @return the const SSL connection data if this is an SSL connection, or nullptr if it is not.
     */
    // TODO(snowp): Remove this in favor of StreamInfo::downstreamSslConnection.
    //virtual Ssl::ConnectionInfoConstSharedPtr ssl() const = 0;

    /**
     * @return requested server name (e.g. SNI in TLS), if any.
     */
    virtual std::string requestedServerName() const = 0;

    /**
     * @return State the current state of the connection.
     */
    virtual State state() const = 0;

    /**
     * @return true if the connection has not completed connecting, false if the connection is
     * established.
     */
    virtual bool connecting() const = 0;

    /**
     * Write data to the connection. Will iterate through downstream filters with the buffer if any
     * are installed.
     * @param data Supplies the data to write to the connection.
     * @param end_stream If true, this indicates that this is the last write to the connection. If
     *        end_stream is true, the connection is half-closed. This may only be set to true if
     *        enableHalfClose(true) has been set on this connection.
     */
    //virtual void write(Instance& data, bool end_stream) = 0;

    /**
     * Set a soft limit on the size of buffers for the connection.
     * For the read buffer, this limits the bytes read prior to flushing to further stages in the
     * processing pipeline.
     * For the write buffer, it sets watermarks. When enough data is buffered it triggers a call to
     * onAboveWriteBufferHighWatermark, which allows subscribers to enforce flow control by disabling
     * reads on the socket funneling data to the write buffer. When enough data is drained from the
     * write buffer, onBelowWriteBufferHighWatermark is called which similarly allows subscribers
     * resuming reading.
     */
    virtual void setBufferLimits(uint32_t limit) = 0;

    /**
     * Get the value set with setBufferLimits.
     */
    virtual uint32_t bufferLimit() const = 0;

    /**
     * @return boolean telling if the connection is currently above the high watermark.
     */
    virtual bool aboveHighWatermark() const = 0;

    /**
     * Get the socket options set on this connection.
     */
    //virtual const OptionsSharedPtr& socketOptions() const = 0;

    /**
     * The StreamInfo object associated with this connection. This is typically
     * used for logging purposes. Individual filters may add specific information
     * via the FilterState object within the StreamInfo object. The StreamInfo
     * object in this context is one per connection i.e. different than the one in
     * the http ConnectionManager implementation which is one per request.
     *
     * @return StreamInfo object associated with this connection.
     */
    //virtual StreamInfo& streamInfo() = 0;
    //virtual const StreamInfo::StreamInfo& streamInfo() const = 0;

    /**
     * Set the timeout for delayed connection close()s.
     * This can only be called prior to issuing a close() on the connection.
     * @param timeout The timeout value in milliseconds
     */
    virtual void setDelayedCloseTimeout(std::chrono::milliseconds timeout) = 0;

    /**
     * @return std::string the failure reason of the underlying transport socket, if no failure
     *         occurred an empty string is returned.
     */
    virtual std::string transportFailureReason() const = 0;

    /**
     * Instructs the connection to start using secure transport.
     * Note: Not all underlying transport sockets support such operation.
     * @return boolean telling if underlying transport socket was able to
               start secure transport.
     */
    virtual bool startSecureTransport() = 0;

    /**
     *  @return absl::optional<std::chrono::milliseconds> An optional of the most recent round-trip
     *  time of the connection. If the platform does not support this, then an empty optional is
     *  returned.
     */
    virtual std::optional<std::chrono::milliseconds> lastRoundTripTime() const = 0;

    /**
     * Try to configure the connection's initial congestion window.
     * The operation is advisory - the connection may not support it, even if it's supported, it may
     * not do anything after the first few network round trips with the peer.
     * @param bandwidth_bits_per_sec The estimated bandwidth between the two endpoints of the
     * connection.
     * @param rtt The estimated round trip time between the two endpoints of the connection.
     *
     * @note Envoy does not provide an implementation for TCP connections because
     * there is no portable system api to do so. Applications can implement it
     * with a proprietary api in a customized TransportSocket.
     */
    virtual void configureInitialCongestionWindow(uint64_t bandwidth_bits_per_sec,
                                                  std::chrono::microseconds rtt) = 0;

    /**
     * @return the current congestion window in bytes, or unset if not available or not
     * congestion-controlled.
     * @note some congestion controller's cwnd is measured in number of packets, in that case the
     * return value is cwnd(in packets) times the connection's MSS.
     */
    virtual std::optional<uint64_t> congestionWindowInBytes() const = 0;
};

using ConnectionPtr = std::unique_ptr<Connection>;

/**
 * Connections servicing inbound connects.
 */
class ServerConnection : public virtual Connection {
public:
    /**
     * Set the amount of time allowed for the transport socket to report that a connection is
     * established. The provided timeout is relative to the current time. If this method is called
     * after a connection has already been established, it is a no-op.
     *
     * If a timeout occurs, `timeout_stat` will be incremented.
     */
    virtual void setTransportSocketConnectTimeout(std::chrono::milliseconds timeout) = 0;
};

using ServerConnectionPtr = std::unique_ptr<ServerConnection>;

/**
 * Connections capable of outbound connects.
 */
class ClientConnection : public virtual Connection {
public:
    /**
     * Connect to a remote host. Errors or connection events are reported via the event callback
     * registered via addConnectionCallbacks().
     */
    virtual void connect() = 0;
};

using ClientConnectionPtr = std::unique_ptr<ClientConnection>;

#endif //LIBEVENT_TEST1_CONNECTION_H
