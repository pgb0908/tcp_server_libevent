//
// Created by bong on 23. 1. 11.
//

#ifndef EVENTLOOP_BASE_SERVER_EXCEPTION_H
#define EVENTLOOP_BASE_SERVER_EXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * Base class for all envoy exceptions.
 */
class EnvoyException : public std::runtime_error {
public:
    EnvoyException(const std::string& message) : std::runtime_error(message) {}
};

#endif //EVENTLOOP_BASE_SERVER_EXCEPTION_H
