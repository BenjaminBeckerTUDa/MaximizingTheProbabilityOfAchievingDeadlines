#include "SimpleRouting.h"

Define_Module(SimpleRouting);

void SimpleRouting::startup()
{
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
    if (appModule->hasPar("isSink"))
        isSink = appModule->par("isSink");
    else
        opp_error("\nSimple Routing has to be used with an application that defines the parameter isSink");

    if (isSink)
        hopCount = 0;
    else
        hopCount = INT_MAX;

    hopCountPeriod = ((double)par("hopCountPeriod")) / 1000.0;

    // start with broadcasting hop count
    setTimer(BROADCAST_HOPCOUNT, hopCountPeriod);
}
void SimpleRouting::timerFiredCallback(int timer)
{
    switch (timer)
    {
    case BROADCAST_HOPCOUNT:
    {
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

void SimpleRouting::fromApplicationLayer(cPacket *pkt, const char *destination)
{
    if (isSink || receivers.empty())
    {
        // trace() << "receiver list is empty";
        return;
    }

    ApplicationPacket *appPkt = check_and_cast<ApplicationPacket *>(pkt);
    // create unique data packet ID
    unsigned int packetId = appPkt->getSequenceNumber() * 100000 + atoi(SELF_NETWORK_ADDRESS);
    ReceiversContainer receiversContainer;
    receiversContainer.setReceivers(this->receivers);

    SimpleRoutingPacket *netPacket = new SimpleRoutingPacket("SimpleRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    netPacket->setSimpleRoutingKind(SIMPLE_ROUTING_DATA_PACKET);
    netPacket->setPacketId(packetId);
    netPacket->setReceiversContainer(receiversContainer);
    encapsulatePacket(netPacket, pkt);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
    // trace() << " send to MAC";
}

void SimpleRouting::fromMacLayer(cPacket *pkt, int srcMacAddress, double rssi, double lqi)
{

    if (neighbors.find(srcMacAddress) == neighbors.end())
    {
        neighbors.insert(srcMacAddress);
        neighborHopCounts.insert({srcMacAddress, INT_MAX});
    }

    SimpleRoutingPacket *netPacket = dynamic_cast<SimpleRoutingPacket *>(pkt);

    if (netPacket)
    {
        switch (netPacket->getSimpleRoutingKind())
        {
        case SIMPLE_ROUTING_DATA_PACKET:
        {
            if (isSink)
            {
                toApplicationLayer(decapsulatePacket(pkt));
                trace() << "Packet ID " << netPacket->getPacketId() << "     send to app layer";
            }
            else
            {
                ReceiversContainer receiversContainer;
                receiversContainer.setReceivers(this->receivers);

                SimpleRoutingPacket *dupPacket = netPacket->dup();
                dupPacket->setSource(SELF_NETWORK_ADDRESS);
                dupPacket->setSequenceNumber(currentSequenceNumber++);
                dupPacket->setReceiversContainer(receiversContainer);
                toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS);
                // trace() << "Packet ID " << netPacket->getPacketId() << "     send to next hop";
            }
            break;
        }

        case SIMPLE_ROUTING_HOPCOUNT_PACKET:
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

void SimpleRouting::handleNetworkControlCommand(cMessage *pkt)
{
    SimpleRoutingControl *controlPacket = check_and_cast<SimpleRoutingControl *>(pkt);
    if (!controlPacket)
        return;

    switch (controlPacket->getSimpleRoutingControlKind())
    {
    // send receivers list to mac layer
    case SIMPLE_ROUTING_RECEIVERS_REQUEST:
    {
        ReceiversContainer receiversContainer;
        receiversContainer.setReceivers(this->receivers);

        OMacControl *controlPacketToOMac = new OMacControl("Simple Routing receivers list to MAC", MAC_CONTROL_COMMAND);
        controlPacketToOMac->setOMacControlKind(OMAC_RECEIVERS_LIST_REPLY);
        controlPacketToOMac->setReceiversContainer(receiversContainer);
        toMacLayer(controlPacketToOMac);
        // trace() << "send receivers to mac layer ";
        break;
    }

    default:
        break;
    }
}

void SimpleRouting::updateReceiverList()
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

void SimpleRouting::broadcastHopCount()
{
    SimpleRoutingPacket *netPacket = new SimpleRoutingPacket("Simple Routing hopcount packet", NETWORK_LAYER_PACKET);
    netPacket->setSimpleRoutingKind(SIMPLE_ROUTING_HOPCOUNT_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination("ALL");
    netPacket->setHopcount(hopCount);
    netPacket->setSequenceNumber(currentSequenceNumber++);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}

void SimpleRouting::finish()
{
    trace() << "----------------------Network monitoring infos----------------------";

    string str = "";
    trace() << "broadcast hop count " << hopCount;

    str = "neighbors: ";
    for (int neighbor : neighbors)

        str = str + " " + std::to_string(neighbor);

    trace() << str;

    str = "receivers: ";
    for (int receiver : receivers)
        str = str + " " + std::to_string(receiver);
    trace() << str;
}
