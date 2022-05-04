//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/PLR/PLRPacket.msg.
//

#ifndef _PLRPACKET_M_H_
#define _PLRPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "RoutingPacket_m.h"
#include "CDF.h"
// }}

/**
 * Enum generated from <tt>src/node/communication/routing/PLR/PLRPacket.msg:21</tt> by nedtool.
 * <pre>
 * enum PLRPacketDef
 * {
 * 
 *     PLR_DATA_PACKET = 1;
 *     PLR_CDF_PACKET = 2;
 *     PLR_DELAY_PACKET = 3;
 *     PLR_PROBE_PACKET = 4;
 * }
 * </pre>
 */
enum PLRPacketDef {
    PLR_DATA_PACKET = 1,
    PLR_CDF_PACKET = 2,
    PLR_DELAY_PACKET = 3,
    PLR_PROBE_PACKET = 4
};

/**
 * Class generated from <tt>src/node/communication/routing/PLR/PLRPacket.msg:28</tt> by nedtool.
 * <pre>
 * packet PLRPacket extends RoutingPacket
 * {
 *     int PLRPacketKind @enum(PLRPacketDef);
 *     CDF nodeCDF;
 *     double avgDelay;
 *     int round;
 *     long deadline;
 * }
 * </pre>
 */
class PLRPacket : public ::RoutingPacket
{
  protected:
    int PLRPacketKind_var;
    CDF nodeCDF_var;
    double avgDelay_var;
    int round_var;
    long deadline_var;

  private:
    void copy(const PLRPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PLRPacket&);

  public:
    PLRPacket(const char *name=NULL, int kind=0);
    PLRPacket(const PLRPacket& other);
    virtual ~PLRPacket();
    PLRPacket& operator=(const PLRPacket& other);
    virtual PLRPacket *dup() const {return new PLRPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getPLRPacketKind() const;
    virtual void setPLRPacketKind(int PLRPacketKind);
    virtual CDF& getNodeCDF();
    virtual const CDF& getNodeCDF() const {return const_cast<PLRPacket*>(this)->getNodeCDF();}
    virtual void setNodeCDF(const CDF& nodeCDF);
    virtual double getAvgDelay() const;
    virtual void setAvgDelay(double avgDelay);
    virtual int getRound() const;
    virtual void setRound(int round);
    virtual long getDeadline() const;
    virtual void setDeadline(long deadline);
};

inline void doPacking(cCommBuffer *b, PLRPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PLRPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _PLRPACKET_M_H_
