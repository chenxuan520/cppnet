#!/bin/bash

# get argv[1], if not exist, default nil
if [ -z $1 ]; then
    CMAKE_BUILD_TYPE=""
else
    CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=${1}"
    echo $CMAKE_BUILD_TYPE
fi


mkdir -p ./build
cd build
cmake ${CMAKE_BUILD_TYPE} ..
make
