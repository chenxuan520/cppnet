#!/bin/bash
rm ./libserver

mkdir -p build
cd build
cmake ..
make
mv ./libserver ../../bin

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
mv ./compile_commands.json ..
