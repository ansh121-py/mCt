include(FetchContent)

# Download GoogleTest at configure time. Pinned to a known release tag
# so builds are reproducible.
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

# Prevent GoogleTest from overriding our compiler and install settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(mct_events_tests
    events_bus_test.cpp
)

target_link_libraries(mct_events_tests PRIVATE
    mct_events
    GTest::gtest_main
)

add_executable(command_test command_test.cpp)
target_link_libraries(command_test PRIVATE mct_commands GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mct_events_tests)
gtest_discover_tests(command_test)