#!/bin/bash

echo "---- Crypto Course Design ----"

CORES=$(nproc)
echo "-- Using $CORES cores."
CORES=$((CORES - 1))
spin() {
	local -a spinner=("⠋" "⠙" "⠹" "⠸" "⠼" "⠴" "⠦" "⠧" "⠇" "⠏")
	local str=$1

	while true; do
		for i in "${spinner[@]}"; do
			echo -ne "\r\033[32m$i  $str \033[0m "
			sleep 0.1
		done
	done
}
if [ ! -x "$(command -v dg_ctl)" ]; then
	echo "-- drogon does not exist, downloading..."
	git submodule update --init
	sleep 2
	cd drogon
	git submodule update --init
	sleep 2
	mkdir build
	cd build
	cmake ..
	echo "-- make -j$CORES"
	make -j$CORES
	sudo make install
	cd ../..
fi
if [ ! -f "/usr/local/include/jwt-cpp/jwt.h" ]; then
	echo "-- jwt-cpp does not exist, downloading..."
	git submodule update --init
	cd jwt-cpp
	mkdir build
	cd build
	cmake ..
	echo "-- make -j$CORES"
	make -j$CORES
	sudo make install
	cd ../..
fi

if [ "$(uname)" == "Darwin" ]; then
	echo "-- Macos detected"
	export PostgreSQL_INCLUDE_DIR=$(brew --prefix postgresql@16)/include
	export PostgreSQL_LIBRARY=$(brew --prefix postgresql@16)/lib
else
	echo "-- Linux detected"
fi
#检测是否存在build文件夹
if [ ! -d "build" ]; then
	mkdir build
else
	echo "-- build folder already exists "
fi
# 获取机器的核心数
cd build
cmake ..
make clean
echo "-- make -j$CORES"
make -j$CORES
echo
echo "======================================================="
echo "-- ███████╗██╗░░░░░██╗░░░██╗██████╗░███████╗███╗░░██╗--"
echo "-- ██╔════╝██║░░░░░╚██╗░██╔╝██╔══██╗██╔════╝████╗░██║--"
echo "-- █████╗░░██║░░░░░░╚████╔╝░██████╔╝█████╗░░██╔██╗██║--"
echo "-- ██╔══╝░░██║░░░░░░░╚██╔╝░░██╔═══╝░██╔══╝░░██║╚████║--"
echo "-- ██║░░░░░███████╗░░░██║░░░██║░░░░░███████╗██║░╚███║--"
echo "-- ╚═╝░░░░░╚══════╝░░░╚═╝░░░╚═╝░░░░░╚══════╝╚═╝░░╚══╝--"
echo "======================================================="
echo
sleep 1
echo "-- Working in the background..."
spin "Running ..." &
SPIN_PID=$!
./flypen
kill -9 "$SPIN_PID" &>/dev/null
echo -ne "\r\033[K"
echo -e "\e[1;31m-- WRONG\e[0m"
cd ..
