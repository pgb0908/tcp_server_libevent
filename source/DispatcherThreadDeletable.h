//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_DISPATCHERTHREADDELETABLE_H
#define LIBEVENT_TEST1_DISPATCHERTHREADDELETABLE_H

#include <memory>

/**
 * If an object derives from this class, it can be passed to the destination dispatcher who
 * guarantees to delete it in that dispatcher thread. The common use case is to ensure config
 * related objects are deleted in the main thread.
 */
class DispatcherThreadDeletable {
public:
    virtual ~DispatcherThreadDeletable() = default;
};

using DispatcherThreadDeletableConstPtr = std::unique_ptr<const DispatcherThreadDeletable>;

#endif //LIBEVENT_TEST1_DISPATCHERTHREADDELETABLE_H
