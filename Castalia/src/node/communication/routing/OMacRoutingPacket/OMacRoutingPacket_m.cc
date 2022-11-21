//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/OMacRoutingPacket/OMacRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "OMacRoutingPacket_m.h"

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
    cEnum *e = cEnum::find("OMacRoutingPacket_type");
    if (!e) enums.getInstance()->add(e = new cEnum("OMacRoutingPacket_type"));
    e->insert(OMAC_ROUTING_DATA_PACKET, "OMAC_ROUTING_DATA_PACKET");
    e->insert(OMAC_ROUTING_HOPCOUNT_PACKET, "OMAC_ROUTING_HOPCOUNT_PACKET");
    e->insert(OMAC_ROUTING_CONTROL_PACKET, "OMAC_ROUTING_CONTROL_PACKET");
);

Register_Class(OMacRoutingPacket);

OMacRoutingPacket::OMacRoutingPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->OMacRoutingKind_var = 0;
    this->packetId_var = 0;
    this->deadline_var = 0;
}

OMacRoutingPacket::OMacRoutingPacket(const OMacRoutingPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

OMacRoutingPacket::~OMacRoutingPacket()
{
}

OMacRoutingPacket& OMacRoutingPacket::operator=(const OMacRoutingPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void OMacRoutingPacket::copy(const OMacRoutingPacket& other)
{
    this->OMacRoutingKind_var = other.OMacRoutingKind_var;
    this->packetId_var = other.packetId_var;
    this->receiversContainer_var = other.receiversContainer_var;
    this->deadline_var = other.deadline_var;
}

void OMacRoutingPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->OMacRoutingKind_var);
    doPacking(b,this->packetId_var);
    doPacking(b,this->receiversContainer_var);
    doPacking(b,this->deadline_var);
}

void OMacRoutingPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->OMacRoutingKind_var);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->receiversContainer_var);
    doUnpacking(b,this->deadline_var);
}

int OMacRoutingPacket::getOMacRoutingKind() const
{
    return OMacRoutingKind_var;
}

void OMacRoutingPacket::setOMacRoutingKind(int OMacRoutingKind)
{
    this->OMacRoutingKind_var = OMacRoutingKind;
}

unsigned int OMacRoutingPacket::getPacketId() const
{
    return packetId_var;
}

void OMacRoutingPacket::setPacketId(unsigned int packetId)
{
    this->packetId_var = packetId;
}

ReceiversContainer& OMacRoutingPacket::getReceiversContainer()
{
    return receiversContainer_var;
}

void OMacRoutingPacket::setReceiversContainer(const ReceiversContainer& receiversContainer)
{
    this->receiversContainer_var = receiversContainer;
}

double OMacRoutingPacket::getDeadline() const
{
    return deadline_var;
}

void OMacRoutingPacket::setDeadline(double deadline)
{
    this->deadline_var = deadline;
}

class OMacRoutingPacketDescriptor : public cClassDescriptor
{
  public:
    OMacRoutingPacketDescriptor();
    virtual ~OMacRoutingPacketDescriptor();

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

Register_ClassDescriptor(OMacRoutingPacketDescriptor);

OMacRoutingPacketDescriptor::OMacRoutingPacketDescriptor() : cClassDescriptor("OMacRoutingPacket", "RoutingPacket")
{
}

OMacRoutingPacketDescriptor::~OMacRoutingPacketDescriptor()
{
}

bool OMacRoutingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OMacRoutingPacket *>(obj)!=NULL;
}

const char *OMacRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OMacRoutingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int OMacRoutingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *OMacRoutingPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "OMacRoutingKind",
        "packetId",
        "receiversContainer",
        "deadline",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int OMacRoutingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='O' && strcmp(fieldName, "OMacRoutingKind")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "receiversContainer")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "deadline")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OMacRoutingPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "unsigned int",
        "ReceiversContainer",
        "double",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *OMacRoutingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "OMacRoutingPacket_type";
            return NULL;
        default: return NULL;
    }
}

int OMacRoutingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OMacRoutingPacket *pp = (OMacRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OMacRoutingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OMacRoutingPacket *pp = (OMacRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOMacRoutingKind());
        case 1: return ulong2string(pp->getPacketId());
        case 2: {std::stringstream out; out << pp->getReceiversContainer(); return out.str();}
        case 3: return double2string(pp->getDeadline());
        default: return "";
    }
}

bool OMacRoutingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OMacRoutingPacket *pp = (OMacRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setOMacRoutingKind(string2long(value)); return true;
        case 1: pp->setPacketId(string2ulong(value)); return true;
        case 3: pp->setDeadline(string2double(value)); return true;
        default: return false;
    }
}

const char *OMacRoutingPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 2: return opp_typename(typeid(ReceiversContainer));
        default: return NULL;
    };
}

void *OMacRoutingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OMacRoutingPacket *pp = (OMacRoutingPacket *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getReceiversContainer()); break;
        default: return NULL;
    }
}


