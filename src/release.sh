#!/bin/bash

rm ./cppnet.tar.gz
rm ./cppnet-ssl.tar.gz
rm ./lib/libcppnet.a

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
cp ../LICENSE $cppnet_dir

# del all cpp file
find $include_dir -name "*.cpp" -delete

# run build.sh first
./build.sh ON Release
cp ./lib/libcppnet.a $lib_dir

# tar release dir
cd $release_dir
tar -zcvf ../cppnet-ssl.tar.gz .
cd ..

# build with out ssl
./build.sh OFF Release
cp ./lib/libcppnet.a $lib_dir

# tar release dir
cd $release_dir
tar -zcvf ../cppnet.tar.gz .
cd ..

# del release dir
rm -rf $release_dir

