#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>

namespace mct::events {

// A task is a callback that performs one delivery.
using Task = std::function<void()>;

// Thread-safe FIFO queue. Producers call push(); one consumer calls pop().
class EventQueue {
public:
    EventQueue() = default;

    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    // Add a task. Ignored after close().
    void push(Task task);

    // Wait until a task is available or the queue is closed.
    // Returns std::nullopt only when the queue is closed and empty.
    std::optional<Task> pop();

    // Wake all waiting consumers and reject new tasks.
    void close();

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::queue<Task> tasks_;
    bool closed_ = false;
};

}  // namespace mct::events