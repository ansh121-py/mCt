#pragma once

#include <string>
#include <utility>

namespace mct::commands {

// Error codes are an enum so callers compare values, never message text.
enum class CommandError {
    None,
    UnknownCommand,
    InvalidPayload,
    HandlerFailed
};

struct CommandResult {
    bool ok = false;
    CommandError error = CommandError::None;
    std::string message;

    static CommandResult success(std::string message = "") {
        return CommandResult{true, CommandError::None, std::move(message)};
    }

    static CommandResult failure(CommandError error, std::string message) {
        return CommandResult{false, error, std::move(message)};
    }
};

}  // namespace mct::commands