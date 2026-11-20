#pragma once

#include <string>
#include <vector>

#include "Command.hpp"
#include "CommandResult.hpp"

namespace mct::commands {

// Every command handler implements this interface.
// The virtual destructor is required so deleting a handler through a
// base pointer runs the derived class's destructor.
class CommandHandler {
public:
    virtual ~CommandHandler() = default;

    // Names of payload keys this command must include.
    virtual std::vector<std::string> requiredParameters() const = 0;

    // Does the actual work. Only called after validation passes.
    virtual CommandResult execute(const Command& command) = 0;
};

}  // namespace mct::commands