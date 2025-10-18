#!/bin/bash
mkdir -p ../bin
mkdir -p ../pack/web-server
rm ../bin/webserver

mkdir -p build
cd build
cmake  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make
cp ./compile_commands.json ..
cd ..

mv ./build/webserver ../bin

cp ../bin/webserver ../pack/web-server
cp ./config.json ../pack/web-server

# this is for author self local test dir
SOURCE_FILE=../bin/webserver
TARGET_DIR=~/file/data/web/chenxuanweb
if [ -d "$TARGET_DIR" ]; then
    cp "$SOURCE_FILE" "$TARGET_DIR/"
    echo "copy $SOURCE_FILE to $TARGET_DIR success"
fi
