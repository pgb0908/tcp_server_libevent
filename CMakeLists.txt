cmake_minimum_required(VERSION 3.16)
project(libevent_test1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rd/libevent)
add_subdirectory(example)




