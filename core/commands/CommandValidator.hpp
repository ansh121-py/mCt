#pragma once

#include "Command.hpp"
#include "CommandHandler.hpp"
#include "CommandResult.hpp"

namespace mct::commands {

// Checks the payload against the handler's requirements.
// Runs before the handler, so bad input never reaches it.
class CommandValidator {
public:
    CommandResult validate(const Command& command,
                           const CommandHandler& handler) const;
};

}  // namespace mct::commands