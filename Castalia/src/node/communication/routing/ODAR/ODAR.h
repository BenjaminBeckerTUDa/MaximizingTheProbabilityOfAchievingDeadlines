#ifndef _ODAR_H_
#define _ODAR_H_

#include <map>
#include <list>
#include <set>
#include <climits>
#include "VirtualRouting.h"
#include "ODARPacket_m.h"
#include "ODARControl_m.h"
#include "DeadlinePacket_m.h"
#include "CFP.h"
#include "OMAC.h"
#include "Mask.h"
#include <vector>

using namespace std;

enum ODARTimers
{
    BROADCAST_HOPCOUNT = 1,
    REQUEST_TIMES_FROM_MAC = 2,
    BROADCAST_CONTROL = 3,
    INC_ROUND = 4,
    PDR_BROADCAST = 5,
    CDF_BROADCAST = 6,
    MONITOR_DAR = 7
};

enum potentialReceiverSets
{
    NONE = 1,
    CLIQUES = 2,
};



class ODAR : public VirtualRouting
{
private:
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
    double * CDF_transmitted;
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



    int cdfSlots;
    int potentialReceiverSetsStrategy;

    bool changeInCDF;
    

    list<int*> monitoring_seenPackets;


    list<long> cliques_long;
    list<list<int>> cliques;

    int hopCount;
    int packetSize;
    bool isSink;
    unsigned int packetNumber = 0; // sequence number of the pakcet sent from this node

    /*--- Monitoring parameters ---*/
    int hopCountCount = 0;
    int pktCount = 0;
    int pktCountInterval = 0;
    int pktCountToApp = 0;
    int pktCountToAppInterval = 0;
    int deadlineExpiredCount = 0;
    int intervalCounter = 1;

    int txCount = 0; // replaced by dataTransmissionTimes in resilient version
    std::vector<double> dataTransmissionTimes; // timestamps of transmitted packets

    map<int, int> rxCount;  // replaced by dataReceivedTimes in resilient version
    map<int, std::vector<double>> dataReceivedTimes; // timestamps of received packets for every neighbor node

    long lastPdrBroadcast; // time of last PDR broadcast
    long lastCdfBroadcast; // time of last CDF broadcast
    map<int, double> currentPdrs; // current PDRs (transmittedPdrs + recent changes)
    map<int, double> transmittedPdrs; // PDRs transmitted in last PDR broadcast
    map<int, long> pdrBroadcastTimes; // time values for next PDR broadcast for every neighbor node
    double sum_cdf_abs_differences; // The sum of absolute differences of current and last transmitted CDF for all received data packets since last CDF broadcast
    int packets_since_cdf_broadcast; // Number of data packets received since last CDF broadcast  yx

    bool minHopOnly;

    bool resilientVersion = true; // toggle resilient modifications to ODAR
    
    map<int, int> packetCountToAppIntervalPerNode;

    //
    int * test123 = new int[10]{0};
    int test123counter = 0;


    /*--- The .ned file's parameters ---*/
    simtime_t hopCountPeriod;

protected:
    void startup();
    void timerFiredCallback(int);

    void timeToSlot(simtime_t);

    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);

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

    void calculatePdrBroadcastTimes(double, long);
    void checkPdrBroadcast();
    void executePdrBroadcast();
    void checkCdfBroadcast();
    void executeCdfBroadcast();
    

    void finish();

public:
    int getPacketCreatedCount();
    int getPacketDeadlineExpiredCount();
    int calculateTransmissionCount();
    void overheardPacket(double);
    bool getResilientVersion();
    int getAndResetPacketCreatedCount();
    void debugTrace(int);
};

#endif // ODARMODULE