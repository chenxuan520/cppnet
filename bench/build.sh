#!/bin/bash

path=$(pwd)

# build
cd ../demo/simple-http-server
./build.sh
cd $path

end=5500
begin=500
step=500

echo -e "\nBegin cppweb bench test"

for ((i=$begin; i< $end; i+=$step)); do

    cd ../demo/simple-http-server
    # ./build.sh
    ../bin/demo >/dev/null &
    cd $path

    echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=${i}qps  -rate=$i  -duration=2s > result${i}.bin
    killall demo
    cat result${i}.bin | ./vegeta report
    ./vegeta report -type=json result${i}.bin > metrics${i}.json
done
# ./vegeta plot *.bin > result.html

# build gin demo
cd ./gin-demo
go mod tidy
go build .
cd ..

echo -e "\nBegin gin bench test"

for ((i=$begin; i< $end; i+=$step)); do

    ./gin-demo/demo >/dev/null &

    echo "GET http://127.0.0.1:8080/" | ./vegeta attack -name=${i}qps  -rate=$i  -duration=2s > result_gin_${i}.bin
    killall demo
    cat result_gin_${i}.bin | ./vegeta report
    ./vegeta report -type=json result_gin_${i}.bin > metrics_gin_${i}.json
done

rm *.bin
