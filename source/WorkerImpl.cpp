//
// Created by bong on 22. 12. 14.
//

#include "WorkerImpl.h"
#include "DispatcherImpl.h"
#include "ConnectionHandlerImpl.h"

WorkerPtr ProdWorkerFactory::createWorker(uint32_t index, const std::string &worker_name) {
    DispatcherPtr dispatcher = std::make_unique<DispatcherImpl>(worker_name);
    auto conn_handler = std::make_unique<ConnectionHandlerImpl>(*dispatcher, index);
    return std::make_unique<WorkerImpl>(std::move(dispatcher), std::move(conn_handler));
}

WorkerImpl::WorkerImpl(DispatcherPtr &&dispatcher, ConnectionHandlerPtr handler) :
dispatcher_(std::move(dispatcher)),handler_(std::move(handler)){

}

void WorkerImpl::addListener(std::optional<uint64_t> overridden_listener) {

}

uint64_t WorkerImpl::numConnections() const {
    return 0;
}

void WorkerImpl::start(const PostCb &cb) {

}

void WorkerImpl::stop() {

}

void WorkerImpl::threadRoutine(const PostCb &cb) {

}
