cmake_minimum_required(VERSION 3.16)
project(libevent_test1)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)

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
        ./source/Address.h
        ./source/Server.cpp
        ./source/Server.h source/Worker.h source/ListenerImpl.cpp source/ListenerImpl.h source/WorkerImpl.cpp source/WorkerImpl.h source/ConnectionHandler.h source/ConnectionBalancer.h source/ListenSocket.h source/Libevent.cpp source/Libevent.h source/LibeventScheduler.cpp source/LibeventScheduler.h source/TimerImpl.cpp source/TimerImpl.h source/EventImplBase.cpp source/EventImplBase.h source/SchedulableCallbackImpl.cpp source/SchedulableCallbackImpl.h source/FileEventImpl.cpp source/FileEventImpl.h source/Thread.cpp source/Thread.h source/Filesystem.h source/ThreadImpl.cpp source/ThreadImpl.h source/ScaledRangeTimerManager.h source/SignalImpl.cpp source/SignalImpl.h source/ConnectionHandlerImpl.cpp source/ConnectionHandlerImpl.h source/ActiveTcpListener.cpp source/ActiveTcpListener.h)

add_library(envoy SHARED ${SOURCE})
target_link_libraries(envoy event)

add_subdirectory(example)




