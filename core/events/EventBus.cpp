#include "events/EventBus.hpp"

#include <algorithm>
#include <exception>
#include <iostream>

namespace mct::events {

SubscriptionId EventBus::addSubscription(
    std::type_index type,
    std::function<void(const void*)> invoke) {
    std::lock_guard lock(mutex_);

    SubscriptionId id = nextId_++;
    subscribers_[type].push_back(Subscription{id, std::move(invoke)});
    idToType_.emplace(id, type);
    return id;
}   

void EventBus::unsubscribe(SubscriptionId id) {
    std::lock_guard lock(mutex_);

    auto typeIt = idToType_.find(id);
    if (typeIt == idToType_.end()) {
        return;  // Unknown or already removed: ignore, per the API contract.
    }

    auto& list = subscribers_[typeIt->second];
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [id](const Subscription& s) { return s.id == id; }),
        list.end());
    idToType_.erase(typeIt);
}

void EventBus::deliver(std::type_index type, const void* event) {
    // Copy the subscriber list under the lock, then release the lock before
    // calling anything. A callback may publish or unsubscribe without deadlock.
    std::vector<Subscription> snapshot;
    {
        std::lock_guard lock(mutex_);
        auto it = subscribers_.find(type);
        if (it == subscribers_.end()) {
            return;
        }
        snapshot = it->second;
    }

    for (const auto& sub : snapshot) {
        try {
            sub.invoke(event);
        } catch (const std::exception& ex) {
            // One failing subscriber must not prevent delivery to the others.
            std::cerr << "[EventBus] subscriber " << sub.id
                      << " threw: " << ex.what() << '\n';
        } catch (...) {
            std::cerr << "[EventBus] subscriber " << sub.id
                      << " threw an unknown exception\n";
        }
    }
}

}  // namespace mct::events