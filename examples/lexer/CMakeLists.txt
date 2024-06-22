cmake_minimum_required(VERSION 3.13)

set(PROJECT lexer)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    main.cpp)

add_executable(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC .)
target_link_libraries(${PROJECT} m)

