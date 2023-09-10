 export LD_LIBRARY_PATH=/home/lgy/c++ks/mysql-connector/lib64:$LD_LIBRARY_PATH
 cmake ..
 make clean
 make -j 14
 ./flypen