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
  

    while true; do
        for i in "${spinner[@]}"; do
            echo -ne "\r\033[32m$i  Running...\033[0m "
            sleep 0.1
        done
    done
}


if [ "$(uname)" == "Darwin" ]; then
    echo "-- Macos detected"
    export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
else
    echo "-- Linux detected"
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
fi
 #检测是否存在build文件夹
    if [ ! -d "build" ]; then
        mkdir build
    else
        echo "-- build folder already exists "
    fi
 cd build
 cmake ..
 make clean 
 make -j16
 echo "-- Working in the background..."
 spin &
 SPIN_PID=$!
 ./flypen
 kill -9 "$SPIN_PID" &> /dev/null
 echo -ne "\r\033[K"
echo -e "\e[1;31m-- WRONG\e[0m"
 cd ..
