#include "ODAR.h"

Define_Module(ODAR);

void ODAR::startup()
{
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
    if (appModule->hasPar("isSink"))
        isSink = appModule->par("isSink");
    else
        opp_error("\nODAR has to be used with an application that defines the parameter isSink");

    if (isSink)
        hopCount = 0;
    else
        hopCount = INT_MAX;

    hopCountPeriod = ((double)par("hopCountPeriod")) / 1000.0;

    // start with broadcasting hop count
    setTimer(BROADCAST_HOPCOUNT, hopCountPeriod);
}
void ODAR::timerFiredCallback(int timer)
{
    switch (timer)
    {
    case BROADCAST_HOPCOUNT:
    {
        hopCountCount++;
        broadcastHopCount();
        setTimer(BROADCAST_HOPCOUNT, hopCountPeriod);
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
        // trace() << "receiver list is empty";
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

void ODAR::fromMacLayer(cPacket *pkt, int srcMacAddress, double rssi, double lqi)
{

    if (neighbors.find(srcMacAddress) == neighbors.end())
    {
        neighbors.insert(srcMacAddress);
        neighborHopCounts.insert({srcMacAddress, INT_MAX});
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
                // trace() << "Packet ID " << netPacket->getPacketId() << "   SUCCESSFUL";
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

        case OMAC_ROUTING_HOPCOUNT_PACKET:
        {
            int srcHopCount = netPacket->getHopcount();
            neighborHopCounts[srcMacAddress] = srcHopCount;
            if (srcHopCount < hopCount)
            {
                hopCount = srcHopCount + 1;
                updateReceiverList();
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

void ODAR::broadcastHopCount()
{
    ODARPacket *netPacket = new ODARPacket("ODAR hopcount packet", NETWORK_LAYER_PACKET);
    netPacket->setOMacRoutingKind(OMAC_ROUTING_HOPCOUNT_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setHopcount(hopCount);
    netPacket->setSequenceNumber(currentSequenceNumber++);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}

void ODAR::finish()
{
    trace() << "----------------------ODAR Network monitoring infos----------------------";

    string str = "";
    trace() << "hop count " << hopCount;

    str = "neighbors: ";
    for (int neighbor : neighbors)

        str = str + " " + std::to_string(neighbor);

    trace() << str;

    str = "receivers: ";
    for (int receiver : receivers)
        str = str + " " + std::to_string(receiver);
    trace() << str;

    trace() << "hopcount cout: " << hopCountCount;
    trace() << "sent packets: " << pktCount;
    trace() << "sent packets to App layer: " << pktCountToApp;
}
