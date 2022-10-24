//********************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                             *
//*  Developed at the ATP lab, Networked Systems research theme                  *
//*  Author(s): Athanassios Boulis, Dimosthenis Pediaditakis, Yuriy Tselishchev  *
//*  This file is distributed under the terms in the attached LICENSE file.      *
//*  If you do not find this file, copies can be found by writing to:            *
//*                                                                              *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                 *
//*      Attention:  License Inquiry.                                            *
//*                                                                              *
//*******************************************************************************/

package node.communication.routing.PLR;

simple PLR like node.communication.routing.iRouting {
 parameters:
 	//*******************************************************************************/
 	//* mandatory & defined in the iRouting.ned interface                            *
	//*******************************************************************************/
	bool collectTraceInfo = default (false);
	bool collectReducedTraceInfo = default (false);
	int maxNetFrameSize = default (0);					// bytes, 0 means no limit
	int netBufferSize = default (32);					// number of messages
	int netDataFrameOverhead = default (14);				// bytes
	//*******************************************************************************/
 	//* Specific for PLR                                                             *
	//*******************************************************************************/
	int neighborSelectionStrategy = default(0); // 0 = none, 1 = above average; 2 = above median; 3 = above threshold
	double neighborSelectionStrategy_value = default(0.0);
	int pDFSlots = default (1000);					// The number of timeslots for PDFs and CDFs	
	int pDFMessageTimes = default (20);					// The number of slots for PDFs in messages
	bool useAverageDelay = default (true);	
	bool useAvgSecond = default (false);
	double simTime = default(800.0);
	double probeInterval = default(60.0);
	double cdfBroadcastInterval = default(5.0);
	double newRoundInterval = default(60.0);
	double riceK = default(-1.0);
 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

