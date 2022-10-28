//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/ODAR/ODARPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ODARPacket_m.h"

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

Register_Class(ODARPacket);

ODARPacket::ODARPacket(const char *name, int kind) : ::OMacRoutingPacket(name,kind)
{
    this->hopcount_var = 0;
}

ODARPacket::ODARPacket(const ODARPacket& other) : ::OMacRoutingPacket(other)
{
    copy(other);
}

ODARPacket::~ODARPacket()
{
}

ODARPacket& ODARPacket::operator=(const ODARPacket& other)
{
    if (this==&other) return *this;
    ::OMacRoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void ODARPacket::copy(const ODARPacket& other)
{
    this->hopcount_var = other.hopcount_var;
    this->CDF_var = other.CDF_var;
    this->neighbors_var = other.neighbors_var;
    this->overheardPackets_var = other.overheardPackets_var;
}

void ODARPacket::parsimPack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimPack(b);
    doPacking(b,this->hopcount_var);
    doPacking(b,this->CDF_var);
    doPacking(b,this->neighbors_var);
    doPacking(b,this->overheardPackets_var);
}

void ODARPacket::parsimUnpack(cCommBuffer *b)
{
    ::OMacRoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->hopcount_var);
    doUnpacking(b,this->CDF_var);
    doUnpacking(b,this->neighbors_var);
    doUnpacking(b,this->overheardPackets_var);
}

int ODARPacket::getHopcount() const
{
    return hopcount_var;
}

void ODARPacket::setHopcount(int hopcount)
{
    this->hopcount_var = hopcount;
}

CFP& ODARPacket::getCDF()
{
    return CDF_var;
}

void ODARPacket::setCDF(const CFP& CDF)
{
    this->CDF_var = CDF;
}

CFP& ODARPacket::getNeighbors()
{
    return neighbors_var;
}

void ODARPacket::setNeighbors(const CFP& neighbors)
{
    this->neighbors_var = neighbors;
}

CFP& ODARPacket::getOverheardPackets()
{
    return overheardPackets_var;
}

void ODARPacket::setOverheardPackets(const CFP& overheardPackets)
{
    this->overheardPackets_var = overheardPackets;
}

class ODARPacketDescriptor : public cClassDescriptor
{
  public:
    ODARPacketDescriptor();
    virtual ~ODARPacketDescriptor();

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

Register_ClassDescriptor(ODARPacketDescriptor);

ODARPacketDescriptor::ODARPacketDescriptor() : cClassDescriptor("ODARPacket", "OMacRoutingPacket")
{
}

ODARPacketDescriptor::~ODARPacketDescriptor()
{
}

bool ODARPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ODARPacket *>(obj)!=NULL;
}

const char *ODARPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ODARPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int ODARPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *ODARPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "hopcount",
        "CDF",
        "neighbors",
        "overheardPackets",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int ODARPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopcount")==0) return base+0;
    if (fieldName[0]=='C' && strcmp(fieldName, "CDF")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbors")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "overheardPackets")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ODARPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "CFP",
        "CFP",
        "CFP",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *ODARPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ODARPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ODARPacket *pp = (ODARPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ODARPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ODARPacket *pp = (ODARPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopcount());
        case 1: {std::stringstream out; out << pp->getCDF(); return out.str();}
        case 2: {std::stringstream out; out << pp->getNeighbors(); return out.str();}
        case 3: {std::stringstream out; out << pp->getOverheardPackets(); return out.str();}
        default: return "";
    }
}

bool ODARPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ODARPacket *pp = (ODARPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopcount(string2long(value)); return true;
        default: return false;
    }
}

const char *ODARPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(CFP));
        case 2: return opp_typename(typeid(CFP));
        case 3: return opp_typename(typeid(CFP));
        default: return NULL;
    };
}

void *ODARPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ODARPacket *pp = (ODARPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getCDF()); break;
        case 2: return (void *)(&pp->getNeighbors()); break;
        case 3: return (void *)(&pp->getOverheardPackets()); break;
        default: return NULL;
    }
}


