//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_ADDRESS_H
#define LIBEVENT_TEST1_ADDRESS_H

#include <sys/types.h>
#include <cstdint>
#include <memory>
#include <string>
#include <arpa/inet.h>

class NetAddrInstance;
using InstanceConstSharedPtr = std::shared_ptr<const NetAddrInstance>;

/**
 * Interface for an Ipv4 address.
 */
class Ipv4 {
public:
    virtual ~Ipv4() = default;

    /**
     * @return the 32-bit IPv4 address in network byte order.
     */
    virtual uint32_t address() const = 0 ;
};

/**
 * Interface for an Ipv6 address.
 */
class Ipv6 {
public:
    virtual ~Ipv6() = default;

    /**
     * @return the absl::uint128 IPv6 address in network byte order.
     */
    //virtual absl::uint128 address() const = 0 ;

    /**
     * @return the uint32_t scope/zone identifier of the IPv6 address.
     */
    virtual uint32_t scopeId() const = 0 ;

    /**
     * @return true if address is Ipv6 and Ipv4 compatibility is disabled, false otherwise
     */
    virtual bool v6only() const = 0 ;

    /**
     * @return Ipv4 address from Ipv4-compatible Ipv6 address. Return `nullptr`
     * if the Ipv6 address isn't Ipv4 mapped.
     */
    virtual InstanceConstSharedPtr v4CompatibleAddress() const = 0 ;
};

enum class IpVersion { v4, v6 }; // NOLINT(readability-identifier-naming)

/**
 * Interface for a generic IP address.
 */
class Ip {
public:
    virtual ~Ip() = default;

    /**
     * @return the address as a string. E.g., "1.2.3.4" for an IPv4 address.
     */
    virtual const std::string& addressAsString() const = 0 ;

    /**
     * @return whether this address is wild card, i.e., '0.0.0.0'.
     */
    virtual bool isAnyAddress() const = 0 ;

    /**
     * @return whether this address is a valid unicast address, i.e., not an wild card, broadcast, or
     * multicast address.
     */
    virtual bool isUnicastAddress() const = 0 ;

    /**
     * @return Ipv4 address data IFF version() == IpVersion::v4, otherwise nullptr.
     */
    virtual const Ipv4* ipv4() const = 0 ;

    /**
     * @return Ipv6 address data IFF version() == IpVersion::v6, otherwise nullptr.
     */
    virtual const Ipv6* ipv6() const = 0 ;

    /**
     * @return the port associated with the address. Port may be zero if not specified, not
     * determinable before socket creation, or not applicable.
     * The port is in host byte order.
     */
    virtual uint32_t port() const = 0 ;

    /**
     * @return the version of IP address.
     */
    virtual IpVersion version() const = 0 ;
};

/**
 * Interface for a generic Pipe address.
 */
class Pipe {
public:
    virtual ~Pipe() = default;
    /**
     * @return abstract namespace flag.
     */
    virtual bool abstractNamespace() const = 0 ;

    /**
     * @return pipe mode.
     */
    virtual mode_t mode() const = 0 ;
};

/**
 * Interface for a generic internal address.
 */
class EnvoyInternalAddress {
public:
    virtual ~EnvoyInternalAddress() = default;

    /**
     * @return The unique id of the internal address. If the address represents the destination
     * internal listener, the address id is that listener name.
     */
    virtual const std::string& addressId() const = 0 ;

    /**
     * @return The optional endpoint id of the internal address.
     */
    virtual const std::string& endpointId() const = 0 ;
};

enum class Type { Ip, Pipe, EnvoyInternal };

/**
 * Interface for all network addresses.
 */
class NetAddrInstance {
public:
    virtual ~NetAddrInstance() = default;

    virtual bool operator==(const NetAddrInstance& rhs) const = 0 ;
    bool operator!=(const NetAddrInstance& rhs) const { return !operator==(rhs); }

    /**
     * @return a human readable string for the address that represents the
     * physical/resolved address. (This will not necessarily include port
     * information, if applicable, since that may not be resolved until bind()).
     *
     * This string will be compatible with the following example formats:
     * For IPv4 addresses: "1.2.3.4:80"
     * For IPv6 addresses: "[1234:5678::9]:443"
     * For pipe addresses: "/foo"
     */
    virtual const std::string& asString() const = 0 ;

    /**
     * @return Similar to asString but returns a string view.
     */
    virtual std::string asStringView() const = 0 ;

    /**
     * @return a human readable string for the address that represents the
     * logical/unresolved name.
     *
     * This string has a source-dependent format and should preserve the original
     * name for Address::Instances resolved by a Network::Address::Resolver.
     */
    virtual const std::string& logicalName() const = 0 ;

    /**
     * @return the IP address information IFF type() == Type::Ip, otherwise nullptr.
     */
    virtual const Ip* ip() const = 0 ;

    /**
     * @return the pipe address information IFF type() == Type::Pipe, otherwise nullptr.
     */
    virtual const Pipe* pipe() const = 0 ;

    /**
     * @return the envoy internal address information IFF type() ==
     * Type::EnvoyInternal, otherwise nullptr.
     */
    virtual const EnvoyInternalAddress* envoyInternalAddress() const = 0 ;

    /**
     * @return the underlying structure wherein the address is stored. Return nullptr if the address
     * type is internal address.
     */
    virtual const sockaddr* sockAddr() const = 0 ;

    /**
     * @return length of the address container.
     */
    virtual socklen_t sockAddrLen() const = 0 ;

    /**
     * @return the type of address.
     */
    virtual Type type() const = 0 ;

    /**
     * Return the address type in string_view. The returned type name is used to find the
     * ClientConnectionFactory.
     */
    virtual std::string addressType() const = 0 ;

    /**
     * @return SocketInterface to be used with the address.
     */
    //virtual const SocketInterface& socketInterface() const = 0 ;
};

#endif //LIBEVENT_TEST1_ADDRESS_H
