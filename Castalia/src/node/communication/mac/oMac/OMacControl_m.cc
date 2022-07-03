//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/oMac/OMacControl.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "OMacControl_m.h"

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
    cEnum *e = cEnum::find("OMacControl_type");
    if (!e) enums.getInstance()->add(e = new cEnum("OMacControl_type"));
    e->insert(OMAC_RECEIVERS_LIST_REPLY, "OMAC_RECEIVERS_LIST_REPLY");
);

Register_Class(OMacControl);

OMacControl::OMacControl(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->OMacControlKind_var = 0;
}

OMacControl::OMacControl(const OMacControl& other) : ::MacPacket(other)
{
    copy(other);
}

OMacControl::~OMacControl()
{
}

OMacControl& OMacControl::operator=(const OMacControl& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void OMacControl::copy(const OMacControl& other)
{
    this->OMacControlKind_var = other.OMacControlKind_var;
    this->receiversContainer_var = other.receiversContainer_var;
}

void OMacControl::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->OMacControlKind_var);
    doPacking(b,this->receiversContainer_var);
}

void OMacControl::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->OMacControlKind_var);
    doUnpacking(b,this->receiversContainer_var);
}

int OMacControl::getOMacControlKind() const
{
    return OMacControlKind_var;
}

void OMacControl::setOMacControlKind(int OMacControlKind)
{
    this->OMacControlKind_var = OMacControlKind;
}

ReceiversContainer& OMacControl::getReceiversContainer()
{
    return receiversContainer_var;
}

void OMacControl::setReceiversContainer(const ReceiversContainer& receiversContainer)
{
    this->receiversContainer_var = receiversContainer;
}

class OMacControlDescriptor : public cClassDescriptor
{
  public:
    OMacControlDescriptor();
    virtual ~OMacControlDescriptor();

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

Register_ClassDescriptor(OMacControlDescriptor);

OMacControlDescriptor::OMacControlDescriptor() : cClassDescriptor("OMacControl", "MacPacket")
{
}

OMacControlDescriptor::~OMacControlDescriptor()
{
}

bool OMacControlDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<OMacControl *>(obj)!=NULL;
}

const char *OMacControlDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int OMacControlDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int OMacControlDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *OMacControlDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "OMacControlKind",
        "receiversContainer",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int OMacControlDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='O' && strcmp(fieldName, "OMacControlKind")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "receiversContainer")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *OMacControlDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *OMacControlDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "OMacControl_type";
            return NULL;
        default: return NULL;
    }
}

int OMacControlDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    OMacControl *pp = (OMacControl *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string OMacControlDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    OMacControl *pp = (OMacControl *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOMacControlKind());
        case 1: {std::stringstream out; out << pp->getReceiversContainer(); return out.str();}
        default: return "";
    }
}

bool OMacControlDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    OMacControl *pp = (OMacControl *)object; (void)pp;
    switch (field) {
        case 0: pp->setOMacControlKind(string2long(value)); return true;
        default: return false;
    }
}

const char *OMacControlDescriptor::getFieldStructName(void *object, int field) const
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

void *OMacControlDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    OMacControl *pp = (OMacControl *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getReceiversContainer()); break;
        default: return NULL;
    }
}

