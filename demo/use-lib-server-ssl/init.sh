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
    file_name=cppnet-$sysname-latest-ssl-release.tar.gz
    wget https://github.com/chenxuan520/cppnet/releases/latest/download/$file_name
    tar -zxvf $file_name
    rm $file_name
fi

# if not exist ssl dir
if [ ! -d "./ssl" ]; then
    mkdir ssl
    cd ssl
    # openssl req -x509 -newkey rsa:4096 -sha256 -nodes -keyout privkey.pem -out cacert.pem -days 3650
    openssl req -x509 -newkey rsa:4096 -sha256 -nodes -keyout privkey.pem -out cacert.pem -days 3650 -subj '/C=US/ST=YourState/L=YourCity/O=YourOrganization/OU=YourUnit/CN=example.com'
fi
