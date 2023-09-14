 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
 cd build 
 cmake ..
 make clean 
 make
 ./flypen
 cd ..
    