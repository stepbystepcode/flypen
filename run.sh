#!/bin/bash
echo "-- ███████╗██╗░░░░░██╗░░░██╗██████╗░███████╗███╗░░██╗--" 
echo "-- ██╔════╝██║░░░░░╚██╗░██╔╝██╔══██╗██╔════╝████╗░██║--"
echo "-- █████╗░░██║░░░░░░╚████╔╝░██████╔╝█████╗░░██╔██╗██║--"
echo "-- ██╔══╝░░██║░░░░░░░╚██╔╝░░██╔═══╝░██╔══╝░░██║╚████║--"
echo "-- ██║░░░░░███████╗░░░██║░░░██║░░░░░███████╗██║░╚███║--"
echo "-- ╚═╝░░░░░╚══════╝░░░╚═╝░░░╚═╝░░░░░╚══════╝╚═╝░░╚══╝--"
#is macos

spin() {
    local -a spinner=( "⠋" "⠙" "⠹" "⠸" "⠼" "⠴" "⠦" "⠧" "⠇" "⠏" )
    local str=$1

    while true; do
        for i in "${spinner[@]}"; do
            echo -ne "\r\033[32m$i  $str \033[0m "
            sleep 0.1
        done
    done
}


if [ "$(uname)" == "Darwin" ]; then
    echo "-- Macos detected"
    #检测是否存在mysql-connector文件夹
    if [ ! -d "mysql-connector" ]; then
        echo "-- mysql-connector folder does not exist, downloading..."
        wget https://cdn.mysql.com//Downloads/Connector-C++/mysql-connector-c++-8.1.0-macos13-arm64.tar.gz
        #下载是否成功
        if [ $? -ne 0 ]; then
            echo "-- Download failed, please check your network"
            exit 1
        fi
        tar -xvf mysql-connector-c++-8.1.0-macos13-arm64.tar.gz
        mv mysql-connector-c++-8.1.0-macos13-arm64 mysql-connector
        rm mysql-connector-c++-8.1.0-macos13-arm64.tar.gz
    else
        echo "-- mysql-connector folder already exists "
    fi
    export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
else
    echo "-- Linux detected"
    #检测是否存在mysql-connector文件夹
    if [ ! -d "mysql-connector" ]; then
        echo "-- mysql-connector folder does not exist, downloading..."
        wget https://cdn.mysql.com//Downloads/Connector-C++/mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz
        #下载是否成功
        if [ $? -ne 0 ]; then
            echo "-- Download failed, please check your network"
            exit 1
        fi
        tar -xvf mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz
        mv mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit mysql-connector
        rm mysql-connector-c++-8.1.0-linux-glibc2.28-x86-64bit.tar.gz
    else
        echo "-- mysql-connector folder already exists "
    fi
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
fi
 #检测是否存在build文件夹
    if [ ! -d "build" ]; then
        mkdir build
    else
        echo "-- build folder already exists "
    fi
# 获取机器的核心数
 CORES=$(nproc) 
 echo "Using $CORES cores."
 CORES=$((CORES-1))
 cd build
 cmake ..
 make clean 
 echo "-- make -j$CORES"
 make -j$CORES
 echo "-- Working in the background..."
 spin "Running ..." &
 SPIN_PID=$!
 ./flypen 
 kill -9 "$SPIN_PID" &> /dev/null
 echo -ne "\r\033[K"
 echo -e "\e[1;31m-- WRONG\e[0m"
 cd ..
