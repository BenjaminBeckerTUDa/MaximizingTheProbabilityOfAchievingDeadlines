//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/OMacRoutingPacket/OMacRoutingPacket.msg.
//

#ifndef _OMACROUTINGPACKET_M_H_
#define _OMACROUTINGPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "RoutingPacket_m.h"
#include "ReceiversContainer.h"
// }}

/**
 * Enum generated from <tt>src/node/communication/routing/OMacRoutingPacket/OMacRoutingPacket.msg:21</tt> by nedtool.
 * <pre>
 * enum OMacRoutingPacket_type
 * {
 * 
 *     OMAC_ROUTING_DATA_PACKET = 1;
 *     OMAC_ROUTING_HOPCOUNT_PACKET = 2;
 *     OMAC_ROUTING_CONTROL_PACKET = 3;
 * }
 * </pre>
 */
enum OMacRoutingPacket_type {
    OMAC_ROUTING_DATA_PACKET = 1,
    OMAC_ROUTING_HOPCOUNT_PACKET = 2,
    OMAC_ROUTING_CONTROL_PACKET = 3
};

/**
 * Class generated from <tt>src/node/communication/routing/OMacRoutingPacket/OMacRoutingPacket.msg:27</tt> by nedtool.
 * <pre>
 * packet OMacRoutingPacket extends RoutingPacket
 * {
 *     int OMacRoutingKind @enum(OMacRoutingPacket_type);
 *     unsigned int packetId;
 *     ReceiversContainer receiversContainer;
 *     double deadline;
 * }
 * </pre>
 */
class OMacRoutingPacket : public ::RoutingPacket
{
  protected:
    int OMacRoutingKind_var;
    unsigned int packetId_var;
    ReceiversContainer receiversContainer_var;
    double deadline_var;

  private:
    void copy(const OMacRoutingPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const OMacRoutingPacket&);

  public:
    OMacRoutingPacket(const char *name=NULL, int kind=0);
    OMacRoutingPacket(const OMacRoutingPacket& other);
    virtual ~OMacRoutingPacket();
    OMacRoutingPacket& operator=(const OMacRoutingPacket& other);
    virtual OMacRoutingPacket *dup() const {return new OMacRoutingPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getOMacRoutingKind() const;
    virtual void setOMacRoutingKind(int OMacRoutingKind);
    virtual unsigned int getPacketId() const;
    virtual void setPacketId(unsigned int packetId);
    virtual ReceiversContainer& getReceiversContainer();
    virtual const ReceiversContainer& getReceiversContainer() const {return const_cast<OMacRoutingPacket*>(this)->getReceiversContainer();}
    virtual void setReceiversContainer(const ReceiversContainer& receiversContainer);
    virtual double getDeadline() const;
    virtual void setDeadline(double deadline);
};

inline void doPacking(cCommBuffer *b, OMacRoutingPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, OMacRoutingPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _OMACROUTINGPACKET_M_H_

