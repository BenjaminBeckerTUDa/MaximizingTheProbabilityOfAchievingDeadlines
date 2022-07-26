import os
import pickle


def getFilesFrom(foldername):
    for f in os.listdir(foldername):
        pre = ""
        if foldername:
            pre = foldername + os.sep
        if os.path.isdir(pre + f):
            getFilesFrom(pre + f)
        else:
            if (len(f.split(".")) == 1):
                if (len(f.split("_")) == 6):
                    if (f.split("_")[0] == "n" and f.split("_")[2] == "d" and f.split("_")[4] == "n"):
                        if(f.split("_")[1].isnumeric() and f.split("_")[3].isnumeric() and f.split("_")[5].isnumeric()):
                            #if (int(f.split("_")[1])) < 41:
                                splitFile(foldername, f)

def pickleDumpAll():
    for filename in os.listdir("testfolder"):
        print(filename)
        createPickle(filename)


def splitFile(folder, filename):
    appMaxTTD = "100"
    appSendInterval = "10000"
    Algorithm = ""
    riceK = -1.0

    raw_file_date = os.path.getmtime(folder+ os.sep +filename)

    nodes = filename.split("_")[1]
    density = filename.split("_")[3]
    number = filename.split("_")[5]

    x = open(folder + os.sep + filename, "r")
    y = False
    new_filename = False
    for line in x.readlines():
        if "Algorithm" in line:
            y = False
            Algorithm = line.split(":")[1].strip()
            if Algorithm == "PLR; Avg secondary":
                Algorithm = "PLR;Avgsec"
        if "appSendInterval" in line:
            appSendInterval = line.split(":")[1].strip()
        if "riceK" in line:
            riceK = line.split(":")[1].strip()
        if "appMaxTTD" in line:
            appMaxTTD = line.split(":")[1].strip()
        if "received at sink in time" in line and "SN.node[0]" in line:
            new_filename = False
            new_path = "testfolder" + os.sep+Algorithm+"_interval_"+appSendInterval+"_ttd_"+appMaxTTD + "_nodes_"+nodes+"_density_"+density+"_nr_"+number+"_riceK_"+riceK
            if os.path.exists(new_path):
                raw_file_split_date = os.path.getmtime(new_path)
                if (raw_file_split_date > raw_file_date):
                    y = False
                else:
                    print(new_path, " replaced by newer version")
                    os.remove(new_path)
                    y = open(new_path, "w")
            else:
                y = open(new_path, "w")
                if y:
                    new_filename = Algorithm + "_interval_" + appSendInterval + "_ttd_" + appMaxTTD + "_nodes_" + nodes + "_density_" + density + "_nr_" + number + "_riceK_" + riceK
        if y:
            if "[" in line:
                node = line.split("[")[1].split("]")[0]
                if ".Routing" in line:
                    line = "[" + node + "]\t" + line.split(".Routing")[1].strip() + "\n"
                if ".MAC" in line:
                    line = "["+node+"]\t" + line.split(".MAC")[1].strip()+"\n"
            y.write(line)
    x.close()
    #os.remove(folder + os.sep + filename)
    print(folder + os.sep + filename)


def helperDivide (x,y):
    if y==0:
        return y
    return x/y

def createPickle(filename):
    Algorithm = filename.split("_")[0]
    appSendInterval = filename.split("_")[2]
    appMaxTTD = filename.split("_")[4]
    nodes = filename.split("_")[6]
    density = filename.split("_")[8]
    number = filename.split("_")[10]

    if len(filename.split("_")) > 10:
        riceK = filename.split("_")[12]
    else:
        riceK = "-1"

    path = "testfolder" + os.sep + Algorithm + "_interval_" + appSendInterval + "_ttd_" + appMaxTTD + "_nodes_" + nodes + "_density_" + density + "_nr_" + number + "_riceK_" + riceK

    file = open(path, "r")

    """
    stats directly from file
    """
    deliveredInTime = 0
    source_delivered = {}
    node_createdPackets = {}
    node_receivedCDFs = {}
    node_broadcastedCDFs = {}
    node_sentProbes = {}
    node_receivedProbes = {}
    node_sentData = {}
    node_DAP_prediction = {}

    node_neighbours = {}
    node_sent_sender_receiver = {}
    node_received_sender_receiver = {}
    node_sent_receiver_sender = {}
    node_received_receiver_sender = {}

    """
    stats calculated
    """
    node_receivedDataFromOthers = {}
    node_othersReceivedData = {}
    network_sentData = 0
    network_receivedData = 0

    for i in range(-1, int(nodes)):
        source_delivered.update({i: 0.})
        node_createdPackets.update({i: 0.})
        node_receivedCDFs.update({i: 0.})
        node_broadcastedCDFs.update({i: 0.})
        node_sentProbes.update({i: 0.})
        node_receivedProbes.update({i: 0.})
        node_sentData.update({i: 0.})
        node_DAP_prediction.update({i: 0.})
        node_receivedDataFromOthers.update({i: 0.})
        node_neighbours.update({i: 0.})
        node_othersReceivedData.update({i: 0.})

        node_sent_sender_receiver.update({i: {}})
        node_received_sender_receiver.update({i: {}})

        node_sent_receiver_sender.update({i: {}})
        node_received_receiver_sender.update({i: {}})

        for j in range(-1, int(nodes)):
            node_sent_sender_receiver[i].update({j: 0.})
            node_received_sender_receiver[i].update({j: 0.})
            node_sent_receiver_sender[i].update({j: 0.})
            node_received_receiver_sender[i].update({j: 0.})

    for line in file:
        if "receivedFrom" in line[:30]:
            receiver = int(line.split("[")[1].split("]")[0].strip())
            sender = int(line.split(":")[1])
            value = sum([int(x) for x in line.split(":")[2].strip().split("\t")][-25:]) / 25
            if sender not in node_received_sender_receiver:
                node_received_sender_receiver.update({sender: {}})
            node_received_sender_receiver[sender].update({receiver: value})

            if receiver not in node_received_receiver_sender:
                node_received_receiver_sender.update({receiver: {}})
            node_received_receiver_sender[receiver].update({sender: value})
            continue

        if "forwardedTo" in line[:30]:
            sender = int(line.split("[")[1].split("]")[0].strip())
            receiver = int(line.split(":")[1])
            value = sum([int(x) for x in line.split(":")[2].strip().split("\t")][-25:]) / 25
            if sender not in node_sent_sender_receiver:
                node_sent_sender_receiver.update({sender: {}})
            node_sent_sender_receiver[sender].update({receiver: value})

            if receiver not in node_sent_receiver_sender:
                node_sent_receiver_sender.update({receiver: {}})
            node_sent_receiver_sender[receiver].update({sender: value})
            continue

        if "sentData" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_sentData.update({key: value})
            continue

        if "nodeCDF" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = float(line.split(":")[1].strip().split("\t")[-2])
            node_DAP_prediction.update({key: value})
            continue

        '''
        # problem: receivedData beinhaltet doppelt empfangene pakete...
        if "receivedData" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:])/25
            node_receivedData.update({key: value})
            continue
        '''
        if "neighbours" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            if (len(line.split(":")[1])) > 1:
                value = [int(x) for x in line.split(":")[1].strip().split("\t")]
                node_neighbours.update({key: value})
            else:
                node_neighbours.update({key: []})
            continue

        if "created packets" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_createdPackets.update({key: value})
            continue

        if "receivedCDFs" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_receivedCDFs.update({key: value})
            continue

        if "broadcastedCDFs" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_broadcastedCDFs.update({key: value})
            continue

        if "sentProbes" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_sentProbes.update({key: value})
            continue

        if "receivedProbes" in line[:30]:
            key = int(line.split("[")[1].split("]")[0].strip())
            value = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            node_receivedProbes.update({key: value})
            continue

        if "received at sink in time" in line[:40]:
            deliveredInTime = sum([int(x) for x in line.split(":")[1].strip().split("\t")][-25:]) / 25
            continue

        if "delivered source" in line[:30]:
            value = sum([int(x) for x in line.split(":")[2].strip().split("\t")][-25:]) / 25
            key = int(line.split(":")[1].strip())
            source_delivered.update({key: value})
            continue

    if node_neighbours[0] == 0:
        return

    for receiver in node_received_receiver_sender:
        received = 0.0
        for sender in node_received_receiver_sender[receiver]:
            if sender != -1:
                received += node_received_receiver_sender[receiver][sender]
        node_receivedDataFromOthers.update({receiver: received})

    network_deliveredInTime = 0
    for node in source_delivered:
        network_deliveredInTime += source_delivered[node]

    network_createdPackets = 0
    for node in node_createdPackets:
        network_createdPackets += node_createdPackets[node]

    """Alle Pakete, die ein knoten gesendet hat, der empfänger aber nicht empfangen, sind in den unteren layern verloren gegangen"""
    network_lostData = 0
    node_packetLoss = {}
    for sender in node_sent_sender_receiver:
        if sender > 0:
            sender_sentTo = node_sentData[sender]

            sender_receivedBy = 0
            for receiver in node_received_sender_receiver[sender]:
                if receiver != -1:
                    sender_receivedBy += node_received_sender_receiver[sender][receiver]
            node_othersReceivedData[sender] = sender_receivedBy
            network_sentData += sender_sentTo
            network_receivedData += sender_receivedBy
            node_packetLoss.update({sender: sender_sentTo - sender_receivedBy})
    network_lostData = network_sentData - network_receivedData

    """Alle Pakete, die ein knoten senden sollte, es aber nicht tut, wurden aufgrund TTL verworfen"""
    network_droppedDataTTL = 0
    help_sent = 0
    help_toSend = 0
    node_droppedDataTTL = {}
    for currentNode in node_received_receiver_sender:
        if currentNode > 0:
            node_sentPackets = 0
            node_packetsToSend = 0
            for sender in node_received_receiver_sender[currentNode]:
                node_packetsToSend += node_received_receiver_sender[currentNode][sender]
            for receiver in node_sent_sender_receiver[currentNode]:
                if receiver > -1:
                    node_sentPackets += node_sent_sender_receiver[currentNode][receiver]
            help_sent += node_sentPackets
            help_toSend += node_packetsToSend
            node_droppedDataTTL.update({currentNode: node_packetsToSend - node_sentPackets})
    # sink fehlt!!
    help_toSend += node_receivedDataFromOthers[0]
    help_sent += network_deliveredInTime

    node_droppedDataTTL.update({0: node_receivedDataFromOthers[0] - network_deliveredInTime})
    network_droppedDataTTL = help_toSend - help_sent

    """
    deliveredInTime = 0 - anzahl pakete, die rechtzeitig beim sink angekommen sind
    source_delivered = {}
    node_createdPackets = {}
    node_receivedCDFs = {}
    node_broadcastedCDFs = {}
    node_sentProbes = {}
    node_receivedProbes = {}
    node_sentData = {} - anzahl pakete, die von einem knoten gesendet werden sollten
    node_receivedData = {} - anzahl pakete, die von einem node erfolgreich empfangen wurden


    node_sent_sender_receiver = {{}} - anzahl pakete, die von einem knoten zum anderen gesendet werden sollten; enthält -1 als receiver für drop
    node_received_sender_receiver = {{}} - anzahl pakete, die von einem knoten zum anderen erfolgreich gesendet wurden; enthält -1 als receiver für drop

    node_sent_receiver_sender = {{}} - ditto wie node_sent_sender_receiver
    node_received_receiver_sender = {{}} - ditto wie node_received_sender_receiver

    node_neighbours = {}

    """

    node_dap = {}
    for node in source_delivered:
        node_dap.update({node: helperDivide(source_delivered[node], node_createdPackets[node])})

    network_dap = helperDivide(network_deliveredInTime, network_createdPackets)

    network_stats = {"dap": network_dap,
                     "packetsCreated": network_createdPackets,
                     "packetsDeliveredInTime": network_deliveredInTime,
                     "packetTransmissions": network_sentData,
                     "packetsExpiredTTL": network_droppedDataTTL,
                     "packetsLost": network_lostData,
                     "packetsDeliveredInTimeRatio": network_deliveredInTime / network_createdPackets,
                     "packetsExpiredTTLRatio": network_droppedDataTTL / network_createdPackets,
                     "packetsLostRatio": network_lostData / network_createdPackets
                     }
    node_stats = {"dap": node_dap,
                  "deliveredInTime": source_delivered,
                  "createdPackets": node_createdPackets,
                  "receivedCDFs": node_receivedCDFs,
                  "broadcastedCDFs": node_broadcastedCDFs,
                  "sentProbes": node_sentProbes,
                  "receivedProbes": node_receivedProbes,
                  "sentData": node_sentData,
                  "node_DAP_prediction": node_DAP_prediction,
                  "receivedData": node_receivedDataFromOthers,
                  "neighbours": node_neighbours,
                  "packetLoss": node_packetLoss}
    for metric in node_stats:
        if -1 in node_stats[metric]:
            node_stats[metric].pop(-1)

    pickle.dump((network_stats, node_stats), open("pickleDumps" + os.sep + filename + ".p", "wb"))







getFilesFrom(None)
import time
time.sleep(5.0)
pickleDumpAll()
