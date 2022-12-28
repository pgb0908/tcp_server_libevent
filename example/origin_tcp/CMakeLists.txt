
add_executable(libevent_test1 main.cpp ../../source/src/server/Server.cpp ../../source/src/server/Server.h)
target_link_libraries(libevent_test1 PUBLIC envoy)