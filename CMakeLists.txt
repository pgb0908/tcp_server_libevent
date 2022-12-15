cmake_minimum_required(VERSION 3.16)
project(eventLoop_base_server)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rd/libevent)

include_directories(${PROJECT_SOURCE_DIR}/source)

set(SOURCE
    source/src/Server.cpp
        )

add_library(envoy SHARED ${SOURCE})
target_link_libraries(envoy event)

add_subdirectory(example)




