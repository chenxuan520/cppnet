#!/bin/bash

git clone https://gitee.com/chenxuan520/cppjson
git clone https://gitee.com/chenxuan520/deeplearning.git

# if not exist demo.param
if [ ! -f "./demo.param" ]; then
    wget https://gitee.com/chenxuan520/deeplearning/releases/download/v0.0.1-beta/demo.param
fi

# if not exist cppnet dir
if [ ! -d "./cppnet" ]; then
    file_name=cppnet-ubuntu-latest-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi
