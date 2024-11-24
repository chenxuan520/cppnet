#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/mnist
rm ../bin/mnist

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make
cp ./compile_commands.json ../
cd ..

mv ./build/mnist ../bin/

cp ./index.html ../pack/mnist/
cp ../bin/mnist ../pack/mnist/
cp ./demo.param ../pack/mnist/

