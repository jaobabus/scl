cmake_minimum_required(VERSION 3.13)

set(PROJECT command)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS program_options)

set(SOURCES
    allexamples.hpp
    exs/baseexample.hpp
    exs/example01.hpp
    exs/example01.cpp
    exs/example02.hpp
    exs/example02.cpp
    exs/example03.hpp
    exs/example03.cpp
    exs/example04.hpp
    exs/example04.cpp
    exs/example05.hpp
    exs/example05.cpp
    exs/example06.hpp
    exs/example06.cpp
    exs/example07.hpp
    exs/example07.cpp
    exs/example08.hpp
    exs/example08.cpp
    exs/example09.hpp
    exs/example09.cpp
    exs/example10.hpp
    exs/example10.cpp
    exs/example11.hpp
    exs/example11.cpp
    exs/example12.hpp
    exs/example12.cpp
    exs/example13.hpp
    exs/example13.cpp
    exs/example14.hpp
    exs/example14.cpp
    exs/example15.hpp
    exs/example15.cpp
    main.cpp)

add_executable(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC .)
#target_compile_options(${PROJECT} PUBLIC "-fsanitize=address" "-fsanitize=undefined")
#target_link_options(${PROJECT} PUBLIC "-fsanitize=address" "-fsanitize=undefined")
#target_link_libraries(${PROJECT} asan ubsan)
target_link_libraries(${PROJECT} common Boost::program_options)
