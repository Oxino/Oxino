file(GLOB SRC_FILES
    *.h
    *.cpp)

generate_energia_firmware(Timo_Expired
    SKETCH Timo_Expired.ino
    SRCS ${SRC_FILES}
    BOARD witii_cc3200
    PORT /dev/tty.usbserial-cc3101B)