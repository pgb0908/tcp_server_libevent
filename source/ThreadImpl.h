//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_THREADIMPL_H
#define LIBEVENT_TEST1_THREADIMPL_H

#include "Thread.h"

/**
 * Implementation of ThreadFactory
 */
class ThreadFactoryImplPosix : public ThreadFactory {
public:
    // Thread::ThreadFactory
    ThreadPtr createThread(std::function<void()> thread_routine, OptionsOptConstRef options) override;
    ThreadId currentThreadId() override;
};



#endif //LIBEVENT_TEST1_THREADIMPL_H
