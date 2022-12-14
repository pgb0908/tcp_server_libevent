//
// Created by bong on 22. 12. 14.
//

#ifndef LIBEVENT_TEST1_SCHEDULABLECALLBACKIMPL_H
#define LIBEVENT_TEST1_SCHEDULABLECALLBACKIMPL_H

#include "SchedulableCallback.h"
#include "EventImplBase.h"
#include "Libevent.h"

class DispatcherImpl;

/**
 * libevent implementation of SchedulableCallback.
 */
class SchedulableCallbackImpl : public SchedulableCallback, EventImplBase {
public:
    SchedulableCallbackImpl(BasePtr& libevent, std::function<void()> cb);

    // SchedulableCallback implementation.
    void scheduleCallbackCurrentIteration() override;
    void scheduleCallbackNextIteration() override;
    void cancel() override;
    bool enabled() override;
private:
    std::function<void()> cb_;
};


#endif //LIBEVENT_TEST1_SCHEDULABLECALLBACKIMPL_H
