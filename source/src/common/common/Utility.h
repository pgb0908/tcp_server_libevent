//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_UTILITY_H
#define EVENTLOOP_BASE_SERVER_UTILITY_H


#include "include/common/Time.h"

/**
 * Real-world time implementation of TimeSource.
 */
class RealTimeSource : public TimeSource {
public:
    // TimeSource
    SystemTime systemTime() override { return std::chrono::system_clock::now(); }
    MonotonicTime monotonicTime() override { return std::chrono::steady_clock::now(); }
};


#endif //EVENTLOOP_BASE_SERVER_UTILITY_H
