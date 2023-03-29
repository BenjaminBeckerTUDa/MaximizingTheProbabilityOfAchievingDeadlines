//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/ODAR/ODARControl.msg.
//

#ifndef _ODARCONTROL_M_H_
#define _ODARCONTROL_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Enum generated from <tt>src/node/communication/routing/ODAR/ODARControl.msg:14</tt> by nedtool.
 * <pre>
 * enum ODARControlDef
 * {
 * 
 *     INC_RCV = 1;
 *     INC_TX = 2;
 *     REPLY_TIMES = 3;
 *     REQUEST_TIMES = 4;
 * }
 * </pre>
 */
enum ODARControlDef {
    INC_RCV = 1,
    INC_TX = 2,
    REPLY_TIMES = 3,
    REQUEST_TIMES = 4
};

/**
 * Class generated from <tt>src/node/communication/routing/ODAR/ODARControl.msg:21</tt> by nedtool.
 * <pre>
 * message ODARControlMessage
 * {
 *     int ODARControlMessageKind @enum(ODARControlDef);
 *     int senderAddress;
 *     int databytelength;
 *     simtime_t requiredTimeForACK;
 *     simtime_t requiredTimeForDATA;
 *     simtime_t minTimeForCA;
 *     simtime_t maxTimeForCA;
 *     int selfMACAdress;
 *     int packetCounter;
 * }
 * </pre>
 */
class ODARControlMessage : public ::cMessage
{
  protected:
    int ODARControlMessageKind_var;
    int senderAddress_var;
    int databytelength_var;
    simtime_t requiredTimeForACK_var;
    simtime_t requiredTimeForDATA_var;
    simtime_t minTimeForCA_var;
    simtime_t maxTimeForCA_var;
    int selfMACAdress_var;
    int packetCounter_var;

  private:
    void copy(const ODARControlMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ODARControlMessage&);

  public:
    ODARControlMessage(const char *name=NULL, int kind=0);
    ODARControlMessage(const ODARControlMessage& other);
    virtual ~ODARControlMessage();
    ODARControlMessage& operator=(const ODARControlMessage& other);
    virtual ODARControlMessage *dup() const {return new ODARControlMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getODARControlMessageKind() const;
    virtual void setODARControlMessageKind(int ODARControlMessageKind);
    virtual int getSenderAddress() const;
    virtual void setSenderAddress(int senderAddress);
    virtual int getDatabytelength() const;
    virtual void setDatabytelength(int databytelength);
    virtual simtime_t getRequiredTimeForACK() const;
    virtual void setRequiredTimeForACK(simtime_t requiredTimeForACK);
    virtual simtime_t getRequiredTimeForDATA() const;
    virtual void setRequiredTimeForDATA(simtime_t requiredTimeForDATA);
    virtual simtime_t getMinTimeForCA() const;
    virtual void setMinTimeForCA(simtime_t minTimeForCA);
    virtual simtime_t getMaxTimeForCA() const;
    virtual void setMaxTimeForCA(simtime_t maxTimeForCA);
    virtual int getSelfMACAdress() const;
    virtual void setSelfMACAdress(int selfMACAdress);
    virtual int getPacketCounter() const;
    virtual void setPacketCounter(int packetCounter);
};

inline void doPacking(cCommBuffer *b, ODARControlMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ODARControlMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef _ODARCONTROL_M_H_
