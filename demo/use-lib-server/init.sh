#!/bin/bash

# if not exist cppnet dir
if [ ! -d "./cppnet" ]; then
    file_name=cppnet-ubuntu-latest-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi
