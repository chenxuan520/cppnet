#!/bin/bash
git clone https://gitee.com/chenxuan520/cppjson

# if not exist ssl dir
if [ ! -d "./ssl" ]; then
    mkdir ssl
    cd ssl
    # openssl req -x509 -newkey rsa:4096 -sha256 -nodes -keyout privkey.pem -out cacert.pem -days 3650
    openssl req -x509 -newkey rsa:4096 -sha256 -nodes -keyout privkey.pem -out cacert.pem -days 3650 -subj '/C=US/ST=YourState/L=YourCity/O=YourOrganization/OU=YourUnit/CN=example.com'
fi
