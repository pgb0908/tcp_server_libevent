//
// Created by bong on 22. 12. 14.
//

#include "EventImplBase.h"
#include "event2/event.h"

EventImplBase::~EventImplBase() {
    // Derived classes are assumed to have already assigned the raw event in the constructor.
    event_del(&raw_event_);
}
