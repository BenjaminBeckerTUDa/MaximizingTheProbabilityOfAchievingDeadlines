//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/AggPacket/AggPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AggPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(AggPacket);

AggPacket::AggPacket(const char *name, int kind) : ::OMacRoutingPacket(name,kind)
{
}

AggPacket::AggPacket(const AggPacket& other) : ::OMacRoutingPacket(other)
{
    copy(other);
}

AggPacket::~AggPacket()
{
}

AggPacket& AggPacket::operator=(const AggPacket& other)
{
    if (this==&other) return *this;
    ::OMacRoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void AggPacket::copy(const AggPacket& other)
{
    this->packets_var = other.packets_var;
    this->deadlines_var = other.deadlines_var;
}

void AggPacket::parsimPack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimPack(b);
    doPacking(b,this->packets_var);
    doPacking(b,this->deadlines_var);
}

void AggPacket::parsimUnpack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->packets_var);
    doUnpacking(b,this->deadlines_var);
}

AGGL& AggPacket::getPackets()
{
    return packets_var;
}

void AggPacket::setPackets(const AGGL& packets)
{
    this->packets_var = packets;
}

CFP& AggPacket::getDeadlines()
{
    return deadlines_var;
}

void AggPacket::setDeadlines(const CFP& deadlines)
{
    this->deadlines_var = deadlines;
}

class AggPacketDescriptor : public cClassDescriptor
{
  public:
    AggPacketDescriptor();
    virtual ~AggPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(AggPacketDescriptor);

AggPacketDescriptor::AggPacketDescriptor() : cClassDescriptor("AggPacket", "OMacRoutingPacket")
{
}

AggPacketDescriptor::~AggPacketDescriptor()
{
}

bool AggPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AggPacket *>(obj)!=NULL;
}

const char *AggPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AggPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int AggPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *AggPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packets",
        "deadlines",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int AggPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packets")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "deadlines")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AggPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "AGGL",
        "CFP",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *AggPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int AggPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AggPacket *pp = (AggPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AggPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AggPacket *pp = (AggPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getPackets(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDeadlines(); return out.str();}
        default: return "";
    }
}

bool AggPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AggPacket *pp = (AggPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *AggPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(AGGL));
        case 1: return opp_typename(typeid(CFP));
        default: return NULL;
    };
}

void *AggPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AggPacket *pp = (AggPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getPackets()); break;
        case 1: return (void *)(&pp->getDeadlines()); break;
        default: return NULL;
    }
}


