//
// Created by bong on 23. 1. 11.
//

#ifndef EVENTLOOP_BASE_SERVER_CONSTSINGLETON_H
#define EVENTLOOP_BASE_SERVER_CONSTSINGLETON_H

/**
 * ConstSingleton allows easy global cross-thread access to a const object.
 *
 * This singleton should be used for data which is initialized once at
 * start-up and then be treated as immutable const data thereafter.
 */
template <class T> class ConstSingleton {
public:
    /**
     * Obtain an instance of the singleton for class T.
     * @return const T& a reference to the singleton for class T.
     */
    static const T& get() {
        static T* instance = new T();
        return *instance;
    }
};


#endif //EVENTLOOP_BASE_SERVER_CONSTSINGLETON_H
