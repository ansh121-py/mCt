cmake_minimum_required(VERSION 3.21)

project(mCt
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tests stay off until GoogleTest is set up in Phase 1.
option(mCt_BUILD_TESTS "Build C++ tests" OFF)

add_subdirectory(core)

if(mCt_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/cpp)
endif()