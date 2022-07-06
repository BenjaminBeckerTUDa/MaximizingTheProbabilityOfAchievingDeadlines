#ifndef _SIMPLEMAC_H_
#define _SIMPLEMAC_H_

#include <omnetpp.h>
#include "VirtualMac.h"

using namespace std;

class SimpleMac : public VirtualMac
{
    /* In order to create a MAC based on VirtualMacModule, we need to define only two
     * functions: one to handle a packet received from the layer above (routing),
     * and one to handle a packet from the layer below (radio)
     */

private:
    queue<MacPacket *> buffer;

protected:
    void fromRadioLayer(cPacket *, double, double);
    void fromNetworkLayer(cPacket *, int);
    void timerFiredCallback(int);
    void sendData();

    virtual void finishSpecific();
};

#endif