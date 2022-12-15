//
// Created by bong on 22. 12. 15.
//

#include "BaseListenerImpl.h"


namespace Network {

    BaseListenerImpl::BaseListenerImpl(Event::DispatcherImpl& dispatcher, SocketSharedPtr socket)
            : local_address_(nullptr), dispatcher_(dispatcher), socket_(std::move(socket)) {
        const auto ip = socket_->connectionInfoProvider().localAddress()->ip();

        // Only use the listen socket's local address for new connections if it is not the all hosts
        // address (e.g., 0.0.0.0 for IPv4).
        if (!(ip && ip->isAnyAddress())) {
            local_address_ = socket_->connectionInfoProvider().localAddress();
        }
    }

} // namespace Network