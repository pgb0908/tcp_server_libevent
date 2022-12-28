//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_THREADIMPL_H
#define EVENTLOOP_BASE_SERVER_THREADIMPL_H


#include "include/thread/Thread.h"

namespace Thread {

/**
 * Implementation of ThreadFactory
 */
    class ThreadFactoryImplPosix : public ThreadFactory {
    public:
        // Thread::ThreadFactory
        ThreadPtr createThread(std::function<void()> thread_routine, OptionsOptConstRef options) override;
        ThreadId currentThreadId() override;
    };

} // namespace Thread

#endif //EVENTLOOP_BASE_SERVER_THREADIMPL_H
