//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_DEFERREDDELETABLE_H
#define LIBEVENT_TEST1_DEFERREDDELETABLE_H

#include <memory>

/**
 * If an object derives from this class, it can be passed to the dispatcher who guarantees to delete
 * it in a future event loop cycle. This allows clear ownership with unique_ptr while not having
 * to worry about stack unwind issues during event processing.
 */
class DeferredDeletable {
public:
    virtual ~DeferredDeletable() = default;

    /**
     * Called when an object is passed to `deferredDelete`. This signals that the object will soon
     * be deleted.
     */
    virtual void deleteIsPending() {}
};

using DeferredDeletablePtr = std::unique_ptr<DeferredDeletable>;

#endif //LIBEVENT_TEST1_DEFERREDDELETABLE_H