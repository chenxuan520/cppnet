#!/bin/bash

git clone https://gitee.com/chenxuan520/cppjson
git clone https://gitee.com/chenxuan520/deeplearning.git

# if not exist demo.param
if [ ! -f "./demo.param" ]; then
    wget https://gitee.com/chenxuan520/deeplearning/releases/download/v0.0.1-beta/demo.param
fi

# if not exist cppnet dir
if [ ! -d "./cppnet" ]; then
    # check macos or linux
    uname_result=$(uname)
    sysname=""
    if [ "$uname_result" == "Linux" ]; then
        sysname="ubuntu"
    elif [ "$uname_result" == "Darwin" ]; then
        sysname="macos"
    else
        echo "未知系统"
        exit
    fi
    file_name=cppnet-$sysname-latest-ssl-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi
