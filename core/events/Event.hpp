#pragma once

#include <typeindex>
#include <typeinfo>

namespace mct::events {

// Marker base for all events. Events are plain value types, so this base
// carries no data. It exists so the bus can document and constrain what an
// event is. Any copyable or movable class may be published.
struct EventBase {
    virtual ~EventBase() = default;
};

// Concept-like helper: every published type must be a class type.
template <typename TEvent>
constexpr std::type_index typeIdOf() {
    return std::type_index(typeid(TEvent));
}

}  // namespace mct::events