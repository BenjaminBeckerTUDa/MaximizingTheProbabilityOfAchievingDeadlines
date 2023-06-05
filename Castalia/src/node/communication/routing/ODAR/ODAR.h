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

using namespace std;

enum ODARTimers
{
    BROADCAST_HOPCOUNT = 1,
    REQUEST_TIMES_FROM_MAC = 2,
    BROADCAST_CONTROL = 3,
    INC_ROUND = 4,
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
    int pktCountToApp = 0;
    int deadlineExpiredCount = 0;
    int txCount = 0;
    map<int, int> rxCount;

    bool minHopOnly;

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

    void finish();

public:
    int getPacketCreatedCount();
    int getPacketDeadlineExpiredCount();
};

#endif // ODARMODULE