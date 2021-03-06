
cmake_minimum_required(VERSION 2.8)
#set(CMAKE_TOOLCHAIN_FILE /home/balp/src/git/arduino-cmake/cmake/ArduinoToolchain.cmake)
if(NOT DEFINED BOARDPORT)
	set(BOARDPORT "/dev/ttyACM0")
endif()
project(BalpBotOne)

if(MSVC)
    add_definitions( -D_VARIADIC_MAX=10 -DGTEST_USE_OWN_TR1_TUPLE=0 )
endif()

if(ARDUINO_FOUND)
	include_directories( avr-stl/include )

	generate_arduino_firmware(balpbotone
		HDRS engine.h debug.h
		SRCS main.cpp drive.cpp
		PORT ${BOARDPORT}
		BOARD uno
	)
else()
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
			"${CMAKE_SOURCE_DIR}/cmake/Modules/")
	message(STATUS "${CMAKE_MODULE_PATH}" )
	enable_testing()
	add_subdirectory(gmock)
    include_directories(${gmock_SOURCE_DIR}/include ${gtest_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/ArduinoMock )


    if(MSVC)
        foreach(flag_var CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endforeach(flag_var)
        SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "MSVC C Debug MT flags " FORCE)
        SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
        SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "MSVC C Release MT flags " FORCE)
        SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
        SET (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Debug MT flags " FORCE)
        SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Release MT flags " FORCE)
        SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
        SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
    endif()


	add_executable(engine_test
		engine.h
		test/engine_test.cpp
		ArduinoMock/arduino.cpp
	)
	target_link_libraries(engine_test gtest gmock gmock_main )
	add_test(AllTestsInEngine engine_test  --gtest_color=on --gtest_output=xml)

    add_custom_command(OUTPUT EngineMock.h
        COMMAND ${gmock_SOURCE_DIR}/scripts/generator/gmock_gen.py ${CMAKE_SOURCE_DIR}/engine.h > EngineMock.h
        DEPENDS ${CMAKE_SOURCE_DIR}/engine.h
        )

    add_executable(driver_test
        driver.h
        engine.h
        EngineMock.h
        test/driver_test.cpp
		ArduinoMock/arduino.cpp
        )
	target_link_libraries(driver_test gtest gmock gmock_main )
    add_test(AllTestsInDriver driver_test  --gtest_color=on --gtest_output=xml)

    add_custom_command(OUTPUT LightSensorMock.h
        COMMAND ${gmock_SOURCE_DIR}/scripts/generator/gmock_gen.py ${CMAKE_SOURCE_DIR}/lightsensor.h > LightSensorMock.h
        DEPENDS ${CMAKE_SOURCE_DIR}/lightsensor.h
        )

    add_executable(lightsensor_test
        LightSensorMock.h
        lightsensor.h
        test/lightsensor_test.cpp
		ArduinoMock/arduino.cpp
        )
	target_link_libraries(lightsensor_test gtest gmock gmock_main )
    add_test(AllTestsForLightSensor lightsensor_test  --gtest_color=on --gtest_output=xml)

    add_executable(state_test
        state.h
        test/state_test.cpp
		ArduinoMock/arduino.cpp
        )
	target_link_libraries(state_test gtest gmock gmock_main )
    add_test(AllTestsForState state_test  --gtest_color=on --gtest_output=xml)

    add_executable(timer_test
        timer.h
        test/timer_test.cpp
		ArduinoMock/arduino.cpp
        )
	target_link_libraries(timer_test gtest gmock gmock_main )
    add_test(AllTestsForTimer timer_test  --gtest_color=on --gtest_output=xml)

endif()

