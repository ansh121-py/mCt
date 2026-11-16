#include "common/Version.hpp"

#include <iostream>

int main() {
    std::cout << mct::common::APP_NAME
              << " core v" << mct::common::APP_VERSION << '\n';
    return 0;
}