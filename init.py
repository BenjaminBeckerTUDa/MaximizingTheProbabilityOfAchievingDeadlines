from types import GenericAlias


export PATH=$PATH:~/omnetpp-4.6/bin
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib
cd /home/benny/ODAR/Castalia
make

cd /home/benny/ODAR/Castalia && make && cd /home/benny/ODAR/Castalia/Simulations/OMac && ../../bin/Castalia -c [General]

