#!/bin/bash
rm ./demo

mkdir -p build
cd build
cmake ..
make
mv ./demo ../../bin
