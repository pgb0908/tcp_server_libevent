//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_SOCKET_H
#define LIBEVENT_TEST1_SOCKET_H

#include <memory>
#include <optional>
#include <vector>
#include "Address.h"

// SocketOptionName is an optional value that captures the setsockopt(2)
// arguments. The idea here is that if a socket option is not supported
// on a platform, we can make this the empty value, which allows us to
// avoid #ifdef proliferation.
struct SocketOptionName {
    SocketOptionName() = default;
    SocketOptionName(int level, int option, const std::string& name)
            : value_(std::make_tuple(level, option, name)) {}

    int level() const { return std::get<0>(value_.value()); }
    int option() const { return std::get<1>(value_.value()); }
    const std::string& name() const { return std::get<2>(value_.value()); }

    bool hasValue() const { return value_.has_value(); }
    bool operator==(const SocketOptionName& rhs) const { return value_ == rhs.value_; }

private:
    std::optional<std::tuple<int, int, std::string>> value_;
};

// ENVOY_MAKE_SOCKET_OPTION_NAME is a helper macro to generate a
// SocketOptionName with a descriptive string name.
#define ENVOY_MAKE_SOCKET_OPTION_NAME(level, option)                                               \
  Network::SocketOptionName(level, option, #level "/" #option)

/**
 * Interfaces for providing a socket's various addresses. This is split into a getters interface
 * and a getters + setters interface. This is so that only the getters portion can be overridden
 * in certain cases.
 */
class ConnectionInfoProvider {
public:
    virtual ~ConnectionInfoProvider() = default;

    /**
     * @return the local address of the socket.
     */
    virtual const InstanceConstSharedPtr& localAddress() const = 0 ;

    /**
     * @return true if the local address has been restored to a value that is different from the
     *         address the socket was initially accepted at.
     */
    virtual bool localAddressRestored() const = 0 ;

    /**
     * @return the remote address of the socket.
     */
    virtual const InstanceConstSharedPtr& remoteAddress() const = 0 ;

    /**
     * @return the direct remote address of the socket. This is the address of the directly
     *         connected peer, and cannot be modified by listener filters.
     */
    virtual const InstanceConstSharedPtr& directRemoteAddress() const = 0 ;

    /**
     * @return SNI value for downstream host.
     */
    virtual std::string requestedServerName() const = 0 ;

    /**
     * @return Connection ID of the downstream connection, or unset if not available.
     **/
    virtual std::optional<uint64_t> connectionID() const = 0 ;

    /**
     * @return the name of the network interface used by local end of the connection, or unset if not
     *available.
     **/
    virtual std::optional<std::string> interfaceName() const = 0 ;

    /**
     * Dumps the state of the ConnectionInfoProvider to the given ostream.
     *
     * @param os the std::ostream to dump to.
     * @param indent_level the level of indentation.
     */
    virtual void dumpState(std::ostream& os, int indent_level) const = 0 ;

    /**
     * @return the downstream SSL connection. This will be nullptr if the downstream
     * connection does not use SSL.
     */
    //virtual Ssl::ConnectionInfoConstSharedPtr sslConnection() const = 0 ;

    /**
     * @return ja3 fingerprint hash of the downstream connection, if any.
     */
    virtual std::string ja3Hash() const = 0 ;
};

class ConnectionInfoSetter : public ConnectionInfoProvider {
public:
    /**
     * Set the local address of the socket. On accepted sockets the local address defaults to the
     * one at which the connection was received at, which is the same as the listener's address, if
     * the listener is bound to a specific address.
     *
     * @param local_address the new local address.
     */
    virtual void setLocalAddress(const InstanceConstSharedPtr& local_address) = 0 ;

    /**
     * Restores the local address of the socket. On accepted sockets the local address defaults to the
     * one at which the connection was received at, which is the same as the listener's address, if
     * the listener is bound to a specific address. Call this to restore the address to a value
     * different from the one the socket was initially accepted at. This should only be called when
     * restoring the original destination address of a connection redirected by iptables REDIRECT. The
     * caller is responsible for making sure the new address is actually different.
     *
     * @param local_address the new local address.
     */
    virtual void restoreLocalAddress(const InstanceConstSharedPtr& local_address) = 0 ;

    /**
     * Set the remote address of the socket.
     */
    virtual void setRemoteAddress(const InstanceConstSharedPtr& remote_address) = 0 ;

    /**
     * @param SNI value requested.
     */
    virtual void setRequestedServerName(const std::string requested_server_name) = 0 ;

    /**
     * @param id Connection ID of the downstream connection.
     **/
    virtual void setConnectionID(uint64_t id) = 0 ;

    /**
     * @param enable whether to enable or disable setting interface name. While having an interface
     *               name might be helpful for debugging, it might come at a performance cost.
     */
    virtual void enableSettingInterfaceName(const bool enable) = 0 ;

    /**
     * @param interface_name the name of the network interface used by the local end of the
     *connection.
     **/
    //virtual void maybeSetInterfaceName(IoHandle& io_handle) = 0 ;

    /**
     * @param connection_info sets the downstream ssl connection.
     */
    //virtual void setSslConnection(const Ssl::ConnectionInfoConstSharedPtr& ssl_connection_info) = 0 ;

    /**
     * @param JA3 fingerprint.
     */
    virtual void setJA3Hash(const std::string ja3_hash) = 0 ;
};

using ConnectionInfoSetterSharedPtr = std::shared_ptr<ConnectionInfoSetter>;
using ConnectionInfoProviderSharedPtr = std::shared_ptr<const ConnectionInfoProvider>;

/**
 * Base class for Sockets
 */
class Socket {
public:
    virtual ~Socket() = default;

    /**
     * Type of sockets supported. See man 2 socket for more details
     */
    enum class Type { Stream, Datagram };

    /**
     * @return the connection info provider backing this socket.
     */
    virtual ConnectionInfoSetter& connectionInfoProvider() = 0 ;
    virtual const ConnectionInfoProvider& connectionInfoProvider() const = 0 ;
    virtual ConnectionInfoProviderSharedPtr connectionInfoProviderSharedPtr() const = 0 ;

    /**
     * @return IoHandle for the underlying connection
     */
    virtual IoHandle& ioHandle() = 0 ;

    /**
     * @return const IoHandle for the underlying connection
     */
    virtual const IoHandle& ioHandle() const = 0 ;

    /**
     * Used to duplicate the underlying file descriptor of the socket.
     * @return a pointer to the new Socket.
     */
    virtual std::unique_ptr<Socket> duplicate() = 0 ;

    /**
     * @return the type (stream or datagram) of the socket.
     */
    virtual Socket::Type socketType() const = 0 ;

    /**
     * @return the type (IP or pipe) of addresses used by the socket (subset of socket domain)
     */
    virtual Type addressType() const = 0 ;

    /**
     * @return the IP version used by the socket if address type is IP, absl::nullopt otherwise
     */
    virtual std::optional<IpVersion> ipVersion() const = 0 ;

    /**
     * Close the underlying socket.
     */
    virtual void close() = 0 ;

    /**
     * Return true if close() hasn't been called.
     */
    virtual bool isOpen() const = 0 ;

    /**
     * Bind a socket to this address. The socket should have been created with a call to socket()
     * @param address address to bind the socket to.
     * @return a Api::SysCallIntResult with rc_ = 0 for success and rc_ = -1 for failure. If the call
     *   is successful, errno_ shouldn't be used.
     */
    virtual SysCallIntResult bind(const InstanceConstSharedPtr address) = 0 ;

    /**
     * Listen on bound socket.
     * @param backlog maximum number of pending connections for listener
     * @return a Api::SysCallIntResult with rc_ = 0 for success and rc_ = -1 for failure. If the call
     *   is successful, errno_ shouldn't be used.
     */
    virtual SysCallIntResult listen(int backlog) = 0 ;

    /**
     * Connect a socket to this address. The socket should have been created with a call to socket()
     * on this object.
     * @param address remote address to connect to.
     * @return a Api::SysCallIntResult with rc_ = 0 for success and rc_ = -1 for failure. If the call
     *   is successful, errno_ shouldn't be used.
     */
    virtual SysCallIntResult connect(const InstanceConstSharedPtr address) = 0 ;

    /**
     * @see MSDN WSAIoctl. Controls the mode of a socket.
     */
    virtual SysCallIntResult ioctl(unsigned long control_code, void* in_buffer,
                                        unsigned long in_buffer_len, void* out_buffer,
                                        unsigned long out_buffer_len,
                                        unsigned long* bytes_returned) = 0 ;

    /**
     * Propagates option to underlying socket (@see man 2 setsockopt)
     */
    virtual SysCallIntResult setSocketOption(int level, int optname, const void* optval,
                                                  socklen_t optlen) = 0 ;

    /**
     * Retrieves option from underlying socket (@see man 2 getsockopt)
     */
    virtual SysCallIntResult getSocketOption(int level, int optname, void* optval,
                                                  socklen_t* optlen) const = 0 ;

    /**
     * Toggle socket blocking state
     */
    virtual SysCallIntResult setBlockingForTest(bool blocking) = 0 ;

    /**
     * Visitor class for setting socket options.
     */
    class Option {
    public:
        virtual ~Option() = default;

        /**
         * @param socket the socket on which to apply options.
         * @param state the current state of the socket. Significant for options that can only be
         *        set for some particular state of the socket.
         * @return true if succeeded, false otherwise.
         */
       // virtual bool setOption(Socket& socket, envoy::config::core::v3::SocketOption::SocketState state) const = 0 ;

        /**
         * @param vector of bytes to which the option should append hash key data that will be used
         *        to separate connections based on the option. Any data already in the key vector must
         *        not be modified.
         */
        virtual void hashKey(std::vector<uint8_t>& key) const = 0 ;

        /**
         * Contains details about what this option applies to a socket.
         */
        struct Details {
            SocketOptionName name_;
            std::string value_; ///< Binary string representation of an option's value.

            bool operator==(const Details& other) const {
                return name_ == other.name_ && value_ == other.value_;
            }
        };

        /**
         * @param socket The socket for which we want to know the options that would be applied.
         * @param state The state at which we would apply the options.
         * @return What we would apply to the socket at the provided state. Empty if we'd apply nothing.
         */
        //virtual std::optional<Details> getOptionDetails(const Socket& socket,envoy::config::core::v3::SocketOption::SocketState state) const = 0 ;

        /**
         * Whether the socket implementation is supported. Real implementations should typically return
         * true. Placeholder implementations may indicate such by returning false. Note this does NOT
         * inherently prevent an option from being applied if it's passed to socket/connection
         * interfaces.
         * @return Whether this is a supported socket option.
         */
        virtual bool isSupported() const = 0 ;
    };

    using OptionConstPtr = std::unique_ptr<const Option>;
    using OptionConstSharedPtr = std::shared_ptr<const Option>;
    using Options = std::vector<OptionConstSharedPtr>;
    using OptionsSharedPtr = std::shared_ptr<Options>;

    static OptionsSharedPtr& appendOptions(OptionsSharedPtr& to, const OptionsSharedPtr& from) {
        to->insert(to->end(), from->begin(), from->end());
        return to;
    }

/*    static bool applyOptions(const OptionsSharedPtr& options, Socket& socket, envoy::config::core::v3::SocketOption::SocketState state) {
        if (options == nullptr) {
            return true;
        }
        for (const auto& option : *options) {
            if (!option->setOption(socket, state)) {
                return false;
            }
        }
        return true;
    }*/

    /**
     * Add a socket option visitor for later retrieval with options().
     */
    virtual void addOption(const OptionConstSharedPtr&) = 0 ;

    /**
     * Add socket option visitors for later retrieval with options().
     */
    virtual void addOptions(const OptionsSharedPtr&) = 0 ;

    /**
     * @return the socket options stored earlier with addOption() and addOptions() calls, if any.
     */
    virtual const OptionsSharedPtr& options() const = 0 ;
};

using SocketPtr = std::unique_ptr<Socket>;
using SocketSharedPtr = std::shared_ptr<Socket>;
using SocketOptRef = std::optional<std::reference_wrapper<Socket>>;


#endif //LIBEVENT_TEST1_SOCKET_H
