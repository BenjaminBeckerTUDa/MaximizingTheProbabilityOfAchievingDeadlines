./makemake
make


export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -debug-on-errors=true -c [General] 

export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -d -c [N_1,N_2],[aggModus_true,aggModus_false] 

export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -d -c [N_1,N_2,N_3,N_4,N_5,N_6,N_7,N_8,N_9,N_10],[thresholdWaitingTime_02,thresholdWaitingTime_05,thresholdWaitingTime_08],[diff_CDF_0005,diff_CDF_002]

export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -d -c [N_2,N_3,N_4,N_5,N_6,N_7,N_8,N_9,N_10,N_11,N_12,N_13,N_14,N_15,N_16,N_17,N_18,N_19,N_20],[sendInterval_1000,sendInterval_2000,sendInterval_5000],[packetSize_10],[aggModus_true,aggModus_false]

export PATH=$PATH:~/omnetpp-4.6/bin 
export LD_LIBRARY_PATH=~/omnetpp-4.6/lib 

cd /home/dorian/castalia-git/castaliaplr/Castalia 
make 
cd /home/dorian/castalia-git/castaliaplr/Castalia/Simulations/OMac
../../bin/Castalia -c [General] 


cat ODAR.ned | sed -e 's/ODAR/AggODAR/g' > AggODAR.ned
cat ODAR.cc | sed -e 's/ODAR/AggODAR/g' > AggODAR.cc
cat ODAR.h | sed -e 's/ODAR/AggODAR/g' > AggODAR.h