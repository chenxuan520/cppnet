#!/bin/bash
rm ./libserver

mkdir -p build
cd build
cmake ..
make
mv ./libserver ../../bin
