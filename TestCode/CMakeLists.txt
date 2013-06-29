
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE ${HOME}/src/git/arduino-cmake/cmake/ArduinoToolchain.cmake)

project(blinktest)

generate_arduino_firmware(blinksk
	SKETCH Blink
	PORT /dev/ttyACM0
	BOARD uno
)

generate_arduino_firmware(blinkcpp
        HDRS blink.h
	SRCS blink.cpp
	PORT /dev/ttyACM0
	BOARD uno
)

