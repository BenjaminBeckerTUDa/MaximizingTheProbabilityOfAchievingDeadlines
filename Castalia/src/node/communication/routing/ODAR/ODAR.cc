#include "ODAR.h"

Define_Module(ODAR);

void ODAR::startup()
{
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
    if (appModule->hasPar("isSink"))
        isSink = appModule->par("isSink"); // one node is used as a sink node
    else
        opp_error("\nODAR has to be used with an application that defines the parameter isSink");

    if (appModule->hasPar("packetSize"))
            packetSize = (int) appModule->par("packetSize"); // used to calculate the transmission delay
    else
        opp_error("\nODAR has to be used with an application that defines the parameter packetSize");

    if (appModule->hasPar("maxTTD"))
            maxTTD = (int) appModule->par("maxTTD"); // maximal time to deadline; used to calculate the time per slot in the CDF
    else
        opp_error("\nODAR has to be used with an application that defines the parameter maxTTD");


    cdfSlots = (int) par("cdfSlots"); // number of slots used for the CDF; granularity, which changes the accuracy of deadline-achievement-probability-predictions

    // no previous PDR/CDF broadcast has taken place, since we are starting up
    lastPdrBroadcast = -1;
    lastCdfBroadcast = -1;
    sum_cdf_abs_differences = 0;
    packets_since_cdf_broadcast = 0;
    CDF_transmitted = new double[cdfSlots]{0};

    currRound = 0; // rounds are used to avoid the count to infinity problem:
    /*
    Every node starts with a round with the value 0 and encloses it with every control-message that is transmitted. The sink increments its own round in intervals of hopCountPeriod*3. This increment propagates through the network in a flooding fashion as follows when control-messages are exchanged. Each time a node receives a new control-message from a neighbor, it inspects the enclosed round and acts accordingly as follows: 

    If the received round is smaller than the own round, then the received CDF is considered outdated and ignored.

    If the received round is larger than the own round, then:
      - the own round is updated to the received round 
      - a new empty routing table (routingTable_calculation) is allocated (but not yet used). This table will be updated several times (steps explained below) and will become operational (i.e. used for forwarding decisions) the next time a sequence number larger than the own is received (when these steps here are applied the next time and a new routingTable_calculation is allocated).
      - all locally stored CDFs of neighbors and the nodes own CDF are erased.
      - what is not implemented yet, but should be done: all link succes-rates are calculated from the current histograms.

    In the previous case, as well as in case the received round is equal to the own number, the node-CDF is locally updated
    */

    hopCountPeriod = ((double)par("hopCountPeriod")) / 1000.0; // fixed time-intervals, in which control messages are broadcast

    minHopOnly = false; // set this parameter to true, to use minHop instead of ODAR
    /*
    minHop: use all nodes as forwarding set, which have a smaller hop-count towards the sink than the current node.
    */
    

    if (isSink)
    {
        hopCount = 0; 
        CDF_calculation = new double[cdfSlots]{0};
        for (int i = 0; i < cdfSlots; i++)
        {
            CDF_calculation[i]=1; // the sinks CDF is set to 1 for each time-to-deadline
        }
        setTimer(INC_ROUND, hopCountPeriod*3); // the sink increments its round in fixed time-intervals
    }
    else
    {
        hopCount = INT_MAX;
        CDF_calculation = new double[cdfSlots]{0}; // the nodes CDF is initialized by 0 for each time-to-deadline
        list<int> x;
        for (int i = 0; i < cdfSlots; i++){
            routingTable_calculation.insert({i, x}); // routing table is empty; see "currRound" for more information
            routingTable_inUse.insert({i, x}); // routing table is empty; see "currRound" for more information
        }
    }
    
    setTimer(BROADCAST_CONTROL, hopCountPeriod); // control messages are broadcast in fixed time-intervals
    setTimer(REQUEST_TIMES_FROM_MAC, .1); // get information from MAC (e.g., time between transmissions, time for each transmission etc.) after 0.1 seconds. so we can be sure, that the MAC is initialized.
    
    // offset to prevent collisions due to synchronous clocks
    double offset = ((double) rand() / (RAND_MAX)) / 10;
    double offset2 = ((double) rand() / (RAND_MAX)) / 10;

    if(resilientVersion) {
        setTimer(PDR_BROADCAST, offset);
        setTimer(CDF_BROADCAST, offset2);
    }
    
    createMask(0); // the mask is used for convolution in ODAR::convoluteCDFAtSinglePosition; for more information, see comments there.
    
    potentialReceiverSetsStrategy = CLIQUES; // we can restrict the allowed sets of forwarding candidates
    /*
    two strategies are implemented, to define the possible sets of receivers we consider:
    NONE: potential forwarding sets are arbitrary
    CLIQUES: nodes in potential forwarding sets must be neighboured to each other
    */
    
    neighborSuccRateThreshold = .8; // a neighbor for mode "CLIQUES" is defined as any node, where the packet success rate is above "neighborSuccRateThreshold"
}



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
        case INC_ROUND:
        {
            currRound++;
            setTimer(INC_ROUND, hopCountPeriod*10);
            break;
        }
        case PDR_BROADCAST:
        {
            checkPdrBroadcast();
            setTimer(PDR_BROADCAST, 1);
            break;
        }
        case CDF_BROADCAST:
        {
            checkCdfBroadcast();
            setTimer(CDF_BROADCAST, 1);
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
    if (isSink || receiversByHopcount.empty())
    {
        return;
    }

    DeadlinePacket *dlp = dynamic_cast<DeadlinePacket *>(pkt);
	if (!dlp)
	{
		return;
	}
    double deadline = dlp->getDeadline();
    double ttd = deadline - getClock().dbl()*1000;
    int slot = (int) (ttd/maxTTD * (cdfSlots-1));
    list<int> receivers = routingTable_inUse[slot];

    if (receivers.empty()) // use minhop if no receivers
    {
        receivers = receiversByHopcount;
    }
    if (minHopOnly){
        receivers = receiversByHopcount;
    }
    if (receivers.empty())
    {
        return;
    }

    // create unique data packet ID
    packetNumber++;
    unsigned int packetId = packetNumber * 100000 + atoi(SELF_NETWORK_ADDRESS);
    ReceiversContainer receiversContainer;
    receiversContainer.setReceivers(receivers);
    

    ODARPacket *netPacket = new ODARPacket("ODAR packet", NETWORK_LAYER_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_DATA_PACKET);
    netPacket->setPacketId(packetId);

    netPacket->setDeadline(deadline);

    netPacket->setReceiversContainer(receiversContainer);

    if(this->resilientVersion) {
                
        // remove all transmission time entries older than 180 seconds
        std::vector<long>::iterator it = dataTransmissionTimes.begin();
        int now = int(getClock().dbl());
        while(it != dataTransmissionTimes.end()){
            if ((now - 180) > *it){
                it = dataTransmissionTimes.erase(it);
            } else {
                it++;
            }
        }

        // count transmissions
        int packetCount = dataTransmissionTimes.size() + 1;

        // put count in packet
        netPacket->setPacketCounter(packetCount);

        trace() << "node " << SELF_MAC_ADDRESS << " sent packet with packet counter " << packetCount << " and packet ID " << packetId;
    }

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
            // converting the times to [ms]
            timeForACK = oc->getRequiredTimeForACK().dbl()*1000;
            timeForDATA = oc->getRequiredTimeForDATA().dbl()*1000;
            maxTimeForCA = oc->getMaxTimeForCA().dbl()*1000;
            minTimeForCA = oc->getMinTimeForCA().dbl()*1000;
            break;
        }
        case INC_RCV:
        {
            int srcMacAddress = oc -> getSenderAddress();
            
            if(this->resilientVersion) {
                // read piggybacked packet counters
                int packetCounter = oc->getPacketCounter();
                
                OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
                int selfMacAdress = omac->getMacAdress();
                //trace() << "node " << selfMacAdress << " heard data packet from node " << srcMacAddress << " with packet counter " << packetCounter;

                // store srcmac + timestamp of received message in dataReceivedTimes
                int time = int(getClock().dbl());
                if(dataReceivedTimes.find(srcMacAddress) == dataReceivedTimes.end()) {
                    dataReceivedTimes.insert(pair<int,vector<long> >(srcMacAddress, vector<long>()));
                }
                dataReceivedTimes[srcMacAddress].push_back(time);

                // remove all received packet timestamps older than 180s
                for(auto& [key, value]: dataReceivedTimes) {
                    std::vector<long>::iterator it = value.begin();
                    int now = int(getClock().dbl());
                    while(it != value.end()){
                        if ((now - 180) > *it){
                            it = value.erase(it);
                        } else {
                            it++;
                        }
                    }
                }
                
                int receivedPackets = dataReceivedTimes[srcMacAddress].size();

                // calculate PDR
                double pdr = (double)receivedPackets / (double)packetCounter;

                // calculate new PDR broadcast times
                calculatePdrBroadcastTimes(pdr, srcMacAddress);

                // logging only
                trace() << "node " << selfMacAdress << " overheard packet from node " << srcMacAddress << " and calculated PDR of " << pdr << " (" << receivedPackets << " / " << packetCounter << ")";
                
            }

            if (rxCount.find(srcMacAddress) == rxCount.end()) // this block for original protocol
            {
                rxCount.insert({srcMacAddress, 0});
            }
                rxCount[srcMacAddress]++;
            break;
        }
        case INC_TX:
        {
            txCount ++; // not needed for resilient protocol version

            // store unix timestamp of sent packets
            int time = int(simTime().dbl());
            dataTransmissionTimes.push_back(time);

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
        
        // initialize PDR to 1
        currentPdrs.insert({srcMacAddress, 1});

        int id = MACToLong.size();
        if (id > 63)
        {
            trace() << "we have more than 63 neighbors :("; 
        }
        long macAsLong = 1L << id;  

        createMask(id+1); // the mask is used for convolution in ODAR::convoluteCDFAtSinglePosition

        MACToLong.insert({srcMacAddress, macAsLong});
        longToMAC.insert({macAsLong, srcMacAddress});
        
        neighborCDFs_byMAC.insert({srcMacAddress, new double[cdfSlots]{0}});

        neighborConvolutedCDFs_withoutACKs_byMAC.insert({srcMacAddress, new double[cdfSlots]{0}});
        neighborConvolutedCDFs_withACKs_byMAC.insert({srcMacAddress, new double[cdfSlots]{0}});
        
    }

    ODARPacket *netPacket = dynamic_cast<ODARPacket *>(pkt);

    if (netPacket)
    {
        switch (netPacket->getOMacRoutingKind())
        {
        case OMAC_ROUTING_DATA_PACKET:
        {
            double deadline = netPacket->getDeadline();
            double ttd = deadline - getClock().dbl()*1000;
            if (ttd < 0){
                deadlineExpiredCount++;
                break;
            }

            if (isSink)
            {
                toApplicationLayer(decapsulatePacket(pkt));
                pktCountToApp++;
            }
            else
            {
                int slot = (int) (ttd/maxTTD * (cdfSlots-1));
                list<int> receivers = routingTable_inUse[slot];
                if (receivers.empty()) // use minhop if no receivers
                {
                    receivers = receiversByHopcount;
                }
                if (minHopOnly){
                    receivers = receiversByHopcount;
                }              
                if (receivers.empty())
                {
                    break;
                }

                OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
                int selfMacAdress = omac->getMacAdress();
                if (selfMacAdress == 21)
                {
                    string str1 = "receivers by odar: ";
                    for (int i : routingTable_inUse[slot])
                    {
                        str1 += std::to_string(i) + ", ";
                    }
                    str1 += " minhop: ";
                    for (int i : receiversByHopcount)
                    {
                        str1 += std::to_string(i) + ", ";
                    }
                    //trace() << str1 << "ttd" << ttd;
                }

                //trace() << "ttd " << ttd << "\tslot " << slot << "\tlengths of minhop vs our approach: " << receiversByHopcount.size() << " <min vs our> " << routingTable_inUse[slot].size();


                
                ReceiversContainer receiversContainer;
                receiversContainer.setReceivers(receivers);
                ODARPacket *dupPacket = netPacket->dup();
                dupPacket->setSource(SELF_NETWORK_ADDRESS);
                dupPacket->setSequenceNumber(currentSequenceNumber++);
                dupPacket->setReceiversContainer(receiversContainer);

                if(this->resilientVersion) {
                
                    // remove all transmission time entries older than 180 seconds
                    std::vector<long>::iterator it = dataTransmissionTimes.begin();
                    int now = int(getClock().dbl());
                    while(it != dataTransmissionTimes.end()){
                        if ((now - 180) > *it){
                            it = dataTransmissionTimes.erase(it);
                        } else {
                            it++;
                        }
                    }

                    // count transmissions
                    int packetCount = dataTransmissionTimes.size() + 1;

                    // put count in packet
                    dupPacket->setPacketCounter(packetCount);

                    trace() << "node " << SELF_MAC_ADDRESS << " forwarded packet with packet counter " << packetCount << " and packet ID " << dupPacket->getPacketId();
                    
                    double difference = abs(CDF_transmitted[slot] - CDF_calculation[slot]);
                    sum_cdf_abs_differences += difference;
                    packets_since_cdf_broadcast++;
                }

                toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS); // sure?
            }
            break;
        }
        case OMAC_ROUTING_PDR_PACKET:
        {
            // get own MAC address
            OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
            int selfMacAdress = omac->getMacAdress();

            // retrieve relevant PDR from packet
            CFP cfp = netPacket->getPdr();
            map<int, double> receivedPdrs = cfp.getMapIntDouble();

            trace() << selfMacAdress << " received PDR packet from node " << srcMacAddress << ": " << receivedPdrs[selfMacAdress];

            if (txSuccessRates.find(srcMacAddress) == txSuccessRates.end()){
                txSuccessRates.insert({srcMacAddress, 0});
            }
            txSuccessRates[srcMacAddress] = receivedPdrs[selfMacAdress];

            calculateCDF();
            break;
        }
        case OMAC_ROUTING_CDF_PACKET:
        {
            // next two lines for logging only
            OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
            int selfMacAdress = omac->getMacAdress();
            trace() << selfMacAdress << " received CDF from node " << srcMacAddress;

            /*
            "Die IDs aller ihm bekannten Nachbarn."
            */
            long idLong = MACToLong[srcMacAddress];
            if (twoHopNeighborhoods_byLong.find(idLong) == twoHopNeighborhoods_byLong.end())
            {
                twoHopNeighborhoods_byLong.insert({idLong, 0L});
            }

            /*
            "Bilde aus Knoten Mengen von Knoten, welche gegenseitig benachbart sind."
            */
            long neighborsAsLong = nodesetToLong(netPacket -> getNeighbors().getSetInt());
            if (twoHopNeighborhoods_byLong[idLong] != neighborsAsLong)
            {
                
                twoHopNeighborhoods_byLong[idLong] = neighborsAsLong;
                findPotentialReceiverSets();
                //findCliques();
            }

            /*
            CDFs und routing table berechnen
            */
            int r = netPacket -> getRound();
            if (r < currRound)
            {
                break;
            }
            if (r > currRound)
            {
                currRound = r;
                for ( const auto &p : neighborCDFs_byMAC)
                {
                    neighborCDFs_byMAC[p.first] = new double[cdfSlots]{0};
                }

                for (int i = 0; i < cdfSlots; i++){
                    list<int> y;
                    for (int node : routingTable_calculation[i])
                    {
                        y.push_back(node);
                    }
                    routingTable_inUse.insert({i, y});
                    routingTable_inUse[i] = y;

                    list<int> x;
                    routingTable_calculation.insert({i, x});
                    CDF_calculation[i] = 0;
                }
            }

            if (r == currRound)
            {
                neighborCDFs_byMAC[srcMacAddress] = netPacket -> getCDF().getDoubleArray();
                calculateCDF();
            } 
            break;
        }

        case OMAC_ROUTING_CONTROL_PACKET:
        {
            if (isSink)
            {
                break;
            }

            /*
            For Min-Hop
            */
            int srcHopCount = netPacket->getHopcount();
            neighborHopCounts[srcMacAddress] = srcHopCount;
            if (srcHopCount < hopCount)
            {
                hopCount = srcHopCount + 1;
                updateReceiverList();
            }
            /*
            "Die IDs aller ihm bekannten Nachbarn."
            */
            long idLong = MACToLong[srcMacAddress];
            if (twoHopNeighborhoods_byLong.find(idLong) == twoHopNeighborhoods_byLong.end())
            {
                twoHopNeighborhoods_byLong.insert({idLong, 0L});
            }

            /*
            "Bilde aus Knoten Mengen von Knoten, welche gegenseitig benachbart sind."
            */
            long neighborsAsLong = nodesetToLong(netPacket -> getNeighbors().getSetInt());
            if (twoHopNeighborhoods_byLong[idLong] != neighborsAsLong)
            {
                
                twoHopNeighborhoods_byLong[idLong] = neighborsAsLong;
                findPotentialReceiverSets(); 
                //findCliques();
            }

            /*
            For txSuccessRate
            "Die von jedem einzelnen seiner Nachbarn erfolgreich gehörten Datenpakete."
            Die Übertragungswahrscheinlichkeit eines Knotens x zu einem Nachbarn y berechnet sich als „von y
            erfolgreich gehörten Übertragungen von x“ / „Übertragungsversuche von Datenpaketen durch x“.
            */
            map<int,int> ohc = netPacket -> getOverheardPackets().getMapIntInt();
            OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
            int selfMacAdress = omac->getMacAdress();
            if (ohc.find(selfMacAdress) != ohc.end())
            {
                double txSuccessRate = (double)ohc[selfMacAdress] / (double)txCount;
                if (txSuccessRates.find(srcMacAddress) == txSuccessRates.end()){
                    txSuccessRates.insert({srcMacAddress, 0});
                }
                txSuccessRates[srcMacAddress] = txSuccessRate;
            }
            break;

            /*
            CDFs und routing table berechnen

            */
            int r = netPacket -> getRound();
            if (r < currRound)
            {
                break;
            }
            if (r > currRound)
            {
                currRound = r;
                for ( const auto &p : neighborCDFs_byMAC)
                {
                    neighborCDFs_byMAC[p.first] = new double[cdfSlots]{0};
                }

                for (int i = 0; i < cdfSlots; i++){
                    list<int> y;
                    for (int node : routingTable_calculation[i])
                    {
                        y.push_back(node);
                    }
                    routingTable_inUse.insert({i, y});
                    routingTable_inUse[i] = y;

                    list<int> x;
                    routingTable_calculation.insert({i, x});
                    CDF_calculation[i] = 0;
                }
            }

            if (r == currRound)
            {
                neighborCDFs_byMAC[srcMacAddress] = netPacket -> getCDF().getDoubleArray();
                calculateCDF();
            }     

        }

        default:
        {
            break;
        }
        }
    }
}




void ODAR::calculateCDF()
{
    // convolute all CDFs, assuming a single ACK
    for (const auto &p : neighborCDFs_byMAC)
    {
        neighborConvolutedCDFs_withoutACKs_byMAC[p.first] = convoluteCDF(p.second, 1);
    }

    double * newCDF = new double[cdfSlots]{0}; //hiermit arbeiten, verändern, etc...

    OMAC *omac = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC"));
    int selfMacAdress = omac->getMacAdress();
    bool show = false;//selfMacAdress == 6 && getClock().dbl() > 900;

   
    for (int slot_x = 0; slot_x < cdfSlots; slot_x++){
        double maxProb = 0.0;
        list<int> maxReceivers;
        // checking each single clique for the deadline achievement probability when using it
        for (list<int> clique : cliques)
        {   
            list <int> candidateList;
            // filter out all nodes, which have a lower probability than our own node

            for (int mac : clique)
            {
                if (neighborConvolutedCDFs_withoutACKs_byMAC.find(mac) != neighborConvolutedCDFs_withoutACKs_byMAC.end())
                {
                    //if (neighborConvolutedCDFs_withoutACKs_byMAC[mac][slot_x] > ownConvolutedCDFs_withACKs[1][slot_x])
                    if (neighborConvolutedCDFs_withoutACKs_byMAC[mac][slot_x] > convoluteCDFAtSinglePosition(CDF_calculation,1,slot_x))
                    {
                        candidateList.push_back(mac);
                    }
                }
            }

            

            if (candidateList.size() > 0)
            {
                // for remaining nodes, we convolute again, using the required number of ACKs
                double ownProb = convoluteCDFAtSinglePosition(CDF_calculation,candidateList.size(),slot_x);
                map<double , int> prob_to_mac;
                for (int mac : candidateList)
                {
                    double neighborProb = convoluteCDFAtSinglePosition(neighborCDFs_byMAC[mac], candidateList.size(),slot_x);
                    // maps are already sorted by key; we just insert 1-prob, just to get the right sorting...
                    prob_to_mac.insert({1-neighborProb, mac});

                   
                }

                // calculation of probability, when using the neighbors in that order
                double totalProb = 0;
                double probOfFirst = 1;
                for (const auto &p : prob_to_mac)
                {
                    int mac = p.second;
                    double prob = 1 - p.first;             
                    totalProb += probOfFirst * txSuccessRates[mac] * prob;
                    probOfFirst = probOfFirst * (1 - txSuccessRates[mac]);
                }


                totalProb += probOfFirst * ownProb;
                OMAC *omacInstance = dynamic_cast<OMAC*> (getParentModule()->getSubmodule("MAC"));
				double channelBusy = omacInstance->getChannelBusyCount();
                double channelClear = omacInstance->getChannelClearCount();
                double CBR = channelBusy / (channelClear + channelBusy);

                totalProb *= (1-CBR);
                totalProb += CBR * ownProb;

                if (totalProb > maxProb)
                {
                    // we got a winner!
                    maxProb = totalProb;
                    maxReceivers = candidateList;
                }
            }            
        }
        routingTable_calculation[slot_x] = maxReceivers;
        CDF_calculation[slot_x] = maxProb;
        if (show)
        {
            trace() << "MAXPROB::  " << maxProb;
            string str4 = "RECEIVERS:: ";
            for (int i : maxReceivers){
                str4 += std::to_string(i) + ", ";
            }
            trace() << str4;
        }

    }
}

void ODAR::createMask(int nrOfACKs)
{
    // create mask, to convolute with
    // the mask basically looks like a rectangle:
    // ^
    // |
    // |     __________
    // |    | area = 1 |
    // +--------------------------->
    //    mindelay   maxdelay
    //
    // and this mask is convoluted with the CDF of a node; 
    // this process corresponds to the "-2ms" in "CDF_v(x-2ms)"; but instead of using a constant time (e.g. "-2ms"), we create this mask, which takes the randomness of the delays into account 
    double timePerSlot = (double) maxTTD / (double) cdfSlots; 
    double minTimeDelay = timeForACK * nrOfACKs+timeForDATA+minTimeForCA;
    double maxTimeDelay = timeForACK * nrOfACKs+timeForDATA+maxTimeForCA;
    double a = minTimeDelay / timePerSlot;
    double b = maxTimeDelay / timePerSlot;
    int s = (int) a;
    int slots = (int) b - (int) a + 2;
    double * mask = new double[slots]{0};
    for (int i = 0; i < slots; i++)
    {
        mask[i] = 1;
    }
    double x = int(a) + 1 - a;
    mask[0] = x*x/2;
    mask[1] = 1-((1-x)*(1-x)/2);
    double y = b - (int) b;
    mask[slots-1] = y*y;
    mask[slots-2] = 1-((1-y)*(1-y)/2);
    double sum = 0;
    for (int i = 0; i < slots; i++)
    {
        sum += mask[i];
    }
    for (int i = 0; i < slots; i++)
    {
        mask[i]/=sum;
    }
    Mask m;
    m.setMask(mask);
    m.setShift(s);
    m.setSlots(slots);
    masks.insert({nrOfACKs, m});
}


double ODAR::convoluteCDFAtSinglePosition(double * nodeCDF, int nrOfACKs, int slotNr)
{
    // convolutes a CDF of a neighboring node with the PDF of the delays, which occur in the MAC.
    // very similar to ODAR::convoluteCDF(double * nodeCDF, int nrOfACKs), but here we only calculate it for a single value for x.
    Mask m = masks[nrOfACKs];
    double * mask = m.getMask();
    int slots = m.getSlots();
    int s = m.getShift();
    // convolute the input CDF with the mask
    double result = 0;
    for (int i = 0; i < slots; i++)
    {
        if (slotNr - i - s >= 0){
            result += nodeCDF[slotNr - i - s] * mask[i];
        }   
    }
    return result;
}


double *  ODAR::convoluteCDF(double * nodeCDF, int nrOfACKs)
{
    // convolutes a CDF with the expected delays, which occur in the MAC.
    // this process corresponds to "CDF_v(x-2ms)" in the document; but instead of "CDF_v(x-2ms)" we calculate "CDF_v (convolute with) PDF (x)"
    // delays are: timeForACK, timeForDATA (transmission delay), timeForCA
    // nrOfACKs is required because different sizes of receiver-lists lead to different durations for ACKs
    if (!nodeCDF)
    {
        return new double[cdfSlots]{0};
    }
    // create mask, to convolute with
    Mask m = masks[nrOfACKs];
    double * mask = m.getMask();
    int slots = m.getSlots();
    int s = m.getShift();

    

    // convolute the input CDF with the mask
    double* convolutedCDF = new double[cdfSlots]{0};
    if (nodeCDF[0] != nodeCDF[cdfSlots-1])
    {
        // to avoid unnecessary computations, skip leading 0s and trailing equal values
        int start = 0;
        int end = cdfSlots-1;
        while (nodeCDF[start] == 0)
        {
            start ++;
        }
        
        
        while (nodeCDF[end] == nodeCDF[cdfSlots-1])
        {
            end --;
        }
        end++;
        end += slots;

        if (end >= cdfSlots)
            end = cdfSlots;


        for (int i  = start; i < end - s - slots; i++)
        {
            for (int j = 0; j < slots; j++)
            {
            convolutedCDF[i + s + j] += nodeCDF[i] * mask[j];
            }
        }

        
        for (int i  = end - s - slots; i < end; i++)
        {
            for (int j = 0; j < slots; j++)
            {
                if (i + s + j < cdfSlots)
                {
                    convolutedCDF[i + s + j] += nodeCDF[i] * mask[j];     
                }
            }
        }
        
        for (int i = end; i + s < cdfSlots; i++)
        {
            convolutedCDF[i + s] = nodeCDF[end];      
        }
    }
    else
    {
        if (nodeCDF[0] != 0){
            // if all values are equal and > 0, e.g. for sink-cdf, avoid almost all computations
            for (int i = 0; i < slots; i++)
            {
                for (int j = 0; j < slots; j++)
                {
                    convolutedCDF[i + s + j] += nodeCDF[i] * mask[j];
                }
            }
            for (int i  = slots; i < cdfSlots-s; i++)
            {
                convolutedCDF[i + s] = nodeCDF[0];      
            }
        }
        else
        {
            // in this case all values are equal and == 0. we do not have to do anything.
        }
    }
    return convolutedCDF;
}


void ODAR::broadcastControl()
{
    // broadcast of control message, which 
    ODARPacket *netPacket = new ODARPacket("ODAR control packet", NETWORK_LAYER_PACKET);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_CONTROL_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setHopcount(hopCount);
    netPacket->setSequenceNumber(currentSequenceNumber++);

    CFP cfp; //CFP is a wrapper class for all kinds of data-types. It is used inside packets only.

    // 1. Die IDs aller ihm bekannten Nachbarn. (vgl.: „Nachbarschaften“)
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
    cfp.setSetInt(nei); 

    // 2. Sein eigenes CDF (vgl.: „Cumulative Distribution Function (CDF)”)
    double *c;
	c = new double[cdfSlots]{0};

    for (int i = 0; i < cdfSlots; i++){
        c[i] = CDF_calculation[i];
    }
    cfp.setDoubleArray(c); 

    // 3. Die von jedem einzelnen seiner Nachbarn erfolgreich gehörten Datenpakete. (vgl.: „Messung der Übertragungswahrscheinlichkeiten“)
    std::map<int,int> ohc;
    for ( const auto &p : rxCount)
    {
        ohc.insert({p.first, p.second});
    }
    cfp.setMapIntInt(ohc); 


    netPacket -> setOverheardPackets(cfp);
    netPacket -> setNeighbors(cfp);
    netPacket -> setCDF(cfp);
    netPacket -> setRound(currRound);

    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}

/*
Code for ODAR, when potentialReceiverSetsStrategy == NONE
*/
void ODAR::findNeighbors() {
    cliques.clear();
    list<int> n;
    for (int m : neighbors)
    {
        n.push_back(m);
    }
    cliques.push_back(n);
}

/*
Code for ODAR, when potentialReceiverSetsStrategy == CLIQUES
*/
void ODAR::findCliques() {
    // find all maximal cliques among this nodes neighbors.
    // neighbors of this node are all nodes in the neighbor list.

    // result is written in list<long> cliques;

    // neighbors of neighbors must be neighbored in both directions. e.g., nodes a and b are neighbored, if a is in b's neighbor-list and b is in a's neighbor list
    // twoHopNeighborhoods_byLong_bidirectional will be used to calculate this.

    // all calculations are done, using a single long to represent a set of nodes. (performance reasons)
    // e.g. a long = ...001001101 corresponds to nodes with ids (0,2,3,6), since those are the positions of the 1's in the long
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
}


void ODAR::calculatePdrBroadcastTimes(double pdr, long srcMacAddress)
{
    // add PDR to currentPdrs
    currentPdrs[srcMacAddress] = pdr;

    // if there has been no previous broadcast of the source node's PDR, set its transmitted PDR to zero for calculating difference
    if(transmittedPdrs.count(srcMacAddress) == 0) {
        transmittedPdrs[srcMacAddress] = 0;
    }

    // calculate difference to previous PDR
    double pdr_difference = fabs(transmittedPdrs[srcMacAddress] - pdr);

    // calculate PDR broadcast timer
    int result;
    if(pdr_difference == 0) {
        result = 600;
    } else {
        result = std::min((double)600, 6/pdr_difference);
    }
    
    long timer;

    if(lastPdrBroadcast == -1) {
        int now = int(getClock().dbl());
        lastPdrBroadcast = now;
    }
    
    //trace() << "last broadcast: " << lastPdrBroadcast << ", result: " << result;

    timer = lastPdrBroadcast + result;

    // store broadcast timer
    pdrBroadcastTimes[srcMacAddress] = timer;

    //trace() << "setting timer to: " << timer;
}


void ODAR::checkPdrBroadcast(){
    int now = int(getClock().dbl());
    
    if(pdrBroadcastTimes.size() == 0) {
        return;
    }

    for (auto const& x : pdrBroadcastTimes) {
        if (x.second < now) {
            //trace() << "timer: " << x.second << ", now: " << now;
            executePdrBroadcast();
            return;
        }
    }
}

void ODAR::executePdrBroadcast()
{
    // assemble broadcast packet
    ODARPacket *netPacket = new ODARPacket("ODAR pdr packet", NETWORK_LAYER_PACKET);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_PDR_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setSequenceNumber(currentSequenceNumber++);

    CFP cfp;
    cfp.setMapIntDouble(currentPdrs);
    netPacket->setPdr(cfp);
    
    trace() << "node " << SELF_MAC_ADDRESS << " is sending PDR broadcast";
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);

    transmittedPdrs = currentPdrs;

    int now = int(getClock().dbl());
    lastPdrBroadcast = now;

    // set all broadcast times to 10min in the future
    for (auto& x : pdrBroadcastTimes) {
        x.second = now + 600;
    }

}


void ODAR::checkCdfBroadcast()
{
    // calculate avg difference of current and last transmitted CDF
    double avg_difference;
    if(packets_since_cdf_broadcast == 0) {
        avg_difference = 0;
    } else {
        avg_difference = sum_cdf_abs_differences / packets_since_cdf_broadcast;
    }
    
    // calculate CDF broadcast timer
    int result;
    if(avg_difference == 0) {
        result = 600;
    } else {
        result = std::min((double)600, 6/avg_difference);
    }

    int now = int(getClock().dbl());
    if(lastCdfBroadcast == -1) {
        lastCdfBroadcast = now;
    }
    
    long timer = lastCdfBroadcast + result;
    if (timer < now) {
        executeCdfBroadcast();
    }

    return;
}

void ODAR::executeCdfBroadcast()
{
    // assemble broadcast packet
    ODARPacket *netPacket = new ODARPacket("ODAR cdf packet", NETWORK_LAYER_PACKET);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_CDF_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setSequenceNumber(currentSequenceNumber++);

    CFP cfp;

    // 1. Die IDs aller ihm bekannten Nachbarn. (vgl.: „Nachbarschaften“)
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
    cfp.setSetInt(nei); 

    // 2. Sein eigenes CDF (vgl.: „Cumulative Distribution Function (CDF)”)
    double *c;
	c = new double[cdfSlots]{0};

    for (int i = 0; i < cdfSlots; i++){
        c[i] = CDF_calculation[i];
        CDF_transmitted[i] = CDF_calculation[i];
    }
    cfp.setDoubleArray(c); 
    netPacket->setCDF(cfp);
    netPacket->setNeighbors(cfp);

    int now = int(getClock().dbl());
    lastCdfBroadcast = now;

    sum_cdf_abs_differences = 0;
    packets_since_cdf_broadcast = 0;

    netPacket->setRound(currRound);

    trace() << selfMacAdress << " is sending CDF broadcast!";

    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);

    
}


string ODAR::longToBitString(long nodes)
{
    // function to return a string of 0's and 1's, for a long
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

int ODAR::getPacketCreatedCount()
{
    // function used in "void ODAR::finish()" to gather this information at the sink node and trace it
    return pktCount;
}

int ODAR::getPacketDeadlineExpiredCount()
{
    // function used in "void ODAR::finish()" to gather this information at the sink node and trace it
    return deadlineExpiredCount;
}


/*
Code for Tracing at end of simulation
*/


void ODAR::finish()
{
    // this function is used for tracing purposes at the end of simulation only
    if (isSink)
    {
        bool flag = true;
		int id = 1;
		double createdPackets = 0;
        double deadlineExpired = 0;
		while (flag)
		{
			cModule *node_ = getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id);
			if (node_)
			{
				ODAR *odarInstance = dynamic_cast<ODAR*> (getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id)->getSubmodule("Communication")->getSubmodule("Routing"));
				createdPackets += odarInstance->getPacketCreatedCount();
                deadlineExpired += odarInstance->getPacketDeadlineExpiredCount();
				id ++;
			}	
			else
				flag = false;
		}

        flag = true;
		id = 1;
		double channelBusy = 0;
        double channelClear = 0;
        double channelBusyRatio;
        double reachedMaxRetriesCount = 0;

		while (flag)
		{
			cModule *node_ = getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id);
			if (node_)
			{
				OMAC *omacInstance = dynamic_cast<OMAC*> (getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id)->getSubmodule("Communication")->getSubmodule("MAC"));
				channelBusy += omacInstance->getChannelBusyCount();
                channelClear += omacInstance->getChannelClearCount();
                channelBusyRatio += omacInstance->getChannelBusyCount() / (omacInstance->getChannelBusyCount() + omacInstance->getChannelClearCount());
                reachedMaxRetriesCount += omacInstance->getMaxRetriesCount();
				id ++;
			}	
			else
				flag = false;
		}
        channelBusyRatio /=id;

        double arrivedInTime = pktCountToApp;

        trace() << "NETWORK-STATS:";
        if (minHopOnly){
            trace() << "Min-Hop";
        }
        else
        {
            trace() << "ODAR";
        }

        if (potentialReceiverSetsStrategy == NONE){
            trace() << "potentialReceiverSetsStrategy = NONE";
        }

        if (potentialReceiverSetsStrategy == CLIQUES){
            trace() << "potentialReceiverSetsStrategy = CLIQUES";
        }




        trace() << "channelBusy " << channelBusy;
        trace() << "channelClear " << channelClear;
        trace() << "reachedMaxRetriesCount " << reachedMaxRetriesCount;
        trace() << "createdPackets " << createdPackets;
        trace() << "deadlineExpired " << deadlineExpired;
        trace() << "arrivedInTime " << arrivedInTime;
        trace() << "deadlineachievementratio " << arrivedInTime / createdPackets;
        trace() << "channelBusyRatio " << channelBusy / (channelClear + channelBusy);
    }


    string str1 = "receivers for minHop: ";
    str1 = str1 + "[";
    for (int receiver : receiversByHopcount){
        str1 = str1 + " " + std::to_string(receiver);
    }
    str1 = str1 +"] ";
    
    trace() << str1;

    if (!minHopOnly)
    {
        string str1 = "receivers for each TTD: ";
        set<int> allReceivers;
        for (int slot = 0; slot < cdfSlots; slot++){
            str1 = str1 + "[";
            list<int> r = routingTable_inUse[slot];
            for (int receiver : r){
                str1 = str1 + " " + std::to_string(receiver);
                allReceivers.insert({receiver});
            }
            str1 = str1 +"] ";
        }
        trace() << str1;
    }
}

/*
Code for Min-Hop only
*/
void ODAR::updateReceiverList()
{
    receiversByHopcount.clear();
    for (int neighbor : neighbors)
    {
        if (neighborHopCounts[neighbor] < hopCount)
        {
            receiversByHopcount.push_back(neighbor);
        }
    }
}

/*
Code for Min-Hop only
*/
void ODAR::findPotentialReceiverSets() {
    switch (potentialReceiverSetsStrategy)
    {
        // NONE: potential forwarding sets are arbitrary
        // CLIQUES: nodes in potential forwarding sets must be neighboured to each other
        case NONE:
        {
            findNeighbors();
            break;
        }
        case CLIQUES:
        {
            findCliques();
            break;
        }
    }
}