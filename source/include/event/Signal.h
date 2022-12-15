//
// Created by bong on 22. 12. 15.
//

#ifndef LIBEVENT_TEST1_SIGNAL_H
#define LIBEVENT_TEST1_SIGNAL_H


#include <functional>
#include <memory>

namespace Event {

/**
 * Callback invoked when a signal event fires.
 */
    using SignalCb = std::function<void()>;

/**
 * An abstract signal event. Free the event to stop listening on the signal.
 */
    class SignalEvent {
    public:
        virtual ~SignalEvent() = default;
    };

    using SignalEventPtr = std::unique_ptr<SignalEvent>;

} // namespace Event

#endif //LIBEVENT_TEST1_SIGNAL_H
