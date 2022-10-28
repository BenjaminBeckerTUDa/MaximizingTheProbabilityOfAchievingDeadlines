//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/ODAR/ODARControl.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ODARControl_m.h"

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
    cEnum *e = cEnum::find("ODARControlDef");
    if (!e) enums.getInstance()->add(e = new cEnum("ODARControlDef"));
    e->insert(INC_RCV, "INC_RCV");
    e->insert(INC_TX, "INC_TX");
    e->insert(REPLY_TIMES, "REPLY_TIMES");
    e->insert(REQUEST_TIMES, "REQUEST_TIMES");
);

Register_Class(ODARControlMessage);

ODARControlMessage::ODARControlMessage(const char *name, int kind) : ::cMessage(name,kind)
{
    this->ODARControlMessageKind_var = 0;
    this->senderAddress_var = 0;
    this->databytelength_var = 0;
    this->requiredTimeForACK_var = 0;
    this->requiredTimeForDATA_var = 0;
    this->minTimeForCA_var = 0;
    this->maxTimeForCA_var = 0;
    this->selfMACAdress_var = 0;
}

ODARControlMessage::ODARControlMessage(const ODARControlMessage& other) : ::cMessage(other)
{
    copy(other);
}

ODARControlMessage::~ODARControlMessage()
{
}

ODARControlMessage& ODARControlMessage::operator=(const ODARControlMessage& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ODARControlMessage::copy(const ODARControlMessage& other)
{
    this->ODARControlMessageKind_var = other.ODARControlMessageKind_var;
    this->senderAddress_var = other.senderAddress_var;
    this->databytelength_var = other.databytelength_var;
    this->requiredTimeForACK_var = other.requiredTimeForACK_var;
    this->requiredTimeForDATA_var = other.requiredTimeForDATA_var;
    this->minTimeForCA_var = other.minTimeForCA_var;
    this->maxTimeForCA_var = other.maxTimeForCA_var;
    this->selfMACAdress_var = other.selfMACAdress_var;
}

void ODARControlMessage::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->ODARControlMessageKind_var);
    doPacking(b,this->senderAddress_var);
    doPacking(b,this->databytelength_var);
    doPacking(b,this->requiredTimeForACK_var);
    doPacking(b,this->requiredTimeForDATA_var);
    doPacking(b,this->minTimeForCA_var);
    doPacking(b,this->maxTimeForCA_var);
    doPacking(b,this->selfMACAdress_var);
}

void ODARControlMessage::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->ODARControlMessageKind_var);
    doUnpacking(b,this->senderAddress_var);
    doUnpacking(b,this->databytelength_var);
    doUnpacking(b,this->requiredTimeForACK_var);
    doUnpacking(b,this->requiredTimeForDATA_var);
    doUnpacking(b,this->minTimeForCA_var);
    doUnpacking(b,this->maxTimeForCA_var);
    doUnpacking(b,this->selfMACAdress_var);
}

int ODARControlMessage::getODARControlMessageKind() const
{
    return ODARControlMessageKind_var;
}

void ODARControlMessage::setODARControlMessageKind(int ODARControlMessageKind)
{
    this->ODARControlMessageKind_var = ODARControlMessageKind;
}

int ODARControlMessage::getSenderAddress() const
{
    return senderAddress_var;
}

void ODARControlMessage::setSenderAddress(int senderAddress)
{
    this->senderAddress_var = senderAddress;
}

int ODARControlMessage::getDatabytelength() const
{
    return databytelength_var;
}

void ODARControlMessage::setDatabytelength(int databytelength)
{
    this->databytelength_var = databytelength;
}

simtime_t ODARControlMessage::getRequiredTimeForACK() const
{
    return requiredTimeForACK_var;
}

void ODARControlMessage::setRequiredTimeForACK(simtime_t requiredTimeForACK)
{
    this->requiredTimeForACK_var = requiredTimeForACK;
}

simtime_t ODARControlMessage::getRequiredTimeForDATA() const
{
    return requiredTimeForDATA_var;
}

void ODARControlMessage::setRequiredTimeForDATA(simtime_t requiredTimeForDATA)
{
    this->requiredTimeForDATA_var = requiredTimeForDATA;
}

simtime_t ODARControlMessage::getMinTimeForCA() const
{
    return minTimeForCA_var;
}

void ODARControlMessage::setMinTimeForCA(simtime_t minTimeForCA)
{
    this->minTimeForCA_var = minTimeForCA;
}

simtime_t ODARControlMessage::getMaxTimeForCA() const
{
    return maxTimeForCA_var;
}

void ODARControlMessage::setMaxTimeForCA(simtime_t maxTimeForCA)
{
    this->maxTimeForCA_var = maxTimeForCA;
}

int ODARControlMessage::getSelfMACAdress() const
{
    return selfMACAdress_var;
}

void ODARControlMessage::setSelfMACAdress(int selfMACAdress)
{
    this->selfMACAdress_var = selfMACAdress;
}

class ODARControlMessageDescriptor : public cClassDescriptor
{
  public:
    ODARControlMessageDescriptor();
    virtual ~ODARControlMessageDescriptor();

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

Register_ClassDescriptor(ODARControlMessageDescriptor);

ODARControlMessageDescriptor::ODARControlMessageDescriptor() : cClassDescriptor("ODARControlMessage", "cMessage")
{
}

ODARControlMessageDescriptor::~ODARControlMessageDescriptor()
{
}

bool ODARControlMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ODARControlMessage *>(obj)!=NULL;
}

const char *ODARControlMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ODARControlMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int ODARControlMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *ODARControlMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ODARControlMessageKind",
        "senderAddress",
        "databytelength",
        "requiredTimeForACK",
        "requiredTimeForDATA",
        "minTimeForCA",
        "maxTimeForCA",
        "selfMACAdress",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int ODARControlMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='O' && strcmp(fieldName, "ODARControlMessageKind")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "databytelength")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "requiredTimeForACK")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "requiredTimeForDATA")==0) return base+4;
    if (fieldName[0]=='m' && strcmp(fieldName, "minTimeForCA")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "maxTimeForCA")==0) return base+6;
    if (fieldName[0]=='s' && strcmp(fieldName, "selfMACAdress")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ODARControlMessageDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *ODARControlMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "ODARControlDef";
            return NULL;
        default: return NULL;
    }
}

int ODARControlMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ODARControlMessage *pp = (ODARControlMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ODARControlMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ODARControlMessage *pp = (ODARControlMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getODARControlMessageKind());
        case 1: return long2string(pp->getSenderAddress());
        case 2: return long2string(pp->getDatabytelength());
        case 3: return double2string(pp->getRequiredTimeForACK());
        case 4: return double2string(pp->getRequiredTimeForDATA());
        case 5: return double2string(pp->getMinTimeForCA());
        case 6: return double2string(pp->getMaxTimeForCA());
        case 7: return long2string(pp->getSelfMACAdress());
        default: return "";
    }
}

bool ODARControlMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ODARControlMessage *pp = (ODARControlMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setODARControlMessageKind(string2long(value)); return true;
        case 1: pp->setSenderAddress(string2long(value)); return true;
        case 2: pp->setDatabytelength(string2long(value)); return true;
        case 3: pp->setRequiredTimeForACK(string2double(value)); return true;
        case 4: pp->setRequiredTimeForDATA(string2double(value)); return true;
        case 5: pp->setMinTimeForCA(string2double(value)); return true;
        case 6: pp->setMaxTimeForCA(string2double(value)); return true;
        case 7: pp->setSelfMACAdress(string2long(value)); return true;
        default: return false;
    }
}

const char *ODARControlMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *ODARControlMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ODARControlMessage *pp = (ODARControlMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


