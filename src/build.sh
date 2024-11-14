#!/bin/bash

# get argv[1], if not exist, default true
if [ -z $1 ]; then
    CMAKE_ENABLE_SSL="-DENABLE_SSL=ON"
else
    CMAKE_ENABLE_SSL="-DENABLE_SSL=${1}"
fi

# get argv[2], if not exist, default nil
# Default build type is Release, if you want to build Debug, you can use `./build.sh Debug`
if [ -z $2 ]; then
    CMAKE_BUILD_TYPE=""
else
    CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=${2}"
fi

mkdir -p ./build
cd build

# for win
result=$(uname)
if echo $result | grep -q "WIN"; then
    cmake -G "MinGW Makefiles" ${CMAKE_ENABLE_SSL} ${CMAKE_BUILD_TYPE} ..
else
    cmake ${CMAKE_ENABLE_SSL} ${CMAKE_BUILD_TYPE} ..
fi

make
