//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2011                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/

cplusplus {{
#include "RoutingPacket_m.h"
#include "CDF.h"
}}

class RoutingPacket;
class noncobject CDF;

enum PLRPacketDef {
	PLR_DATA_PACKET = 1;
	PLR_CDF_PACKET = 2;
	PLR_DELAY_PACKET = 3;
	PLR_PROBE_PACKET = 4;
} 

packet PLRPacket extends RoutingPacket {
	int PLRPacketKind enum (PLRPacketDef);
	CDF nodeCDF;
	double avgDelay;
	double receivedPackets;
	int round;
	long deadline;
}

