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
#include "RoutingPacket_m.h"
#include "ReceiversContainer.h"
}}

class RoutingPacket;
class noncobject ReceiversContainer;

enum OMacRoutingPacket_type {
	OMAC_ROUTING_DATA_PACKET = 1;
	OMAC_ROUTING_HOPCOUNT_PACKET = 2;
	OMAC_ROUTING_CONTROL_PACKET = 3;
}

packet OMacRoutingPacket extends RoutingPacket {
	int OMacRoutingKind enum (OMacRoutingPacket_type);
	unsigned int packetId;
    ReceiversContainer receiversContainer;
}