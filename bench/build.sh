#!/bin/bash

path=$(pwd)

cd ../demo/simple-http-server
./build.sh
../bin/demo >/dev/null &

cd $path
echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=200qps -rate=200 -duration=2s > result200.bin
echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=400qps -rate=400 -duration=2s > result400.bin
echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=800qps -rate=800 -duration=2s > result800.bin
echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=1000qps -rate=1000 -duration=2s > result1000.bin
./vegeta plot result200.bin result400.bin result800.bin result1000.bin > plot.html

rm ./results.bin
killall demo
