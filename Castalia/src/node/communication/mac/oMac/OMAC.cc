#include "OMAC.h"

Define_Module(OMAC);

void OMAC::startup()
{
    contentionPeriod = ((double)par("contentionPeriod")) / 1000.0;   // just convert msecs to secs
    phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0; // parameter given in ms in the omnetpp.ini
    ackPacketSize = par("ackPacketSize");
    maxTxRetries = par("maxTxRetries");
    waitTimeout = ((double)par("waitTimeout")) / 1000.0;

    phyDataRate = par("phyDataRate");
    phyLayerOverhead = par("phyFrameOverhead");

    stateDescr[102] = "MAC_STATE_ACTIVE";
    stateDescr[104] = "MAC_STATE_IN_TX";
    stateDescr[111] = "MAC_CARRIER_SENSE_FOR_TX_DATA";
    stateDescr[122] = "MAC_STATE_WAIT_FOR_ACK";

    if (getParentModule()->getParentModule()->findSubmodule("Application") != -1)
    {
        cModule *tmpApplication = getParentModule()->getParentModule()->getSubmodule("Application");
        isSink = tmpApplication->hasPar("isSink") ? tmpApplication->par("isSink") : false;
    }
    else
    {
        isSink = false;
    }

    channelBusy = 0;
    channelClear = 0;
    dataTransmissions = 0;
    hopCountTransmission = 0;
    controlTransmission = 0;
    reachedMaxRetriesCount = 0;

    // clear buffers
    overheardAcks.clear();
    sentPackets.clear();
    ackedPackets.clear();
    overheardPackets.clear();

    while (!ackBuffer.empty())
        ackBuffer.pop();

    while (!pktToNetBuffer.empty())
        pktToNetBuffer.pop();

    macState = MAC_STATE_ACTIVE;
}

int OMAC::getMacAdress()
{
    return SELF_MAC_ADDRESS;
}

void OMAC::timerFiredCallback(int timer)
{
    switch (timer)
    {
    case FRAME_START:
    {
        if (macState == MAC_STATE_ACTIVE)
        {
            resetDefaultState("new frame started");
        }
        break;
    }

    case CARRIER_SENSE:
    {
        if (macState != MAC_CARRIER_SENSE_FOR_TX_DATA)
        {
            break;
        }

        switch (radioModule->isChannelClear())
        {
        case CLEAR:
        {
            carrierIsClear();
            break;
        }

        case BUSY:
        {
            carrierIsBusy();
            break;
        }

        case CS_NOT_VALID_YET:
        {
            setTimer(CARRIER_SENSE, phyDelayForValidCS);
            break;
        }

        case CS_NOT_VALID:
        {
            toRadioLayer(createRadioCommand(SET_STATE, RX));
            setTimer(CARRIER_SENSE, phyDelayForValidCS);
            break;
        }
        }
        break;
    }

    case TRANSMISSION_TIMEOUT:
    {
        resetDefaultState("timeout");
        break;
    }

    case HANDLE_SEND_ACK:
    {
        handleSendAck();
        break;
    }

    default:
    {
        break;
    }
    }
}

void OMAC::resetDefaultState(const char *descr)
{
    simtime_t randomContentionInterval = genk_dblrand(1) * contentionPeriod; // genk_dblrand ist was??
    while (!TXBuffer.empty())
    {
        if (txRetries <= 0)
        {
            unsigned int packetId = check_and_cast<OMacPacket *>(TXBuffer.front())->getPacketId();
            popTxBuffer();
            reachedMaxRetriesCount++;
        }
        else
        {
            performCarrierSense(MAC_CARRIER_SENSE_FOR_TX_DATA, randomContentionInterval);
            return;
        }
    }

    setMacState(MAC_STATE_ACTIVE, "Reset the mac state to default");
}

void OMAC::setMacState(int newState, const char *descr)
{
    if (macState == newState)
    {
        return;
    }
    macState = newState;
}

void OMAC::fromNetworkLayer(cPacket *netPkt, int destination)
{
    OMacRoutingPacket *OMacNetPkt = check_and_cast<OMacRoutingPacket *>(netPkt);
    if (!OMacNetPkt)
    {
        return;
    }
    OMacPacket *macFrame = new OMacPacket("OMAC data packet", MAC_LAYER_PACKET);
    encapsulatePacket(macFrame, netPkt);
    macFrame->setSource(SELF_MAC_ADDRESS);
    macFrame->setDestination(BROADCAST_MAC_ADDRESS);

    switch (OMacNetPkt->getOMacRoutingKind())
    {
    case OMAC_ROUTING_DATA_PACKET:
    {
        unsigned int packetId = OMacNetPkt->getPacketId();
        ReceiversContainer receiversListContainer = OMacNetPkt->getReceiversContainer();

        macFrame->setOMacPacketKind(OMAC_DATA_PACKET);
        macFrame->setPacketId(packetId);
        macFrame->setReceiversContainer(receiversListContainer);

        if (bufferPacket(macFrame))
        {
            if (TXBuffer.size() == 1)
            {
                checkTxBuffer();
            }
            resetDefaultState("add a data frame in buffer");
        }
        else
        {
            trace() << "Buffer is full";
        }

        break;
    }

    case OMAC_ROUTING_HOPCOUNT_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_CONTROL_PACKET);
        toRadioLayer(macFrame);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        hopCountTransmission++;
        break;
    }

    case OMAC_ROUTING_CONTROL_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_CONTROL_PACKET);
        toRadioLayer(macFrame);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        controlTransmission++;
        break;
    }

    default:
        break;
    }
}

void OMAC::fromRadioLayer(cPacket *pkt, double RSSI, double LQI)
{
    OMacPacket *macFrame = check_and_cast<OMacPacket *>(pkt);
    if (!macFrame)
        return;

    int source = macFrame->getSource();
    int destination = macFrame->getDestination();

    switch (macFrame->getOMacPacketKind())
    {
    case OMAC_DATA_PACKET:
    {
        
        updateOverheardPackets(source);

        unsigned int packetId = macFrame->getPacketId();
        ReceiversContainer receiversListContainer = macFrame->getReceiversContainer();
        int indexInReceiversList = getIndexInReceiversList(receiversListContainer.getReceivers());

        // check if this packet has been sent before, YES: drop
        if (sentPackets.count(packetId))
        {
            return;
        }

        // if it's the sink, forward packet to NET layer and send ACK
        if (isSink && isNotDuplicatePacket(macFrame))
        {
            toNetworkLayer(decapsulatePacket(macFrame));
            sentPackets.insert(packetId);

            OMacPacket *ackPkt = new OMacPacket("OMAC ACK packet", MAC_LAYER_PACKET);
            ackPkt->setOMacPacketKind(OMAC_ACK_PACKET);
            ackPkt->setSource(SELF_MAC_ADDRESS);
            ackPkt->setByteLength(ackPacketSize);
            ackPkt->setDestination(source);
            ackPkt->setPacketId(packetId);

            toRadioLayer(ackPkt);
            toRadioLayer(createRadioCommand(SET_STATE, TX));

            setMacState(MAC_STATE_IN_TX, "transmitting ACK packet");
            setTimer(TRANSMISSION_TIMEOUT, TX_TIME(ackPacketSize));

            return;
        }
        // check if the source is in the receiver list, No: drop
        if (indexInReceiversList == -1)
        {
            return;
        }

        // check if the source is in the overheard ACK list. If the acks of this packet have been overheard but the source is not
        // in the overheard acks list, drop the packet.
        if (overheardAcks.count(packetId))
        {
            if (!overheardAcks.at(packetId).count(source))
            {
                // return; // old code
            }
        }

        // generate new MAC fram and buffer it
        OMacPacket *dupMac = new OMacPacket("OMAC duplicated DATA packet", MAC_LAYER_PACKET);
        cPacket *netPkt = decapsulatePacket(macFrame);
        encapsulatePacket(dupMac, netPkt);
        dupMac->setOMacPacketKind(OMAC_DATA_PACKET);
        dupMac->setSource(macFrame->getSource());
        dupMac->setDestination(macFrame->getDestination());
        dupMac->setSequenceNumber(macFrame->getSequenceNumber());
        dupMac->setPacketId(macFrame->getPacketId());
        dupMac->setReceiversContainer(macFrame->getReceiversContainer());
        pktToNetBuffer.push(dupMac);

        // generate ACK and buffer it
        OMacPacket *ackPkt = new OMacPacket("OMAC ACK packet", MAC_LAYER_PACKET);
        ackPkt->setOMacPacketKind(OMAC_ACK_PACKET);
        ackPkt->setSource(SELF_MAC_ADDRESS);
        ackPkt->setDestination(source);
        ackPkt->setPacketId(packetId);
        ackBuffer.push(ackPkt);

        waitTimeout = TX_TIME(ackPacketSize + 2);
        setTimer(HANDLE_SEND_ACK, indexInReceiversList * waitTimeout);
        break;
    }

    case OMAC_ACK_PACKET:
    {
        unsigned int packetId = macFrame->getPacketId();

        if (destination == SELF_MAC_ADDRESS)
        {
            if (macState == MAC_STATE_WAIT_FOR_ACK && !ackedPackets.count(packetId))
            {
                cancelTimer(TRANSMISSION_TIMEOUT);
                popTxBuffer();
                sentPackets.insert(packetId);
                resetDefaultState("transmission successful (ACK received)");
            }
            else
            {
                if (ackedPackets.count(packetId))
                {
                    // trace() << "Packet ID " << packetId << "   ACK    " << source << "->" << SELF_MAC_ADDRESS << "   DROP   "
                    //        << "received ACK before";
                }
            }
        }
        else
        {
            updateOverheardAcks(packetId, destination);
        }
        break;
    }

    case OMAC_CONTROL_PACKET:
    {
        if (isNotDuplicatePacket(macFrame))
            toNetworkLayer(decapsulatePacket(macFrame));
        break;
    }

    default:
        break;
    }
}

/**
 * Send ACK and forward the data packet to the network layer
 **/
void OMAC::handleSendAck()
{
    while (!ackBuffer.empty())
    {
        OMacPacket *ackPkt = ackBuffer.front();
        unsigned int packetId = ackPkt->getPacketId();

        if (overheardAcks.count(packetId))
        {
            if (overheardAcks.at(packetId).count(ackPkt->getDestination()))
            {
                delete ackBuffer.front();
                ackBuffer.pop();
                delete pktToNetBuffer.front();
                pktToNetBuffer.pop();
                return;
            }
        }

        toRadioLayer(ackPkt);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        ackBuffer.pop();
        setMacState(MAC_STATE_IN_TX, "transmitting ACK packet");
        // important here: packet gets to NET after ACK has been sent!

        toNetworkLayer(decapsulatePacket(pktToNetBuffer.front()));
        pktToNetBuffer.pop();
    }
}

int OMAC::handleRadioControlMessage(cMessage *msg)
{
    RadioControlMessage *radioMsg = check_and_cast<RadioControlMessage *>(msg);
    if (!radioMsg)
        return 0;
    if (radioMsg->getRadioControlMessageKind() == CARRIER_SENSE_INTERRUPT)
    {
        carrierIsBusy();
    }

    toNetworkLayer(msg);
    return 1;
}


int OMAC::handleControlCommand(cMessage *msg)
{
    ODARControlMessage *odarMsg = check_and_cast<ODARControlMessage *>(msg);
    if (!odarMsg)
        return 0;
    if (odarMsg->getODARControlMessageKind() == REQUEST_TIMES)
    {
        ODARControlMessage *oc = new ODARControlMessage("REPLY_TIMES", NETWORK_CONTROL_COMMAND);
	    oc->setODARControlMessageKind(REPLY_TIMES);
        oc->setRequiredTimeForACK(TX_TIME(ackPacketSize + 2));
        oc->setRequiredTimeForDATA(TX_TIME(odarMsg->getDatabytelength() + macFrameOverhead));
        oc->setMaxTimeForCA(contentionPeriod);
        oc->setMinTimeForCA(0);
        oc->setSelfMACAdress(SELF_MAC_ADDRESS);
	    toNetworkLayer(oc);
    }
    return 1;
}


void OMAC::performCarrierSense(int newState, simtime_t delay)
{
    setMacState(newState, "Perform carrier sense");
    setTimer(CARRIER_SENSE, delay);
}

void OMAC::carrierIsClear()
{
    channelClear++;
    if (macState == MAC_CARRIER_SENSE_FOR_TX_DATA)
    {
        sendDataPacket();
    }
}

void OMAC::carrierIsBusy()
{
    channelBusy++;
    if (macState == MAC_CARRIER_SENSE_FOR_TX_DATA)
    {
        resetDefaultState("sensed carrier");
    }
}

void OMAC::sendDataPacket()
{
    if (TXBuffer.empty())
    {
        resetDefaultState("empty transmission buffer");
        return;
    }

    OMacPacket *macFrame = check_and_cast<OMacPacket *>(TXBuffer.front());

    if (macFrame->getOMacPacketKind() == OMAC_CONTROL_PACKET)
    {
        toRadioLayer(macFrame->dup());
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        setMacState(MAC_STATE_IN_TX, "sent hop count packet");
        popTxBuffer();
        setTimer(TRANSMISSION_TIMEOUT, TX_TIME(macFrame->getByteLength()));
        return;
    }

    toRadioLayer(macFrame->dup());
    double txTime = TX_TIME(macFrame->getByteLength());

    txRetries--;

    waitTimeout = TX_TIME(ackPacketSize + 2);
    int waitTimeoutCount = macFrame->getReceiversContainer().getReceivers().size();
    setMacState(MAC_STATE_WAIT_FOR_ACK, "sent DATA packet");
    setTimer(TRANSMISSION_TIMEOUT, txTime + waitTimeoutCount * waitTimeout);
    dataTransmissions++;

    ODARControlMessage *oc = new ODARControlMessage("INC_TX", NETWORK_CONTROL_COMMAND);
	oc->setODARControlMessageKind(INC_TX);
	toNetworkLayer(oc);

    toRadioLayer(createRadioCommand(SET_STATE, TX));
}

void OMAC::checkTxBuffer()
{
    if (TXBuffer.empty())
        return;

    txRetries = maxTxRetries;
}

void OMAC::popTxBuffer()
{
    cancelAndDelete(TXBuffer.front());
    TXBuffer.pop();
    checkTxBuffer();
}

void OMAC::updateOverheardAcks(unsigned int packetId, int node)
{
    if (overheardAcks.count(packetId))
    {
        overheardAcks.at(packetId).insert(node);
    }
    else
    {
        set<int> tempSet;
        tempSet.insert(node);
        overheardAcks.insert({packetId, tempSet});
    }
}

void OMAC::updateOverheardPackets(int node)
{
    if (overheardPackets.count(node))
        overheardPackets.at(node)++;
    else
        overheardPackets.insert({node, 1});

    ODARControlMessage *oc = new ODARControlMessage("INC_RCV", NETWORK_CONTROL_COMMAND);
	oc->setODARControlMessageKind(INC_RCV);
	oc->setSenderAddress(node);
	toNetworkLayer(oc);
}

/**
 * get the index in the receiver list. If it does not exist, return -1
 */
int OMAC::getIndexInReceiversList(std::list<int> receivers)
{
    int index = 0;
    auto iter = receivers.begin();
    while (iter != receivers.end())
    {
        if (*iter == SELF_MAC_ADDRESS)
            return index;
        index++;
        iter++;
    }
    return -1;
}

int OMAC::getChannelClearCount()
{
    return channelClear;
}

int OMAC::getChannelBusyCount()
{
    return channelBusy;
}

int OMAC::getMaxRetriesCount()
{
    return reachedMaxRetriesCount;
}

void OMAC::finishSpecific()
{
    trace() << "----------------------MAC monitoring infos----------------------";
    trace() << "channel clear count " << channelClear;
    trace() << "channel busy count " << channelBusy;
    trace() << "data transmission count " << dataTransmissions;
    trace() << "hop count transmission count " << hopCountTransmission;
    trace() << "reached MAX retries count " << reachedMaxRetriesCount;
}

string OMAC::displayReceiverList(std::list<int> receivers)
{
    string str = " ";
    for (int receiver : receivers)
    {
        str = str + std::to_string(receiver) + " ";
    }
    return str;
}