import os
import pickle

def removeFilesFromSimulations(foldername):
    for f in os.listdir(foldername):
        pre = ""
        if foldername:
            pre = foldername + os.sep
        if os.path.isdir(pre + f):
            removeFilesFromSimulations(pre + f)
        else:
            if foldername:
                if ("PLR" in foldername.split(os.sep)[-1]):
                    if (os.sep+"Simulations"+os.sep) in foldername:
                        if foldername.split(os.sep)[-1].split("PLR")[1].isnumeric():
                            if (len(f.split(".")) == 1):
                                if (len(f.split("_")) == 6):
                                    if (f.split("_")[0] == "n" and f.split("_")[2] == "d" and f.split("_")[4] == "n"):
                                        if(f.split("_")[1].isnumeric() and f.split("_")[3].isnumeric() and f.split("_")[5].isnumeric()):
                                            os.remove(foldername + os.sep + f)
                                            print("removing", foldername, f)
                            if (len(f.split(".")) == 2):
                                if (f.split(".")[1] == "log"):
                                    if "PLRConsoleLog" in f:
                                        print("removing", foldername, f)
                                        os.remove(foldername + os.sep + f)
                                if (f.split(".")[1] == "txt"):
                                    if (len(f.split(".")[0].split("-")) == 2):
                                        if (f.split(".")[0].split("-"))[0].isnumeric():
                                            if (f.split(".")[0].split("-"))[1].isnumeric():
                                                print("removing", foldername, f)
                                                os.remove(foldername + os.sep + f)

def removeTestFolder():
    for f in os.listdir("testfolder"):
        print("removing", "testfolder", f)
        os.remove("testfolder" + os.sep + f)

def removePickleDumps():
    for f in os.listdir("pickleDumps"):
        print("removing", "pickleDumps", f)
        os.remove("pickleDumps" + os.sep + f)

removeFilesFromSimulations(None)
removeTestFolder()
removePickleDumps()