cmake_minimum_required(VERSION 3.16)
project(libevent_test1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rd/libevent)

set(SOURCE
        ./source/DispatcherImpl.cpp
        ./source/DispatcherImpl.h
        ./source/Dispatcher.h
        ./source/FileEvent.h
        ./source/Timer.h
        ./source/ScopeTrackedObject.h
        ./source/TimeSource.h
        ./source/SchedulableCallback.h
        ./source/ScaledTimer.h
        ./source/IntervalValue.h
        ./source/Connection.h
        ./source/StreamInfo.h
        ./source/DispatcherThreadDeletable.h
        ./source/Signal.h
        ./source/DeferredDeletable.h
        ./source/Socket.h
        ./source/Address.h)

add_library(envoy SHARED ${SOURCE})
target_link_libraries(envoy event)

add_subdirectory(example)




