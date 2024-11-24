#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/web-server
rm ../bin/webserver

mkdir -p build
cd build
cmake  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make
cp ./compile_commands.json ..
cd ..

mv ./build/webserver ../bin

cp ../bin/webserver ../pack/web-server
cp ./config.json ../pack/web-server
