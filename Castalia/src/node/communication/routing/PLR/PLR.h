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
// PLR-related variables
 	// configurable
	int pDFSlots;		// The number of timeslots for PDFs and CDFs	
	long long maxDelay;		// same as appMaxTTD
	double probeInterval;		// interval between sending probes
	double cdfBroadcastInterval;		// interval between broadcasting control messages
	double newRoundInterval;		// interval, after which a new round starts
	int appMaxTTD;		// maximum(=initial) time to deadline; used to calculate the time-slots of CDFs and PDFs
	// PLR-related member variables
	int packetSize;		// size of packets from app layer
	long long delayStep;		// time-span for an individual slot in a CDF, i.e. maxDelay/pDFSlots;
	bool isSink;		//is a .ned file parameter of the Application module
	std::set<int> neighbors;		// set of nodes, which are in communication range
	std::map<int, double*> neighbor_cdfs;		// collection of most recently received cdfs from neighbors
	std::map<int, double*> neighbor_pdfs;		// collection of most recently calculated pdfs towards neighbors
	std::map<int, double*> neighbor_pdfs_fixed;		// collection of pdfs towards neighbors, which were calculated during last round
	std::map<int, double*> neighbor_hop_cdfs;		// collection of most recently calculated cdfs towards neighbors (e.g. pdf*cdf)
	std::map<int, int*> neighbor_histograms;		// measured delays towards neighbors
	std::map<int,double> sendCount;		// number of data-packets and probes sent to specific neighbors; used for monitoring network statistics
	std::map<int,double> receiveCount;		// number of packets received from different senders; used for monitoring network statistics
	int* routingTable;		// routing table in use
	int* routingTable_calc;		// routing table during calculations, which will be used in the next round
	double* cdf;		// cdf of this node


// used for average delay only
// the following parameters used for average delay and not related to PLR
	bool useAverageDelay;		// true = average delay for routing; false = PLR for routing
	bool useAvgSecond;		// true = use average delay, when there is no entry in the PLR tables
	int neighborSelectionStrategy;		// strategy to filter neighbors, when using average delay 0 = none, 1 = above average; 2 = above median; 3 = above threshold
	double neighborSelectionStrategy_value;		// used as parameter when neighborSelectionStrategy = 3
	std::map<int, double> neighbor_avgNodeDelays;		// used for average delay only
	std::map<int, double> neighbor_avgLinkDelays;		// used for average delay only
	std::map<int, double> neighbor_avgLinkDelays_fixed;		// used for average delay only
	std::map<int, double> neighbor_avgHopDelays;		// used for average delay only
	std::map<int, double> neighbor_avgLinkDelaysCounter;		// used for average delay only
	int nextHop;		// used for average delay only
	int nextHop_calc;		// used for average delay only
	double avgDelay;		// used for average delay only
	int currentRound;		// the current round of a node; i.e., the highest received round number so far (except for sink, as the sink initiates the rounds)


// depricated
// the following parameters are depricated and not used anymore.
	std::map<int, std::set<int>> seenPackets;		// depricated
	bool betterCDF;		// depricated
	std::map<int, double> currentRound_neighbor_avgLinkDelays;		// depricated
	std::map<int, double*> currentRound_neighbor_pdfs;		// depricated
	bool noTiming;		// depricated
	int pDFMessageTimes;		// depricated
	bool collectReducedTraceInfo;		// depricated


// for monitoring
// the following parameters are used for monitoring and logging of network statistics only.
	double riceK;
	int appSendInterval;
	int txRetries;
	double* cdf_forTrace;
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
	int test();		// for monitoring

 protected:
 	void insertDelay(long long, int);		// inserts a delay (which was measured at the MAC) at the correct position in the histogram
 	void updateAverageLinkDelay(double, int);		// used for average delay only
 	void createPDF(int);		// calculates a PDF from a histogram
 	void convoluteCDF(int);		// concolutes a CDF with the corresponding link-pdf and stores the result as hop-cdf
 	void addMaxToCDF();		// [...]
 	int getNextHop(long long);		// returns the routing-table entry for a given time-to-deadline
 	long long toNanoseconds(double);		// converts a given double (representing seconds) to long long (representing nanoseconds) 
 	
 	void nextRound();		// triggered, when the round number increases. [...]
 	void broadcastCDF();	// creates a packet containing the nodes CDF and the current round number, and broadcast it
 	void sendProbe(int);	// sends a probe to the neighbor with the given id

	void startup();		// initializing nodes, loading data from .ned file
	void finish();		// used for generating logging-output only
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	void handleNetworkControlCommand(cMessage *);
	void sendControlMessage(PLRControlDef);

	void timerFiredCallback(int);
	void processBufferedPacket();
};

#endif				//PLRMODULE
