#ifndef _AggODAR_H_
#define _AggODAR_H_

#include <map>
#include <list>
#include <set>
#include <climits>
#include <queue>
#include "VirtualRouting.h"
#include "ODARPacket_m.h"
#include "ODARControl_m.h"
#include "DeadlinePacket_m.h"
#include "CFP.h"
#include "OMAC.h"
#include "Mask.h"
#include "ODAR.h"

using namespace std;

// use CFP(for deadline list, etc) for new Aggregation Packet 

struct ag_pkt {
    ODARPacket * pkt;   // the packet
    double wt;  // max Waiting time
};

struct Compare {
    bool operator()(ag_pkt a, ag_pkt b)
    { return a.wt > b.wt ;};
};

class AggODAR : public ODAR {

private:
    std::priority_queue<ag_pkt, std::vector<ag_pkt>, Compare> pq;

    double maxSizeForPA; // max packet size for aggregation
    double currentSizeForPA; // current sum of size of every paket in query
    double mCDF; // the desired CDF probability for calculating the max Waiting time
    int prio_packetsize; // the Packetsize of the Packet with the lowest waiting time (currently in buffer)

    /*--- Monitoring parameters ---*/
   // int pktBufferCount = 0; // current paket size in buffer
   // int pktCountAgg = 0; // number of aggregated pakets
   // int currentBufferSize = 0; // 
    

protected:
    virtual void timerFiredCallback(int);

    virtual void fromApplicationLayer(cPacket *, const char *);
    virtual void fromMacLayer(cPacket *, int, double, double);

    void toPriorityQueue(ODARPacket *);
    void aggregation();
    void fromPriorityQueue(ODARPacket *);
    double maxWaitingTime(double);
    double packet_timer_calc(double);
    
}

#endif // PACKET AGGREGATION MODULE