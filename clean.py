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
            if (len(f.split(".")) == 1):
                if (len(f.split("_")) == 6):
                    if (f.split("_")[0] == "n" and f.split("_")[2] == "d" and f.split("_")[4] == "n"):
                        if(f.split("_")[1].isnumeric() and f.split("_")[3].isnumeric() and f.split("_")[5].isnumeric()):
                            os.remove(foldername + os.sep + f)
            if foldername:
                if ("PLR" in foldername.split(os.sep)[-1]):
                    if foldername.split(os.sep)[-1].split("PLR")[0].isnumeric():
                        if (os.sep+"Simulations"+os.sep) in foldername:
                            if (len(f.split(".")) > 1):
                                if (f.split(".")[1] == "log"):
                                    if "PLRConsoleLog" in f:
                                        print(foldername, f)
                                if (f.split(".")[1] == "txt"):
                                    if (len(f.split(".")[0].split("-")) == 2):
                                        if (f.split(".")[0].split("-"))[0].isnumeric():
                                            if (f.split(".")[0].split("-"))[1].isnumeric():
                                                print(foldername, f)

def removeTestFolder():
    for filename in os.listdir("testfolder"):
        os.remove("testfolder" + os.sep + filename)


removeFilesFromSimulations(None)
removeTestFolder()
