#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/use-lib-server-ssl
rm ../bin/libserverssl

mkdir -p build
cd build
cmake ..
make
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
mv ./compile_commands.json ..
cd ..

mv ./build/libserverssl ../bin
cp ../bin/libserverssl ../pack/use-lib-server-ssl/
cp -r ./ssl ../pack/use-lib-server-ssl/
