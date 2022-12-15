//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_EVENTIMPLBASE_H
#define EVENTLOOP_BASE_SERVER_EVENTIMPLBASE_H

#include "event2/event_struct.h"

namespace Event {

/**
 * Base class for libevent event implementations. The event struct is embedded inside of this class
 * and derived classes are expected to assign it inside of the constructor.
 */
    class ImplBase {
    protected:
        ~ImplBase();

        event raw_event_;
    };

} // namespace Event
#endif //EVENTLOOP_BASE_SERVER_EVENTIMPLBASE_H
