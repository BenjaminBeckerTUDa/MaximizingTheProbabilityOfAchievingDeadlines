//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/ODAR/ODARPacket.msg.
//

#ifndef _ODARPACKET_M_H_
#define _ODARPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "OMacRoutingPacket_m.h"
#include "CFP.h"
// }}

/**
 * Class generated from <tt>src/node/communication/routing/ODAR/ODARPacket.msg:21</tt> by nedtool.
 * <pre>
 * packet ODARPacket extends OMacRoutingPacket
 * {
 *     int hopcount;
 *     CFP CDF;
 *     CFP neighbors;
 *     CFP overheardPackets;
 *     int round;
 *     int CDFversion;
 * }
 * </pre>
 */
class ODARPacket : public ::OMacRoutingPacket
{
  protected:
    int hopcount_var;
    CFP CDF_var;
    CFP neighbors_var;
    CFP overheardPackets_var;
    int round_var;
    int CDFversion_var;

  private:
    void copy(const ODARPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ODARPacket&);

  public:
    ODARPacket(const char *name=NULL, int kind=0);
    ODARPacket(const ODARPacket& other);
    virtual ~ODARPacket();
    ODARPacket& operator=(const ODARPacket& other);
    virtual ODARPacket *dup() const {return new ODARPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getHopcount() const;
    virtual void setHopcount(int hopcount);
    virtual CFP& getCDF();
    virtual const CFP& getCDF() const {return const_cast<ODARPacket*>(this)->getCDF();}
    virtual void setCDF(const CFP& CDF);
    virtual CFP& getNeighbors();
    virtual const CFP& getNeighbors() const {return const_cast<ODARPacket*>(this)->getNeighbors();}
    virtual void setNeighbors(const CFP& neighbors);
    virtual CFP& getOverheardPackets();
    virtual const CFP& getOverheardPackets() const {return const_cast<ODARPacket*>(this)->getOverheardPackets();}
    virtual void setOverheardPackets(const CFP& overheardPackets);
    virtual int getRound() const;
    virtual void setRound(int round);
    virtual int getCDFversion() const;
    virtual void setCDFversion(int CDFversion);
};

inline void doPacking(cCommBuffer *b, ODARPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ODARPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _ODARPACKET_M_H_

