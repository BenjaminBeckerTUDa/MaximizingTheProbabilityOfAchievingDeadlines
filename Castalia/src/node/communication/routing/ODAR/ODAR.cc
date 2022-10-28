#include "ODAR.h"

Define_Module(ODAR);

void ODAR::startup()
{
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
    if (appModule->hasPar("isSink"))
        isSink = appModule->par("isSink");
    else
        opp_error("\nODAR has to be used with an application that defines the parameter isSink");

    if (appModule->hasPar("isSink"))
            packetSize = (int) appModule->par("packetSize");
    else
        opp_error("\nODAR has to be used with an application that defines the parameter packetSize");


    cdfSlots = (int) par("cdfSlots");

    if (isSink)
        hopCount = 0;
    else
        hopCount = INT_MAX;

    hopCountPeriod = ((double)par("hopCountPeriod")) / 1000.0;
    neighborSuccRateThreshold = .8;
    // start with timer for broadcasting control messages
    setTimer(BROADCAST_CONTROL, hopCountPeriod);
    setTimer(REQUEST_TIMES_FROM_MAC, 1);
}

/*
steps:
Regelmäßig stattfindende Broadcasts zum Informationsaustausch
1.	Die IDs aller ihm bekannten Nachbarn. - done
2.	Sein eigenes CDF (todo)
3.	Die von jedem einzelnen seiner Nachbarn erfolgreich gehörten Datenpakete. - done

Übertragungswahrscheinlichkeit - done
Nachbarschaften - done

Berechnung des CDFs - 
    For x = 0 ms; x < 100 ms; x += 2 ms:
        1.	Filtere die geeigneten Empfänger durch Betrachtung der CDFs an der Stelle x-2ms heraus. (todo)
        2.	Bilde aus den verbleibenden Knoten Mengen von Knoten, welche gegenseitig benachbart sind. - done
        3.	Sortiere die Knoten in jeder Menge nach ihren CDFs an der Stelle x-2ms (todo)
        4.	Berechne für jede der sortierten Gruppen den Wert, den das CDF von u an der Stelle x für diese Gruppe annehmen würde (todo)
        5.	Entscheide dich für die bestmögliche Gruppe (todo)
*/




void ODAR::timerFiredCallback(int timer)
{
    switch (timer)
    {
        case BROADCAST_CONTROL:
        {
            broadcastControl();
            setTimer(BROADCAST_CONTROL, hopCountPeriod);
            break;
        }
        case REQUEST_TIMES_FROM_MAC:
        {
            ODARControlMessage *oc = new ODARControlMessage("REQUEST_TIMES", MAC_CONTROL_COMMAND);
            oc->setODARControlMessageKind(REQUEST_TIMES);
            oc->setDatabytelength(packetSize + netDataFrameOverhead);
            toMacLayer(oc);
            break;
        }
        default:
        {
            break;
        }
    }
}

void ODAR::fromApplicationLayer(cPacket *pkt, const char *destination)
{
    if (isSink || receivers.empty())
    {
        return;
    }

    // create unique data packet ID
    packetNumber++;
    unsigned int packetId = packetNumber * 100000 + atoi(SELF_NETWORK_ADDRESS);
    ReceiversContainer receiversContainer;
    receiversContainer.setReceivers(this->receivers);

    ODARPacket *netPacket = new ODARPacket("ODAR packet", NETWORK_LAYER_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_DATA_PACKET);
    netPacket->setPacketId(packetId);
    netPacket->setReceiversContainer(receiversContainer);
    encapsulatePacket(netPacket, pkt);
    
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);

    pktCount++;
}

void ODAR::handleNetworkControlCommand(cMessage *pkt)
{
    ODARControlMessage *oc = dynamic_cast<ODARControlMessage *>(pkt);
	if (!oc)
	{
		return;
	}
    switch (oc->getODARControlMessageKind())
    {
        case REPLY_TIMES:
        {
            //trace() << oc->getRequiredTimeForDATA();
            break;
        }
        case INC_RCV:
        {
            int srcMacAddress = oc -> getSenderAddress();
            if (rxCount.find(srcMacAddress) == rxCount.end())
            {
                rxCount.insert({srcMacAddress, 0});
            }
            rxCount[srcMacAddress]++;
            break;
        }
        case INC_TX:
        {
            txCount ++;
            break;
        }

    }
}




void ODAR::fromMacLayer(cPacket *pkt, int srcMacAddress, double rssi, double lqi)
{

    if (neighbors.find(srcMacAddress) == neighbors.end())
    {
        neighbors.insert(srcMacAddress);
        neighborHopCounts.insert({srcMacAddress, INT_MAX});

        int id = MACToLong.size();
        if (id > 63)
        {
            trace() << "we have more than 63 neighbors :("; 
        }
        long macAsLong = 1L << id;  

        MACToLong.insert({srcMacAddress, macAsLong});
        longToMAC.insert({macAsLong, srcMacAddress});
        
        neighborCDFs_byLong.insert({macAsLong, new double[cdfSlots]{0}});
        neighborCDFs_byMAC.insert({srcMacAddress, new double[cdfSlots]{0}});
        
    }

    ODARPacket *netPacket = dynamic_cast<ODARPacket *>(pkt);

    if (netPacket)
    {
        switch (netPacket->getOMacRoutingKind())
        {
        case OMAC_ROUTING_DATA_PACKET:
        {
            if (isSink)
            {
                toApplicationLayer(decapsulatePacket(pkt));
                pktCountToApp++;
            }
            else
            {
                ReceiversContainer receiversContainer;
                receiversContainer.setReceivers(this->receivers);
                ODARPacket *dupPacket = netPacket->dup();
                dupPacket->setSource(SELF_NETWORK_ADDRESS);
                dupPacket->setSequenceNumber(currentSequenceNumber++);
                dupPacket->setReceiversContainer(receiversContainer);
                toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS);
            }
            break;
        }

        case OMAC_ROUTING_CONTROL_PACKET:
        {
            int srcHopCount = netPacket->getHopcount();
            neighborHopCounts[srcMacAddress] = srcHopCount;

            if (srcHopCount < hopCount)
            {
                hopCount = srcHopCount + 1;
                updateReceiverList();
            }

            map<int,int> ohc = netPacket -> getOverheardPackets().getMapIntInt();
            OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
            int selfMacAdress = omac->getMacAdress();

            if (ohc.find(selfMacAdress) == ohc.end())
            {
                // "not in ohc...";
            }else
            {
                double txSuccessRate = (double)ohc[selfMacAdress] / (double)txCount;
                if (txSuccessRates.find(srcMacAddress) == txSuccessRates.end()){
                    txSuccessRates.insert({srcMacAddress, 0});
                }
                txSuccessRates[srcMacAddress] = txSuccessRate;
            }

            
            


            long idLong = MACToLong[srcMacAddress];
            if (twoHopNeighborhoods_byLong.find(idLong) == twoHopNeighborhoods_byLong.end()){
                twoHopNeighborhoods_byLong.insert({idLong, 0L});
            }

            long neighborsAsLong = nodesetToLong(netPacket -> getNeighbors().getSetInt());
            twoHopNeighborhoods_byLong[idLong] = neighborsAsLong;

            neighborCDFs_byLong[idLong] = netPacket -> getCDF().getDoubleArray();;
            neighborCDFs_byMAC[srcMacAddress] = netPacket -> getCDF().getDoubleArray();;
            
            findCliques();
            
            break;
        }

        default:
        {
            break;
        }
        }
    }
}

long ODAR::nodesetToLong(set<int> setOfNodes)
{
    // ids of nodes in setOfNodes is converted to a single long, 
    // where each node is represented by a "1" at position corresponding to its id in the binary representation of that long.
    // if we have more than 64 neighbors, this will fail. :(
    // but as long as we have less, its very efficient! 
    long setOfNodesLong = 0;
    for (int node : setOfNodes){
        if (MACToLong.find(node) != MACToLong.end())
        {
            setOfNodesLong += MACToLong[node];
        }
    }
    return setOfNodesLong;
}

list<int> ODAR::longToNodelist(long setOfNodesLong)
{
    // ids of nodes in setOfNodes is converted to a single long, 
    // where each node is represented by a "1" at position corresponding to its id in the binary representation of that long.
    // if we have more than 64 neighbors, this will fail. :(
    // but as long as we have less, its very efficient! 
    list<int> listOfNodes;

    while (setOfNodesLong != 0)
    {
        long nodeLong = setOfNodesLong & ~(setOfNodesLong - 1);
        setOfNodesLong -= nodeLong;
        listOfNodes.push_back(longToMAC[nodeLong]);
    }
    return listOfNodes;
}

void ODAR::updateReceiverList()
{
    receivers.clear();
    for (int neighbor : neighbors)
    {
        if (neighborHopCounts[neighbor] < hopCount)
        {
            receivers.push_back(neighbor);
        }
    }
}

void ODAR::findCliques() {
    // find all maximal cliques among this nodes neighbors.
    // result is written in list<long> cliques;
    // neighbors of this node are all nodes in the neighbor list.
    // neighbors of neighbors must be neighbored in both directions.

    map<long,long> twoHopNeighborhoods_byLong_dir2;
    map<long,long> twoHopNeighborhoods_byLong_bidirectional;
    for (const auto &p : twoHopNeighborhoods_byLong)
    {
        long id1 = p.first;
        long neighborsLong = p.second;
        while (neighborsLong > 0){
            long id2 = neighborsLong & ~(neighborsLong - 1);
            neighborsLong -= id2;
            if (twoHopNeighborhoods_byLong.find(id2) != twoHopNeighborhoods_byLong.end())
            {
                if (twoHopNeighborhoods_byLong_dir2.find(id2) != twoHopNeighborhoods_byLong_dir2.end())
                {
                    twoHopNeighborhoods_byLong_dir2.insert({id2, 0});
                }
                twoHopNeighborhoods_byLong_dir2[id2] += id1;
            }
        }
    }

    for (const auto &p : twoHopNeighborhoods_byLong)
    {
        long id = p.first;
        if (twoHopNeighborhoods_byLong_dir2.find(id) != twoHopNeighborhoods_byLong_dir2.end())
        {
            long bidirectionalNeighborhood = twoHopNeighborhoods_byLong_dir2[id] & twoHopNeighborhoods_byLong[id];
            twoHopNeighborhoods_byLong_bidirectional[id] = bidirectionalNeighborhood & (~id);
        }
    }
    cliques_long.clear();
	bronKerbosch(0, nodesetToLong(neighbors), 0, twoHopNeighborhoods_byLong_bidirectional);

    cliques.clear();
    for (long c : cliques_long)
    {
        list<int> clique = longToNodelist(c);
        cliques.push_back(clique);
    }
    /*
    trace() << "all cliques:";
    for (list<int> c : cliques)
    {
        trace() << "next clique";
        for (int n : c){
            long l = twoHopNeighborhoods_byLong[MACToLong[n]];
            list<int> ns = longToNodelist(l);
            string s = "";
            for (int nss : ns){
                s = s + std::to_string(nss) + ", ";
            }
            trace() << n << " (has neighbors: " << s << ")";

        }
    }
    */
}



string ODAR::longToBitString(long nodes)
{
    string s = "";
    int i = 63;
    while (i > 0)
    {
        if (nodes%2 == 0)
        {
            s  += "0 ";
        }
        else
        {
            s  += "1 ";
        }
        nodes >>=1;
        i -= 1;
    }
    return s;
}

void ODAR::bronKerbosch(long r, long p, long x, map<long,long> N) {
    // recursive function to find all maximal cliques
    // https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
    // if P and X are both empty then
    if (p == 0 & x == 0) {
        // report R as a maximal clique
        cliques_long.push_back(r);
        // trace() << "clique:: "<<longToBitString(r);
        return;
    }
    // choose a pivot vertex u in P ⋃ X
    long u = (p | x) & ~((p | x) - 1); // returns the lowest bit of (p | x)

    // for each vertex v in P \ N(u) do
    long toCheck = p & (~ N[u]);  // P \ N(u)
    while(toCheck != 0) {
        long v = toCheck & ~(toCheck - 1); // returns the lowest bit of toCheck; this is "v" 
        toCheck -= v; // removes v from toCheck
        // BronKerbosch2(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
        bronKerbosch(r | v, p & N[v], x & N[v], N);
        // P := P \ {v}
        p = p - v;
        // X := X ⋃ {v}
        x = x | v;
    }
}




void ODAR::broadcastControl()
{
    ODARPacket *netPacket = new ODARPacket("ODAR control packet", NETWORK_LAYER_PACKET);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_CONTROL_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setHopcount(hopCount);
    netPacket->setSequenceNumber(currentSequenceNumber++);

    std::map<int,int> ohc;
    for ( const auto &p : rxCount)
    {
        ohc.insert({p.first, p.second});
    }
    
    std::set<int> nei;
    for ( const auto &p : txSuccessRates)
    {
        if (p.second > neighborSuccRateThreshold)
        {
            nei.insert(p.first);
        }
    }
    OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
    int selfMacAdress = omac->getMacAdress();
    nei.insert(selfMacAdress);

    /*
    double *c;
	c = new double[cdfSlots]{0};
    for (int i = 0; i < cdfSlots; i++){
        c[i] = CDF_calculation[i];
    }
    cfp.setDoubleArray(c);
    */

    CFP cfp;
    cfp.setMapIntInt(ohc);
    cfp.setSetInt(nei);
    
    netPacket -> setOverheardPackets(cfp);
    netPacket -> setNeighbors(cfp);
    netPacket -> setCDF(cfp);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}



void ODAR::finish()
{
    trace() << "----------------------ODAR Network monitoring infos----------------------";

    string str = "";
    trace() << "hop count " << hopCount;

    str = "neighbors: ";
    for (int neighbor : neighbors)
    {
        str = str + " " + std::to_string(neighbor);
    }

    trace() << str;

    str = "receivers: ";
    for (int receiver : receivers)
        str = str + " " + std::to_string(receiver);
    trace() << str;

    trace() << "hopcount cout: " << hopCountCount;
    trace() << "sent packets: " << pktCount;
    trace() << "sent packets to App layer: " << pktCountToApp;

    str = "seen packets: ";
    for (int* p : monitoring_seenPackets)
    {
        

        //p[0]=atoi(netPacket->getSource());
        //p[1]=netPacket->getPacketId();
        //p[2]=(int)(getClock().dbl() * 1000);

        int src = p[0];
        int id = p[1];
        int time = p[2];
        //trace() << "pckt:: "<< time << " " << id << " "  << src << " ";
        str = str + " " + std::to_string(time);
        str = str + " " + std::to_string(id);
        str = str + " " + std::to_string(src);
    }
    //trace() << str;
}


