#ifndef _SIMPLEROUTING_H_
#define _SIMPLEROUTING_H_

#include <map>
#include <list>
#include <set>
#include <climits>
#include "VirtualRouting.h"
#include "SimpleRoutingPacket_m.h"
#include "SimpleRoutingControl_m.h"
#include "OMacControl_m.h"

using namespace std;

enum SimpleRoutingTimers
{
    BROADCAST_HOPCOUNT = 1,
};

class SimpleRouting : public VirtualRouting
{
private:
    list<int> receivers; // current receivers list
    set<int> neighbors;
    map<int, int> neighborHopCounts;

    int hopCount;
    bool isSink;
    unsigned int packetNumber = 0; // sequence number of the pakcet sent from this node

    /*--- Monitoring parameters ---*/
    int hopCountCount = 0;
    int pktCount = 0;
    int pktCountToApp = 0;

    /*--- The .ned file's parameters ---*/
    simtime_t hopCountPeriod;

protected:
    void startup();
    void timerFiredCallback(int);

    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);

    // void handleNetworkControlCommand(cMessage *pkt);

    void broadcastHopCount();
    void updateReceiverList();

    void finish();
};

#endif // SIMPLEROUTINGMODULE