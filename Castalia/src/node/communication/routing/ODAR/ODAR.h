#ifndef _ODAR_H_
#define _ODAR_H_

#include <map>
#include <list>
#include <set>
#include <climits>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include "VirtualRouting.h"
#include "ODARPacket_m.h"
#include "ODARControl_m.h"
#include "DeadlinePacket_m.h"
#include "AggPacket_m.h"
#include "AGGL.h"
#include "CFP.h"
#include "OMAC.h"
#include "Mask.h"

using namespace std;

enum ODARTimers
{
    BROADCAST_HOPCOUNT = 1,
    REQUEST_TIMES_FROM_MAC = 2,
    BROADCAST_CONTROL = 3,
    INC_ROUND = 4,
    AGGREGATION_TRIGGER = 5,
};

enum potentialReceiverSets
{
    NONE = 1,
    CLIQUES = 2,
};

struct ag_pkt {
    ODARPacket * pkt;   // the packet
    double wt;  // max Waiting time
};

struct Compare {
    bool operator()(ag_pkt a, ag_pkt b)
    { return a.wt > b.wt ;};
};

template <class T, class I>
bool listContains(const list<T>& v, I& t)
{
    bool found = (std::find(v.begin(), v.end(), t) != v.end());
    return found;
}

template <class T, class I>
void listRemove(list<T>& v, I& t)
{
    if (listContains(v, t)) {
        v.erase(std::remove(v.begin(), v.end(), t), v.end());
    } else {
        throw std::invalid_argument("argument is not in list");
    }
    
}

class ODAR : public VirtualRouting
{
protected:
    list<int> receiversByHopcount; // current receivers list
    set<int> neighbors;
    map<int, int> neighborHopCounts;

    

    double neighborSuccRateThreshold;
    std::map<int,long> MACToLong;
    std::map<long,int> longToMAC;

	std::map<int,int> neighborIDsForSetOperations;
    map<int, long> twoHopNeighborhoods_byMAC;
    map<long, long> twoHopNeighborhoods_byLong;

    map<int, double> txSuccessRates;
    double * convolutedCDF_withoutACKs;
    double * convolutedCDF_withACKs;
    double * CDF_inUse;
    double * CDF_calculation;

    map<int, Mask> masks;

    int currRound;

    map<int, list<int>> routingTable_inUse;
    map<int, list<int>> routingTable_calculation;

    map<long, double * > neighborCDFs_byLong;
    map<int, double * > neighborCDFs_byMAC;
    map<int, double * > neighborConvolutedCDFs_withoutACKs_byMAC;
    map<int, double * > neighborConvolutedCDFs_withACKs_byMAC;



    double maxTTD; // in ms
    double timeForACK; // in ms
    double timeForDATA; // in ms
    double maxTimeForCA; // in ms
    double minTimeForCA; // in ms

    /**********************************************Aggregation****************************************************/ 
    std::priority_queue<ag_pkt, std::vector<ag_pkt>, Compare> pq;
    list<ODARPacket*> packetqueueIN;

    double maxSizeForPA = 300; // max packet size for aggregation
    int currentSizeForPA = 0; // current sum of size of every paket in query
    double diff_CDF = 0.01; // the desired CDF probability for calculating the max Waiting time
    double thresholdWaitingTime = 0.1;
    int prio_packetsize; // the Packetsize of the Packet with the lowest waiting time (currently in buffer)

    list<int> packetsreceived;

    list<unsigned int> t_aggpkt;

    bool aggModus = true;

    /*--- Monitoring parameters ---*/
    int aggpktCount = 0; // number of packets which are aggregated
    int aggCount = 0;   // number of created aggregation packets
    int deaggCount = 0; // number of deaggregations
    int aggViaSize = 0;
    int aggViaTime = 0;
    /*************************************************************************************************************/ 

    int cdfSlots;
    int potentialReceiverSetsStrategy;

    bool changeInCDF;
    

    list<int*> monitoring_seenPackets;


    list<long> cliques_long;
    list<list<int>> cliques;

    bool isSink;
    int hopCount;
    int packetSize;
    unsigned int packetNumber = 0; // sequence number of the pakcet sent from this node

    /*--- Monitoring parameters ---*/
    int hopCountCount = 0;
    int pktCount = 0;
    int pktCountToApp = 0;
    int deadlineExpiredCount = 0;
    int txCount = 0;
    map<int, int> rxCount;



    bool minHopOnly;

    /*--- The .ned file's parameters ---*/
    simtime_t hopCountPeriod;

    void startup();
    void timerFiredCallback(int);

    void timeToSlot(simtime_t);

    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);

    void toPriorityQueue(ODARPacket *);
    void aggregation(int, list<ODARPacket*>);
    void deaggregation(AggPacket *);
    double maxWaitingTime(double);
    double packet_timer_calc(double);

    void handleNetworkControlCommand(cMessage *);

    void broadcastControl();
    void updateReceiverList();

    void createMask(int);
    double * convoluteCDF(double * , int);
    double convoluteCDFAtSinglePosition(double * , int, int);
    void calculateCDF();

    void bronKerbosch(long, long, long, map<long,long>);
    void findCliques();
    void findNeighbors();
    void findPotentialReceiverSets();
    string longToBitString(long);

    long nodesetToLong(set<int>);
    list<int> longToNodelist(long);

    void finish();

public:
    int getPacketCreatedCount();
    int getPacketDeadlineExpiredCount();
    int getAggregationCount();
    int getAggregationPacketCount();
    int getDeaggregationPacketCount();
    int getAggregationViaSize();
    int getAggregationViaTime();
};

#endif // ODARMODULE