# comment this and add -D CMAKE_TOOLCHAIN_FILE=[CMAKE_TOOLCHAIN_FILE path]
set(CMAKE_TOOLCHAIN_FILE ~/Dev/github/energia-cmake/cmake/EnergiaToolchain.cmake)

cmake_minimum_required(VERSION 3.1)
project(Oxino)

set(CMAKE_VERBOSE_MAKEFILE on)

set(ENERGIA_DEFAULT_BOARD witii_cc3200)
#set(ENERGIA_DEFAULT_PORT /dev/tty.usbserial-cc3101B)
set(ENERGIA_DEFAULT_PORT /dev/tty.usbserial-AI02S971)

file(GLOB INC_FILES
    libs/ATOS/*.h
    libs/Button/*.h
    libs/Led/*.h
    libs/Timo/*.h
    libs/Randify/*.h
    libs/Watchdog/*.h
    *.h)

file(GLOB SRC_FILES
    libs/ATOS/*.c*
    libs/Button/*.c*
    libs/Led/*.c*
    libs/Timo/*.c*
    libs/Randify/*.c*
    libs/Watchdog/*.c*
    *.c*)

generate_energia_library(Oxino
    HDRS ${INC_FILES}
    SRCS ${SRC_FILES}
    BOARD witii_cc3200)

add_subdirectory(examples)