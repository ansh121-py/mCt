#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "core/commands/Command.hpp"
#include "core/commands/CommandRegistry.hpp"
#include "core/commands/CommandValidator.hpp"

using mct::commands::Command;
using mct::commands::CommandError;
using mct::commands::CommandHandler;
using mct::commands::CommandRegistry;
using mct::commands::CommandResult;
using mct::commands::CommandValidator;

// A simple handler for tests. It requires a "level" parameter.
class SetVolumeHandler : public CommandHandler {
public:
    std::vector<std::string> requiredParameters() const override {
        return {"level"};
    }

    CommandResult execute(const Command&) override {
        return CommandResult::success("volume set");
    }
};

TEST(CommandRegistryTest, FindsRegisteredHandler) {
    CommandRegistry registry;
    EXPECT_TRUE(registry.registerHandler(
        "SET_VOLUME", std::make_unique<SetVolumeHandler>()));

    EXPECT_NE(registry.find("SET_VOLUME"), nullptr);
}

TEST(CommandRegistryTest, UnknownNameReturnsNull) {
    CommandRegistry registry;
    EXPECT_EQ(registry.find("FORMAT_DISK"), nullptr);
}

TEST(CommandRegistryTest, RejectsDuplicateName) {
    CommandRegistry registry;
    registry.registerHandler("SET_VOLUME",
                             std::make_unique<SetVolumeHandler>());

    EXPECT_FALSE(registry.registerHandler(
        "SET_VOLUME", std::make_unique<SetVolumeHandler>()));
}

TEST(CommandRegistryTest, RejectsNullHandler) {
    CommandRegistry registry;
    EXPECT_FALSE(registry.registerHandler("SET_VOLUME", nullptr));
}

TEST(CommandValidatorTest, RejectsMissingParameter) {
    SetVolumeHandler handler;
    Command command{"SET_VOLUME", {}};  // empty payload

    CommandValidator validator;
    CommandResult result = validator.validate(command, handler);

    EXPECT_FALSE(result.ok);
    EXPECT_EQ(result.error, CommandError::InvalidPayload);
}

TEST(CommandValidatorTest, AcceptsValidPayload) {
    SetVolumeHandler handler;
    Command command{"SET_VOLUME", {{"level", "40"}}};

    CommandValidator validator;
    CommandResult result = validator.validate(command, handler);

    EXPECT_TRUE(result.ok);
    EXPECT_EQ(result.error, CommandError::None);
}