file(GLOB SRC_FILES
    *.h
    *.cpp)

generate_energia_firmware(Watchdog_Reboot
    SKETCH Watchdog_Reboot.ino
    SRCS ${SRC_FILES}
    BOARD witii_cc3200
    PORT /dev/tty.usbserial-cc3101B)