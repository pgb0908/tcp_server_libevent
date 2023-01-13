//
// Created by bong on 23. 1. 11.
//

#ifndef EVENTLOOP_BASE_SERVER_INTERVALSET_H
#define EVENTLOOP_BASE_SERVER_INTERVALSET_H

#include <utility>
#include <vector>

/**
 * Maintains sets of numeric intervals. As new intervals are added, existing ones in the
 * set are combined so that no overlapping intervals remain in the representation.
 *
 * Value can be any type that is comparable with <, ==, and >.
 */
template <typename Value> class IntervalSet {
public:
    virtual ~IntervalSet() = default;

    using Interval = std::pair<Value, Value>;

    /**
     * Inserts a new interval into the set, merging any overlaps. The intervals are in
     * the form [left_inclusive, right_exclusive). E.g. an interval [3, 5) includes the
     * numbers 3 and 4, but not 5.
     * @param left_inclusive Value the left-bound, inclusive.
     * @param right_exclusive Value the right-bound, which is exclusive.
     */
    virtual void insert(Value left_inclusive, Value right_exclusive) =0;

    /**
     * @return std::vector<Interval> the interval-set as a vector.
     */
    virtual std::vector<Interval> toVector() const =0;

    /**
     * Clears the contents of the interval set.
     */
    virtual void clear() =0;

    /**
     * Determines whether the specified Value is in any of the intervals.
     *
     * @param value the value
     * @return true if value is covered in the inteval set.
     */
    virtual bool test(Value value) const =0;
};

#endif //EVENTLOOP_BASE_SERVER_INTERVALSET_H
