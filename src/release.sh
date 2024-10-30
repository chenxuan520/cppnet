#!/bin/bash

rm ./cppnet.tar.gz
rm ./lib/libcppnet.a

# run build.sh first
./build.sh

# copy src and lib to release dir
release_dir=./release
cppnet_dir=./release/cppnet
include_dir=./release/cppnet/include
lib_dir=./release/cppnet/lib

rm -rf $release_dir
mkdir -p $release_dir
mkdir -p $include_dir
mkdir -p $lib_dir
cp -r ./cppnet $include_dir
cp ./lib/libcppnet.a $lib_dir
cp ../LICENSE $cppnet_dir

# del all cpp file
find $include_dir -name "*.cpp" -delete

# tar release dir
cd $release_dir
tar -zcvf ../cppnet.tar.gz .
cd ..

# del release dir
rm -rf $release_dir
