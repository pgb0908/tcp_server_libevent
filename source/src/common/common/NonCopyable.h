//
// Created by bong on 22. 12. 15.
//

#ifndef EVENTLOOP_BASE_SERVER_NONCOPYABLE_H
#define EVENTLOOP_BASE_SERVER_NONCOPYABLE_H

/**
 * Mixin class that makes derived classes not copyable and not moveable. Like boost::noncopyable
 * without boost.
 */
class NonCopyable {
protected:
    NonCopyable() = default;

    // Non-moveable.
    NonCopyable(NonCopyable&&) noexcept = delete;
    NonCopyable& operator=(NonCopyable&&) noexcept = delete;

    // Non-copyable.
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif //EVENTLOOP_BASE_SERVER_NONCOPYABLE_H
