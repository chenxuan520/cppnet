#!/bin/bash
# only run in windows

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
mkdir build
cd build
cmake -G "MinGW Makefiles" .. -DENABLE_SSL=OFF
make
cd ..

cp ./lib/libcppnet.a $lib_dir

# tar release dir
cd $release_dir
tar -zcvf ../cppnet.tar.gz .
cd ..

# del release dir
rm -rf $release_dir

