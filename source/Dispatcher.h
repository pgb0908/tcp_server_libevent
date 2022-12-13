//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_DISPATCHER_H
#define LIBEVENT_TEST1_DISPATCHER_H

#include <functional>
#include "FileEvent.h"
#include "Timer.h"
#include "ScaledTimer.h"
#include "Connection.h"
#include "DispatcherThreadDeletable.h"
#include "Signal.h"
#include "DeferredDeletable.h"

typedef int os_fd_t;
typedef int signal_t;

using PostCb = std::function<void()>;

/**
 * Minimal interface to the dispatching loop used to create low-level primitives. See Dispatcher
 * below for the full interface.
 */
class DispatcherBase {
public:
    virtual ~DispatcherBase() = default;

    /**
     * Posts a functor to the dispatcher. This is safe cross thread. The functor runs in the context
     * of the dispatcher event loop which may be on a different thread than the caller.
     */
    virtual void post(PostCb callback) = 0;

    /**
     * Validates that an operation is thread-safe with respect to this dispatcher; i.e. that the
     * current thread of execution is on the same thread upon which the dispatcher loop is running.
     */
    virtual bool isThreadSafe() const = 0;
};

/**
 * Abstract event dispatching loop.
 */
class Dispatcher : DispatcherBase {
public:
    /**
     * Returns the name that identifies this dispatcher, such as "worker_2" or "main_thread".
     * @return const std::string& the name that identifies this dispatcher.
     */
    virtual const std::string& name() = 0;

    /**
     * Creates a file event that will signal when a file is readable or writable. On UNIX systems this
     * can be used for any file like interface (files, sockets, etc.).
     * @param fd supplies the fd to watch.
     * @param cb supplies the callback to fire when the file is ready.
     * @param trigger specifies whether to edge or level trigger.
     * @param events supplies a logical OR of FileReadyType events that the file event should
     *               initially listen on.
     */
    virtual FileEventPtr createFileEvent(os_fd_t fd, FileReadyCb cb, FileTriggerType trigger,
                                         uint32_t events) = 0;

    /**
     * Allocates a timer. @see Timer for docs on how to use the timer.
     * @param cb supplies the callback to invoke when the timer fires.
     */
    virtual TimerPtr createTimer(TimerCb cb) = 0;

    /**
     * Allocates a scaled timer. @see Timer for docs on how to use the timer.
     * @param timer_type the type of timer to create.
     * @param cb supplies the callback to invoke when the timer fires.
     */
    virtual TimerPtr createScaledTimer(ScaledTimerType timer_type, TimerCb cb) = 0;

    /**
     * Allocates a scaled timer. @see Timer for docs on how to use the timer.
     * @param minimum the rule for computing the minimum value of the timer.
     * @param cb supplies the callback to invoke when the timer fires.
     */
    virtual TimerPtr createScaledTimer(ScaledTimerMinimum minimum, TimerCb cb) = 0;

    /**
     * Allocates a schedulable callback. @see SchedulableCallback for docs on how to use the wrapped
     * callback.
     * @param cb supplies the callback to invoke when the SchedulableCallback is triggered on the
     * event loop.
     */
    virtual SchedulableCallbackPtr createSchedulableCallback(std::function<void()> cb) = 0;

    /**
     * Returns a time-source to use with this dispatcher.
     */
    virtual TimeSource& timeSource() = 0;

    /**
     * Returns a recently cached MonotonicTime value.
     */
    virtual MonotonicTime approximateMonotonicTime() const = 0;

    /**
     * Clears any items in the deferred deletion queue.
     */
    virtual void clearDeferredDeleteList() = 0;

    /**
     * Wraps an already-accepted socket in an instance of Envoy's server Network::Connection.
     * @param socket supplies an open file descriptor and connection metadata to use for the
     *        connection. Takes ownership of the socket.
     * @param transport_socket supplies a transport socket to be used by the connection.
     * @param stream_info info object for the server connection
     * @return Network::ConnectionPtr a server connection that is owned by the caller.
     */
    virtual ServerConnectionPtr
    createServerConnection(ConnectionSocketPtr&& socket,
                           TransportSocketPtr&& transport_socket,
                           StreamInfo& stream_info) = 0;

    /**
     * Creates an instance of Envoy's Network::ClientConnection. Does NOT initiate the connection;
     * the caller must then call connect() on the returned Network::ClientConnection.
     * @param address supplies the address to connect to.
     * @param source_address supplies an address to bind to or nullptr if no bind is necessary.
     * @param transport_socket supplies a transport socket to be used by the connection.
     * @param options the socket options to be set on the underlying socket before anything is sent
     *        on the socket.
     * @param transport socket options used to create the transport socket.
     * @return Network::ClientConnectionPtr a client connection that is owned by the caller.
     */
/*    virtual ClientConnectionPtr createClientConnection(
            InstanceConstSharedPtr address,
            InstanceConstSharedPtr source_address,
            TransportSocketPtr&& transport_socket,
            const OptionsSharedPtr& options,
            const TransportSocketOptionsConstSharedPtr& transport_options) =0;*/


    /**
     * Creates a listener on a specific port.
     * @param socket supplies the socket to listen on.
     * @param cb supplies the callbacks to invoke for listener events.
     * @param runtime supplies the runtime for this server.
     * @param bind_to_port controls whether the listener binds to a transport port or not.
     * @param ignore_global_conn_limit controls whether the listener is limited by the global
     * connection limit.
     * @return Network::ListenerPtr a new listener that is owned by the caller.
     */
    virtual ListenerPtr createListener(SocketSharedPtr&& socket,
                                                TcpListenerCallbacks& cb, bool bind_to_port,
                                                bool ignore_global_conn_limit) = 0;
    /**
      * Creates a logical udp listener on a specific port.
      * @param socket supplies the socket to listen on.
      * @param cb supplies the udp listener callbacks to invoke for listener events.
      * @param config provides the UDP socket configuration.
      * @return Network::ListenerPtr a new listener that is owned by the caller.
      */
/*    virtual Network::UdpListenerPtr
    createUdpListener(Network::SocketSharedPtr socket, Network::UdpListenerCallbacks& cb,
                      const envoy::config::core::v3::UdpSocketConfig& config) = 0;*/
    /**
     * Submits an item for deferred delete. @see DeferredDeletable.
     */
    virtual void deferredDelete(DeferredDeletablePtr&& to_delete) = 0;

    /**
     * Exits the event loop.
     */
    virtual void exit() = 0;

    /**
     * Listens for a signal event. Only a single dispatcher in the process can listen for signals.
     * If more than one dispatcher calls this routine in the process the behavior is undefined.
     *
     * @param signal_num supplies the signal to listen on.
     * @param cb supplies the callback to invoke when the signal fires.
     * @return SignalEventPtr a signal event that is owned by the caller.
     */
    virtual SignalEventPtr listenForSignal(signal_t signal_num, SignalCb cb) = 0;

    /**
     * Post the deletable to this dispatcher. The deletable objects are guaranteed to be destroyed on
     * the dispatcher's thread before dispatcher destroy. This is safe cross thread.
     */
    virtual void deleteInDispatcherThread(DispatcherThreadDeletableConstPtr deletable) = 0;

    /**
     * Runs the event loop. This will not return until exit() is called either from within a callback
     * or from a different thread.
     * @param type specifies whether to run in blocking mode (run() will not return until exit() is
     *              called) or non-blocking mode where only active events will be executed and then
     *              run() will return.
     */
    enum class RunType {
        Block,       // Runs the event-loop until there are no pending events.
        NonBlock,    // Checks for any pending events to activate, executes them,
        // then exits. Exits immediately if there are no pending or
        // active events.
        RunUntilExit // Runs the event-loop until loopExit() is called, blocking
        // until there are pending or active events.
    };
    virtual void run(RunType type) = 0;

    /**
     * Returns a factory which connections may use for watermark buffer creation.
     * @return the watermark buffer factory for this dispatcher.
     */
    //virtual WatermarkFactory& getWatermarkFactory() = 0;

    /**
     * Updates approximate monotonic time to current value.
     */
    virtual void updateApproximateMonotonicTime() = 0;

    /**
     * Shutdown the dispatcher by clear dispatcher thread deletable.
     */
    virtual void shutdown() = 0;
};

using DispatcherPtr = std::unique_ptr<Dispatcher>;

#endif //LIBEVENT_TEST1_DISPATCHER_H
