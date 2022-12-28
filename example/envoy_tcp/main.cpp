//
// Created by bong on 22. 12. 14.
//

#include <iostream>
#include "../../source/src/Server.h"
#include "src/common/common/RandomGeneratorImpl.h"
#include "src/common/common/posix/ThreadImpl.h"
#include "src/common/api/ApiImpl.h"

int main() {
    myTime time;
    Random::RandomGeneratorImpl random;
    Thread::ThreadFactoryImplPosix threadFactoryImplPosix;
    auto api = Api::Impl(threadFactoryImplPosix, time, random);

    auto server = Server(api);

    server.run();

    return 0;
}