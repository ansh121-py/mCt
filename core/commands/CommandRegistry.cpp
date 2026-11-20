#include "CommandRegistry.hpp"

#include <utility>

namespace mct::commands {

bool CommandRegistry::registerHandler(
    const std::string& name,
    std::unique_ptr<CommandHandler> handler) {
    if (!handler) {
        return false;
    }
    if (handlers_.count(name) > 0) {
        return false;
    }
    handlers_[name] = std::move(handler);
    return true;
}

CommandHandler* CommandRegistry::find(const std::string& name) const {
    auto it = handlers_.find(name);
    if (it == handlers_.end()) {
        return nullptr;
    }
    return it->second.get();
}

}  // namespace mct::commands