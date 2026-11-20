cmake_minimum_required(VERSION 3.21)

project(mCt
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(mCt_BUILD_TESTS "Build C++ tests" ON)

add_subdirectory(core)

add_library(mct_commands STATIC
    core/commands/CommandRegistry.hpp
    core/commands/CommandValidator.cpp
)

# Lets files include headers as "commands/Foo.hpp".
target_include_directories(mct_commands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(mCt_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/cpp)
endif()