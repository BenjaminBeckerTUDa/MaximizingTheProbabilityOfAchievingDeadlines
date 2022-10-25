#ifndef _ODAR_H_
#define _ODAR_H_

#include <map>
#include <list>
#include <set>
#include <climits>
#include "VirtualRouting.h"
#include "ODARPacket_m.h"

using namespace std;

enum ODARTimers
{
    BROADCAST_HOPCOUNT = 1,
};

class ODAR : public VirtualRouting
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

#endif // ODARMODULE