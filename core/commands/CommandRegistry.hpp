#pragma once

#include <map>
#include <memory>
#include <string>

#include "CommandHandler.hpp"

namespace mct::commands {

// A whitelist: only names registered here can ever run.
class CommandRegistry {
public:
    // Takes ownership of the handler.
    // Returns false if the handler is null or the name is already taken.
    bool registerHandler(const std::string& name,
                         std::unique_ptr<CommandHandler> handler);

    // Returns nullptr for unknown names. The registry still owns the handler.
    CommandHandler* find(const std::string& name) const;

private:
    std::map<std::string, std::unique_ptr<CommandHandler>> handlers_;
};

}  // namespace mct::commands