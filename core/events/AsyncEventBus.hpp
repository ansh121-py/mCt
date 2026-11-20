#pragma once

#include <thread>

#include "events/EventBus.hpp"
#include "events/EventQueue.hpp"

namespace mct::events {

// Publishes events to a queue. A single background thread delivers them
// through the synchronous EventBus, so subscribers run off the caller's thread.
class AsyncEventBus {
public:
    AsyncEventBus();
    ~AsyncEventBus();

    AsyncEventBus(const AsyncEventBus&) = delete;
    AsyncEventBus& operator=(const AsyncEventBus&) = delete;

    // Subscribe and unsubscribe mirror EventBus.
    template <typename TEvent>
    SubscriptionId subscribeTo(
        std::function<void(const TEvent&)> callback) {
        return bus_.subscribeTo<TEvent>(std::move(callback));
    }

    void unsubscribe(SubscriptionId id) { bus_.unsubscribe(id); }

    // Queue an event for asynchronous delivery. The event is copied, so
    // the caller's object may be destroyed right after this returns.
    template <typename TEvent>
    void publish(TEvent event) {
        queue_.push([this, event = std::move(event)]() {
            bus_.publish(event);
        });
    }

    // Stop accepting events, deliver everything already queued, then join
    // the thread. Safe to call more than once.
    void shutdown();

private:
    void run();

    EventBus bus_;
    EventQueue queue_;
    std::thread worker_;
    bool stopped_ = false;
};

}  // namespace mct::events