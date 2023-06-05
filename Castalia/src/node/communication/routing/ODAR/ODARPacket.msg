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

cplusplus {{
#include "OMacRoutingPacket_m.h"
#include "CFP.h"
}}

class OMacRoutingPacket;
class noncobject CFP;

packet ODARPacket extends OMacRoutingPacket {
	int hopcount;
	CFP CDF;
	CFP neighbors;
	CFP overheardPackets;
	int round;
	int CDFversion;

	double time_ForMonitoring;
}
