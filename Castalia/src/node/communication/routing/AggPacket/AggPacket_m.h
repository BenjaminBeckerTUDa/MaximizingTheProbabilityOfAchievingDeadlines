//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/AggPacket/AggPacket.msg.
//

#ifndef _AGGPACKET_M_H_
#define _AGGPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "OMacRoutingPacket_m.h"
#include "CFP.h"
#include "AGGL.h"
// }}

/**
 * Class generated from <tt>src/node/communication/routing/AggPacket/AggPacket.msg:23</tt> by nedtool.
 * <pre>
 * packet AggPacket extends OMacRoutingPacket
 * {
 *     AGGL packets;
 *     CFP deadlines;
 * }
 * </pre>
 */
class AggPacket : public ::OMacRoutingPacket
{
  protected:
    AGGL packets_var;
    CFP deadlines_var;

  private:
    void copy(const AggPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AggPacket&);

  public:
    AggPacket(const char *name=NULL, int kind=0);
    AggPacket(const AggPacket& other);
    virtual ~AggPacket();
    AggPacket& operator=(const AggPacket& other);
    virtual AggPacket *dup() const {return new AggPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual AGGL& getPackets();
    virtual const AGGL& getPackets() const {return const_cast<AggPacket*>(this)->getPackets();}
    virtual void setPackets(const AGGL& packets);
    virtual CFP& getDeadlines();
    virtual const CFP& getDeadlines() const {return const_cast<AggPacket*>(this)->getDeadlines();}
    virtual void setDeadlines(const CFP& deadlines);
};

inline void doPacking(cCommBuffer *b, AggPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, AggPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _AGGPACKET_M_H_

