//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_THREADLOCALOBJECT_H
#define EVENTLOOP_BASE_SERVER_THREADLOCALOBJECT_H

#include <cstdint>
#include <functional>
#include <memory>

namespace ThreadLocal {

/**
 * All objects that are stored via the ThreadLocal interface must derive from this type.
 */
    class ThreadLocalObject {
    public:
        virtual ~ThreadLocalObject() = default;

        /**
         * Return the object casted to a concrete type. See getTyped() below for comments on the casts.
         */
        template <class T> T& asType() {
            ASSERT(dynamic_cast<T*>(this) != nullptr);
            return *static_cast<T*>(this);
        }
    };

    using ThreadLocalObjectSharedPtr = std::shared_ptr<ThreadLocalObject>;

    template <class T = ThreadLocalObject> class TypedSlot;

} // namespace ThreadLocal

#endif //EVENTLOOP_BASE_SERVER_THREADLOCALOBJECT_H
