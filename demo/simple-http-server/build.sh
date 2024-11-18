#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/simple-http-server
rm ./demo

mkdir -p build
cd build
cmake ..
make
cd ..

mv ./build/demo ../bin
cp ../bin/demo ../pack/simple-http-server/
