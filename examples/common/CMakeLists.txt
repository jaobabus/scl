cmake_minimum_required(VERSION 3.13)

set(PROJECT common)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    include/utils.hpp
    src/utils.cpp
    include/consolearea.hpp
    include/consolewidget.hpp
    src/consolewidget.cpp
    include/buttonwidget.hpp
    src/buttonwidget.cpp
    include/textmap.hpp
    src/textmap.cpp
    include/properties.hpp
    src/properties.cpp
    )

add_library(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC include)
