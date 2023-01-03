//
// Created by bong on 23. 1. 2.
//

#ifndef EVENTLOOP_BASE_SERVER_SCOPETRACEDOBJECTSTACK_H
#define EVENTLOOP_BASE_SERVER_SCOPETRACEDOBJECTSTACK_H

#include <functional>
#include <vector>

#include "include/common/ScopeTrackedObject.h"

// Encapsulates zero or more ScopeTrackedObjects.
//
// This is currently used to restore the underlying request context if a
// filter continues processing a request or response due to being invoked directly from an
// asynchronous callback.
class ScopeTrackedObjectStack : public ScopeTrackedObject {
public:
    ScopeTrackedObjectStack() = default;

    // Not copyable or movable
    ScopeTrackedObjectStack(const ScopeTrackedObjectStack&) = delete;
    ScopeTrackedObjectStack& operator=(const ScopeTrackedObjectStack&) = delete;

    void add(const ScopeTrackedObject& object) { tracked_objects_.push_back(object); }

    void dumpState(std::ostream& os, int indent_level) const override {
        for (auto iter = tracked_objects_.rbegin(); iter != tracked_objects_.rend(); ++iter) {
            iter->get().dumpState(os, indent_level);
        }
    }

private:
    std::vector<std::reference_wrapper<const ScopeTrackedObject>> tracked_objects_;
};


#endif //EVENTLOOP_BASE_SERVER_SCOPETRACEDOBJECTSTACK_H
