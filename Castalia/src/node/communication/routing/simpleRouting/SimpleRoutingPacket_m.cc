//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/simpleRouting/SimpleRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SimpleRoutingPacket_m.h"

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
    cEnum *e = cEnum::find("SimpleRoutingPacket_type");
    if (!e) enums.getInstance()->add(e = new cEnum("SimpleRoutingPacket_type"));
    e->insert(SIMPLE_ROUTING_DATA_PACKET, "SIMPLE_ROUTING_DATA_PACKET");
    e->insert(SIMPLE_ROUTING_HOPCOUNT_PACKET, "SIMPLE_ROUTING_HOPCOUNT_PACKET");
    e->insert(SIMPLE_ROUTING_HOPCOUNT_ACK_PACKET, "SIMPLE_ROUTING_HOPCOUNT_ACK_PACKET");
);

Register_Class(SimpleRoutingPacket);

SimpleRoutingPacket::SimpleRoutingPacket(const char *name, int kind) : ::OMacRoutingPacket(name,kind)
{
    this->SimpleRoutingKind_var = 0;
    this->hopcount_var = 0;
}

SimpleRoutingPacket::SimpleRoutingPacket(const SimpleRoutingPacket& other) : ::OMacRoutingPacket(other)
{
    copy(other);
}

SimpleRoutingPacket::~SimpleRoutingPacket()
{
}

SimpleRoutingPacket& SimpleRoutingPacket::operator=(const SimpleRoutingPacket& other)
{
    if (this==&other) return *this;
    ::OMacRoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void SimpleRoutingPacket::copy(const SimpleRoutingPacket& other)
{
    this->SimpleRoutingKind_var = other.SimpleRoutingKind_var;
    this->hopcount_var = other.hopcount_var;
}

void SimpleRoutingPacket::parsimPack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimPack(b);
    doPacking(b,this->SimpleRoutingKind_var);
    doPacking(b,this->hopcount_var);
}

void SimpleRoutingPacket::parsimUnpack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->SimpleRoutingKind_var);
    doUnpacking(b,this->hopcount_var);
}

int SimpleRoutingPacket::getSimpleRoutingKind() const
{
    return SimpleRoutingKind_var;
}

void SimpleRoutingPacket::setSimpleRoutingKind(int SimpleRoutingKind)
{
    this->SimpleRoutingKind_var = SimpleRoutingKind;
}

int SimpleRoutingPacket::getHopcount() const
{
    return hopcount_var;
}

void SimpleRoutingPacket::setHopcount(int hopcount)
{
    this->hopcount_var = hopcount;
}

class SimpleRoutingPacketDescriptor : public cClassDescriptor
{
  public:
    SimpleRoutingPacketDescriptor();
    virtual ~SimpleRoutingPacketDescriptor();

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

Register_ClassDescriptor(SimpleRoutingPacketDescriptor);

SimpleRoutingPacketDescriptor::SimpleRoutingPacketDescriptor() : cClassDescriptor("SimpleRoutingPacket", "OMacRoutingPacket")
{
}

SimpleRoutingPacketDescriptor::~SimpleRoutingPacketDescriptor()
{
}

bool SimpleRoutingPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SimpleRoutingPacket *>(obj)!=NULL;
}

const char *SimpleRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SimpleRoutingPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int SimpleRoutingPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SimpleRoutingPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "SimpleRoutingKind",
        "hopcount",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int SimpleRoutingPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='S' && strcmp(fieldName, "SimpleRoutingKind")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopcount")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SimpleRoutingPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *SimpleRoutingPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "SimpleRoutingPacket_type";
            return NULL;
        default: return NULL;
    }
}

int SimpleRoutingPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingPacket *pp = (SimpleRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SimpleRoutingPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingPacket *pp = (SimpleRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSimpleRoutingKind());
        case 1: return long2string(pp->getHopcount());
        default: return "";
    }
}

bool SimpleRoutingPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingPacket *pp = (SimpleRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSimpleRoutingKind(string2long(value)); return true;
        case 1: pp->setHopcount(string2long(value)); return true;
        default: return false;
    }
}

const char *SimpleRoutingPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *SimpleRoutingPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingPacket *pp = (SimpleRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


