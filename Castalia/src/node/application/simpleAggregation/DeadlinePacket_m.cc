//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/simpleAggregation/DeadlinePacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "DeadlinePacket_m.h"

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

Register_Class(DeadlinePacket);

DeadlinePacket::DeadlinePacket(const char *name, int kind) : ::ApplicationPacket(name,kind)
{
    this->deadline_var = 0;
}

DeadlinePacket::DeadlinePacket(const DeadlinePacket& other) : ::ApplicationPacket(other)
{
    copy(other);
}

DeadlinePacket::~DeadlinePacket()
{
}

DeadlinePacket& DeadlinePacket::operator=(const DeadlinePacket& other)
{
    if (this==&other) return *this;
    ::ApplicationPacket::operator=(other);
    copy(other);
    return *this;
}

void DeadlinePacket::copy(const DeadlinePacket& other)
{
    this->deadline_var = other.deadline_var;
}

void DeadlinePacket::parsimPack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimPack(b);
    doPacking(b,this->deadline_var);
}

void DeadlinePacket::parsimUnpack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimUnpack(b);
    doUnpacking(b,this->deadline_var);
}

double DeadlinePacket::getDeadline() const
{
    return deadline_var;
}

void DeadlinePacket::setDeadline(double deadline)
{
    this->deadline_var = deadline;
}

class DeadlinePacketDescriptor : public cClassDescriptor
{
  public:
    DeadlinePacketDescriptor();
    virtual ~DeadlinePacketDescriptor();

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

Register_ClassDescriptor(DeadlinePacketDescriptor);

DeadlinePacketDescriptor::DeadlinePacketDescriptor() : cClassDescriptor("DeadlinePacket", "ApplicationPacket")
{
}

DeadlinePacketDescriptor::~DeadlinePacketDescriptor()
{
}

bool DeadlinePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DeadlinePacket *>(obj)!=NULL;
}

const char *DeadlinePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DeadlinePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int DeadlinePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *DeadlinePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "deadline",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int DeadlinePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "deadline")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DeadlinePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *DeadlinePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int DeadlinePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DeadlinePacket *pp = (DeadlinePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DeadlinePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DeadlinePacket *pp = (DeadlinePacket *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getDeadline());
        default: return "";
    }
}

bool DeadlinePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DeadlinePacket *pp = (DeadlinePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDeadline(string2double(value)); return true;
        default: return false;
    }
}

const char *DeadlinePacketDescriptor::getFieldStructName(void *object, int field) const
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

void *DeadlinePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DeadlinePacket *pp = (DeadlinePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


