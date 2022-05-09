/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2012                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include <cmath>
#include "TMAC.h"
#include "PLRPacket_m.h"

	
Define_Module(TMAC);

void TMAC::startup()
{
	printStateTransitions = par("printStateTransitions");
	ackPacketSize = par("ackPacketSize");
	frameTime = ((double)par("frameTime")) / 1000.0;	// just convert msecs to secs
	rtsPacketSize = par("rtsPacketSize");
	ctsPacketSize = par("ctsPacketSize");
	contentionPeriod = ((double)par("contentionPeriod")) / 1000.0;	// just convert msecs to secs
	listenTimeout = ((double)par("listenTimeout")) / 1000.0;	// TA: just convert msecs to secs (15ms default);
	waitTimeout = ((double)par("waitTimeout")) / 1000.0;
	useFRTS = par("useFrts");
	useRtsCts = par("useRtsCts");
	maxTxRetries = par("maxTxRetries");
	simTime = par("simTime");
	
	
	
	
	disableTAextension = par("disableTAextension");
	conservativeTA = par("conservativeTA");
	collisionResolution = par("collisionResolution");
	
	
	
	if (collisionResolution != 2 && collisionResolution != 1 && collisionResolution != 0) {
		//"Unknown value for parameter 'collisionResolution', will default to 1";
		collisionResolution = 1;
	}
	//Initialise state descriptions used in debug output
	if (printStateTransitions) {
		stateDescr[102] = "MAC_STATE_ACTIVE";
		stateDescr[103] = "MAC_STATE_ACTIVE_SILENT";
		stateDescr[104] = "MAC_STATE_IN_TX";
		stateDescr[110] = "MAC_CARRIER_SENSE_FOR_TX_RTS";
		stateDescr[111] = "MAC_CARRIER_SENSE_FOR_TX_DATA";
		stateDescr[120] = "MAC_STATE_WAIT_FOR_DATA";
		stateDescr[121] = "MAC_STATE_WAIT_FOR_CTS";
		stateDescr[122] = "MAC_STATE_WAIT_FOR_ACK";
	}

	phyDataRate = par("phyDataRate");
	phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0;	//parameter given in ms in the omnetpp.ini
	phyLayerOverhead = par("phyFrameOverhead");

	//try to obtain the value of isSink parameter from application module
	if (getParentModule()->getParentModule()->findSubmodule("Application") != -1) {
		cModule *tmpApplication = getParentModule()->getParentModule()->getSubmodule("Application");
		isSink = tmpApplication->hasPar("isSink") ? tmpApplication->par("isSink") : false;
	} else {
		isSink = false;
	}

	rtsPacket = NULL;
	ackPacket = NULL;
	ctsPacket = NULL;

	macState = MAC_STATE_ACTIVE;
	scheduleTable.clear();
	primaryWakeup = true;
	currentFrameStart = -1;

	declareOutput("Sent packets breakdown");

	toRadioLayer(createRadioCommand(SET_CS_INTERRUPT_ON));
	
	// monitoring
	monitoring_slots = 100.0;
	monitoring_slots_1 = ((int) monitoring_slots)+1;
	monitoring_queueDelay = new double[monitoring_slots_1]{0};
	monitoring_queueDelayCounter = new int[monitoring_slots_1]{0};
	
	
	
}

void TMAC::timerFiredCallback(int timer)
{
	switch (timer) {
		case FRAME_START:{

			// record the current time and extend activation timeout
			currentFrameStart = getClock();
			if (macState == MAC_STATE_ACTIVE || macState == MAC_STATE_ACTIVE_SILENT) {
				resetDefaultState("new frame started");
			}
			break;
		}

		case CARRIER_SENSE:{
			/* First it is important to check for valid MAC state
			 * If we heard something on the radio while waiting to start carrier sense,
			 * then MAC was set to MAC_STATE_ACTIVE_SILENT. In this case we can not transmit
			 * and there is no point to perform carrier sense
			 */
			if (macState == MAC_STATE_ACTIVE_SILENT)
				break;

			// At this stage MAC can only be in one of the states MAC_CARRIER_SENSE_...
			if (macState != MAC_CARRIER_SENSE_FOR_TX_RTS && macState != MAC_CARRIER_SENSE_FOR_TX_DATA) {
				//"WARNING: bad MAC state for MAC_SELF_PERFORM_CARRIER_SENSE: " << stateDescr[macState];
				break;
			}
			switch (radioModule->isChannelClear()) {
				case CLEAR:{
					carrierIsClear();
					break;
				}

				case BUSY:{
					carrierIsBusy();
					break;
				}

				case CS_NOT_VALID_YET:{
					setTimer(CARRIER_SENSE, phyDelayForValidCS);
					break;
				}

				case CS_NOT_VALID:{
					toRadioLayer(createRadioCommand(SET_STATE, RX));
					setTimer(CARRIER_SENSE, phyDelayForValidCS);
					break;
				}
			}
			break;
		}

		case TRANSMISSION_TIMEOUT:{
			resetDefaultState("timeout");
			break;
		}


		default:{
			break;
		}
	}
}

int TMAC::handleRadioControlMessage(cMessage * msg)
{
	RadioControlMessage *radioMsg = check_and_cast <RadioControlMessage*>(msg);
	if (radioMsg->getRadioControlMessageKind() == CARRIER_SENSE_INTERRUPT)
		carrierIsBusy();

	toNetworkLayer(msg);
	return 1;
}

void TMAC::fromNetworkLayer(cPacket * netPkt, int destination)
{
	/*
	PLRPacket *netPacket = dynamic_cast <PLRPacket*>(netPkt);
	if (netPacket)
	{
		switch (netPacket->getPLRPacketKind()) 
		{
			case PLR_DATA_PACKET:
			{
				int sourceInt = std::stoi(netPacket->getSource());
				
				if (sourceInt == 18)
				{
					trace() << "mac from net = source, sn, buffersize " << sourceInt <<","<<netPacket->getSequenceNumber()<<","<<TXBuffer.size();
				}
			}
		}
	}
	*/
	
	
	// Create a new MAC frame from the received packet and buffer it (if possible)
	TMacPacket *macPkt = new TMacPacket("TMAC data packet", MAC_LAYER_PACKET);
	encapsulatePacket(macPkt, netPkt);
	macPkt->setType(DATA_TMAC_PACKET);
	macPkt->setSource(SELF_MAC_ADDRESS);
	macPkt->setDestination(destination);
	//macPkt->setSequenceNumber(txSequenceNum); no need for TMAC specific seq number, virtualMAC takes care of this 
	if (bufferPacket(macPkt)) 
	{	// this is causing problems
		//"state " << stateDescr[macState];
		if (TXBuffer.size() == 1)
			checkTxBuffer();
		// bb : add a timestamp for this specific packet; maybe include sequence number. encapsulatePacket(macPkt, netPkt); added a sequence number, which we can use here
		if (destination != BROADCAST_MAC_ADDRESS){
			int sn = macPkt->getSequenceNumber();
			enqueueTimes[sn] = getClock();
			//"bb: delay: enqueueTime for sn [" << sn << "] "<< " to " << destination << " = " << getClock();
		}
		resetDefaultState("why not??");
	} 
	else 
	{
		if (destination != BROADCAST_MAC_ADDRESS)
		{
			// bb : send a control message to upper layer 
			//"bb: delay: inf (full buffer)" << macPkt->getSequenceNumber();		
			PLRControlMessage* plrc = new PLRControlMessage("PLR delay packet", NETWORK_CONTROL_COMMAND);
			plrc->setPLRControlMessageKind(FAIL);
			plrc->setTxAddr(destination);
			toNetworkLayer(plrc);	
		}
		


		
		
		// cancelAndDelete(macPkt);
		//We could send a control message to upper layer to inform of full buffer
	}
	
}

/* This function will reset the internal MAC state in the following way:
 * 3 -  IF this is not primary wakeup, MAC can only listen, thus set state to MAC_STATE_ACTIVE_SILENT
 */
void TMAC::resetDefaultState(const char *descr)
{
	if (descr)
	{
		//"Resetting MAC state to default, reason: " << descr;
	}

	simtime_t randomContentionInterval = genk_dblrand(1) * contentionPeriod;

	while (!TXBuffer.empty()) {
		if (txRetries <= 0) {
			// bb no txRetries left => max delay
			TMacPacket *macPkt = check_and_cast < TMacPacket * >(TXBuffer.front());
			if (macPkt -> getDestination() != BROADCAST_MAC_ADDRESS) {					
				enqueueTimes.erase(macPkt -> getSequenceNumber());
				PLRControlMessage* plrc = new PLRControlMessage("PLR delay packet", NETWORK_CONTROL_COMMAND);
				plrc->setPLRControlMessageKind(FAIL);
				plrc->setTxAddr(macPkt -> getDestination());
				maxTxRetriesPerReceiver[macPkt -> getDestination()] = maxTxRetriesPerReceiver[macPkt -> getDestination()]/2;
				if (maxTxRetriesPerReceiver[macPkt -> getDestination()] == 0){
					maxTxRetriesPerReceiver[macPkt -> getDestination()] = 1;
				}
				toNetworkLayer(plrc);
				//"bb: delay: inf (max txRetries)" << txAddr;
			}
			popTxBuffer();
		} else {
			if (useRtsCts && txAddr != BROADCAST_MAC_ADDRESS) {
				performCarrierSense(MAC_CARRIER_SENSE_FOR_TX_RTS, randomContentionInterval);
			} else {
				performCarrierSense(MAC_CARRIER_SENSE_FOR_TX_DATA, randomContentionInterval);
			}
			return;
		}
	}
	setMacState(MAC_STATE_ACTIVE, "nothing to transmit");

}

/* Helper function to change internal MAC state and print a debug statement if neccesary */
void TMAC::setMacState(int newState, const char *descr)
{
	if (macState == newState)
		return;
	if (printStateTransitions) {
		if (descr)
		{
			//"state changed from " << stateDescr[macState] <<					" to " << stateDescr[newState] << ", reason: " << descr;
		}
		else
		{
			//"state changed from " << stateDescr[macState] <<					" to " << stateDescr[newState];
		}
	}
	macState = newState;
}

/* This function will handle a MAC frame received from the lower layer (physical or radio)
 * We try to see if the received packet is TMacPacket, otherwise we discard it
 * TMAC ignores values of RSSI and LQI
 */
void TMAC::fromRadioLayer(cPacket * pkt, double RSSI, double LQI)
{
	TMacPacket *macPkt = dynamic_cast < TMacPacket * >(pkt);
	if (macPkt == NULL)
		return;

	int source = macPkt->getSource();
	int destination = macPkt->getDestination();
	simtime_t nav = macPkt->getNav();

	// first of all, check if the packet is to this node or not
	if (destination != SELF_MAC_ADDRESS && destination != BROADCAST_MAC_ADDRESS) {
		if (macState == MAC_CARRIER_SENSE_FOR_TX_RTS && useFRTS) {
			//FRTS would need to be implemented here, for now just keep silent
		}
		if (collisionResolution != 2 && nav > 0)
			setTimer(TRANSMISSION_TIMEOUT, nav);
		//setMacState(MAC_STATE_ACTIVE_SILENT, "overheard a packet");
		return;
	}

	switch (macPkt->getType()) {

		/* received a RTS frame */
		case RTS_TMAC_PACKET:{
			//Since this node is the destination (checked above), reply with a CTS
			if (ctsPacket)
				cancelAndDelete(ctsPacket);
			ctsPacket = new TMacPacket("TMAC CTS packet", MAC_LAYER_PACKET);
			ctsPacket->setType(CTS_TMAC_PACKET);
			ctsPacket->setSource(SELF_MAC_ADDRESS);
			ctsPacket->setDestination(source);
			ctsPacket->setNav(nav - TX_TIME(ctsPacketSize));
			ctsPacket->setByteLength(ctsPacketSize);

			// Send CTS packet to radio
			toRadioLayer(ctsPacket);
			toRadioLayer(createRadioCommand(SET_STATE, TX));

			collectOutput("Sent packets breakdown", "CTS");
			ctsPacket = NULL;

			// update MAC state
			setMacState(MAC_STATE_WAIT_FOR_DATA, "sent CTS packet");

			// create a timeout for expecting a DATA packet reply
			setTimer(TRANSMISSION_TIMEOUT, TX_TIME(ctsPacketSize) + waitTimeout);
			break;
		}

		/* received a CTS frame */
		case CTS_TMAC_PACKET:{
			if (macState == MAC_STATE_WAIT_FOR_CTS) {
				if (TXBuffer.empty()) {
					//"WARNING: invalid MAC_STATE_WAIT_FOR_CTS while buffer is empty";
					resetDefaultState("invalid state while buffer is empty");
				} else if (source == txAddr) {
					cancelTimer(TRANSMISSION_TIMEOUT);
					sendDataPacket();
				} else {
					//"WARNING: recieved unexpected CTS from " << source;
					resetDefaultState("unexpected CTS");
				}
			}
			break;
		}

		/* received DATA frame */
		case DATA_TMAC_PACKET:{
			
			
			
			/*
			PLRPacket *netPacket = dynamic_cast <PLRPacket*>(decapsulatePacket(macPkt));
			if (netPacket)
			{
				switch (netPacket->getPLRPacketKind()) 
				{
					case PLR_DATA_PACKET:
					{
						int sourceInt = std::stoi(netPacket->getSource());
						
						if (sourceInt == 18)
						{
							trace() << "mac received = source, sn " << sourceInt <<","<<netPacket->getSequenceNumber();
						}
					}
				}
			}
			*/
			
			
			
			// Forward the frame to upper layer first
			if (isNotDuplicatePacket(macPkt))
				toNetworkLayer(decapsulatePacket(macPkt));

			// If the frame was sent to broadcast address, nothing else needs to be done
			if (destination == BROADCAST_MAC_ADDRESS)
				break;

			// If MAC was expecting this frame, clear the timeout
			if (macState == MAC_STATE_WAIT_FOR_DATA)
				cancelTimer(TRANSMISSION_TIMEOUT);

			// Create and send an ACK frame (since this node is the destination for DATA frame)
			if (ackPacket)
				cancelAndDelete(ackPacket);
			ackPacket = new TMacPacket("TMAC ACK packet", MAC_LAYER_PACKET);
			ackPacket->setSource(SELF_MAC_ADDRESS);
			ackPacket->setDestination(source);
			ackPacket->setType(ACK_TMAC_PACKET);
			ackPacket->setByteLength(ackPacketSize);
			ackPacket->setSequenceNumber(macPkt->getSequenceNumber());

			// Send ACK packet to the radio
			toRadioLayer(ackPacket);
			toRadioLayer(createRadioCommand(SET_STATE, TX));

			collectOutput("Sent packets breakdown", "ACK");
			ackPacket = NULL;

			// update MAC state
			setMacState(MAC_STATE_IN_TX, "transmitting ACK packet");

			// create a timeout for this transmission - nothing is expected in reply
			// so MAC is only waiting for the RADIO to finish the packet transmission
			setTimer(TRANSMISSION_TIMEOUT, TX_TIME(ackPacketSize));
			break;
		}

		/* received ACK frame */
		case ACK_TMAC_PACKET:{
			if (macState == MAC_STATE_WAIT_FOR_ACK && source == txAddr) {
				//"Transmission succesful to " << txAddr;
				cancelTimer(TRANSMISSION_TIMEOUT);
				popTxBuffer();
				resetDefaultState("transmission successful (ACK received)");
				
				// bb : retrieve timestamp for this ack			
				double time_ = getClock().dbl() - enqueueTimes[waitForAck_].dbl();
				enqueueTimes.erase(waitForAck_);
				
				//"queue+send-Time of " << waitForAck_ << " = " << time_;
				
				//"bb: just received ACK for packet with sn [" << waitForAck_ << "] to adress " << source <<" with time: " << time_;
				PLRControlMessage* plrc = new PLRControlMessage("PLR delay packet", NETWORK_CONTROL_COMMAND);
				plrc->setDelay(time_);
				plrc->setPLRControlMessageKind(DELAY);
				plrc->setTxAddr(source);
				toNetworkLayer(plrc);
				
				maxTxRetriesPerReceiver[macPkt -> getDestination()] = maxTxRetriesPerReceiver[macPkt -> getDestination()]*2;
				if (maxTxRetriesPerReceiver[macPkt -> getDestination()] > maxTxRetries){
					maxTxRetriesPerReceiver[macPkt -> getDestination()] = maxTxRetries;
				}

			}
			break;
		}

		default:{
			//"Packet with unknown type (" << macPkt->getType() << ") received: [" << macPkt->getName() << "]";
		}
	}
}

void TMAC::carrierIsBusy()
{
	/* Since we are hearing some communication on the radio we need to do two things:
	 * 2 - set MAC state to MAC_STATE_ACTIVE_SILENT unless we are actually expecting to receive
	 *     something 
	 */

	/*
	if (TXBuffer.empty()) {
	}
	else
	{
		switch (macState) {
			case MAC_CARRIER_SENSE_FOR_TX_RTS:{
				MacPacket *macPacket1 = dynamic_cast <MacPacket*>(TXBuffer.front());
				MacPacket *macPacket2 = macPacket1->dup();
				PLRPacket *netPacket = dynamic_cast <PLRPacket*>(decapsulatePacket(macPacket2));
				if (netPacket)
				{
					switch (netPacket->getPLRPacketKind()) 
					{
						case PLR_DATA_PACKET:
						{
							int sourceInt = std::stoi(netPacket->getSource());
							
							if (sourceInt == 18)
							{
								trace() << "mac carrier busy; cannot tx rts = source, sn " << sourceInt <<","<<netPacket->getSequenceNumber();
							}
						}
					}
				}
				break;
			}

			case MAC_CARRIER_SENSE_FOR_TX_DATA:{
				MacPacket *macPacket1 = dynamic_cast <MacPacket*>(TXBuffer.front());
				MacPacket *macPacket2 = macPacket1->dup();
				PLRPacket *netPacket = dynamic_cast <PLRPacket*>(decapsulatePacket(macPacket2));
				if (netPacket)
				{
					switch (netPacket->getPLRPacketKind()) 
					{
						case PLR_DATA_PACKET:
						{
							int sourceInt = std::stoi(netPacket->getSource());
							
							if (sourceInt == 18)
							{
								trace() << "mac carrier busy; cannot tx data = source, sn " << sourceInt <<","<<netPacket->getSequenceNumber();
							}
						}
					}
				}
				break;
			}
		}
	}
	*/
	
	
	
	
	
	
	
	if (collisionResolution == 0) {
		if (macState == MAC_CARRIER_SENSE_FOR_TX_RTS
				|| macState == MAC_CARRIER_SENSE_FOR_TX_DATA) {
			resetDefaultState("sensed carrier");
		}
	} else {
		if (macState != MAC_STATE_WAIT_FOR_ACK
				&& macState != MAC_STATE_WAIT_FOR_DATA
				&& macState != MAC_STATE_WAIT_FOR_CTS)
			setMacState(MAC_STATE_ACTIVE_SILENT, "sensed carrier");
	}
}

/* This function handles carrier clear message, received from the radio module.
 * That is sent in a response to previous request to perform a carrier sense
 */
void TMAC::carrierIsClear()
{
	switch (macState) {
		/* MAC requested carrier sense to transmit an RTS packet */
		case MAC_CARRIER_SENSE_FOR_TX_RTS:{
			/*
			if (TXBuffer.empty()) {
			}
			else
			{
				MacPacket *macPacket1 = dynamic_cast <MacPacket*>(TXBuffer.front());
				MacPacket *macPacket2 = macPacket1->dup();
				PLRPacket *netPacket = dynamic_cast <PLRPacket*>(decapsulatePacket(macPacket2));
				if (netPacket)
				{
					switch (netPacket->getPLRPacketKind()) 
					{
						case PLR_DATA_PACKET:
						{
							int sourceInt = std::stoi(netPacket->getSource());
							
							if (sourceInt == 18)
							{
								trace() << "mac tx rts = source, sn " << sourceInt <<","<<netPacket->getSequenceNumber();
							}
						}
					}
				}
			}
			*/
			
			if (TXBuffer.empty()) {
				//"WARNING! BUFFER_IS_EMPTY in MAC_CARRIER_SENSE_FOR_TX_RTS, will reset state";
				resetDefaultState("empty transmission buffer");
				break;
			}
			// create and send RTS frame
			if (rtsPacket)
				cancelAndDelete(rtsPacket);
			rtsPacket = new TMacPacket("RTS message", MAC_LAYER_PACKET);
			rtsPacket->setSource(SELF_MAC_ADDRESS);
			rtsPacket->setDestination(txAddr);
			rtsPacket->setNav(TX_TIME(rtsPacketSize) + TX_TIME(ctsPacketSize) +
					  TX_TIME(ackPacketSize) + TX_TIME(TXBuffer.front()->getByteLength()));
			rtsPacket->setType(RTS_TMAC_PACKET);
			rtsPacket->setByteLength(rtsPacketSize);
			toRadioLayer(rtsPacket);
			toRadioLayer(createRadioCommand(SET_STATE, TX));

			if (useRtsCts)
				txRetries--;
			collectOutput("Sent packets breakdown", "RTS");
			rtsPacket = NULL;

			// update MAC state
			setMacState(MAC_STATE_WAIT_FOR_CTS, "sent RTS packet");

			// create a timeout for expecting a CTS reply
			setTimer(TRANSMISSION_TIMEOUT, TX_TIME(rtsPacketSize) + waitTimeout);
			break;
		}

		/* MAC requested carrier sense to transmit DATA packet */
		case MAC_CARRIER_SENSE_FOR_TX_DATA:
		{
			/*
			if (TXBuffer.empty()) {
			}
			else
			{
				MacPacket *macPacket1 = dynamic_cast <MacPacket*>(TXBuffer.front());
				MacPacket *macPacket2 = macPacket1->dup();
				PLRPacket *netPacket = dynamic_cast <PLRPacket*>(decapsulatePacket(macPacket2));
				if (netPacket)
				{
					switch (netPacket->getPLRPacketKind()) 
					{
						case PLR_DATA_PACKET:
						{
							int sourceInt = std::stoi(netPacket->getSource());
							
							if (sourceInt == 18)
							{
								trace() << "mac tx data = source, sn " << sourceInt <<","<<netPacket->getSequenceNumber();
							}
						}
					}
				}
			}
			*/
	
			sendDataPacket();
			break;
		}
	}
}

void TMAC::sendDataPacket()
{
	if (TXBuffer.empty()) {
		//"WARNING: Invalid MAC_CARRIER_SENSE_FOR_TX_DATA while TX buffer is empty";
		resetDefaultState("empty transmission buffer");
		return;
	}
	
	

	
	// create a copy of first message in the TX buffer and send it to the radio
	toRadioLayer(TXBuffer.front()->dup());
	collectOutput("Sent packets breakdown", "DATA");

	//update MAC state based on transmission time and destination address
	double txTime = TX_TIME(TXBuffer.front()->getByteLength());

	if (txAddr == BROADCAST_MAC_ADDRESS) {
		// This packet is broadcast, so no reply will be received
		// The packet can be cleared from transmission buffer
		// and MAC timeout is only to allow RADIO to finish the transmission
		popTxBuffer();
		setMacState(MAC_STATE_IN_TX, "sent DATA packet to BROADCAST address");
		setTimer(TRANSMISSION_TIMEOUT, txTime);
	} else {
		// bb: adding packet-SequenceNumber here, assuming next ACK is for it 
		MacPacket *macPacket = dynamic_cast <MacPacket*>(TXBuffer.front());
		if (waitForAck_ != macPacket->getSequenceNumber())
		{
			
			
			double time_ = getClock().dbl() - enqueueTimes[macPacket->getSequenceNumber()].dbl();
			//"queue-Time of " << macPacket->getSequenceNumber() << " = " << time_ << " queue-Length " << TXBuffer.size();
			monitoring_queueDelayCounter[(int) (getClock().dbl()/simTime * monitoring_slots)]++;
			monitoring_queueDelay[(int) (getClock().dbl()/simTime * monitoring_slots)] += time_;
		}
		waitForAck_ = macPacket->getSequenceNumber();
		
		//"bb: just send packet with sn [" << waitForAck_ << "]" << " to " << macPacket->getDestination();
		
		// This packet is unicast, so MAC will be expecting an ACK
		// packet in reply, so the timeout is longer
		// If we are not using RTS/CTS exchange, then this attempt
		// also decreases the txRetries counter
		// (NOTE: with RTS/CTS exchange sending RTS packet decrements counter)
		if (!useRtsCts)
			txRetries--;
		setMacState(MAC_STATE_WAIT_FOR_ACK, "sent DATA packet to UNICAST address");
		setTimer(TRANSMISSION_TIMEOUT, txTime + waitTimeout);
		
	}

	//update RADIO state
	toRadioLayer(createRadioCommand(SET_STATE, TX));
}

/* This function will set a timer to perform carrier sense.
 * MAC state is important when performing CS, so setMacState is always called here.
 * delay allows to perform a carrier sense after a choosen delay (useful for
 * randomisation of transmissions)
 */
void TMAC::performCarrierSense(int newState, simtime_t delay)
{
	setMacState(newState);
	setTimer(CARRIER_SENSE, delay);
}


/* This function will check the transmission buffer, and if it is not empty, it will update
 * current communication parameters: txAddr and txRetries
 */
void TMAC::checkTxBuffer()
{
	if (TXBuffer.empty())
		return;
	TMacPacket *macPkt = check_and_cast < TMacPacket * >(TXBuffer.front());
	txAddr = macPkt->getDestination();
	
	if (maxTxRetriesPerReceiver.find(txAddr) == maxTxRetriesPerReceiver.end()) 
	{
		maxTxRetriesPerReceiver.insert({txAddr, maxTxRetries});
	}
	
	
	txRetries = maxTxRetriesPerReceiver[txAddr];
	// txSequenceNum = 0;  no need for TMAC specific seq number, virtualMAC takes care of this 
}

/* This function will remove the first packet from MAC transmission buffer
 * checkTxBuffer is called in case there are still packets left in the buffer to transmit
 */
void TMAC::popTxBuffer()
{
	cancelAndDelete(TXBuffer.front());

	TXBuffer.pop();
	checkTxBuffer();
}

void TMAC::finish()
{
	if (isSink) return;
	string s1 = "";
	// output queueDelay
	s1 = "queueDelay:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_queueDelay[i]) + "\t";
	}
	trace() << s1;	
	
	s1 = "queueDelayCounter:\t";
	for (int i = 0; i < (monitoring_slots_1-1); i++)
	{
		s1 += std::to_string(monitoring_queueDelayCounter[i]) + "\t";
	}
	trace() << s1;	
}
