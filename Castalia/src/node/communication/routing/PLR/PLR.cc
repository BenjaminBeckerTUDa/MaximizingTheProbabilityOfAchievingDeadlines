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
#include <algorithm>
using namespace std;

#include "PLR.h"
#include "DeadlinePacket_m.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "VirtualMobilityManager.h"


Define_Module(PLR);


void PLR::startup()
{
	// check that the Application module used has the boolean parameter "isSink"
	cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
	

	if (appModule->hasPar("isSink"))
		isSink = appModule->par("isSink");
	else
		opp_error("\nPLR has to be used with an application that defines the parameter isSink");
	
	if (appModule->hasPar("maxTTD"))
		maxDelay = ((long long) (((long) appModule->par("maxTTD")))) * (long long) 1000; 
	else
		opp_error("\nPLR has to be used with an application that defines the parameter maxTTD");
		
	if (appModule->hasPar("packetSize"))
		packetSize = (int) appModule->par("packetSize"); 
	else
		opp_error("\nPLR has to be used with an application that defines the parameter packetSize");

	if (appModule->hasPar("maxTTD"))
		appMaxTTD = appModule->par("maxTTD"); 
	else
		opp_error("\nPLR has to be used with an application that defines the parameter maxTTD");
	if (appModule->hasPar("sendInterval"))
		appSendInterval = appModule->par("sendInterval"); 
	else
		opp_error("\nPLR has to be used with an application that defines the parameter sendInterval");

	
	collectReducedTraceInfo = par("collectReducedTraceInfo");
	riceK = par("riceK");

	cModule *macModule = getParentModule()->getParentModule()->getSubmodule("Communication")->getSubmodule("MAC");
	if (macModule->hasPar("maxTxRetries"))
		txRetries = macModule->par("maxTxRetries"); 
	//else
	//	txRetries = -1;

	useAverageDelay = par("useAverageDelay");
	useAvgSecond = par("useAvgSecond");

	if (useAverageDelay)
	{
		neighborSelectionStrategy = par("neighborSelectionStrategy"); // 0 = none, 1 = above average; 2 = above median; 3 = above threshold
		neighborSelectionStrategy_value = par("neighborSelectionStrategy_value");
	}

	pDFSlots = par("pDFSlots");
	pDFMessageTimes = par("pDFMessageTimes");
	
	probeInterval = par("probeInterval");
	simTime = par("simTime");
	cdfBroadcastInterval = par("cdfBroadcastInterval");
	newRoundInterval = par("newRoundInterval");	

	//monitoring
	
	monitoring_slots = 100.0;
	monitoring_slots_1 = ((int) monitoring_slots)+1;
	
	monitoring_receivedPacketsInTime = 0;
	monitoring_receivedCDFs = new int[monitoring_slots_1]{0};
	monitoring_broadcastedCDFs = new int[monitoring_slots_1]{0};
	monitoring_receivedProbes = new int[monitoring_slots_1]{0};
	monitoring_sentProbes = new int[monitoring_slots_1]{0};
	monitoring_receivedData = new int[monitoring_slots_1]{0};
	monitoring_sentData = new int[monitoring_slots_1]{0};
	monitoring_droppedData = new int[monitoring_slots_1]{0};
	monitoring_receivedFrom.insert({-1, new int[monitoring_slots_1]{0}});
	monitoring_forwardedTo.insert({-1, new int[monitoring_slots_1]{0}});
	monitoring_forwardedTo.insert({-2, new int[monitoring_slots_1]{0}});	

	// seed for random number generator 
	stringstream strValue; strValue << SELF_NETWORK_ADDRESS; unsigned int intValue; strValue >> intValue; std::srand(intValue); 
        
        
	
	if ( maxDelay < (long long) pDFSlots){
		pDFSlots = maxDelay;
	}
	
	delayStep = maxDelay/(long long)pDFSlots;
	currentRound = 0;
	currentSequenceNumber = 0;
	
	if (isSink)
	{
		monitoring_forwardedTo.insert({0, new int[monitoring_slots_1]{0}});
	}
	
	// routing table and node delay

	if (isSink)
	{
		avgDelay = 0;
	}
	else
	{
		avgDelay = 99999;
	}
	nextHop = -1;
	nextHop_calc=-1;

	cdf = new double[pDFSlots+1]{0};
	cdf_forTrace = new double[pDFSlots+1]{0};
	if (isSink)
	{
		for (int i = 0; i <= pDFSlots; i++)
		{
			cdf[i] = 1;
			cdf_forTrace[i] = 1;
		}
	}
	else
	{
		routingTable = new int[pDFSlots+1]{0};
		routingTable_calc = new int[pDFSlots+1]{0};
		for (int i = 0; i <= pDFSlots; i++)
		{
			routingTable[i] = -1;
			routingTable_calc[i] = -1;
		}
	}

	

	
	
	// set all timers
	double time = (static_cast <double> (rand()) / static_cast <double> (RAND_MAX)) * cdfBroadcastInterval;
	setTimer(1, time); // broadcast CDF
	if (isSink)
	{
		setTimer(0, 0); // start of new Round
	}
	setTimer(2, probeInterval); // probing ; alle nodes, damit sink bei der PDR-ermittlung helfen kann
}

void PLR::timerFiredCallback(int index)
{
	if (index == 0)
	{
		// increase round
		currentRound++;
		setTimer(0, newRoundInterval);
		return;
	}
	if (index == 1)
	{
		// broadcast CDF
		broadcastCDF();
		monitoring_broadcastedCDFs[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
		double time = (static_cast <double> (rand()) / static_cast <double> (RAND_MAX)) * cdfBroadcastInterval;
		setTimer(1, time);
		return;
	}
	if (index == 2){
		// set timer for sending individual probe messages
		for ( int address : neighbors) 
		{
			double randomTime = (static_cast <double> (rand()) / static_cast <double> (RAND_MAX)) * 1.0;
			setTimer(3+address, address+randomTime);
		}
		setTimer(2, probeInterval);
		return;
	}
	// send individual probe message
	sendProbe(index-3);
}


void PLR::broadcastCDF()
{

	
	PLRPacket *netPacket = new PLRPacket("PLR cdf packet", NETWORK_LAYER_PACKET);
	netPacket->setPLRPacketKind(PLR_CDF_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination("all");
	netPacket->setSequenceNumber(currentSequenceNumber);
	currentSequenceNumber++;

	netPacket->setAvgDelay(avgDelay);
	
	if (useAverageDelay)
		netPacket->setByteLength(4+1);
	else
		netPacket->setByteLength(100+1);
	
	double* cdf2;
	cdf2 = new double[pDFSlots+1]{0};
	for (int i = 0; i <= pDFSlots; i++)
	{	
		cdf2[i] = cdf[i];
	}
	CDF cdf1;
	cdf1.setCDF(cdf2);
	netPacket->setNodeCDF(cdf1);
	
	netPacket->setRound(currentRound);
	
	toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}


void PLR::handleNetworkControlCommand(cMessage * pkt)
{
	PLRControlMessage *plrc = dynamic_cast <PLRControlMessage*>(pkt);
	if (!plrc){
		return;
	}

	if (plrc->getPLRControlMessageKind() == PDR)
	{
		// monitoring_pdr[plrc -> getTxAddr()] = plrc -> getPdr();
		// hier: trace für vergleich
		// trace() << "pdr durch mac acks >" << plrc -> getPdr();
	}
	if (plrc->getPLRControlMessageKind() == FAIL)
	{
		monitoring_fails[plrc -> getTxAddr()]++;
	}
	if (plrc->getPLRControlMessageKind() == DELAY)
	{
		monitoring_successes[plrc -> getTxAddr()]++;
		updateAverageLinkDelay(plrc -> getDelay(), plrc -> getTxAddr());
		insertDelay(toNanoseconds(plrc -> getDelay()), plrc -> getTxAddr());
	}
}

long long PLR::toNanoseconds(double delay)
{
	long long fac = 1000000;
	return (long long) (delay * fac);
}

void PLR::updateAverageLinkDelay(double delay_, int address)
{
	if (neighbor_avgLinkDelaysCounter[address] == 99999)
	{
		neighbor_avgLinkDelays[address] = delay_;
		neighbor_avgLinkDelaysCounter[address]++;
	}
	else
	{
		neighbor_avgLinkDelays[address] = ((neighbor_avgLinkDelays[address] * neighbor_avgLinkDelaysCounter[address]) + delay_) / (neighbor_avgLinkDelaysCounter[address] + 1);
		neighbor_avgLinkDelaysCounter[address]++;
	}
}

void PLR::insertDelay(long long delay_, int address)
{
	if (delay_ > maxDelay)
	{
		delay_ = maxDelay;
	}
	int slot = (int)(delay_ / delayStep);
	if (slot < pDFSlots)
	{
		slot = slot;
	}
	
	// trace() << "inserted into slot: " << slot << " which corresponds to time interval [" << slot * delayStep << ", " << (slot+1) * delayStep << "]";
	
	(neighbor_histograms[address])[slot] = (neighbor_histograms[address])[slot]+1;

	//for (int i = 0; i <= pDFSlots; i++)
	//{
	//	trace() << "Histogram at position " << i << ":" << (neighbor_histograms[address])[i];
	//}
}


void PLR::addMaxToCDF()
{

	// neighborSelectionStrategy_value = default(0.0);
	// neighborSelectionStrategy = default(0); // 0 = none, 1 = above average; 2 = above median; 3 = above threshold

	double pdr_min = 0.0;
	if (neighborSelectionStrategy == 0){
		pdr_min = 0.0;
	}
	if (neighborSelectionStrategy == 1){
		// iterate over all neighbors to find a threshold for pdr
		double i = 0.0;
		double pdr_sum = 0.0;
		
		for (int address : neighbors)
		{
			pdr_sum += monitoring_pdr[address];
			i ++;
		}
		pdr_min = pdr_sum / i;
	}
	if (neighborSelectionStrategy == 2){
		int i=0;
		for (int address : neighbors)
		{
			i ++;
		}
		double* pdrs;
		pdrs = new double[i]{0};
		i=0;
		for (int address : neighbors)
		{
			pdrs[i] = monitoring_pdr[address];
			i ++;
		}

		// TODO: hier noch mal genau prüfen, ob das mit dem median so klappt
		int n = sizeof(pdrs) / sizeof(pdrs[0]);
		sort(pdrs, pdrs+n);
		pdr_min = pdrs[n/2];
	}
	if (neighborSelectionStrategy == 3){
		pdr_min = neighborSelectionStrategy_value;
	}



	// iterate over all neighbors to find the one with minimum delay
	//avgDelay = 99999;
	for (int address : neighbors)
	{
		double neighbor_avg = neighbor_avgHopDelays[address];
		double pdr = monitoring_pdr[address];
		if (neighbor_avg < avgDelay && pdr >= pdr_min)
		{
			avgDelay = neighbor_avg;
			nextHop_calc = address;
		}
	}

	// iterate over all neighbors to find the max values for each cdf slot
	for (int address : neighbors)
	{
		for (int i = 0; i <= pDFSlots; i++)
		{
			if(neighbor_hop_cdfs[address][i] > cdf[i])
			{
				cdf[i] = neighbor_hop_cdfs[address][i];
				routingTable_calc[i] = address;
			}
		}
	}
	// making sure no loops
	for (int i = 0; i <= pDFSlots; i++)
	{
		if(cdf[i+1] == cdf[i])
		{
			routingTable_calc[i+1] = routingTable_calc[i];

		}
	}	
}

int PLR::getNextHop(long long ttl)
{
	if (ttl < 0)
	{
		// no next hop for packets with expired deadlines
		return -1;
	}
	
	if (useAverageDelay)
	{
		// always same next hop for average delay
		return nextHop;
	}
	else
	{		
		// return the next hop which corresponds to the ttl (time to deadline)
		int slot = (int)(ttl / delayStep);
		if (slot >= pDFSlots)
		{
			slot = pDFSlots-1;
		}
		if (routingTable[slot] == -1)
		{
			if (useAvgSecond)
			{
				return nextHop;
			}
		}
		return routingTable[slot];
	}
}

void PLR::createPDF(int address)
{
	int* histogram = neighbor_histograms[address];
	double* pdf = neighbor_pdfs[address];
	int sum = 0;
	for (int i = 0; i <= pDFSlots; i++)
	{
		sum = sum + histogram[i];
	}
	if (sum == 0)
	{
		return;
	}
	for (int i = 0; i <= pDFSlots; i++)
	{
		pdf[i] = (double) histogram[i] / (double) sum * monitoring_pdr[address];
	}
}

void PLR::convoluteCDF(int address)
{
	neighbor_avgHopDelays[address] = neighbor_avgLinkDelays[address] + neighbor_avgNodeDelays[address];

	double* neighbor_pdf = neighbor_pdfs[address];
	double* neighbor_cdf = neighbor_cdfs[address];
	double* neighbor_hop_cdf = neighbor_hop_cdfs[address];
	
	// do we even need this cdf?
	bool makesSense = false;
	for (int i = 0; i < pDFSlots; i++)
	{
		makesSense |= neighbor_cdf[i] > cdf[i];
	}
	if (!makesSense)
	{
		return;
	}

	for (int i = 0; i < pDFSlots; i++)
	{
		neighbor_hop_cdf[i] = 0;
	}

	// find lowest slot in cdf which has a value > 0
	int minC = -1;
	for (int i = 0; i < pDFSlots && minC == -1; i++)
	{
		minC = (neighbor_cdf[i] > 0) * (i+1) -1;
	}
	if (minC == -1)
	{
		return;
	}
	
	// find lowest slot in pdf which has a value > 0
	int minP = -1;
	for (int i = 0; i < pDFSlots && minP == -1; i++)
	{
		minP = (neighbor_pdf[i] > 0) * (i+1) -1;
	}
	if (minP == -1)
	{
		return;
	}
	
	// find highest slot in pdf which has a value > 0
	int maxP = -1;
	for (int i = pDFSlots-1; i >= 0 && maxP == -1; i--)
	{
		maxP = (neighbor_pdf[i] > 0) * (i+1) -1;
	}
	
	// convolute CDF
	for (int i = minP; i <= maxP; i++)
	{
		for (int j = minC; i+j <= pDFSlots; j++){
			neighbor_hop_cdf[i+j] += neighbor_pdf[i] * neighbor_cdf[j]/2.0;
			neighbor_hop_cdf[i+j+1] += neighbor_pdf[i] * neighbor_cdf[j]/2.0;
		}
	}
	
	// to avoid rounding errors
	for (int i = 0; i <= pDFSlots; i++)
	{	
		neighbor_hop_cdf[i] = round(neighbor_hop_cdf[i] * 1000000.0) / 1000000.0;
	}
}

void PLR::sendProbe(int address)
{
	PLRPacket *netPacket =
	    new PLRPacket("PLR probe packet", NETWORK_LAYER_PACKET);
	netPacket->setPLRPacketKind(PLR_PROBE_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination("none");
	netPacket->setReceivedPackets(receiveCount[address]);
	netPacket->setSequenceNumber(currentSequenceNumber);
	netPacket->setByteLength(packetSize + 22);
	currentSequenceNumber++;
	// trace() << "toMacLayer(PLR_PROBE_PACKET, " << address << ");"  << " size " << netPacket->getByteLength();
	monitoring_probeTo[address][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
	monitoring_sentProbes[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
	toMacLayer(netPacket, address);
	if (sendCount.find(address) == sendCount.end()) 
	{
		sendCount.insert({address, 0.0});
	}
	sendCount[address]++;
}	
	
void PLR::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	if (isSink == true)
	{
		return;
	}
	string dst(destination);
		
	DeadlinePacket *dlPacket = dynamic_cast <DeadlinePacket*>(pkt);
	if (!dlPacket)
	{
		return;
	}
	
	PLRPacket *netPacket = new PLRPacket("PLR data packet", NETWORK_LAYER_PACKET);
	netPacket->setPLRPacketKind(PLR_DATA_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination(destination);
	netPacket->setSequenceNumber(currentSequenceNumber);
	netPacket->setDeadline(dlPacket->getDeadline());
	currentSequenceNumber++;
	encapsulatePacket(netPacket, pkt);

	monitoring_receivedFrom[-1][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
	
	if (netPacket->getDeadline() > toNanoseconds(getClock().dbl()))
	{
		long long ttl = netPacket->getDeadline() - toNanoseconds(getClock().dbl());
		int nh = getNextHop(ttl);
		
		monitoring_forwardedTo[nh][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
		if (not (nh == -1))
		{
			toMacLayer(netPacket, nh);
			monitoring_sentData[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			if (sendCount.find(nh) == sendCount.end()) 
			{
				sendCount.insert({nh, 0.0});
			}
			sendCount[nh]++;
		}
		else
		{
			monitoring_droppedData[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			
			int sourceInt = std::stoi(netPacket->getSource());
			// trace() << "no next hop = " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<ttl;
		}
	}
	else
	{

	}
}

void PLR::fromMacLayer(cPacket * pkt, int macAddress, double rssi, double lqi)
{
	// create new neighbor table entry, if missing
	if (neighbors.find(macAddress) == neighbors.end()) 
	{
		neighbors.insert(macAddress);
		monitoring_forwardedTo.insert({macAddress, new int[monitoring_slots_1]{0}});
		monitoring_receivedFrom.insert({macAddress, new int[monitoring_slots_1]{0}});
		monitoring_probeTo.insert({macAddress, new int[monitoring_slots_1]{0}});
		monitoring_probeFrom.insert({macAddress, new int[monitoring_slots_1]{0}});
		monitoring_fails.insert({macAddress, 0});
		monitoring_successes.insert({macAddress, 0});

		neighbor_avgNodeDelays.insert({macAddress, 99999});
		neighbor_avgLinkDelays.insert({macAddress, 99999});
		neighbor_avgLinkDelays_fixed.insert({macAddress, 99999});
		neighbor_avgHopDelays.insert({macAddress, 99999});
		neighbor_avgLinkDelaysCounter.insert({macAddress, 0});

		neighbor_cdfs.insert({macAddress, new double[pDFSlots+1]{0}});
		neighbor_pdfs.insert({macAddress, new double[pDFSlots+1]{0}});
		neighbor_hop_cdfs.insert({macAddress, new double[pDFSlots+2]{0}});
		neighbor_histograms.insert({macAddress, new int[pDFSlots+1]{0}});

		monitoring_pdr.insert({macAddress, 0.0});

		receiveCount.insert({macAddress, 0.0});
	}

	

	PLRPacket *netPacket = dynamic_cast <PLRPacket*>(pkt);
	if (!netPacket)
	{
		return;
	}
		
	switch (netPacket->getPLRPacketKind()) 
	{
		case PLR_PROBE_PACKET:
		{
			monitoring_probeFrom[macAddress][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			monitoring_receivedProbes[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			receiveCount[macAddress]++;
			if (sendCount[macAddress] > 0)
			{
				monitoring_pdr[macAddress] = netPacket->getReceivedPackets() / sendCount[macAddress];
			}
			else
			{
				monitoring_pdr[macAddress] = 0.0;
			}
			break;
		}
		case PLR_DATA_PACKET:
		{
			int sourceInt = std::stoi(netPacket->getSource());
			long long ttl = netPacket->getDeadline() - toNanoseconds(getClock().dbl());
			
			receiveCount[macAddress]++;

			if ( seenPackets.find(sourceInt) == seenPackets.end() ) {
				std::set<int> a;
				seenPackets[sourceInt] = a;
			}
			if ((seenPackets[sourceInt]).find(netPacket->getSequenceNumber()) == (seenPackets[sourceInt]).end()){
				(seenPackets[sourceInt]).insert(netPacket->getSequenceNumber()); 
				monitoring_receivedFrom[macAddress][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
				
				if (isSink)
				{
					long long ttl = netPacket->getDeadline() - toNanoseconds(getClock().dbl());
					if (netPacket->getDeadline() > toNanoseconds(getClock().dbl()))
					{
						monitoring_receivedPacketsInTime++; //monitoring
						monitoring_forwardedTo[0][(int) (getClock().dbl()/simTime * monitoring_slots)]++;

						stringstream strValue; strValue << netPacket->getSource(); unsigned int source; strValue >> source; 
						if (monitoring_SourceOfsuccessfullyDeliveredPackets.find(source) == monitoring_SourceOfsuccessfullyDeliveredPackets.end())
						{
							monitoring_SourceOfsuccessfullyDeliveredPackets.insert({source, new int[pDFSlots+1]{0}});
						}
						monitoring_SourceOfsuccessfullyDeliveredPackets[source][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
					}
					else
					{
						// trace() << "expired TTL = " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<ttl;
						monitoring_forwardedTo[-1][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
					}
				}
				else
				{	
					if (netPacket->getDeadline() > toNanoseconds(getClock().dbl()))
					{
						int nh = getNextHop(ttl);
						monitoring_forwardedTo[nh][(int) (getClock().dbl()/simTime * monitoring_slots)]++;
						if (not (nh == -1))
						{
							PLRPacket *dupPacket = netPacket->dup();
							dupPacket->setSequenceNumber(currentSequenceNumber);
							currentSequenceNumber++;
							
							toMacLayer(dupPacket, nh);
							if (sendCount.find(nh) == sendCount.end()) 
							{
								sendCount.insert({nh, 0.0});
							}
							sendCount[nh]++;

							monitoring_sentData[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
						}
						else
						{
							// trace() << "no next hop = " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<ttl;
							monitoring_droppedData[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
						}
					}
					else
					{
						// trace() << "expired TTL = " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<ttl;
					}
				}
			}
			else
			{
				// trace() << "seen twice: source,id,ttl = " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<ttl;
				//hier ist ein drop wegen loop, oder verlorengegangenem ACK...
			}
			monitoring_receivedData[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			break;
		}
		case PLR_CDF_PACKET:
		{
			monitoring_receivedCDFs[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			/*
			- prüfe, ob neue runde
			*/
			if (currentRound < netPacket->getRound() && !isSink)
			{
				currentRound = netPacket->getRound();
				nextRound();
			}
			if (currentRound == netPacket->getRound() && !isSink)
			{
				/*
				- speichere das empfangene cdf
				*/
				double received_delay = netPacket->getAvgDelay();
				neighbor_avgNodeDelays[macAddress] = netPacket->getAvgDelay();
	
				for (int i = 0; i <= pDFSlots; i++)
				{	
					neighbor_cdfs[macAddress][i] = netPacket->getNodeCDF().getCDF()[i];
				}
				/*
				- berechne neues CDF
				*/
				convoluteCDF(macAddress);
				addMaxToCDF();	
			}
			break;
		}
		case PLR_DELAY_PACKET:
		{
			break;
		}
	}
}

void PLR::nextRound()
{
	/*
	- set neighbor-delays to 99999
	- fix link avg delays
	*/
	for ( int addr : neighbors) 
	{
		neighbor_avgNodeDelays[addr] = 99999;
		neighbor_avgLinkDelays_fixed[addr] = neighbor_avgLinkDelays[addr];
	}
	/*
	- set own delay to 99999
	- delete next hop
	*/
	avgDelay = 99999;
	nextHop = nextHop_calc;	
	nextHop_calc = -1;

	/*
	- set hop cdfs 0
	- fix link pdfs
	*/
	for ( int addr : neighbors) 
	{
		for (int i = 0; i < pDFSlots; i++)
		{
			neighbor_cdfs[addr][i] = 0;
			neighbor_hop_cdfs[addr][i] = 0;
		}
		createPDF(addr);
	}
	/*
	- setze eigenes cdf auf 0
	- lösche routing-table
	*/
	
	for (int i = 0; i <= pDFSlots; i++)
	{	
		cdf_forTrace[i] = cdf[i];
		cdf[i] = 0;
		routingTable[i] = routingTable_calc[i];
		routingTable_calc[i] = -1;
	}
}


















int PLR::test()
{
	int sum = 0;
	for ( const auto &keyVal : monitoring_receivedFrom) 
	{
		if (keyVal.first == -1 && !isSink) // -1 means, a packet comes from app
		{
			int* vals = keyVal.second;
			
			for (int i = (monitoring_slots_1 * 3) / 4; i < (monitoring_slots_1-1); i++)
			{
				sum += vals[i];
			}	
		}
	}
	return sum;
}



void PLR::finish()
{
	string s1 = "";
	// output, general information
	if (isSink)
	{
		if (useAverageDelay)
		{
			// 0 = none, 1 = above average; 2 = above median; 3 = above threshold
			if (neighborSelectionStrategy == 0)
				trace() << "Algorithm: Avg;none";
			if (neighborSelectionStrategy == 1)
				trace() << "Algorithm: Avg;average";
			if (neighborSelectionStrategy == 2)
				trace() << "Algorithm: Avg;median";
			if (neighborSelectionStrategy == 3)
			{
				trace() << "Algorithm: Avg;thrh"<< neighborSelectionStrategy_value;
			}
		}
		else
		{
			if (useAvgSecond)
			{
				trace() << "Algorithm: PLR; Avg secondary";
			}
			else
			{
				trace() << "Algorithm: PLR";
			}
			
		}
		trace() << "appSendInterval(ms):\t" << appSendInterval;
		trace() << "appMaxTTD(ms):\t" << appMaxTTD;
		trace() << "riceK:\t" << riceK;
		trace() << "txRetries:\t" << txRetries;



		bool flag = true;
		int id = 1;
		double sentPackets = 0;
		while (flag)
		{
			cModule *node_ = getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id);
			if (node_)
			{
				PLR *plrInstance = dynamic_cast<PLR*> (getParentModule()->getParentModule()->getParentModule()->getSubmodule("node",id)->getSubmodule("Communication")->getSubmodule("Routing"));
				sentPackets += plrInstance->test();
				id ++;
			}	
			else
				flag = false;
		}

		double receivedInTime = 0;
		for ( const auto &keyVal : monitoring_SourceOfsuccessfullyDeliveredPackets) 
		{
			int * vals = keyVal.second;
			for (int i = (monitoring_slots_1 * 3) / 4; i < (monitoring_slots_1-1); i++)
			{
				receivedInTime += vals[i];
			}
		}

		double dar = receivedInTime / sentPackets;
		trace() << "created packets in network (during last 25\% of time):\t" << sentPackets;
		trace() << "received in time at sink (during last 25\% of time):\t" << receivedInTime;
		trace() << "DAR (during last 25\% of time):\t" << dar;

		if (! collectReducedTraceInfo)
		{
			trace() << "result time per slot:    " << (simTime/monitoring_slots) << "s";
		}
	}

	
	
	// output, where packets where forwarded to
	for ( const auto &keyVal : monitoring_forwardedTo) 
	{
		s1 = "";
		int* vals = keyVal.second;
		int sum = 0;
		for (int i = 0; i < (monitoring_slots_1-1); i++)
		{
			sum += vals[i];
		}
		if (sum > 0)
		{
			if (keyVal.first == 0 && isSink) // 0 means, the packet arrived in time
			{
				s1 += "received at sink in time:\t";
				for (int i = 0; i < (monitoring_slots_1-1); i++)
				{
					s1 += std::to_string(vals[i]) + "\t";
				}
				trace() << s1;
			}
			if (!isSink)
			{
				s1 += "forwardedTo:\t" + std::to_string(keyVal.first) + ":\t";
				for (int i = 0; i < (monitoring_slots_1-1); i++)
				{
					s1 += std::to_string(vals[i]) + "\t";
				}
				trace() << s1;
			}
		}	
	}
	
	
	// output, how many packets where created at this node
	s1 = "";
	for ( const auto &keyVal : monitoring_receivedFrom) 
	{
		if (keyVal.first == -1 && !isSink) // -1 means, a packet comes from app
		{
			int* vals = keyVal.second;
			int sum = 0;
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				sum += vals[i];
			}
			if (sum > 0)
			{
				stringstream strValue;
				strValue << SELF_NETWORK_ADDRESS;
				unsigned int intValue;
				strValue >> intValue;
				s1 += "created packets:\t";
				for (int i = 0; i < (monitoring_slots_1-1); i++)
				{
					s1 += std::to_string(vals[i]) + "\t";
				}
				trace() << s1;
			}	
			
		}
	}
	
	// output source of packets received at sink successfully
	if (isSink)
	{
		for ( const auto &keyVal : monitoring_SourceOfsuccessfullyDeliveredPackets) 
		{
			s1 = "";
			int * vals = keyVal.second;
			s1 += "delivered source:\t" + std::to_string(keyVal.first) + ":\t";
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				s1 += std::to_string(vals[i]) + "\t";
			}
			trace() << s1;
		}
	}
	
	
	
	// output, where data came from
	for ( const auto &keyVal : monitoring_receivedFrom) 
	{
		s1 = "";
		int* vals = keyVal.second;
		int sum = 0;
		for (int i = 0; i < (monitoring_slots_1-1); i++)
		{
			sum += vals[i];
		}
		if (sum > 0)
		{
			s1 += "receivedFrom:\t" + std::to_string(keyVal.first) + ":\t";
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				s1 += std::to_string(vals[i]) + "\t";
			}
			trace() << s1;
		}	
	}
	
	
	
	// output, how many cdfs where received
	s1 = "receivedCDFs:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedCDFs[i]) + "\t";
	}
	trace() << s1;	

	// output, how many cdfs where broadcasted
	s1 = "broadcastedCDFs:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_broadcastedCDFs[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many probes where sent
	s1 = "sentProbes:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_sentProbes[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many probes where received
	s1 = "receivedProbes:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedProbes[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many data where sent
	s1 = "sentData: \t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_sentData[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many data where received
	s1 = "receivedData:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedData[i]) + "\t";
	}
	trace() << s1;
	
	// output neighbours
	s1 = "neighbours:\t";
	for (int i : neighbors)
	{
		s1 += std::to_string(i) + "\t";
	}
	trace() << s1;
	
	

	string s = "";
	for (int i = 0; i <= pDFSlots; i++)
	{
		s = s + std::to_string(cdf_forTrace[i]) + "\t";
	}
	trace() << "nodeCDF: \t" << s ;

	
	
	for ( const auto &keyVal : monitoring_pdr) 
	{
		trace() << "pdr: " << keyVal.first << ";" << monitoring_pdr[keyVal.first];
	}

	for ( const auto &keyVal : neighbor_avgLinkDelays) 
	{
		trace() << "avg delay: " << keyVal.first << ";" << neighbor_avgLinkDelays[keyVal.first];
	}

	

	/*
	for ( const auto &keyVal : neighbor_pdfs) 
	{
		double* pdf = neighbor_pdfs[keyVal.first];
		s = "";
		double sum = 0;
		for (int i = 0; i <= pDFSlots; i++)
		{
			sum += pdf[i];
			s = s + std::to_string(sum) + "\t";
		}
		trace() << "cdf for link to " << keyVal.first <<":\n"<< s ;	
		trace() << "pdr: " << monitoring_pdr[keyVal.first];
	}
	*/


	
	if (!isSink){
		/*
		// output: routing table; node-cdf; link-cdfs
		string s = "";
		for (int i = 0; i < 25; i++)
		{
			s = s + std::to_string(routingTable[i*pDFSlots/25]) + "\t";
		}
		s = s + std::to_string(routingTable[pDFSlots]);
		trace() << "routingTable: \n" << s ;
		
		
		s = "";
		for (int i = 0; i <= pDFSlots; i++)
		{
			s = s + std::to_string(cdf[i]) + "\t";
		}
		trace() << "nodes cdf: \n" << s ;	
		
		
		
		for ( const auto &keyVal : neighbor_pdfs) 
		{
			double* pdf = neighbor_pdfs[keyVal.first];
			s = "";
			double sum = 0;
			for (int i = 0; i <= pDFSlots; i++)
			{
				sum += pdf[i];
				s = s + std::to_string(sum) + "\t";
			}
			trace() << "cdf for link to " << keyVal.first <<":\n"<< s ;	
		}
		
		for ( const auto &keyVal : neighbor_hop_cdfs) 
		{
			double* hop_cdf = neighbor_hop_cdfs[keyVal.first];
			s = "";
			for (int i = 0; i <= pDFSlots; i++)
			{
				s = s + std::to_string(hop_cdf[i]) + "\t";
			}
			trace() << "cdf for hop to " << keyVal.first <<":\n"<< s ;	
		}
		*/
		
		// output nexthop and its pdr and delay
		int nh = 0;

		nh = routingTable[pDFSlots-1];
		trace() << "PLR nextHop: " << nh  << "; pdr for that link: "<< std::to_string((monitoring_successes[nh] / (monitoring_successes[nh] + monitoring_fails[nh]) ))<< "; hop avgdelay: " << neighbor_avgLinkDelays[nh] << "; succ:" << std::to_string(monitoring_successes[nh])<< "; fails:" << std::to_string(monitoring_fails[nh]);	
		
		nh = nextHop;
		trace() << "Avg nextHop: " << nh  << "; pdr for that link: "<< std::to_string((monitoring_successes[nh] / (monitoring_successes[nh] + monitoring_fails[nh]) ))<< "; hop avgdelay: " << neighbor_avgLinkDelays[nh] << "; succ:" << std::to_string(monitoring_successes[nh])<< "; fails:" << std::to_string(monitoring_fails[nh]);	



		/*
		for ( const auto &keyVal : neighbor_avgLinkDelays) 
		{
			trace() << "avg for link to " << keyVal.first <<":\n"<< std::to_string(neighbor_avgLinkDelays[keyVal.first]);	
		}
		
		for ( const auto &keyVal : neighbor_avgHopDelays) 
		{
			trace() << "avg for hop to " << keyVal.first <<":\n"<< std::to_string(neighbor_avgHopDelays[keyVal.first]);	
		}
		*/
	}
	
	
	
	
	return;
	
	
	
	/*	
	// output source of packets received at sink successfully
	if (isSink)
	{
		s1 = "";
		for ( const auto &keyVal : monitoring_SourceOfsuccessfullyDeliveredPackets) 
		{
			int val = keyVal.second;
			int source = keyVal.first;
			trace() << "received packets with source " << source << " : " << val;
		}
	}

	// output node-cdfs (or avg delays)
	if (useAverageDelay)
	{
		trace() << "avgDelay: \t" << std::to_string(avgDelay);
	}
	else
	{
		s1 = "";
		for (int i = 0; i < 25; i++)
		{
			s1 = s1 + std::to_string(cdf[i*pDFSlots/25]) + "\t";
		}
		s1 = s1 + std::to_string(cdf[pDFSlots]);
		trace() << "cdf: \t" << s1 ;	
	}

	// output, how many packets where created at this node
	s1 = "";
	for ( const auto &keyVal : monitoring_receivedFrom) 
	{
		if (keyVal.first == -1 && !isSink) // -1 means, a packet comes from app
		{
			int* vals = keyVal.second;
			int sum = 0;
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				sum += vals[i];
			}
			if (sum > 0)
			{
				stringstream strValue;
				strValue << SELF_NETWORK_ADDRESS;
				unsigned int intValue;
				strValue >> intValue;
				s1 += "created packets:\t" + std::to_string(sum) + ":\t" ;
				for (int i = 0; i < (monitoring_slots_1-1); i++)
				{
					s1 += std::to_string(vals[i]) + "\t";
				}
				trace() << s1;
			}	
		}
	}
	
	// output, routing table (or nexthop); and nexthop-cdfs (or avg delays)
	if (!isSink)
	{
		if (useAverageDelay)
		{
			trace() << "nextHop:  " << nextHop;
			trace() << "avgDelay: " << std::to_string(avgDelay);
			for ( const auto &keyVal : neighbor_avgLinkDelays) 
			{
				double avg = neighbor_avgLinkDelays[keyVal.first];
				trace() << "avg for link to " << keyVal.first <<": ["<< std::to_string(neighbor_avgLinkDelays[keyVal.first]) << "] + avg of that node: [" << std::to_string(neighbor_avgNodeDelays[keyVal.first]) << "] = hop delay: [" << std::to_string(neighbor_avgHopDelays[keyVal.first]) << "]";	
	    	}
		}
		else
		{
			string s = "";
			
			for (int i = 0; i < 25; i++)
			{
				s = s + std::to_string(routingTable[i*pDFSlots/25]) + "\t";
			}
			s = s + std::to_string(routingTable[pDFSlots]);
			trace() << "routingTable: \n" << s ;
			
			s = "";
			for (int i = 0; i < 25; i++)
			{
				s = s + std::to_string(cdf[i*pDFSlots/25]) + "\t";
			}
			s = s + std::to_string(cdf[pDFSlots]);
			trace() << "cdf: \n" << s ;	
			for ( const auto &keyVal : neighbor_pdfs) 
			{
				double* pdf = neighbor_pdfs[keyVal.first];
				s = "";
				double sum = 0;
				for (int i = 0; i < pDFSlots; i++)
				{
					sum += pdf[i];
					if ((i+1) % (pDFSlots/25) == 0)
					{
						s = s + std::to_string(sum) + "\t";
					}
				}
				trace() << "cdf for link to " << keyVal.first <<":\n"<< s ;	
	    		}
		}
	}

	// output, where data came from
	for ( const auto &keyVal : monitoring_receivedFrom) 
	{
		s1 = "";
		int* vals = keyVal.second;
		int sum = 0;
		for (int i = 0; i < (monitoring_slots_1-1); i++)
		{
			sum += vals[i];
		}
		if (sum > 0)
		{
			s1 += "receivedFrom:\t" + std::to_string(keyVal.first) + ":\t";
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				s1 += std::to_string(vals[i]) + "\t";
			}
			trace() << s1;
		}	
	}
	
	// output, where probes where sent to
	for ( const auto &keyVal : monitoring_probeTo) 
	{
		s1 = "";
		int* vals = keyVal.second;
		int sum = 0;
		for (int i = 0; i < (monitoring_slots_1-1); i++)
		{
			sum += vals[i];
		}
		if (sum > 0)
		{
			s1 += "probeTo:\t" + std::to_string(keyVal.first) + ":\t";
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				s1 += std::to_string(vals[i]) + "\t";
			}
			trace() << s1;	
		}
	}
	
	
	
	// output, where probes came from
	for ( const auto &keyVal : monitoring_probeFrom) 
	{
		s1 = "";
		int* vals = keyVal.second;
		int sum = 0;
		for (int i = 0; i < (monitoring_slots_1-1); i++)
		{
			sum += vals[i];
		}
		if (sum > 0)
		{
			s1 += "probeFrom:\t" + std::to_string(keyVal.first) + ":\t";
			for (int i = 0; i < (monitoring_slots_1-1); i++)
			{
				s1 += std::to_string(vals[i]) + "\t";
			}
			trace() << s1;
		}	
	}

	// output, how many cdfs where received
	s1 = "receivedCDFs:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedCDFs[i]) + "\t";
	}
	trace() << s1;	

	// output, how many cdfs where broadcasted
	s1 = "broadcastedCDFs:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_broadcastedCDFs[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many probes where sent
	s1 = "sentProbes:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_sentProbes[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many probes where received
	s1 = "receivedProbes:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedProbes[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many data where sent
	s1 = "sentData: \t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_sentData[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many data where received
	s1 = "receivedData:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_receivedData[i]) + "\t";
	}
	trace() << s1;
	
	// output, how many data where dropped due to no routing table entry
	s1 = "droppedData:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_droppedData[i]) + "\t";
	}
	trace() << s1;
	*/
}













