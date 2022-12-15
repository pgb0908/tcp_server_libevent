cmake_minimum_required(VERSION 3.16)
project(eventLoop_base_server)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rd/libevent)

include_directories(${PROJECT_SOURCE_DIR}/source)

set(SOURCE
        source/src/Server.cpp
        source/src/common/event/DispatcherImpl.cpp
        source/src/common/event/EventImplBase.cpp
        source/src/common/event/posix/SignalImpl.cpp
        source/src/common/event/LibEvent.cpp
        source/src/common/event/LibeventScheduler.cpp
        source/src/common/event/SchedulableCbImpl.cpp
        source/src/common/event/TimerImpl.cpp
        source/src/common/event/FileEventImpl.cpp
        source/src/common/common/ThreadBase.cpp source/src/common/network/TcpListenerImpl.cpp source/src/common/network/TcpListenerImpl.h source/src/common/network/BaseListenerImpl.cpp source/src/common/network/BaseListenerImpl.h source/src/common/network/ListenSocketImpl.cpp source/src/common/network/ListenSocketImpl.h source/src/common/network/SocketImpl.cpp source/src/common/network/SocketImpl.h source/src/common/api/posix/OsSysCallsImpl.cpp source/src/common/api/posix/OsSysCallsImpl.h source/include/api/OsSysCalls.h source/src/common/network/AddressImpl.cpp source/src/common/network/AddressImpl.h source/src/common/common/Statusor.h source/src/common/network/SocketInterfaceBase.cpp source/src/common/network/SocketInterfaceBase.h source/src/common/common/SafeMemcpy.h)

add_library(envoy SHARED ${SOURCE})
target_link_libraries(envoy event)

add_subdirectory(example)




