#include "events/EventQueue.hpp"

namespace mct::events {

void EventQueue::push(Task task) {
    {
        std::lock_guard lock(mutex_);
        if (closed_) {
            return;
        }
        tasks_.push(std::move(task));
    }
    // Notify after unlocking so the woken thread can acquire the mutex.
    ready_.notify_one();
}

std::optional<Task> EventQueue::pop() {
    std::unique_lock lock(mutex_);
    ready_.wait(lock, [this] { return closed_ || !tasks_.empty(); });

    if (tasks_.empty()) {
        return std::nullopt;  // Closed and drained.
    }

    Task task = std::move(tasks_.front());
    tasks_.pop();
    return task;
}

void EventQueue::close() {
    {
        std::lock_guard lock(mutex_);
        closed_ = true;
    }
    ready_.notify_all();
}

}  // namespace mct::events