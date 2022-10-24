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

enum PLRControlDef {
	DELAY = 1;
	FAIL = 2;
	PDR = 3;
}

message PLRControlMessage {
	int PLRControlMessageKind enum (PLRControlDef);
	double delay;
	double pdr;
	int txAddr;
}

