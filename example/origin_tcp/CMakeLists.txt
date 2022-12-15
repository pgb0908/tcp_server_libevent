
add_executable(libevent_test1 main.cpp ../../source/src/Server.cpp ../../source/src/Server.h)
target_link_libraries(libevent_test1 PUBLIC envoy)