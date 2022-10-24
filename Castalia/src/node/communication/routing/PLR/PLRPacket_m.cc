//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/PLR/PLRPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PLRPacket_m.h"

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
    cEnum *e = cEnum::find("PLRPacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("PLRPacketDef"));
    e->insert(PLR_DATA_PACKET, "PLR_DATA_PACKET");
    e->insert(PLR_CDF_PACKET, "PLR_CDF_PACKET");
    e->insert(PLR_DELAY_PACKET, "PLR_DELAY_PACKET");
    e->insert(PLR_PROBE_PACKET, "PLR_PROBE_PACKET");
);

Register_Class(PLRPacket);

PLRPacket::PLRPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->PLRPacketKind_var = 0;
    this->avgDelay_var = 0;
    this->receivedPackets_var = 0;
    this->round_var = 0;
    this->deadline_var = 0;
}

PLRPacket::PLRPacket(const PLRPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

PLRPacket::~PLRPacket()
{
}

PLRPacket& PLRPacket::operator=(const PLRPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void PLRPacket::copy(const PLRPacket& other)
{
    this->PLRPacketKind_var = other.PLRPacketKind_var;
    this->nodeCDF_var = other.nodeCDF_var;
    this->avgDelay_var = other.avgDelay_var;
    this->receivedPackets_var = other.receivedPackets_var;
    this->round_var = other.round_var;
    this->deadline_var = other.deadline_var;
}

void PLRPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->PLRPacketKind_var);
    doPacking(b,this->nodeCDF_var);
    doPacking(b,this->avgDelay_var);
    doPacking(b,this->receivedPackets_var);
    doPacking(b,this->round_var);
    doPacking(b,this->deadline_var);
}

void PLRPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->PLRPacketKind_var);
    doUnpacking(b,this->nodeCDF_var);
    doUnpacking(b,this->avgDelay_var);
    doUnpacking(b,this->receivedPackets_var);
    doUnpacking(b,this->round_var);
    doUnpacking(b,this->deadline_var);
}

int PLRPacket::getPLRPacketKind() const
{
    return PLRPacketKind_var;
}

void PLRPacket::setPLRPacketKind(int PLRPacketKind)
{
    this->PLRPacketKind_var = PLRPacketKind;
}

CDF& PLRPacket::getNodeCDF()
{
    return nodeCDF_var;
}

void PLRPacket::setNodeCDF(const CDF& nodeCDF)
{
    this->nodeCDF_var = nodeCDF;
}

double PLRPacket::getAvgDelay() const
{
    return avgDelay_var;
}

void PLRPacket::setAvgDelay(double avgDelay)
{
    this->avgDelay_var = avgDelay;
}

double PLRPacket::getReceivedPackets() const
{
    return receivedPackets_var;
}

void PLRPacket::setReceivedPackets(double receivedPackets)
{
    this->receivedPackets_var = receivedPackets;
}

int PLRPacket::getRound() const
{
    return round_var;
}

void PLRPacket::setRound(int round)
{
    this->round_var = round;
}

long PLRPacket::getDeadline() const
{
    return deadline_var;
}

void PLRPacket::setDeadline(long deadline)
{
    this->deadline_var = deadline;
}

class PLRPacketDescriptor : public cClassDescriptor
{
  public:
    PLRPacketDescriptor();
    virtual ~PLRPacketDescriptor();

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

Register_ClassDescriptor(PLRPacketDescriptor);

PLRPacketDescriptor::PLRPacketDescriptor() : cClassDescriptor("PLRPacket", "RoutingPacket")
{
}

PLRPacketDescriptor::~PLRPacketDescriptor()
{
}

bool PLRPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PLRPacket *>(obj)!=NULL;
}

const char *PLRPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PLRPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int PLRPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *PLRPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "PLRPacketKind",
        "nodeCDF",
        "avgDelay",
        "receivedPackets",
        "round",
        "deadline",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int PLRPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='P' && strcmp(fieldName, "PLRPacketKind")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeCDF")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "avgDelay")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedPackets")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "round")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "deadline")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PLRPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "CDF",
        "double",
        "double",
        "int",
        "long",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *PLRPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "PLRPacketDef";
            return NULL;
        default: return NULL;
    }
}

int PLRPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PLRPacket *pp = (PLRPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PLRPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PLRPacket *pp = (PLRPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPLRPacketKind());
        case 1: {std::stringstream out; out << pp->getNodeCDF(); return out.str();}
        case 2: return double2string(pp->getAvgDelay());
        case 3: return double2string(pp->getReceivedPackets());
        case 4: return long2string(pp->getRound());
        case 5: return long2string(pp->getDeadline());
        default: return "";
    }
}

bool PLRPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PLRPacket *pp = (PLRPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPLRPacketKind(string2long(value)); return true;
        case 2: pp->setAvgDelay(string2double(value)); return true;
        case 3: pp->setReceivedPackets(string2double(value)); return true;
        case 4: pp->setRound(string2long(value)); return true;
        case 5: pp->setDeadline(string2long(value)); return true;
        default: return false;
    }
}

const char *PLRPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(CDF));
        default: return NULL;
    };
}

void *PLRPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PLRPacket *pp = (PLRPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getNodeCDF()); break;
        default: return NULL;
    }
}


