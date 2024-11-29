#!/bin/bash

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
    file_name=cppnet-$sysname-latest-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi
