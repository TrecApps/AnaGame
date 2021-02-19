#pragma once
#include <TObject.h>
#include <TString.h>

#include "TVariable.h"
#include "TcDataStruct.h"
#include "TVariableType.h"

class TInterpretor;

#define ACCESS_PUBLIC      0b00000000  // Access is allowed from any-where
#define ACCESS_PROTECTED   0b00000001  // Access is allowed only from within class and subclass 
#define ACCESS_PRIVATE     0b00000010  // Access is allowed only from within class
#define ACCESS_PUBLIC_MOD  0b00000100  // Access is allowed from within the "module"
#define ACCESS_SIBLING     0b00001000  // Objects can access other objects restricted attributes
#define ACCESS_RO_SUB      0b00010000  // If Private, allows Read-Access to subclasses
#define ACCESS_RO_SIB      0b00100000  // If object sibling acess is not enabled, allow siblings read-access
#define ACCESS_RO_MOD      0b01000000  // Allows read-access from code anywhere within the "module"
#define ACCESS_RO_PUB      0b10000000  // Allows Read Access from anywhere

#define ATTRIBUTE_STATIC   0b00000001  // The Attribute is considered Static
#define ATTRIBUTE_CONST    0b00000010  // Attribute is constant, needs to be set either in a constructor or on launch
#define ATTRIBUTE_ARRAY    0b00000100  // Attribute is an array

// ADVANCED: For Lower level langauges like C++ and Rust
#define ATTRIBUTE_VIRTUAL  0b00010000  // For methods, considered virtual, attributes, considered pointers
#define ATTRIBUTE_OVERRIDE 0b00100000  // For Methods, needs to Override a base method
#define ATTRIBUTE_ABSTRACT 0b01000000  // For Methods, must be abstract

class TC_DATA_STRUCT TClassAttribute
{
public:
    TClassAttribute(const TString& n, const TString& t);
    TClassAttribute();
    TClassAttribute(const TClassAttribute& copy);
    TString name, type;
    byte access;

    byte other;
    TrecPointer<TVariable> def;
};

typedef enum class tc_class_type
{
    tct_class,
    tct_interface,
    tct_union,
    tct_struct,
    tct_any
}tc_class_type;


class TC_DATA_STRUCT TClassStruct :
    public TVariableType
{
public:
    TClassStruct();
    bool AddAttribute(const TClassAttribute& att);
    TClassAttribute GetAttributeByName(const TString& name);

    bool GetAttributeByIndex(UINT index, TClassAttribute& att);

    void SetCaseInsensitive();

    virtual var_category GetVarGategory() override;

    void SetClassType(tc_class_type classType);
    tc_class_type GetClassType();

    UINT AddParentClass(const TString& className, tc_class_type cType = tc_class_type::tct_class);
    bool GetParentClass(UINT index, TString& className);

protected:
    TDataArray<TString> parentClasses;

    tc_class_type classType;
    TDataArray<TClassAttribute> attributes;
    TDataArray<TrecSubPointer<TVariable, TInterpretor>> constructors;
    bool caseSensitive;
};

