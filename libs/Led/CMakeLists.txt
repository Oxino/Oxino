# comment this and add -D CMAKE_TOOLCHAIN_FILE=[CMAKE_TOOLCHAIN_FILE path]
set(CMAKE_TOOLCHAIN_FILE ~/Dev/github/energia-cmake/cmake/EnergiaToolchain.cmake)

cmake_minimum_required(VERSION 3.1)
project(Led)

set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB INC_FILES
    utility/*.h
    *.h)

file(GLOB SRC_FILES
    utility/*.c*
    *.cpp)

generate_energia_library(Led
    HDRS ${INC_FILES}
    SRCS ${SRC_FILES}
    BOARD witii_cc3200)

add_subdirectory(examples)