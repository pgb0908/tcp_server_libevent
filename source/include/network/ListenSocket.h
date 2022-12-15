//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_LISTENSOCKET_H
#define LIBEVENT_TEST1_LISTENSOCKET_H

#include <chrono>
#include <memory>
#include <tuple>
#include <vector>

#include "Socket.h"

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace Network {

/**
 * A socket passed to a connection. For server connections this represents the accepted socket, and
 * for client connections this represents the socket being connected to a remote address.
 *
 * TODO(jrajahalme): Hide internals (e.g., fd) from listener filters by providing callbacks filters
 * may need (set/getsockopt(), peek(), recv(), etc.)
 */
    class ConnectionSocket : public virtual Socket, public virtual ScopeTrackedObject {
    public:
        /**
         * Set detected transport protocol (e.g. RAW_BUFFER, TLS).
         */
        virtual void setDetectedTransportProtocol(absl::string_view protocol) = 0;

        /**
         * @return detected transport protocol (e.g. RAW_BUFFER, TLS), if any.
         */
        virtual absl::string_view detectedTransportProtocol() const = 0;

        /**
         * Set requested application protocol(s) (e.g. ALPN in TLS).
         */
        virtual void
        setRequestedApplicationProtocols(const std::vector<absl::string_view>& protocol) = 0;

        /**
         * @return requested application protocol(s) (e.g. ALPN in TLS), if any.
         */
        virtual const std::vector<std::string>& requestedApplicationProtocols() const = 0;

        /**
         * Set requested server name (e.g. SNI in TLS).
         */
        virtual void setRequestedServerName(absl::string_view server_name) = 0;

        /**
         * @return requested server name (e.g. SNI in TLS), if any.
         */
        virtual absl::string_view requestedServerName() const = 0;

        /**
         * @param ja3Hash Connection ja3 fingerprint hash of the downstream connection.
         */
        virtual void setJA3Hash(absl::string_view ja3_hash) = 0;

        /**
         * @return Connection ja3 fingerprint hash of the downstream connection, if any.
         */
        virtual absl::string_view ja3Hash() const = 0;

        /**
         *  @return absl::optional<std::chrono::milliseconds> An optional of the most recent round-trip
         *  time of the connection. If the platform does not support this, then an empty optional is
         *  returned.
         */
        virtual absl::optional<std::chrono::milliseconds> lastRoundTripTime() = 0;

        /**
         * @return the current congestion window in bytes, or unset if not available or not
         * congestion-controlled.
         * @note some congestion controller's cwnd is measured in number of packets, in that case the
         * return value is cwnd(in packets) times the connection's MSS.
         */
        virtual absl::optional<uint64_t> congestionWindowInBytes() const = 0;
    };

    using ConnectionSocketPtr = std::unique_ptr<ConnectionSocket>;

} // namespace Network

#endif //LIBEVENT_TEST1_LISTENSOCKET_H
