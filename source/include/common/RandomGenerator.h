//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_RANDOMGENERATOR_H
#define EVENTLOOP_BASE_SERVER_RANDOMGENERATOR_H

#include <limits>
#include <memory>
#include <string>

#include "src/common/common/Interval_value.h"


namespace Random {

/**
 * Random number generator. Implementations should be thread safe.
 */
    class RandomGenerator {
    public:
        virtual ~RandomGenerator() = default;

        using result_type = uint64_t; // NOLINT(readability-identifier-naming)

        /**
         * @return uint64_t a new random number.
         */
        virtual result_type random() = 0;

        /*
         * @return the smallest value that `operator()` may return. The value is
         * strictly less than `max()`.
         */
        constexpr static result_type min() noexcept { return std::numeric_limits<result_type>::min(); };

        /*
         * @return the largest value that `operator()` may return. The value is
         * strictly greater than `min()`.
         */
        constexpr static result_type max() noexcept { return std::numeric_limits<result_type>::max(); };

        /*
         * @return a value in the closed interval `[min(), max()]`. Has amortized
         * constant complexity.
         */
        result_type operator()() { return result_type(random()); };

        /**
         * @return std::string containing uuid4 of 36 char length.
         * for example, 7c25513b-0466-4558-a64c-12c6704f37ed
         */
        virtual std::string uuid() = 0;

        /**
         * @return a random boolean value, with probability `p` equaling true.
         */
        bool bernoulli(UnitFloat p) {
            if (p == UnitFloat::min()) {
                return false;
            } else if (p == UnitFloat::max()) {
                return true;
            }
            return random() < static_cast<result_type>(p.value() * static_cast<float>(max()));
        }
    };

    using RandomGeneratorPtr = std::unique_ptr<RandomGenerator>;

} // namespace Random

#endif //EVENTLOOP_BASE_SERVER_RANDOMGENERATOR_H
