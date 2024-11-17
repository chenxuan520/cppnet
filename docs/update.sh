#!/bin/bash

# clone web repo
git clone https://github.com/chenxuan520/obsidian2web

# copy readme
cp ../README*.md ./cppnet/

cd ./obsidian2web
./init.sh
./create.sh ../cppnet
cp -r ./book ../..
