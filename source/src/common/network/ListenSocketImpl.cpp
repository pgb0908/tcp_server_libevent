//
// Created by bong on 22. 12. 15.
//

#include "ListenSocketImpl.h"



namespace Network {

    Api::SysCallIntResult ListenSocketImpl::bind(Network::Address::InstanceConstSharedPtr address) {
        connection_info_provider_->setLocalAddress(address);

        const Api::SysCallIntResult result = SocketImpl::bind(connection_info_provider_->localAddress());
        if (SOCKET_FAILURE(result.return_value_)) {
            close();
   /*         throw SocketBindException(fmt::format("cannot bind '{}': {}",
                                                  connection_info_provider_->localAddress()->asString(),
                                                  errorDetails(result.errno_)),
                                      result.errno_);*/
        }
        return {0, 0};
    }

    void ListenSocketImpl::setListenSocketOptions(const Network::Socket::OptionsSharedPtr& options) {
/*        if (!Network::Socket::applyOptions(options, *this,
                                           envoy::config::core::v3::SocketOption::STATE_PREBIND)) {
          //  throw SocketOptionException("ListenSocket: Setting socket options failed");
        }*/
    }

    void ListenSocketImpl::setupSocket(const Network::Socket::OptionsSharedPtr& options) {
        setListenSocketOptions(options);
        bind(connection_info_provider_->localAddress());
    }

    UdsListenSocket::UdsListenSocket(const Address::InstanceConstSharedPtr& address)
            : ListenSocketImpl(ioHandleForAddr(Socket::Type::Stream, address, {}), address) {
       // RELEASE_ASSERT(io_handle_->isOpen(), "");
        bind(connection_info_provider_->localAddress());
    }

    UdsListenSocket::UdsListenSocket(IoHandlePtr&& io_handle,
                                     const Address::InstanceConstSharedPtr& address)
            : ListenSocketImpl(std::move(io_handle), address) {}

    std::atomic<uint64_t> AcceptedSocketImpl::global_accepted_socket_count_;

} // namespace Network