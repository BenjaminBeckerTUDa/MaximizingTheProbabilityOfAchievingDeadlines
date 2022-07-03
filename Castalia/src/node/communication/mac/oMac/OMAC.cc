#include "OMAC.h"

Define_Module(OMAC);

void OMAC::startup()
{
    contentionPeriod = ((double)par("contentionPeriod")) / 1000.0;   // just convert msecs to secs
    phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0; // parameter given in ms in the omnetpp.ini
    ackPacketSize = par("ackPacketSize");
    maxTxRetries = par("maxTxRetries");
    waitTimeout = ((double)par("waitTimeout")) / 1000.0;

    // used to call TX_TIME(x)
    phyDataRate = par("phyDataRate");
    phyLayerOverhead = par("phyFrameOverhead");

    stateDescr[102] = "MAC_STATE_ACTIVE";
    stateDescr[104] = "MAC_STATE_IN_TX";
    stateDescr[111] = "MAC_CARRIER_SENSE_FOR_TX_DATA";
    stateDescr[122] = "MAC_STATE_WAIT_FOR_ACK";

    channelBusy = 0;
    channelClear = 0;
    dataTransmissions = 0;
    hopCountTransmission = 0;

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
    simtime_t randomContentionInterval = genk_dblrand(1) * contentionPeriod;
    while (!TXBuffer.empty())
    {
        if (txRetries <= 0)
        {
            popTxBuffer();
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
    SimpleRoutingPacket *simpleNetPkt = check_and_cast<SimpleRoutingPacket *>(netPkt);
    if (!simpleNetPkt)
    {
        return;
    }

    OMacPacket *macFrame = new OMacPacket("OMAC data packet", MAC_LAYER_PACKET);
    encapsulatePacket(macFrame, netPkt);
    macFrame->setSource(SELF_MAC_ADDRESS);
    macFrame->setDestination(BROADCAST_MAC_ADDRESS);

    switch (simpleNetPkt->getSimpleRoutingKind())
    {
    case SIMPLE_ROUTING_DATA_PACKET:
    {
        unsigned int packetId = simpleNetPkt->getPacketId();
        receiversListContainer = simpleNetPkt->getReceiversContainer();

        // displayReceiverList();

        macFrame->setOMacPacketKind(OMAC_DATA_PACKET);
        macFrame->setPacketId(packetId);
        macFrame->setReceiversContainer(receiversListContainer);

        if (bufferPacket(macFrame))
        {
            if (TXBuffer.size() == 1)
            {
                checkTxBuffer();
            }
            if (macState == MAC_STATE_ACTIVE)
                resetDefaultState("add a data frame in buffer");
        }
        else
        {
            // buffer is full.
        }
        break;
    }

    case SIMPLE_ROUTING_HOPCOUNT_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_HOPCOUNT_PACKET);
        toRadioLayer(macFrame);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        hopCountTransmission++;
        break;
    }

    default:
        break;
    }
}

void OMAC::fromRadioLayer(cPacket *pkt, double RSSI, double LQI)
{
    OMacPacket *macFrame = check_and_cast<OMacPacket *>(pkt);
    int source = macFrame->getSource();
    int destination = macFrame->getDestination();

    switch (macFrame->getOMacPacketKind())
    {
    case OMAC_DATA_PACKET:
    {
        unsigned int packetId = macFrame->getPacketId();
        receiversListContainer = macFrame->getReceiversContainer();
        int indexInReceiversList = getIndexInReceiversList();

        // check if this packet has been sent before
        if (sentPackets.count(packetId))
        {
            return;
        }

        // check if the source of the data packet is in the overheard ACK list
        if (overheardAcks.count(packetId))
        {
            if (!overheardAcks.at(packetId).count(source))
            {
                return;
            }
        }

        // check if in the receiver list
        if (indexInReceiversList == -1)
        {
            return;
        }

        OMacPacket *ackPkt = new OMacPacket("OMAC ACK packet", MAC_LAYER_PACKET);
        ackPkt->setOMacPacketKind(OMAC_ACK_PACKET);
        ackPkt->setSource(SELF_MAC_ADDRESS);
        ackPkt->setDestination(source);
        ackPkt->setPacketId(packetId);
        ackBuffer.push(ackPkt);
        pktToNetBuffer.push(macFrame);

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
                trace() << "Packet ID " << packetId << "     received ACK from " << source;
                cancelTimer(TRANSMISSION_TIMEOUT);
                popTxBuffer();
                sentPackets.insert(packetId);
                resetDefaultState("transmission successful (ACK received)");
            }
        }
        else
        {
            trace() << "Packet ID " << packetId << "     overheard ACK from " << source;
            updateOverheardAcks(packetId, source);
        }
        break;
    }

    case OMAC_HOPCOUNT_PACKET:
    {
        if (isNotDuplicatePacket(macFrame))
            toNetworkLayer(decapsulatePacket(macFrame));
        break;
    }

    default:
        break;
    }
}

void OMAC::handleSendAck()
{
    while (!ackBuffer.empty())
    {
        /*         trace() << "ack buffer " << ackBuffer.size();
                trace() << "pkt to net buffer " << pktToNetBuffer.size(); */
        OMacPacket *ackPkt = ackBuffer.front();
        unsigned int packetId = ackPkt->getPacketId();

        if (overheardAcks.count(packetId))
        {
            trace() << "Packet ID " << packetId << "     heard ACK of this packet";
            /*             if (overheardAcks.at(packetId).size())
            {
                cancelAndDelete(ackBuffer.front());
                cancelAndDelete(pktToNetBuffer.front());
                ackBuffer.pop();
                pktToNetBuffer.pop();
                return;
            }
            */
        }

        toRadioLayer(ackPkt);
        toRadioLayer(createRadioCommand(SET_STATE, TX));
        ackBuffer.pop();
        setMacState(MAC_STATE_IN_TX, "transmitting ACK packet");

        trace() << "Packet ID " << packetId << "     send ACK from " << ackPkt->getSource() << " to " << ackPkt->getDestination();

        /*     toNetworkLayer(decapsulatePacket(pktToNetBuffer.front()));
            pktToNetBuffer.pop(); */
    }
}

int OMAC::handleControlCommand(cMessage *msg)
{
    OMacControl *macControl = check_and_cast<OMacControl *>(msg);
    if (!macControl)
        return 0;

    switch (macControl->getOMacControlKind())
    {
    case OMAC_RECEIVERS_LIST_REPLY:
    {
        receiversListContainer = macControl->getReceiversContainer();
        return 1;
    }

    default:
        break;
    }
    return 0;
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

    toRadioLayer(macFrame->dup());

    trace() << "Packet ID " << macFrame->getPacketId()
            << "     send from " << SELF_MAC_ADDRESS << " (retries " << txRetries << ") "
            << " to " << displayReceiverList();

    double txTime = TX_TIME(macFrame->getByteLength());

    txRetries--;

    int waitTimeoutCount = macFrame->getReceiversContainer().getReceivers().size();
    setMacState(MAC_STATE_WAIT_FOR_ACK, "sent DATA packet");
    setTimer(TRANSMISSION_TIMEOUT, txTime + waitTimeoutCount * waitTimeout);
    dataTransmissions++;

    toRadioLayer(createRadioCommand(SET_STATE, TX));
}

void OMAC::sendReceiversListRequest()
{
    SimpleRoutingControl *simpleControl = new SimpleRoutingControl("Simple routing receivers list request", NETWORK_CONTROL_COMMAND);
    simpleControl->setSimpleRoutingControlKind(SIMPLE_ROUTING_RECEIVERS_REQUEST);
    toNetworkLayer(simpleControl);
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

/**
 * get the index in the receiver list. If it does not exist, return -1
 */
int OMAC::getIndexInReceiversList()
{
    int index = 0;
    std::list<int> receivers = receiversListContainer.getReceivers();
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

void OMAC::finishSpecific()
{
    trace() << "----------------------MAC monitoring infos----------------------";
    trace() << "channel clear count " << channelClear;
    trace() << "channel busy count " << channelBusy;
    trace() << "data transmission count " << dataTransmissions;
    trace() << "hop count transmission count " << hopCountTransmission;
}

string OMAC::displayReceiverList()
{
    string str = " ";
    for (int receiver : receiversListContainer.getReceivers())
    {
        str = str + std::to_string(receiver) + " ";
    }
    return str;
}