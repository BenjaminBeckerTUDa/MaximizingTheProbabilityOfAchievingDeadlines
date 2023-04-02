#include "AggODAR.h"

Define_Module(AggODAR);

void AggODAR::timerFiredCallback(int timer)
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
        case AGGREGATION_TRIGGER:
        {
            ag_pkt pkt_top = pq.top(); // gets the prio_packet from buffer

            if(!pq.empty()) {
                double deadline = pkt_top.pkt -> getDeadline();
                double timer = this->AggODAR::packet_timer_calc(deadline);

                // Checks if the sending time is reached
                if(timer >= getClock().dbl()*1000) {
                    AggODAR::aggregation();
                }
            }
            break; 
        }
        
        default:
        {
            break;
        }
    }
}

void AggODAR::fromApplicationLayer(cPacket *pkt, const char *destination) {

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

    // create unique data packet ID
    packetNumber++;
    unsigned int packetId = packetNumber * 100000 + atoi(SELF_NETWORK_ADDRESS);

    ODARPacket *netPacket = new ODARPacket("ODAR packet", NETWORK_LAYER_PACKET); 
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_DATA_PACKET);
    netPacket->setPacketId(packetId);

    netPacket->setDeadline(deadline);
    encapsulatePacket(netPacket, pkt);

    AggODAR::toPriorityQueue(netPacket);

    pktCount++;
}

void AggODAR::fromMacLayer(cPacket *pkt, int srcMacAddress, double rssi, double lqi)
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
                
                ODARPacket *dupPacket = netPacket->dup();

                dupPacket->setSource(SELF_NETWORK_ADDRESS);
                dupPacket->setSequenceNumber(currentSequenceNumber++);

                AggODAR::toPriorityQueue(dupPacket);
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
        }

        default:
        {
            break;
        }
        }
    }
}

void AggODAR::toPriorityQueue(ODARPacket *pkt) {

    // get the Deadline
    double deadline = pkt -> getDeadline();
    // Max Waiting Time 
    double maxWT = AggODAR::maxWaitingTime(deadline); 
    // Size of Packet
    double packetsize = sizeof(*pkt);

    if(currentSizeForPA+packetsize >= maxSizeForPA) {
        // aggregate all packets in priority queue
        AggODAR::aggregation();
        // reset current priority queue size
        currentSizeForPA = 0;
    } 

    // Add Size to PQ Size
    currentSizeForPA += packetsize; 

    // formate to used struced in priority queue
    ag_pkt packet;
    packet.pkt = pkt;
    packet.wt = maxWT;

    // queue packet into priority queue
    pq.push(packet);
    // set Timer to max Waiting Time
    setTimer(AGGREGATION_TRIGGER, maxWT);
    
}

void AggODAR::aggregation() {

    // make new Aggregation Packet
    ODARPacket *a_pkt = new ODARPacket("ODAR packet", NETWORK_LAYER_PACKET);

    // Data of Aggregated Packets
    double a_data = 0.0;

    // new sequencenumber
    // a_pkt->setSequenceNumber();

    // set Source
    a_pkt->setSource(SELF_NETWORK_ADDRESS);

    // set Routing Packet kind
    a_pkt->setOMacRoutingKind(OMAC_ROUTING_DATA_PACKET);

    // new Packet ID
    packetNumber++;
    unsigned int packetId = packetNumber * 100000 + atoi(SELF_NETWORK_ADDRESS);   
    a_pkt->setPacketId(packetId);

    // set ByteLength to Size of all Packets in Queue combined
    a_pkt->setByteLength(currentSizeForPA);

    // get top packet
    ag_pkt pkt_top = pq.top();

    // set Deadline to Packet with the lowest TTD
    double deadline = pkt_top.pkt -> getDeadline();
    a_pkt->setDeadline(deadline);

    // set Destination to Packet with the lowest TTD
    const char *destination = pkt_top.pkt -> getDestination();
    a_pkt->setDestination(destination);

    pq.pop();
    
    //Aggregate Packets
    while(!pq.empty()) {

        // get a Packet from list
        pkt_top = pq.top();

        // get Data of Packet
      //  double p_data = pkt_top.pkt -> getData();

        // add Data to Aggregated Data
       // a_data += p_data;
        pq.pop();
    }

    // set Data of Aggregated Packet
  //  a_pkt->setData(a_data);

    fromPriorityQueue(a_pkt);

    // toDo Überprüfen wie genau das aggregierte Paket aussehen soll / wie Deadline paket aussieht (übergang)
}

void AggODAR::fromPriorityQueue(ODARPacket * a_pkt) {

    double deadline = a_pkt->getDeadline();
    double ttd = deadline - getClock().dbl()*1000;
    int slot = (int) (ttd/maxTTD * (cdfSlots-1));
    list<int> receivers = routingTable_inUse[slot];

    if (receivers.empty())
    {
        receivers = receiversByHopcount;
    }
    if (minHopOnly){
        receivers = receiversByHopcount;
    }
    if (receivers.empty())
    {
        return; // nicht break?
    }

    // create unique data packet ID -> kann das drin bleiben (Anders als fromMAC)?
    ReceiversContainer receiversContainer;
    receiversContainer.setReceivers(receivers);

    a_pkt->setReceiversContainer(receiversContainer);

    toMacLayer(a_pkt, BROADCAST_MAC_ADDRESS);    

}

    // ToDo
    // Function used for deaggregation
    // Programm child of deadline packet first
    // It should accept multiple dealines and data
    
double AggODAR::maxWaitingTime(double deadline) {

    double ttd = deadline - getClock().dbl()*1000; // time to deadline
    int slot = (int) (ttd/maxTTD * (cdfSlots-1)); // CDF slot of ttd

    double current_CDF = CDF_calculation[slot]; //CDF probability of ttd 
    double diff_CDF = 0.01; // Wartezeit abhängig von traffic; diff_CDF hoch bei viel traffic; 

   if (current_CDF > diff_CDF) {  
        double mCDF = current_CDF-diff_CDF;
   } else {
        double mCDF = current_CDF;
   }
    
    while (current_CDF >= mCDF)  { // mCDF == the desired CDF probability for calculating the max Waiting time
        slot -= 1;
        current_CDF = CDF_calculation[slot];   
    }
    slot++;

    double maxWT = ttd - maxTTD * (slot/(cdfSlots-1));

    return maxWT;
}

double AggODAR::packet_timer_calc(double deadline) {

    // calculates the send time of the packet
    double ptimer = AggODAR::maxWaitingTime(deadline) + getClock().dbl()*1000;

    return ptimer;
}





/* zusammenfassung todos:

code lauffähig machen:

    max waiting time anpassen (kleinigkeiten?)
    aggregation bedingung implementieren:
        max größe = ... (header)
    trigger für aggregation:
        immer wenn packet in die queue, neuer timer; pseudo-code aus letzter mail
    aggregation:
        zuerst nur: app-packet modifizieren, sodass ein app packet mehreren entspricht; nur eine ttd, eine size (= summe der einzel-paketgrößen)
        dann das paket versenden, mit neuer empfänger-liste
    --> das zum laufen bringen. danach(!):
        aggregation verfeinern, sodass neuer paket-typ, der alle ttds und alle größen enthält, damit pakete wieder disaggregiert werden können.

schreiben:

    design anpassen (wenn code fertig)
    related work fertig

waitingTime(packet p){
    ttd = "current time" - p.deadline
    slot = (formel, die den slot für die ttd des paketes ausrechnet)
    cdf_value = cdf[slot]
    currValue = cdf_value
    currSlot = slot
    
    while (currValue > cdf_value * 0.95)  //oder irgendein anderer threshold statt 0.95
    {
        currSlot--
        currValue = cdf[currSlot]
    }
    currSlot++
    
    slotspan = slot - currSlot
    time = (formel, die aus der slotspan die zeit ausrechnet)
    return time
}


    from application (und from mac):
            "paket kommt rein"
            p = paket
            t = berechnete maximale wartezeit
            size = größe der pakete in der queue + größe von p
            wenn size > threshold ODER t <= 0:
                sende alles
            else:
                sendezeitpunkt = t+"aktuelle zeit"
                enqueue(p, sendezeitpunkt)
                setTimer(QUEUE_TIMER, t)
               
        timerFiredCallback(itn timer)
        {
            switch(timer){
                ...
                case(QUEUE_TIMER{
                    wenn queue != empty
                    {
                        p = queue.front.paket
                        wenn (queue.front.sendezeitpunkt = "aktuelle zeit"):
                            sende alles
                    }
                    break
                }
                ...
            }
        }

*/


