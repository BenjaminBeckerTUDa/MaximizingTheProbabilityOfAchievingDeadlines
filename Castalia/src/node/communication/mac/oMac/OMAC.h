/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Yuriy Tselishchev, Athanassios Boulis                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef _OMAC_H_
#define _OMAC_H_

#include <omnetpp.h>
#include <algorithm>
#include "VirtualMac.h"
#include "OMacPacket_m.h"
#include "OMacControl_m.h"
#include "SimpleRoutingPacket_m.h"
#include "SimpleRoutingControl_m.h"

using namespace std;

#define TX_TIME(x) (phyLayerOverhead + x) * 1 / (1000 * phyDataRate / 8.0) // x are in BYTES

enum OMacStates
{
    MAC_STATE_SETUP = 100,
    MAC_STATE_ACTIVE = 102,

    MAC_STATE_IN_TX = 104,
    MAC_CARRIER_SENSE_FOR_TX_DATA = 111,

    MAC_STATE_WAIT_FOR_ACK = 122,

    MAC_STATE_WAIT_FOR_RECEIVER_LIST = 130,
};

enum OMacTimers
{
    FRAME_START = 1,
    CARRIER_SENSE = 2,
    TRANSMISSION_TIMEOUT = 3,
    HANDLE_SEND_ACK = 4,
};

struct OverherdAcksInfo
{
    int senderID;
    unsigned int packetID;
};

typedef list<OverherdAcksInfo> list_overheardAcks;

class OMAC : public VirtualMac
{
private:
    /*--- A map from int value of state to its description (used in debug) ---*/
    map<int, string> stateDescr;

    /*---  ---*/
    map<unsigned int, set<int>> overheardAcks;
    ReceiversContainer receiversListContainer; // store receivers list from network layer
    set<unsigned int> sentPackets;
    set<unsigned int> ackedPackets;
    queue<OMacPacket *> ackBuffer;
    queue<OMacPacket *> pktToNetBuffer;

    /*--- channel monitoring ---*/
    int channelBusy;
    int channelClear;
    int dataTransmissions;
    int hopCountTransmission;
    map<int, int> overheardPackets;

    /*--- The .ned file's parameters ---*/
    int maxTxRetries;
    int ackPacketSize;

    simtime_t contentionPeriod;
    simtime_t phyDelayForValidCS; // delay for valid CS
    simtime_t waitTimeout;

    /*--- General MAC variable ---*/
    int phyLayerOverhead;
    double phyDataRate;

    /*--- OMAC state variables  ---*/
    int macState;
    int txAddr;
    int txRetries;

    /*--- OMAC packet pointers (sometimes packet is created not immediately before sending) ---*/
    OMacPacket *macFromRadio;

protected:
    void startup();
    void timerFiredCallback(int);
    void finishSpecific();

    void fromRadioLayer(cPacket *, double, double);
    void fromNetworkLayer(cPacket *, int);

    int handleControlCommand(cMessage *);
    int handleRadioControlMessage(cMessage *);

    void resetDefaultState(const char *descr = NULL);
    void setMacState(int, const char *);

    void sendDataPacket();
    void sendReceiversListRequest();

    void performCarrierSense(int, simtime_t delay = 0);
    void carrierIsClear();
    void carrierIsBusy();

    void checkTxBuffer();
    void popTxBuffer();

    void updateReceiversLists(ReceiversContainer);
    void updateOverheardAcks(unsigned int, int);

    int getIndexInReceiversList();

    bool checkInReceiversList();
    bool checkSentPackets(unsigned int);
    bool checkAckedPackets(unsigned int);

    bool overheardAckOfPkt(unsigned int);
    bool inOverheardAckList(unsigned int, int);
    bool overheardAckFromOtherNode(unsigned int, int);

    void sendAck();
    void handleSendAck();

    string displayReceiverList();
};

#endif