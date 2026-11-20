#pragma once

#include <map>
#include <string>

namespace mct::commands {

// A request to perform one named operation.
// A struct is used here because it is plain data with no behavior to hide.
struct Command {
    std::string name;
    std::map<std::string, std::string> payload;
};

}  // namespace mct::commands