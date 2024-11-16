#!/bin/bash
rm ../bin/mnist
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
mv ./mnist ../../bin
