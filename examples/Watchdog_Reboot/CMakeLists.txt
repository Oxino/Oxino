file(GLOB SRC_FILES
    ../../*.h
    ../../*.cpp
    *.h
    *.cpp)

generate_energia_firmware(Watchdog_Reboot
    SRCS ${SRC_FILES}
    BOARD witii_cc3200
    PORT /dev/tty.usbserial-cc3101B)