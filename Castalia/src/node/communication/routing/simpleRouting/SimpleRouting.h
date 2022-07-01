#ifndef _SIMPLEROUTING_H_
#define _SIMPLEROUTING_H_

#include "VirtualRouting.h"
#include "SimpleRoutingPacket_m.h"
#include "SimpleRoutingControl_m.h"
#include "OMacControl_m.h"
#include <map>
#include <list>
#include <set>
#include <climits>

using namespace std;

enum SimpleRoutingTimers
{
    BROADCAST_HOPCOUNT = 1,
};

class SimpleRouting : public VirtualRouting
{
private:
    int hopCount;
    list<int> receivers;

    set<int> neighbors;
    map<int, int> neighborHopCounts;

    simtime_t hopCountPeriod;

    bool isSink;
    unsigned int packetId = 0;

protected:
    void startup();
    void timerFiredCallback(int);

    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);

    void handleNetworkControlCommand(cMessage *pkt);

    void broadcastHopCount();
    void updateReceiverList();

    void finish();
};

#endif // SIMPLEROUTINGMODULE