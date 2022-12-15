//
// Created by bong on 22. 12. 15.
//

#include "LibEvent.h"

#include <csignal>
#include "event2/thread.h"

namespace Event {
    namespace Libevent {

        bool Global::initialized_ = false;

        void Global::initialize() {
            evthread_use_pthreads();
            // Ignore SIGPIPE and allow errors to propagate through error codes.
            signal(SIGPIPE, SIG_IGN);
            initialized_ = true;
        }

    } // namespace Libevent
} // namespace Event