# comment this and add -D CMAKE_TOOLCHAIN_FILE=[CMAKE_TOOLCHAIN_FILE path]
set(CMAKE_TOOLCHAIN_FILE ~/Dev/github/energia-cmake/cmake/EnergiaToolchain.cmake)

cmake_minimum_required(VERSION 3.1)
project(Oxino)

set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB INC_FILES
    libs/ATOS/*.h
    libs/Button/*.h
    libs/Led/*.h
    libs/Randify/*.h
    libs/Watchdog/*.h
    *.h)

file(GLOB SRC_FILES
    libs/ATOS/*.c*
    libs/Button/*.c*
    libs/Led/*.c*
    libs/Randify/*.c*
    libs/Watchdog/*.c*
    *.c*)

generate_energia_library(Oxino
    HDRS ${INC_FILES}
    SRCS ${SRC_FILES}
    BOARD witii_cc3200)

add_subdirectory(examples)