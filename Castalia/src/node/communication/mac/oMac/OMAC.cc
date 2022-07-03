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

    // clear buffers
    overheardAcks.clear();
    receiversListFromRadio.clear();
    sentPackets.clear();
    ackedPackets.clear();
    overheardPackets.clear();

    while (!ackBuffer.empty())
        ackBuffer.pop();

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

        macFrame->setOMacPacketKind(OMAC_DATA_PACKET);
        macFrame->setPacketId(packetId);
        macFrame->setReceiversContainer(receiversListContainer);
        break;
    }

    case SIMPLE_ROUTING_HOPCOUNT_PACKET:
    {
        macFrame->setOMacPacketKind(OMAC_HOPCOUNT_PACKET);
        break;
    }

    default:
        break;
    }

    if (bufferPacket(macFrame))
    {
        if (TXBuffer.size() == 1)
        {
            checkTxBuffer();
        }
        resetDefaultState("add a mac frame in buffer");
    }
    else
    {
        // buffer is full.
    }
}

void OMAC::fromRadioLayer(cPacket *pkt, double RSSI, double LQI)
{
    OMacPacket *macFrame = check_and_cast<OMacPacket *>(pkt);
    if (isNotDuplicatePacket(macFrame))
        toNetworkLayer(decapsulatePacket(macFrame));
}

void OMAC::sendAck()
{
}

void OMAC::handleSendAck()
{
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
        carrierIsBusy();

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

    toRadioLayer(TXBuffer.front()->dup());
    dataTransmissions++;

    double txTime = TX_TIME(TXBuffer.front()->getByteLength());
    popTxBuffer();
    setMacState(MAC_STATE_IN_TX, "sent DATA packet to BROADCAST address");
    setTimer(TRANSMISSION_TIMEOUT, txTime);
    toRadioLayer(createRadioCommand(SET_STATE, TX));

    trace() << "send data";
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

void OMAC::updateReceiversLists(ReceiversContainer)
{
}

void OMAC::updateOverheardAcks(unsigned int, int)
{
}

bool OMAC::checkInReceiversList()
{
}

bool OMAC::checkSentPackets(unsigned int)
{
}

bool OMAC::checkAckedPackets(unsigned int)
{
}

bool OMAC::overheardAckOfPkt(unsigned int)
{
}

bool OMAC::inOverheardAckList(unsigned int, int)
{
}

bool OMAC::overheardAckFromOtherNode(unsigned int, int)
{
}

int OMAC::getIndexInReceiversList()
{
}

void OMAC::finishSpecific()
{
}