/*******************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                            *
 *  Developed at the ATP lab, Networked Systems research theme                 *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                           *
 *  This file is distributed under the terms in the attached LICENSE file.     *
 *  If you do not find this file, copies can be found by writing to:           *
 *                                                                             *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                *
 *      Attention:  License Inquiry.                                           *
 *                                                                             *
 *******************************************************************************/

#ifndef _PLR_H_
#define _PLR_H_

#include <map>
#include <set>
#include "VirtualRouting.h"
#include "PLRPacket_m.h"
#include "PLRControl_m.h"
#include "CDF.h"

#define NO_LEVEL  -110
#define NO_SINK   -120

using namespace std;

enum PLRTimers {
	TOPOLOGY_SETUP_TIMEOUT = 1,
};

struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return strcmp(a, b) < 0;
   }
};

class PLR: public VirtualRouting {
 private:
 	// configurable
	int pDFSlots;
	long long maxDelay;
	int pDFMessageTimes;
	bool useAverageDelay;
	bool useAvgSecond;
	double probeInterval;
	double cdfBroadcastInterval;
	double newRoundInterval;
	double riceK;
	bool collectReducedTraceInfo;
	int neighborSelectionStrategy; // 0 = none, 1 = above average; 2 = above median; 3 = above threshold
	double neighborSelectionStrategy_value;
	int appMaxTTD;
	int appSendInterval;
	int txRetries;
	
	
	// PLR-related member variables
	int packetSize;
	long long delayStep;
	bool isSink;		//is a .ned file parameter of the Application module
	std::set<int> neighbors;
	//plr
	std::map<int, double*> neighbor_cdfs;
	std::map<int, double*> neighbor_pdfs;
	std::map<int, double*> neighbor_pdfs_fixed;
	std::map<int, double*> neighbor_hop_cdfs;
	std::map<int, int*> neighbor_histograms;


	std::map<int,double> sendCount;
	std::map<int,double> receiveCount;

	int* routingTable;
	int* routingTable_calc;
	double* cdf;
	double* cdf_forTrace;
	//avg
	std::map<int, double> neighbor_avgNodeDelays;
	std::map<int, double> neighbor_avgLinkDelays;
	std::map<int, double> neighbor_avgLinkDelays_fixed;
	std::map<int, double> neighbor_avgHopDelays;
	std::map<int, double> neighbor_avgLinkDelaysCounter;
	int nextHop;
	int nextHop_calc;
	double avgDelay;
	
	//both
	std::map<int, std::set<int>> seenPackets;
	bool betterCDF;
	
	//rounds
	int currentRound;
	std::map<int, double> currentRound_neighbor_avgLinkDelays;
	std::map<int, double*> currentRound_neighbor_pdfs;
	

	bool noTiming; 
	// if true, then ...
	// ... after 20 minutes: no more probes
	// ... after 30 minutes: no more new rounds rounds
	// ... after 40 minutes: no more broadcasts

	// for monitoring
	double simTime;
	int monitoring_receivedPacketsInTime;
	int monitoring_slots_1;
	double monitoring_slots;
	std::map<int, int*> monitoring_forwardedTo;
	std::map<int, int*> monitoring_receivedFrom;
	std::map<int, int*> monitoring_probeTo;
	std::map<int, int*> monitoring_probeFrom;
	std::map<int, int*> monitoring_SourceOfsuccessfullyDeliveredPackets;
	std::map<int, double> monitoring_fails;
	std::map<int, double> monitoring_successes;
	std::map<int, double> monitoring_pdr;
	int* monitoring_receivedCDFs;
	int* monitoring_broadcastedCDFs;
	int* monitoring_receivedProbes;
	int* monitoring_sentProbes;
	int* monitoring_receivedData;
	int* monitoring_sentData;
	int* monitoring_droppedData;
 public:
	int test();

 protected:
 	void insertDelay(long long, int);
 	void updateAverageLinkDelay(double, int);
 	void createPDF(int);
 	void convoluteCDF(int);
 	void addMaxToCDF();
 	int getNextHop(long long);
 	long long toNanoseconds(double);
 	
 	void nextRound();
 	void broadcastCDF();
 	void sendProbe(int);

	void startup();
	void finish();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void handleNetworkControlCommand(cMessage *);
	void sendControlMessage(PLRControlDef);

	void timerFiredCallback(int);
	void processBufferedPacket();
};

#endif				//PLRMODULE
