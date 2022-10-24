//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/PLR/PLRControl.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PLRControl_m.h"

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
    cEnum *e = cEnum::find("PLRControlDef");
    if (!e) enums.getInstance()->add(e = new cEnum("PLRControlDef"));
    e->insert(DELAY, "DELAY");
    e->insert(FAIL, "FAIL");
    e->insert(PDR, "PDR");
);

Register_Class(PLRControlMessage);

PLRControlMessage::PLRControlMessage(const char *name, int kind) : ::cMessage(name,kind)
{
    this->PLRControlMessageKind_var = 0;
    this->delay_var = 0;
    this->pdr_var = 0;
    this->txAddr_var = 0;
}

PLRControlMessage::PLRControlMessage(const PLRControlMessage& other) : ::cMessage(other)
{
    copy(other);
}

PLRControlMessage::~PLRControlMessage()
{
}

PLRControlMessage& PLRControlMessage::operator=(const PLRControlMessage& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PLRControlMessage::copy(const PLRControlMessage& other)
{
    this->PLRControlMessageKind_var = other.PLRControlMessageKind_var;
    this->delay_var = other.delay_var;
    this->pdr_var = other.pdr_var;
    this->txAddr_var = other.txAddr_var;
}

void PLRControlMessage::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->PLRControlMessageKind_var);
    doPacking(b,this->delay_var);
    doPacking(b,this->pdr_var);
    doPacking(b,this->txAddr_var);
}

void PLRControlMessage::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->PLRControlMessageKind_var);
    doUnpacking(b,this->delay_var);
    doUnpacking(b,this->pdr_var);
    doUnpacking(b,this->txAddr_var);
}

int PLRControlMessage::getPLRControlMessageKind() const
{
    return PLRControlMessageKind_var;
}

void PLRControlMessage::setPLRControlMessageKind(int PLRControlMessageKind)
{
    this->PLRControlMessageKind_var = PLRControlMessageKind;
}

double PLRControlMessage::getDelay() const
{
    return delay_var;
}

void PLRControlMessage::setDelay(double delay)
{
    this->delay_var = delay;
}

double PLRControlMessage::getPdr() const
{
    return pdr_var;
}

void PLRControlMessage::setPdr(double pdr)
{
    this->pdr_var = pdr;
}

int PLRControlMessage::getTxAddr() const
{
    return txAddr_var;
}

void PLRControlMessage::setTxAddr(int txAddr)
{
    this->txAddr_var = txAddr;
}

class PLRControlMessageDescriptor : public cClassDescriptor
{
  public:
    PLRControlMessageDescriptor();
    virtual ~PLRControlMessageDescriptor();

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

Register_ClassDescriptor(PLRControlMessageDescriptor);

PLRControlMessageDescriptor::PLRControlMessageDescriptor() : cClassDescriptor("PLRControlMessage", "cMessage")
{
}

PLRControlMessageDescriptor::~PLRControlMessageDescriptor()
{
}

bool PLRControlMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PLRControlMessage *>(obj)!=NULL;
}

const char *PLRControlMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PLRControlMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int PLRControlMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *PLRControlMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "PLRControlMessageKind",
        "delay",
        "pdr",
        "txAddr",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int PLRControlMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='P' && strcmp(fieldName, "PLRControlMessageKind")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "delay")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pdr")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "txAddr")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PLRControlMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *PLRControlMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "PLRControlDef";
            return NULL;
        default: return NULL;
    }
}

int PLRControlMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PLRControlMessage *pp = (PLRControlMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PLRControlMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PLRControlMessage *pp = (PLRControlMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPLRControlMessageKind());
        case 1: return double2string(pp->getDelay());
        case 2: return double2string(pp->getPdr());
        case 3: return long2string(pp->getTxAddr());
        default: return "";
    }
}

bool PLRControlMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PLRControlMessage *pp = (PLRControlMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setPLRControlMessageKind(string2long(value)); return true;
        case 1: pp->setDelay(string2double(value)); return true;
        case 2: pp->setPdr(string2double(value)); return true;
        case 3: pp->setTxAddr(string2long(value)); return true;
        default: return false;
    }
}

const char *PLRControlMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *PLRControlMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PLRControlMessage *pp = (PLRControlMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


