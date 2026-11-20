# Event Bus library: reusable, independent of main.cpp.
add_library(mct_events STATIC
    events/EventBus.cpp
    events/EventQueue.cpp
    events/AsyncEventBus.cpp
    commands/CommandRegistry.cpp
    commands/CommandValidator.cpp
)

target_include_directories(mct_events PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(Threads REQUIRED)
target_link_libraries(mct_events PUBLIC Threads::Threads)

# The executable stays in core/ and links against the bus.
add_executable(mct_core
    main.cpp
)

target_link_libraries(mct_core PRIVATE mct_events)