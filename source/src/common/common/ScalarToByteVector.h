//
// Created by bong on 23. 1. 11.
//

#ifndef EVENTLOOP_BASE_SERVER_SCALARTOBYTEVECTOR_H
#define EVENTLOOP_BASE_SERVER_SCALARTOBYTEVECTOR_H

#include <cinttypes>
#include <vector>

template <typename T> void pushScalarToByteVector(T val, std::vector<uint8_t>& bytes) {
    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&val);
    for (uint32_t byte_index = 0; byte_index < sizeof val; byte_index++) {
        bytes.push_back(*byte_ptr++);
    }
}

#endif //EVENTLOOP_BASE_SERVER_SCALARTOBYTEVECTOR_H
