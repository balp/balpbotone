#!/bin/bash
#
echo
echo " *** Build for host test code"
echo
if [[ ! -d build_host ]];
then
  mkdir build_host
fi
( cd build_host && cmake ../Software && make all && ctest --output-on-failure ) || exit

echo
echo " *** Build for Arduino Target"
echo
if [[ ! -d build_target ]];
then
  mkdir build_target
fi
( cd build_target && cmake -DCMAKE_TOOLCHAIN_FILE=/home/balp/src/git/arduino-cmake/cmake/ArduinoToolchain.cmake ../Software && make $* )

