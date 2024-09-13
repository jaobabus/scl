cmake_minimum_required(VERSION 3.13)

set(PROJECT command_tests)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)

option(RUN_EXS_TESTS "Run tests of examples" ON)

set(SOURCES
    main.cpp)

add_executable(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC .)


if (${RUN_EXS_TESTS})
    add_custom_command(OUTPUT 0
                       COMMAND ./run_command_tests
                       POST-BUILD)
endif ()
