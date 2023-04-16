/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis                 *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "SimpleAggregation.h"

Define_Module(SimpleAggregation);

void SimpleAggregation::startup()
{
	maxTTD = par("maxTTD");
	sendInterval = (double)par("sendInterval") / 1000;
	packetSize = (int)par("packetSize");
	aggregatedValue = 0.0;
	waitingTimeForLowerLevelData = 0.0;
	lastSensedValue = 0.0;
	totalPackets = 0;

	stringstream strValue;
	strValue << SELF_NETWORK_ADDRESS;
	unsigned int intValue;
	strValue >> intValue;
	std::srand(intValue);
	double randomTime = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * sendInterval;
	setTimer(SEND_AGGREGATED_VALUE, randomTime);

	// Logging configuration parameters
	if(isSink){
    	trace() << "sendInterval: " << sendInterval*1000 << " ms";
    	trace() << "Simlulation time limit: " << ev.getConfig()->getConfigValue("sim-time-limit");
    	trace() << "Packet size: " << (double)par("packetSize") << " byte";
    	trace() << "Max TTD: " << (double)par("maxTTD") << " ms";
	}
	

	// seed for random number generator
}

void SimpleAggregation::timerFiredCallback(int index)
{
	switch (index)
	{
	case SEND_AGGREGATED_VALUE:
	{
		DeadlinePacket *newPacket = new DeadlinePacket("App generic packet", APPLICATION_PACKET);
		newPacket->setSequenceNumber(totalPackets);
		newPacket->setByteLength(packetSize);
		newPacket->setDeadline(maxTTD + (getClock().dbl()*1000) );

		toNetworkLayer(newPacket, "sink");

		totalPackets++;
		double randomTime = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * sendInterval + sendInterval / 2;
		//trace() << "application: toNetworkLayer; next in " << randomTime;
		setTimer(SEND_AGGREGATED_VALUE, randomTime);
		break;
	}
	}
}

void SimpleAggregation::fromNetworkLayer(ApplicationPacket *rcvPacket,
										 const char *source, double rssi, double lqi)
{
	double theData = rcvPacket->getData();

	// do the aggregation bit. For this example aggregation function is a simple max.
	if (theData > aggregatedValue)
		aggregatedValue = theData;
	//if (isSink)
		//trace() << "application: fromNetworkLayer from " << source << "received value " << theData;
}

void SimpleAggregation::handleSensorReading(SensorReadingMessage *rcvReading)
{
	string sensType(rcvReading->getSensorType());
	double sensValue = rcvReading->getSensedValue();
	lastSensedValue = sensValue;
}

void SimpleAggregation::handleNeworkControlMessage(cMessage *msg)
{
	//trace() << "application: handleNeworkControlMessage " << msg->getKind();
}
