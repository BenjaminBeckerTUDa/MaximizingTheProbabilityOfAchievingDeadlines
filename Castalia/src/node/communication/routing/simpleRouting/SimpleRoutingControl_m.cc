//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/simpleRouting/SimpleRoutingControl.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SimpleRoutingControl_m.h"

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
    cEnum *e = cEnum::find("SimpleRoutingControl_type");
    if (!e) enums.getInstance()->add(e = new cEnum("SimpleRoutingControl_type"));
    e->insert(SIMPLE_ROUTING_RECEIVERS_REQUEST, "SIMPLE_ROUTING_RECEIVERS_REQUEST");
    e->insert(SIMPLE_ROUTING_RECEIVERS_REPLY, "SIMPLE_ROUTING_RECEIVERS_REPLY");
);

Register_Class(SimpleRoutingControl);

SimpleRoutingControl::SimpleRoutingControl(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->SimpleRoutingControlKind_var = 0;
}

SimpleRoutingControl::SimpleRoutingControl(const SimpleRoutingControl& other) : ::RoutingPacket(other)
{
    copy(other);
}

SimpleRoutingControl::~SimpleRoutingControl()
{
}

SimpleRoutingControl& SimpleRoutingControl::operator=(const SimpleRoutingControl& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void SimpleRoutingControl::copy(const SimpleRoutingControl& other)
{
    this->SimpleRoutingControlKind_var = other.SimpleRoutingControlKind_var;
}

void SimpleRoutingControl::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->SimpleRoutingControlKind_var);
}

void SimpleRoutingControl::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->SimpleRoutingControlKind_var);
}

int SimpleRoutingControl::getSimpleRoutingControlKind() const
{
    return SimpleRoutingControlKind_var;
}

void SimpleRoutingControl::setSimpleRoutingControlKind(int SimpleRoutingControlKind)
{
    this->SimpleRoutingControlKind_var = SimpleRoutingControlKind;
}

class SimpleRoutingControlDescriptor : public cClassDescriptor
{
  public:
    SimpleRoutingControlDescriptor();
    virtual ~SimpleRoutingControlDescriptor();

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

Register_ClassDescriptor(SimpleRoutingControlDescriptor);

SimpleRoutingControlDescriptor::SimpleRoutingControlDescriptor() : cClassDescriptor("SimpleRoutingControl", "RoutingPacket")
{
}

SimpleRoutingControlDescriptor::~SimpleRoutingControlDescriptor()
{
}

bool SimpleRoutingControlDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SimpleRoutingControl *>(obj)!=NULL;
}

const char *SimpleRoutingControlDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SimpleRoutingControlDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SimpleRoutingControlDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *SimpleRoutingControlDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "SimpleRoutingControlKind",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SimpleRoutingControlDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='S' && strcmp(fieldName, "SimpleRoutingControlKind")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SimpleRoutingControlDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *SimpleRoutingControlDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "SimpleRoutingControl_type";
            return NULL;
        default: return NULL;
    }
}

int SimpleRoutingControlDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingControl *pp = (SimpleRoutingControl *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SimpleRoutingControlDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingControl *pp = (SimpleRoutingControl *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSimpleRoutingControlKind());
        default: return "";
    }
}

bool SimpleRoutingControlDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingControl *pp = (SimpleRoutingControl *)object; (void)pp;
    switch (field) {
        case 0: pp->setSimpleRoutingControlKind(string2long(value)); return true;
        default: return false;
    }
}

const char *SimpleRoutingControlDescriptor::getFieldStructName(void *object, int field) const
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

void *SimpleRoutingControlDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleRoutingControl *pp = (SimpleRoutingControl *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


