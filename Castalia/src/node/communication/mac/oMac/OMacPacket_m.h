//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/oMac/OMacPacket.msg.
//

#ifndef _OMACPACKET_M_H_
#define _OMACPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "MacPacket_m.h"
#include "ReceiversContainer.h"
// }}

/**
 * Enum generated from <tt>src/node/communication/mac/oMac/OMacPacket.msg:26</tt> by nedtool.
 * <pre>
 * enum OMacPacket_type
 * {
 * 
 *     OMAC_DATA_PACKET = 1;
 *     OMAC_ACK_PACKET = 2;
 *     OMAC_HOPCOUNT_PACKET = 3;
 * }
 * </pre>
 */
enum OMacPacket_type {
    OMAC_DATA_PACKET = 1,
    OMAC_ACK_PACKET = 2,
    OMAC_HOPCOUNT_PACKET = 3
};

/**
 * Class generated from <tt>src/node/communication/mac/oMac/OMacPacket.msg:32</tt> by nedtool.
 * <pre>
 * packet OMacPacket extends MacPacket
 * {
 *     int OMacPacketKind @enum(OMacPacket_type);
 *     ReceiversContainer receiversContainer;
 *     unsigned int packetId;
 * }
 * </pre>
 */
class OMacPacket : public ::MacPacket
{
  protected:
    int OMacPacketKind_var;
    ReceiversContainer receiversContainer_var;
    unsigned int packetId_var;

  private:
    void copy(const OMacPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const OMacPacket&);

  public:
    OMacPacket(const char *name=NULL, int kind=0);
    OMacPacket(const OMacPacket& other);
    virtual ~OMacPacket();
    OMacPacket& operator=(const OMacPacket& other);
    virtual OMacPacket *dup() const {return new OMacPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getOMacPacketKind() const;
    virtual void setOMacPacketKind(int OMacPacketKind);
    virtual ReceiversContainer& getReceiversContainer();
    virtual const ReceiversContainer& getReceiversContainer() const {return const_cast<OMacPacket*>(this)->getReceiversContainer();}
    virtual void setReceiversContainer(const ReceiversContainer& receiversContainer);
    virtual unsigned int getPacketId() const;
    virtual void setPacketId(unsigned int packetId);
};

inline void doPacking(cCommBuffer *b, OMacPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, OMacPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _OMACPACKET_M_H_

