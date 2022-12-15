//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_SCHEDULABLECBIMPL_H
#define EVENTLOOP_BASE_SERVER_SCHEDULABLECBIMPL_H

#include "include/event/SchedulableCb.h"

#include "src/common/event/LibEvent.h"
#include "src/common/event/EventImplBase.h"

namespace Event {

    class DispatcherImpl;

/**
 * libevent implementation of SchedulableCallback.
 */
    class SchedulableCallbackImpl : public SchedulableCallback, ImplBase {
    public:
        SchedulableCallbackImpl(Libevent::BasePtr& libevent, std::function<void()> cb);

        // SchedulableCallback implementation.
        void scheduleCallbackCurrentIteration() override;
        void scheduleCallbackNextIteration() override;
        void cancel() override;
        bool enabled() override;

    private:
        std::function<void()> cb_;
    };

} // namespace Event


#endif //EVENTLOOP_BASE_SERVER_SCHEDULABLECBIMPL_H
