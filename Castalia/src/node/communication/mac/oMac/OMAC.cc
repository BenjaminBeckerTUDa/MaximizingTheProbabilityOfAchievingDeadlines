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
    transmissionID_ = 0;

    // clear buffers
    overheardAcks.clear();
    successfullyReceivedPackets.clear();
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
                trace() << SELF_MAC_ADDRESS << " CARRIER_SENSE: CLEAR ";
                carrierIsClear();
                break;
            }

            case BUSY:
            {
                trace() << SELF_MAC_ADDRESS << " CARRIER_SENSE: BUSY ";
                carrierIsBusy();
                break;
            }

            case CS_NOT_VALID_YET:
            {
                trace() << SELF_MAC_ADDRESS << " CARRIER_SENSE: CS_NOT_VALID_YET ";
                setTimer(CARRIER_SENSE, phyDelayForValidCS);
                break;
            }

            case CS_NOT_VALID:
            {
                trace() << SELF_MAC_ADDRESS << " CARRIER_SENSE: CS_NOT_VALID ";
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
    simtime_t randomContentionInterval = genk_dblrand(1) * contentionPeriod;
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
        macFrame->setPacketCounter(OMacNetPkt->getPacketCounter());

        //race() << "MAClayer - node " << SELF_MAC_ADDRESS << " is sending packet counter " << OMacNetPkt->getPacketCounter();

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
            //race() << "Buffer is full";
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

    case OMAC_ROUTING_PDR_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_PDR_PACKET);
        toRadioLayer(macFrame);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        controlTransmission++;
        break;
    }

    case OMAC_ROUTING_CDF_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_CDF_PACKET);
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
        int packetCounter = macFrame->getPacketCounter();
        //race() << "received packet counter: " << packetCounter;
        updateOverheardPackets(source, packetCounter);

        unsigned int packetId = macFrame->getPacketId();
        unsigned int transmissionId = macFrame->getTransmissionID();
        ReceiversContainer receiversListContainer = macFrame->getReceiversContainer();
        int indexInReceiversList = getIndexInReceiversList(receiversListContainer.getReceivers());

        string receivers = "[";
        for (int s : receiversListContainer.getReceivers())
        {
            receivers += std::to_string(s) + ",";
        }
        receivers += "]";

        if (indexInReceiversList > -1)
        {   
            trace()  << "   "<< SELF_MAC_ADDRESS << " RCV DATA from "<< source <<" for " << receivers << " Packet ID " << packetId << " transmissionId " << transmissionId;
        }
        else{
            trace() << "   " << SELF_MAC_ADDRESS << " OHD DATA from "<< source <<" for " << receivers << " Packet ID " << packetId << " transmissionId " << transmissionId << " --> DROP";
            return;
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
        ackPkt->setTransmissionID(transmissionId);
        ackBuffer.push(ackPkt);

        waitTimeout = TX_TIME(ackPacketSize + 2);
        setTimer(HANDLE_SEND_ACK, indexInReceiversList * waitTimeout);
        trace()  << "   "  << "   " << "setTimer(HANDLE_SEND_ACK, timeout:"<<indexInReceiversList * waitTimeout<<"); ";
        break;
    }

    case OMAC_ACK_PACKET:
    {
        unsigned int packetId = macFrame->getPacketId();
        unsigned int transmissionId = macFrame->getTransmissionID();

        if (destination == SELF_MAC_ADDRESS)
        {   
            trace()   << "   "<< SELF_MAC_ADDRESS << " RCV ACK from "<< source <<" for " << destination << " Packet ID " << packetId;
        }
        else{
            trace()  << "   " << SELF_MAC_ADDRESS << " OHD ACK from "<< source <<" for " << destination << " Packet ID " << packetId;
        }
        

        if (destination == SELF_MAC_ADDRESS)
        {
            if (macState == MAC_STATE_WAIT_FOR_ACK)
            {
                cancelTimer(TRANSMISSION_TIMEOUT);
                popTxBuffer();
                resetDefaultState("transmission successful (ACK received)");
                trace()  << "   " << "   " << "transmission successful (ACK received)";
            }
            else
            {
                trace()  << "   " << "   " << "state: macState != MAC_STATE_WAIT_FOR_ACK";
            }
        }
        else
        {   
            trace()  << "   " << "   action: updateOverheardAcks("<< transmissionId << ", " << destination<<");";
            updateOverheardAcks(transmissionId, destination); // changed source to destination
        }
        break;
    }

    case OMAC_CONTROL_PACKET:
    {
        if (isNotDuplicatePacket(macFrame))
            toNetworkLayer(decapsulatePacket(macFrame));
        break;
    }

    case OMAC_PDR_PACKET:
    {
        if (isNotDuplicatePacket(macFrame))
            toNetworkLayer(decapsulatePacket(macFrame));
        //race() << "received a PDR packet from " << source;
        break;
    }

    case OMAC_CDF_PACKET:
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
        unsigned int transmissionId = ackPkt->getTransmissionID();

        if (overheardAcks.count(transmissionId))
        {
            if (overheardAcks.at(transmissionId).count(ackPkt->getDestination())) // removed "!"
            {
                delete ackBuffer.front();
                ackBuffer.pop();
                delete pktToNetBuffer.front();
                pktToNetBuffer.pop();
                return;
            }
        } else {
            //race() << "  overheardAcks = false";
        }
        
        trace() <<SELF_MAC_ADDRESS << " tx ACK to " << ackPkt->getDestination() << " for Packet ID "<< packetId;

        toRadioLayer(ackPkt);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        ackBuffer.pop();
        setMacState(MAC_STATE_IN_TX, "transmitting ACK packet");
        // important here: packet gets to NET after ACK has been sent!
        // check if this packet has been sent before, YES: drop
        if (successfullyReceivedPackets.count(packetId))
        {
            trace()   << "   "<< "   "<< "state: successfullyReceivedPackets.count("<<packetId<<") --> Packet not to NET";
        }
        else
        {
            trace()   << "   "<< "   "<< "state: !successfullyReceivedPackets.count("<<packetId<<") --> Packet to NET";
            toNetworkLayer(decapsulatePacket(pktToNetBuffer.front()));
            successfullyReceivedPackets.insert(packetId);
        }
        
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

    transmissionID_ ++;
    macFrame->setTransmissionID(transmissionID_);
    

    // increment packet counter if packet is retransmitted
    if(txRetries < maxTxRetries) {
        int packetCounter = macFrame->getPacketCounter();
        macFrame->setPacketCounter(packetCounter + (maxTxRetries - txRetries));
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

    if (macFrame->getOMacPacketKind() == OMAC_DATA_PACKET)
    {
        ReceiversContainer receiversListContainer = macFrame->getReceiversContainer();
        string receivers = "[";
        for (int s : receiversListContainer.getReceivers())
        {
            receivers += std::to_string(s) + ",";
        }
        receivers += "]";
        trace() <<SELF_MAC_ADDRESS << " tx DATA to " << receivers << "Packet ID "<< macFrame->getPacketId() <<" transmissionId "<< macFrame->getTransmissionID();
    }


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

void OMAC::updateOverheardAcks(unsigned int transmissionId, int node)
{
    if (overheardAcks.count(transmissionId))
    {
        overheardAcks.at(transmissionId).insert(node);
    }
    else
    {
        set<int> tempSet;
        tempSet.insert(node);
        overheardAcks.insert({transmissionId, tempSet});
    }
}

void OMAC::updateOverheardPackets(int node, int packetCounter)
{
    //race()<< "Maclayer: received packet from node " << node << " with packet counter: " << packetCounter;
    
    if (overheardPackets.count(node))
        overheardPackets.at(node)++;
    else
        overheardPackets.insert({node, 1});

    ODARControlMessage *oc = new ODARControlMessage("INC_RCV", NETWORK_CONTROL_COMMAND);
	oc->setODARControlMessageKind(INC_RCV);
    oc->setPacketCounter(packetCounter);
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
    //race() << "----------------------MAC monitoring infos----------------------";
    //race() << "channel clear count " << channelClear;
    //race() << "channel busy count " << channelBusy;
    //race() << "data transmission count " << dataTransmissions;
    //race() << "hop count transmission count " << hopCountTransmission;
    //race() << "reached MAX retries count " << reachedMaxRetriesCount;
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