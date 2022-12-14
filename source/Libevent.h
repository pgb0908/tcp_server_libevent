//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_LIBEVENT_H
#define LIBEVENT_TEST1_LIBEVENT_H

#include <memory>

struct event_base;
extern "C" {
void event_base_free(event_base *);
}

struct evconnlistener;
extern "C" {
void evconnlistener_free(evconnlistener *);
}

/**
 * Global functionality specific to libevent.
 */
class Global {
public:
    static bool initialized() { return initialized_; }

    /**
     * Initialize the library globally.
     */
    static void initialize();

private:
    // True if initialized() has been called.
    static bool initialized_;
};

/**
 * This is a helper that wraps C style API objects that need to be deleted with a smart pointer.
 */
template <class T, void (*deleter)(T*)>
class CSmartPtr : public std::unique_ptr<T, void (*)(T*)> {
public:
    CSmartPtr() : std::unique_ptr<T, void (*)(T*)>(nullptr, deleter) {}
    CSmartPtr(T* object) : std::unique_ptr<T, void (*)(T*)>(object, deleter) {}
};


using BasePtr = CSmartPtr<event_base, event_base_free>;


#endif //LIBEVENT_TEST1_LIBEVENT_H
