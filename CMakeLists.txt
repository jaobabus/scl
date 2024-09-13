cmake_minimum_required(VERSION 3.13)

set(PROJECT scl)
project(${PROJECT} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)


option(BUILD_EXAMPLES off "Build projects in ./examples")

set(SINGLE_INCLUDE_HEADER single-include/include/scl/scl.hpp)
option(MAKE_SINGLE_INCLUDE off "Build ${SINGLE_INCLUDE_HEADER}")


set(UTIL_HEADERS
    include/scl/calt.h
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
    include/scl/argument.h
    include/scl/argument.hpp
    include/scl/command.h
    include/scl/command.hpp
    include/scl/executor.h
    include/scl/consolebuffer.h
    include/scl/utils.hpp
    include/scl/ard.h)

set(CMD_SOURCES
    src/executor.c
    src/consolebuffer.c
    src/unused.cpp
    src/ard.c)


set(ALL_SOURCES ${UTIL_HEADERS} ${LEXER_HEADERS} ${LEXER_SOURCES} ${CMD_HEADERS} ${CMD_SOURCES})
add_library(${PROJECT} ${ALL_SOURCES})
target_include_directories(${PROJECT} PUBLIC include single-include/include)

if (${MAKE_SINGLE_INCLUDE})
    #message("Single: ${SINGLE_INCLUDE_HEADER}")
    #add_library(
    #    single-include
    #    ${SINGLE_INCLUDE_HEADER})
    #set_target_properties(single-include PROPERTIES LINKER_LANGUAGE CXX)
    #target_include_directories(single-include PUBLIC include single-include/include)

    add_custom_target(
        single-include ALL
        COMMAND single-header "${CMAKE_SOURCE_DIR}/src/singleinclude.cpp" -- "-I${CMAKE_SOURCE_DIR}/include" ">${CMAKE_SOURCE_DIR}/${SINGLE_INCLUDE_HEADER}"
        SOURCES ${SINGLE_INCLUDE_HEADER}
        DEPENDS ${ALL_SOURCES}
    )
endif ()


if (${BUILD_EXAMPLES})
    add_subdirectory(examples/common)
    add_subdirectory(examples/lexer)
    add_subdirectory(examples/command)
endif ()
