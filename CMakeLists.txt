# comment this and add -D CMAKE_TOOLCHAIN_FILE=[CMAKE_TOOLCHAIN_FILE path] in CLion configuration
set(CMAKE_TOOLCHAIN_FILE ~/Dev/github/energia-cmake/cmake/EnergiaToolchain.cmake)

cmake_minimum_required(VERSION 3.1)
project(Oxino)

set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB INC_FILES
    src/LinkedList/*.h
    src/Timers/*.h
    src/ATOS/*.h
    *.h)

file(GLOB SRC_FILES
    src/LinkedList/*.cpp
    src/Timers/*.cpp
    src/ATOS/*.cpp
    *.cpp)

generate_energia_library(Oxino
    HDRS ${INC_FILES}
    SRCS ${SRC_FILES}
    BOARD lpcc3200)

add_subdirectory(examples)