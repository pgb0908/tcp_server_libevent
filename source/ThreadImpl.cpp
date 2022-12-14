//
// Created by bong on 22. 12. 14.
//

#include <unistd.h>
#include <cassert>
#include "ThreadImpl.h"

int64_t getCurrentThreadId(){
    return static_cast<int64_t>(syscall(186));
}

// See https://www.man7.org/linux/man-pages/man3/pthread_setname_np.3.html.
// The maximum thread name is 16 bytes including the terminating nul byte,
// so we need to truncate the string_view to 15 bytes.
#define PTHREAD_MAX_THREADNAME_LEN_INCLUDING_NULL_BYTE 16


/**
 * Wrapper for a pthread thread. We don't use std::thread because it eats exceptions and leads to
 * unusable stack traces.
 */
class ThreadImplPosix : public Thread {
public:
    ThreadImplPosix(std::function<void()> thread_routine, OptionsOptConstRef options)
            : thread_routine_(std::move(thread_routine)) {
        if (options) {
            name_ = options->name_.substr(0, PTHREAD_MAX_THREADNAME_LEN_INCLUDING_NULL_BYTE - 1);
        }
        //RELEASE_ASSERT(Logger::Registry::initialized(), "");
        const int rc = pthread_create(
                &thread_handle_, nullptr,
                [](void* arg) -> void* {
                    static_cast<ThreadImplPosix*>(arg)->thread_routine_();
                    return nullptr;
                },
                this);
        //RELEASE_ASSERT(rc == 0, "");
    }

    ~ThreadImplPosix() override { assert(joined_); }

    std::string name() const override { return name_; }

    // Thread::Thread
    void join() override {
        assert(!joined_);
        joined_ = true;
        const int rc = pthread_join(thread_handle_, nullptr);
        //RELEASE_ASSERT(rc == 0, "");
    }

private:
    std::function<void()> thread_routine_;
    pthread_t thread_handle_;
    std::string name_;
    bool joined_{false};
};

ThreadPtr ThreadFactoryImplPosix::createThread(std::function<void()> thread_routine,
                                                           const std::optional<Options> &options) {
    return std::make_unique<ThreadImplPosix>(thread_routine, options);
}

ThreadId ThreadFactoryImplPosix::currentThreadId() {
    return ThreadId(getCurrentThreadId());
}
