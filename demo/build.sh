#!/bin/bash
mkdir -p bin
mkdir -p pack

# 遍历当前文件夹下的所有子文件夹
for dir in $(find . -type d); do
    # 检查子文件夹下是否存在init.sh文件
    if [ -f "$dir/init.sh" ]; then
        echo "Running init.sh in $dir"
        # 进入子文件夹并运行init.sh
        (cd "$dir" &&./init.sh)

        # 检查子文件夹下是否存在build.sh文件
        if [ -f "$dir/build.sh" ]; then
            echo "Running build.sh in $dir"
            # 进入子文件夹并运行build.sh
            (cd "$dir" &&./build.sh)
        fi
    fi
done
