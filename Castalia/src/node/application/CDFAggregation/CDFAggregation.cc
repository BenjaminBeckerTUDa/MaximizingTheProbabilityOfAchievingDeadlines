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

#include "CDFAggregation.h"

Define_Module(CDFAggregation);

void CDFAggregation::startup()
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
	double randomTime = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * sendInterval + sendInterval / 2;
	setTimer(SEND_AGGREGATED_VALUE, randomTime + 100);

	// seed for random number generator
}

void CDFAggregation::timerFiredCallback(int index)
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
		trace() << "application: toNetworkLayer; next in " << randomTime;
		setTimer(SEND_AGGREGATED_VALUE, randomTime);
		break;
	}
	}
}

void CDFAggregation::fromNetworkLayer(ApplicationPacket *rcvPacket,
										 const char *source, double rssi, double lqi)
{
	double theData = rcvPacket->getData();

	// do the aggregation bit. For this example aggregation function is a simple max.
	if (theData > aggregatedValue)
		aggregatedValue = theData;
	if (isSink)
		trace() << "application: fromNetworkLayer from " << source << "received value " << theData;
}

void CDFAggregation::handleSensorReading(SensorReadingMessage *rcvReading)
{
	string sensType(rcvReading->getSensorType());
	double sensValue = rcvReading->getSensedValue();
	lastSensedValue = sensValue;
}

void CDFAggregation::handleNeworkControlMessage(cMessage *msg)
{
	trace() << "application: handleNeworkControlMessage " << msg->getKind();
}
