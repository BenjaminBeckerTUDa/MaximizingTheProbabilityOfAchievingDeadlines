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
#include "MacPacket_m.h"
#include "ReceiversContainer.h"
}}


class MacPacket;
class noncobject ReceiversContainer;


enum OMacPacket_type {
    OMAC_DATA_PACKET = 1;
    OMAC_ACK_PACKET = 2;
    OMAC_HOPCOUNT_PACKET = 3;
    OMAC_CONTROL_PACKET = 4;
}

packet OMacPacket extends MacPacket {
    int OMacPacketKind enum (OMacPacket_type);
    ReceiversContainer receiversContainer;
    unsigned int packetId;
    int packetCounter;
}
