//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/oMac/OMacControl.msg.
//

#ifndef _OMACCONTROL_M_H_
#define _OMACCONTROL_M_H_

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
 * Enum generated from <tt>src/node/communication/mac/oMac/OMacControl.msg:26</tt> by nedtool.
 * <pre>
 * enum OMacControl_type
 * {
 * 
 *     OMAC_RECEIVERS_LIST_REPLY = 1;
 * }
 * </pre>
 */
enum OMacControl_type {
    OMAC_RECEIVERS_LIST_REPLY = 1
};

/**
 * Class generated from <tt>src/node/communication/mac/oMac/OMacControl.msg:30</tt> by nedtool.
 * <pre>
 * packet OMacControl extends MacPacket
 * {
 *     int OMacControlKind @enum(OMacControl_type);
 *     ReceiversContainer receiversContainer;
 * }
 * </pre>
 */
class OMacControl : public ::MacPacket
{
  protected:
    int OMacControlKind_var;
    ReceiversContainer receiversContainer_var;

  private:
    void copy(const OMacControl& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const OMacControl&);

  public:
    OMacControl(const char *name=NULL, int kind=0);
    OMacControl(const OMacControl& other);
    virtual ~OMacControl();
    OMacControl& operator=(const OMacControl& other);
    virtual OMacControl *dup() const {return new OMacControl(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getOMacControlKind() const;
    virtual void setOMacControlKind(int OMacControlKind);
    virtual ReceiversContainer& getReceiversContainer();
    virtual const ReceiversContainer& getReceiversContainer() const {return const_cast<OMacControl*>(this)->getReceiversContainer();}
    virtual void setReceiversContainer(const ReceiversContainer& receiversContainer);
};

inline void doPacking(cCommBuffer *b, OMacControl& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, OMacControl& obj) {obj.parsimUnpack(b);}


#endif // ifndef _OMACCONTROL_M_H_

