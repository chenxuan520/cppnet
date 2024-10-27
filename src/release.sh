#!/bin/bash

# run build.sh first
./build.sh

# copy src and lib to release dir
release_dir=./release
include_dir=./release/cppnet/include
lib_dir=./release/cppnet/lib

# rm -rf $release_dir
mkdir -p $release_dir
mkdir -p $include_dir
mkdir -p $lib_dir
cp -r ./cppnet $include_dir
cp ./lib/libcppnet.a $lib_dir
cp ../LICENSE $release_dir

# del all cpp file
find $include_dir -name "*.cpp" -delete

# tar release dir
tar -zcvf ./cppnet.tar.gz $release_dir/* ./LICENSE
