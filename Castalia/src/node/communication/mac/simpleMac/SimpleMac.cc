/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Yuriy Tselishchev, Athanassios Boulis                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "SimpleMac.h"
#include "SimpleRoutingPacket_m.h"
#include "OMacPacket_m.h"

Define_Module(SimpleMac);

void SimpleMac::timerFiredCallback(int timer)
{

    switch (timer)
    {
    case 1:
    {
        trace() << "reached the timerCallback";
        sendData();
        break;
    }

    default:
        break;
    }
}

/* Handle packet received from upper (network) layer.
 * We need to create a MAC packet, (here it can just be the generic MacPacket)
 * and encapsulate the received network packet before forwarding it to RadioLayer
 */
void SimpleMac::fromNetworkLayer(cPacket *netPkt, int destination)
{
    MacPacket *macFrame = new MacPacket("BypassRouting packet", MAC_LAYER_PACKET);
    encapsulatePacket(macFrame, netPkt);
    macFrame->setSource(SELF_MAC_ADDRESS);
    macFrame->setDestination(destination);
    toRadioLayer(macFrame);
    toRadioLayer(createRadioCommand(SET_STATE, TX));
    // trace() << "MAC source: " << SELF_MAC_ADDRESS << " destination: " << destination;
}

/* Handle packet received from lower (radio) layer.
 * We accept packets from all MAC protocols (cast to the base class MacPacket)
 * Then we filter by the destination field. By default we set the generic
 * destination field to broadcast when we encapsulate a NET packet. If a
 * specific protocol does not change that field then SimpleMac will be
 * operating in a promiscuous mode.
 */
void SimpleMac::fromRadioLayer(cPacket *pkt, double rssi, double lqi)
{
    MacPacket *macPkt = dynamic_cast<MacPacket *>(pkt);
    MacPacket *newMac = new MacPacket("test", MAC_LAYER_PACKET);
    cPacket *simplePkt = decapsulatePacket(macPkt);
    encapsulatePacket(newMac, simplePkt);
    newMac->setSource(macPkt->getSource());
    newMac->setDestination(macPkt->getDestination());
    newMac->setSequenceNumber(macPkt->getSequenceNumber());

    if (macPkt == NULL)
        return;
    if (macPkt->getDestination() == SELF_MAC_ADDRESS ||
        macPkt->getDestination() == BROADCAST_MAC_ADDRESS)
    {
        buffer.push(newMac);
        setTimer(1, 0.005);
        trace() << "set timer";
    }
}

void SimpleMac::sendData()
{
    toNetworkLayer(decapsulatePacket(buffer.front()));
    buffer.pop();
    trace() << "sent data to net";
}

void SimpleMac::finishSpecific()
{
    trace() << "---------------------------MAC finished---------------------------";
}