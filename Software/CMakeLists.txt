
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE /home/balp/src/git/arduino-cmake/cmake/ArduinoToolchain.cmake)

project(BalpBotOne)

include_directories( avr-stl/include )

generate_arduino_firmware(balpbotone
	HDRS engine.h debug.h
	SRCS main.cpp drive.cpp
	PORT /dev/ttyACM0
	BOARD uno
)

