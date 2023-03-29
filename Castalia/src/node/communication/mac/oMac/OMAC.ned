//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Yuriy Tselishchev                                            *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

package node.communication.mac.oMac;

simple OMAC like node.communication.mac.iMac {
 parameters:
    //debug parameters
	bool collectTraceInfo = true;//default (false);

	//mac layer parameters
	int macMaxPacketSize = default (0);		//no limit on frame size
	int macPacketOverhead = default (11);	//DATA frame overhead is described in TMacFrame.msg
	int macBufferSize = default (32);		//buffer of 32 packets by default
	int ackPacketSize = default (11);


	//OMAC protocol parameters
	int maxTxRetries = default (2);
	double contentionPeriod = default (10);	// 10 ms
	double waitTimeout = default (6);		// timeout for expecting a reply to DATA


	//parameters dependent on physical layer
	double phyDelayForValidCS = default (0.128);
	double phyDataRate = default (250);
	int phyFrameOverhead = default (6);

 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}