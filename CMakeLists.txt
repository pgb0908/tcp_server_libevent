cmake_minimum_required(VERSION 3.16)
project(eventLoop_base_server)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(3rd/libevent)
add_subdirectory(3rd/abseil-cpp)

include_directories(${PROJECT_SOURCE_DIR}/source)

set(SOURCE
        source/src/server/Server.cpp
        source/src/common/event/DispatcherImpl.cpp
        source/src/common/event/EventImplBase.cpp
        source/src/common/event/posix/SignalImpl.cpp
        source/src/common/event/LibEvent.cpp
        source/src/common/event/LibeventScheduler.cpp
        source/src/common/event/SchedulableCbImpl.cpp
        source/src/common/event/TimerImpl.cpp
        source/src/common/event/FileEventImpl.cpp
        source/src/common/common/ThreadBase.cpp
        source/src/common/network/BaseListenerImpl.cpp
        source/src/common/network/BaseListenerImpl.h
        source/src/common/api/posix/OsSysCallsImpl.cpp
        source/src/common/api/posix/OsSysCallsImpl.h
        source/include/api/OsSysCalls.h
        source/src/common/network/AddressImpl.cpp
        source/src/common/network/AddressImpl.h
        source/src/common/common/Statusor.h
        source/src/common/common/SafeMemcpy.h
        source/src/common/api/ApiImpl.cpp
        source/src/common/common/RandomGeneratorImpl.cpp source/src/common/common/RandomGeneratorImpl.h source/src/common/common/posix/ThreadImpl.cpp source/src/common/common/posix/ThreadImpl.h source/include/server/LifecycleNotifier.h source/src/common/common/Cleanup.h source/src/common/event/RealTimeSystem.cpp source/src/common/event/RealTimeSystem.h source/src/common/common/Utility.cpp source/src/common/common/Utility.h source/include/server/Worker.h source/include/server/ListenerManager.h source/src/server/WorkerImpl.cpp source/src/server/WorkerImpl.h source/src/server/ConnectionHanlderImpl.cpp source/src/server/ConnectionHanlderImpl.h source/include/network/ConnectionHandler.h source/include/network/ListenSocket.h source/src/server/ListenerManagerImpl.cpp source/src/server/ListenerManagerImpl.h source/src/server/ListenerImpl.cpp source/src/server/ListenerImpl.h)

add_library(envoy ${SOURCE})

find_package(OpenSSL REQUIRED)

# 명시적으로 헤더파일 위치 지정해줌
target_include_directories(envoy PUBLIC 3rd/libevent/include)

# https://chromium.googlesource.com/external/github.com/abseil/abseil-cpp/+/HEAD/CMake/README.md
target_link_libraries(envoy
        event
        absl::algorithm
        absl::base
        absl::debugging
        absl::flat_hash_map
        absl::flags
        absl::memory
        absl::meta
        absl::numeric
        absl::random_random
        absl::strings
        absl::synchronization
        absl::time
        absl::utility
        OpenSSL::SSL
        fmt
        z)

add_subdirectory(example)




