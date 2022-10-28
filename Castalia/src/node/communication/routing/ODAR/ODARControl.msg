//****************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2010                         *
//*  Developed at the ATP lab, Networked Systems research theme              *
//*  Author(s): Dimosthenis Pediaditakis, Yuriy Tselishchev                  *
//*  This file is distributed under the terms in the attached LICENSE file.  *
//*  If you do not find this file, copies can be found by writing to:        *
//*                                                                          *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
//*      Attention:  License Inquiry.                                        *
//*                                                                          *  
//****************************************************************************/


enum ODARControlDef {
	INC_RCV = 1;
	INC_TX = 2;
	REPLY_TIMES = 3;
	REQUEST_TIMES  = 4;
}

message ODARControlMessage {
	int ODARControlMessageKind enum (ODARControlDef);
	int senderAddress;
	int databytelength;
	simtime_t requiredTimeForACK;
	simtime_t requiredTimeForDATA;
	simtime_t minTimeForCA;
	simtime_t maxTimeForCA;
	int selfMACAdress;
}

