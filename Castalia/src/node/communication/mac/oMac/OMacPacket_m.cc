//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/oMac/OMacPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "OMacPacket_m.h"

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

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("OMacPacket_type");
    if (!e) enums.getInstance()->add(e = new cEnum("OMacPacket_type"));
    e->insert(OMAC_DATA_PACKET, "OMAC_DATA_PACKET");
    e->insert(OMAC_ACK_PACKET, "OMAC_ACK_PACKET");
    e->insert(OMAC_HOPCOUNT_PACKET, "OMAC_HOPCOUNT_PACKET");
    e->insert(OMAC_CONTROL_PACKET, "OMAC_CONTROL_PACKET");
);

Register_Class(OMacPacket);

OMacPacket::OMacPacket(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->OMacPacketKind_var = 0;
    this->packetId_var = 0;
    this->packetCounter_var = 0;
}

OMacPacket::OMacPacket(const OMacPacket& other) : ::MacPacket(other)
{
    copy(other);
}

OMacPacket::~OMacPacket()
{
}

OMacPacket& OMacPacket::operator=(const OMacPacket& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void OMacPacket::copy(const OMacPacket& other)
{
    this->OMacPacketKind_var = other.OMacPacketKind_var;
    this->receiversContainer_var = other.receiversContainer_var;
    this->packetId_var = other.packetId_var;
    this->packetCounter_var = other.packetCounter_var;
}

void OMacPacket::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->OMacPacketKind_var);
    doPacking(b,this->receiversContainer_var);
    doPacking(b,this->packetId_var);
    doPacking(b,this->packetCounter_var);
}

void OMacPacket::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->OMacPacketKind_var);
    doUnpacking(b,this->receiversContainer_var);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->packetCounter_var);
}

int OMacPacket::getOMacPacketKind() const
{
    return OMacPacketKind_var;
}

void OMacPacket::setOMacPacketKind(int OMacPacketKind)
{
    this->OMacPacketKind_var = OMacPacketKind;
}

ReceiversContainer& OMacPacket::getReceiversContainer()
{
    return receiversContainer_var;
}

void OMacPacket::setReceiversContainer(const ReceiversContainer& receiversContainer)
{
    this->receiversContainer_var = receiversContainer;
}

unsigned int OMacPacket::getPacketId() const
{
    return packetId_var;
}

void OMacPacket::setPacketId(unsigned int packetId)
{
    this->packetId_var = packetId;
}

int OMacPacket::getPacketCounter() const
{
    return packetCounter_var;
}

void OMacPacket::setPacketCounter(int packetCounter)
{
    this->packetCounter_var = packetCounter;
}

class OMacPacketDescriptor : public cClassDescriptor
{
  public:
    OMacPacketDescriptor();
    virtual ~OMacPacketDescriptor();

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

Register_ClassDescriptor(OMacPacketDescriptor);

OMacPacketDescriptor::OMacPacketDescriptor() : cClassDescriptor("OMacPacket", "MacPacket")
{
}

OMacPacketDescriptor::~OMacPacketDescriptor()
{
}

bool OMacPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OMacPacket *>(obj)!=NULL;
}

const char *OMacPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OMacPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int OMacPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *OMacPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "OMacPacketKind",
        "receiversContainer",
        "packetId",
        "packetCounter",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int OMacPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='O' && strcmp(fieldName, "OMacPacketKind")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "receiversContainer")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetCounter")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OMacPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "ReceiversContainer",
        "unsigned int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *OMacPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "OMacPacket_type";
            return NULL;
        default: return NULL;
    }
}

int OMacPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OMacPacket *pp = (OMacPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OMacPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OMacPacket *pp = (OMacPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOMacPacketKind());
        case 1: {std::stringstream out; out << pp->getReceiversContainer(); return out.str();}
        case 2: return ulong2string(pp->getPacketId());
        case 3: return long2string(pp->getPacketCounter());
        default: return "";
    }
}

bool OMacPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OMacPacket *pp = (OMacPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setOMacPacketKind(string2long(value)); return true;
        case 2: pp->setPacketId(string2ulong(value)); return true;
        case 3: pp->setPacketCounter(string2long(value)); return true;
        default: return false;
    }
}

const char *OMacPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(ReceiversContainer));
        default: return NULL;
    };
}

void *OMacPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OMacPacket *pp = (OMacPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getReceiversContainer()); break;
        default: return NULL;
    }
}


