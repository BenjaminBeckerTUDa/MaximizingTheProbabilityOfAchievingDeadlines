./makemake
make


export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/PAODAR
../../bin/Castalia -d -c [General] 

export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -c [General] 


cat ODAR.ned | sed -e 's/ODAR/AggODAR/g' > AggODAR.ned
cat ODAR.cc | sed -e 's/ODAR/AggODAR/g' > AggODAR.cc
cat ODAR.h | sed -e 's/ODAR/AggODAR/g' > AggODAR.h