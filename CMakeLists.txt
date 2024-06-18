cmake_minimum_required(VERSION 3.13)

set(PROJECT scl)
project(${PROJECT} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)


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
    include/scl/utils.hpp)

set(CMD_SOURCES
    )


add_library(${PROJECT} ${LEXER_HEADERS} ${LEXER_SOURCES} ${CMD_HEADERS} ${CMD_SOURCES})
target_include_directories(${PROJECT} PUBLIC include)
