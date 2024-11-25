#!/bin/bash

path=$(pwd)

# build
cd ../demo/simple-http-server
./build.sh
cd $path

echo -e "\nBegin bench test"

for ((i=500; i< 7000; i+=500)); do

    cd ../demo/simple-http-server
    # ./build.sh
    ../bin/demo >/dev/null &
    cd $path

    echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=${i}qps  -rate=$i  -duration=2s > result${i}.bin
    killall demo
    cat result${i}.bin | ./vegeta report
    ./vegeta report -type=json result${i}.bin > metrics${i}.json
done
./vegeta plot *.bin > plot.html


rm *.bin
