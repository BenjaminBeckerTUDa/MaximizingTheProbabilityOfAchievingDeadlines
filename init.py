import os
cmd = 'cd /home/benny/Castalia/'
os.system(cmd)
cmd = 'export PATH=$PATH:~/omnetpp-4.6/bin'
os.system(cmd)
cmd = 'export LD_LIBRARY_PATH=~/omnetpp-4.6/lib'
os.system(cmd)
cmd = 'cd /home/benny/Castalia/Castalia'
os.system(cmd)
cmd = 'make'
os.system(cmd)
#cmd = 'zip -r pd.zip pickleDumps'
#os.system(cmd)

