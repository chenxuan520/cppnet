#!/bin/bash
rm ./webserver

mkdir -p build
cd build
cmake ..
make
mv ./webserver ../../bin
