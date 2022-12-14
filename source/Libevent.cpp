//
// Created by bong on 22. 12. 14.
//

#include "Libevent.h"
#include "event2/thread.h"
#include <csignal>

bool Global::initialized_ = false;

void Global::initialize(){
    //evthread_use_pthreads();

    // Ignore SIGPIPE and allow errors to propagate through error codes.
    signal(SIGPIPE, SIG_IGN);
    initialized_ = true;
}