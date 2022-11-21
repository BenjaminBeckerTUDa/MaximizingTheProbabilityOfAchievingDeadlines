//********************************************************************************
//*  Copyright: National ICT Australia,  2007 - 2011                             *
//*  Developed at the ATP lab, Networked Systems research theme                  *
//*  Author(s): Yuriy Tselishchev, Athanassios Boulis                            *
//*  This file is distributed under the terms in the attached LICENSE file.      *
//*  If you do not find this file, copies can be found by writing to:            *
//*                                                                              *
//*      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia                 *
//*      Attention:  License Inquiry.                                            *
//*                                                                              *
//*******************************************************************************/

// We need to pass information between app and routing layer which is external to
// the packet i.e. not carried by a real packet (e.g., which is the destination,
// or what was the RSSI for the packet received) but this information is related
// to the specific packet. Since information is passed between modules with
// messages/packets, we decided to encode this kind of external info as a 
// separate structure in the packet. The fields there are handled by the
// virtualApp and virtualRouting code, setting a framework of interaction.


cplusplus {{
#include "ApplicationPacket_m.h"
}}

class ApplicationPacket;

packet DeadlinePacket extends ApplicationPacket{
	double deadline;
}

