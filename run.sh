echo "███████╗██╗░░░░░██╗░░░██╗██████╗░███████╗███╗░░██╗"
echo "██╔════╝██║░░░░░╚██╗░██╔╝██╔══██╗██╔════╝████╗░██║"
echo "█████╗░░██║░░░░░░╚████╔╝░██████╔╝█████╗░░██╔██╗██║"
echo "██╔══╝░░██║░░░░░░░╚██╔╝░░██╔═══╝░██╔══╝░░██║╚████║"
echo "██║░░░░░███████╗░░░██║░░░██║░░░░░███████╗██║░╚███║"
echo "╚═╝░░░░░╚══════╝░░░╚═╝░░░╚═╝░░░░░╚══════╝╚═╝░░╚══╝"
 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mysql-connector/lib64
 export DYLD_LIBRARY_PATH=/Users/stepbystep/Documents/flypen/mysql-connector/lib64:$DYLD_LIBRARY_PATH
 cd build
 cmake ..
 make clean 
 make -j16
 ./flypen
 cd ..
