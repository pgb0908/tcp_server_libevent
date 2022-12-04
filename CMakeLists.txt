cmake_minimum_required(VERSION 3.16)
project(libevent_test1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 11)

set(EVENT__DISABLE_SAMPLES 0)
set(EVENT__DISABLE_TESTS 0)

add_subdirectory(3rd/libevent)

add_executable(libevent_test1 main.cpp)
target_link_libraries(libevent_test1 PUBLIC event)


