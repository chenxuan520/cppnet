#!/bin/bash

# if not exist cppnet dir
if [ ! -d "./cppnet" ]; then
    file_name=cppnet-ubuntu-latest-ssl-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi

# if not exist ssl dir
if [ ! -d "./ssl" ]; then
    mkdir ssl
    cd ssl
    openssl req -x509 -newkey rsa:4096 -sha256 -nodes -keyout privkey.pem -out cacert.pem -days 3650
fi
