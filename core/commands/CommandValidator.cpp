#include "CommandValidator.hpp"

namespace mct::commands {

CommandResult CommandValidator::validate(
    const Command& command,
    const CommandHandler& handler) const {
    for (const std::string& key : handler.requiredParameters()) {
        // count() returns 0 or 1 for a map, which is simpler than an iterator.
        if (command.payload.count(key) == 0) {
            return CommandResult::failure(
                CommandError::InvalidPayload,
                "Missing parameter: " + key);
        }
    }
    return CommandResult::success();
}

}  // namespace mct::commands