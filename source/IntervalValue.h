//
// Created by bong on 22. 12. 13.
//

#ifndef LIBEVENT_TEST1_INTERVALVALUE_H
#define LIBEVENT_TEST1_INTERVALVALUE_H

#include <algorithm>

// Template helper type that represents a closed interval with the given minimum and maximum values.
template <typename T, T MinValue, T MaxValue> struct Interval {
    static_assert(MinValue <= MaxValue, "min must be <= max");
    static constexpr T min_value = MinValue;
    static constexpr T max_value = MaxValue;
};

// Utility type that represents a value of type T in the given interval.
template <typename T, typename Interval> class ClosedIntervalValue {
public:
    static constexpr ClosedIntervalValue min() { return ClosedIntervalValue(Interval::min_value); }
    static constexpr ClosedIntervalValue max() { return ClosedIntervalValue(Interval::max_value); }

    constexpr explicit ClosedIntervalValue(T value)
            : value_(std::max<T>(Interval::min_value, std::min<T>(Interval::max_value, value))) {}

    T value() const { return value_; }

    // Returns a value that is as far from max as the original value is from min.
    // This guarantees that max().invert() == min() and min().invert() == max().
    ClosedIntervalValue invert() const {
        return ClosedIntervalValue(value_ == Interval::max_value ? Interval::min_value
                                                                 : value_ == Interval::min_value
                                                                   ? Interval::max_value
                                                                   : Interval::max_value - (value_ - Interval::min_value));
    }

    // Comparisons are performed using the same operators on the underlying value
    // type, with the same exactness guarantees.

    bool operator==(ClosedIntervalValue<T, Interval> other) const { return value_ == other.value(); }
    bool operator!=(ClosedIntervalValue<T, Interval> other) const { return value_ != other.value(); }
    bool operator<(ClosedIntervalValue<T, Interval> other) const { return value_ < other.value(); }
    bool operator<=(ClosedIntervalValue<T, Interval> other) const { return value_ <= other.value(); }
    bool operator>=(ClosedIntervalValue<T, Interval> other) const { return value_ >= other.value(); }
    bool operator>(ClosedIntervalValue<T, Interval> other) const { return value_ > other.value(); }

private:
    T value_;
};

#endif //LIBEVENT_TEST1_INTERVALVALUE_H
