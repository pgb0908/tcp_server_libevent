//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_EVENTIMPLBASE_H
#define LIBEVENT_TEST1_EVENTIMPLBASE_H

#include "event2/event_struct.h"

/**
 * Base class for libevent event implementations. The event struct is embedded inside of this class
 * and derived classes are expected to assign it inside of the constructor.
 */
class EventImplBase {
protected:
    ~EventImplBase();

    event raw_event_;
};


#endif //LIBEVENT_TEST1_EVENTIMPLBASE_H
