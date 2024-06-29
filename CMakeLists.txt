cmake_minimum_required(VERSION 3.13)

set(PROJECT scl)
project(${PROJECT} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)


option(BUILD_EXAMPLES off "Build projects in ./examples")


set(UTIL_HEADERS
    include/scl/error.h)


set(LEXER_HEADERS
    include/scl/token.h
    include/scl/parse.h
    include/scl/inplace.h)

set(LEXER_SOURCES
    src/token.c
    src/parse.c
    src/inplace.c)


set(CMD_HEADERS
    include/scl/argument.hpp
    include/scl/command.hpp
    include/scl/executor.h
    include/scl/utils.hpp)

set(CMD_SOURCES
    src/executor.c)


add_library(${PROJECT} ${UTIL_HEADERS} ${LEXER_HEADERS} ${LEXER_SOURCES} ${CMD_HEADERS} ${CMD_SOURCES})
target_include_directories(${PROJECT} PUBLIC include)


if ( ${BUILD_EXAMPLES} )
    add_subdirectory(examples/common)
    add_subdirectory(examples/lexer)
    add_subdirectory(examples/command)
endif ()
