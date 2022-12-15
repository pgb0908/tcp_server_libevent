//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_TCPLISTENERIMPL_H
#define EVENTLOOP_BASE_SERVER_TCPLISTENERIMPL_H


#include "BaseListenerImpl.h"

namespace Network {

/**
 * libevent implementation of Network::Listener for TCP.
 */
    class TcpListenerImpl : public BaseListenerImpl {
    public:
        TcpListenerImpl(Event::DispatcherImpl& dispatcher, Random::RandomGenerator& random,
                        Runtime::Loader& runtime, SocketSharedPtr socket, TcpListenerCallbacks& cb,
                        bool bind_to_port, bool ignore_global_conn_limit);
        ~TcpListenerImpl() override {
            if (bind_to_port_) {
                socket_->ioHandle().resetFileEvents();
            }
        }
        void disable() override;
        void enable() override;
        void setRejectFraction(UnitFloat reject_fraction) override;

        static const absl::string_view GlobalMaxCxRuntimeKey;

    protected:
        TcpListenerCallbacks& cb_;

    private:
        void onSocketEvent(short flags);

        // Returns true if global connection limit has been reached and the accepted socket should be
        // rejected/closed. If the accepted socket is to be admitted, false is returned.
        bool rejectCxOverGlobalLimit() const;

        Random::RandomGenerator& random_;
        Runtime::Loader& runtime_;
        bool bind_to_port_;
        UnitFloat reject_fraction_;
        const bool ignore_global_conn_limit_;
    };

} // namespace Network

#endif //EVENTLOOP_BASE_SERVER_TCPLISTENERIMPL_H
