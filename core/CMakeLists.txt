add_executable(mct_core
    main.cpp
)

target_include_directories(mct_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(NOT MSVC)
    target_compile_options(mct_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif() 