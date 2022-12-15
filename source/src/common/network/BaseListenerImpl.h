//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_BASELISTENERIMPL_H
#define EVENTLOOP_BASE_SERVER_BASELISTENERIMPL_H

#include "include/network/Listener.h"
#include "src/common/event/DispatcherImpl.h"

namespace Network {

/**
 * Base libevent implementation of Network::Listener.
 */
    class BaseListenerImpl : public virtual Listener {
    public:
        /**
         * @param socket the listening socket for this listener. It might be shared
         * with other listeners if all listeners use single listen socket.
         */
        BaseListenerImpl(Event::DispatcherImpl& dispatcher, SocketSharedPtr socket);

    protected:
        Address::InstanceConstSharedPtr local_address_;
        Event::DispatcherImpl& dispatcher_;
        const SocketSharedPtr socket_;
    };

} // namespace Network

#endif //EVENTLOOP_BASE_SERVER_BASELISTENERIMPL_H
