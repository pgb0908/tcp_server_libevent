//
// Created by bong on 22. 12. 14.
//

#include <iostream>
#include "src/server/Server.h"
#include "src/common/common/RandomGeneratorImpl.h"
#include "src/common/common/posix/ThreadImpl.h"
#include "src/common/api/ApiImpl.h"
#include "src/common/event/RealTimeSystem.h"

int main() {
    Event::RealTimeSystem realTimeSystem;
    Random::RandomGeneratorImpl random;
    Thread::ThreadFactoryImplPosix threadFactoryImplPosix;
    auto api = Api::Impl(threadFactoryImplPosix, realTimeSystem, random);

    auto server = Server::Server(api, realTimeSystem);

    server.initialize();
    server.run();

    return 0;
}