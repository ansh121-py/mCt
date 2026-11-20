#include "events/AsyncEventBus.hpp"

namespace mct::events {

AsyncEventBus::AsyncEventBus()
    : worker_(&AsyncEventBus::run, this) {}

AsyncEventBus::~AsyncEventBus() {
    shutdown();
}

void AsyncEventBus::run() {
    // Keep delivering until pop() reports the queue is closed and empty.
    while (auto task = queue_.pop()) {
        (*task)();
    }
}

void AsyncEventBus::shutdown() {
    if (stopped_) {
        return;
    }
    stopped_ = true;
    queue_.close();
    if (worker_.joinable()) {
        worker_.join();
    }
}

}  // namespace mct::events