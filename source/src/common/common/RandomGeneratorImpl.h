//
// Created by bong on 22. 12. 28.
//

#ifndef EVENTLOOP_BASE_SERVER_RANDOMGENERATORIMPL_H
#define EVENTLOOP_BASE_SERVER_RANDOMGENERATORIMPL_H

#include "include/common/RandomGenerator.h"

namespace Random {
/**
 * Implementation of RandomGenerator that uses per-thread RANLUX generators seeded with current
 * time.
 */
    class RandomGeneratorImpl : public RandomGenerator {
    public:
        // Random::RandomGenerator
        uint64_t random() override;
        std::string uuid() override;

        static const size_t UUID_LENGTH;
    };

} // namespace Random


#endif //EVENTLOOP_BASE_SERVER_RANDOMGENERATORIMPL_H
