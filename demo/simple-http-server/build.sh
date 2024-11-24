#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/simple-http-server
rm ./demo

mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cmake ..
make
cp ./compile_commands.json ..
cd ..

mv ./build/demo ../bin
cp ../bin/demo ../pack/simple-http-server/
