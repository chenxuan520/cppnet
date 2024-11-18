#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/use-lib-server
rm ../bin/libserver

mkdir -p build
cd build
cmake ..
make
cd ..

mv ./build/libserver ../bin/

cp ../bin/libserver ../pack/use-lib-server/
