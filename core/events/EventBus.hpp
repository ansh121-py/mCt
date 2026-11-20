#pragma once

#include "events/Event.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <typeindex>
#include <vector>


namespace mct::events {

using SubscriptionId = std::uint64_t;

class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    // Non-copyable: the bus owns its subscriber table and mutex.
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    // Subscribe to events of type TEvent. Returns an ID for unsubscribe.
    template <typename TEvent>
    SubscriptionId subscribeTo(
        std::function<void(const TEvent&)> callback);

    // Remove a subscription. Unknown IDs are ignored.
    void unsubscribe(SubscriptionId id);

    // Deliver an event synchronously to all current subscribers of TEvent.
    template <typename TEvent>
    void publish(const TEvent& event);

    // Number of active subscriptions for TEvent (useful for tests).
    template <typename TEvent>
    [[nodiscard]] std::size_t subscriberCount() const;

private:
    struct Subscription {
        SubscriptionId id;
        // Type-erased callback: receives a pointer to the event as void*.
        std::function<void(const void*)> invoke;
    };

    [[nodiscard]] SubscriptionId addSubscription(
        std::type_index type,
        std::function<void(const void*)> invoke);

    void deliver(std::type_index type, const void* event);

    mutable std::mutex mutex_;
    SubscriptionId nextId_ = 1;
    std::map<std::type_index, std::vector<Subscription>> subscribers_;
    std::map<SubscriptionId, std::type_index> idToType_;
};

// ---- Template implementations (must live in the header) ----

template <typename TEvent>
SubscriptionId EventBus::subscribeTo(
    std::function<void(const TEvent&)> callback) {
    // Wrap the typed callback in a type-erased one. The static_cast is safe
    // because deliver() only calls this with the type registered for TEvent.
    auto invoke = [callback = std::move(callback)](const void* raw) {
        callback(*static_cast<const TEvent*>(raw));
    };
    return addSubscription(typeIdOf<TEvent>(), std::move(invoke));
}

template <typename TEvent>
void EventBus::publish(const TEvent& event) {
    deliver(typeIdOf<TEvent>(), &event);
}

template <typename TEvent>
std::size_t EventBus::subscriberCount() const {
    std::lock_guard lock(mutex_);
    auto it = subscribers_.find(typeIdOf<TEvent>());
    return it == subscribers_.end() ? 0 : it->second.size();
}

}  // namespace mct::events