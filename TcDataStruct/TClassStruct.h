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
#define ATTRIBUTE_MUTABLE  0b00001000  // Attribute is mutable, even in a const object
#define ATTRIBUTE_GETTER   0b00010000  // Attribute is a getter
#define ATTRIBUTE_SETTER   0b00100000  // Attribute is a setter
#define ATTRIBUTE_ASYNC    0b01000000  // Attribute is async

// ADVANCED: For Lower level langauges like C++ and Rust
#define ATTRIBUTE_VIRTUAL  0b00010000  // For methods, considered virtual, attributes, considered pointers
#define ATTRIBUTE_OVERRIDE 0b00100000  // For Methods, needs to Override a base method
#define ATTRIBUTE_ABSTRACT 0b01000000  // For Methods, must be abstract

/**
 * Class: TClassAttribute
 * Purpose: Provides details on a specific attributes of a class
 */
class TC_DATA_STRUCT TClassAttribute
{
public:
    /**
     * Method: TClassAttribute::
     * Purpose: 
     * Parameters: 
     * Returns: 
     */
    TClassAttribute(const TString& n, const TString& t);
    /**
     * Method: TClassAttribute::
     * Purpose:
     * Parameters:
     * Returns:
     */
    TClassAttribute();
    /**
     * Method: TClassAttribute::
     * Purpose:
     * Parameters:
     * Returns:
     */
    TClassAttribute(const TClassAttribute& copy);
    /**
     * Holds the name and type of the attribute
     */
    TString name, type;
    /**
     * Indicates the access allowed for this attribute
     */
    byte access;
    /**
     * Indicates other attributes applicable to this attribute, such as static, array, virtual, etc.
     */
    byte other;
    /**
     * Holds a default value for this attribute (if applicable and available
     */
    TrecPointer<TVariable> def;
};

/**
 * Enum Class: tc_class_type
 * Purpose: provides a hint to interpetors (during compilation) on how to treat this class
 */
typedef enum class tc_class_type
{
    tct_class,
    tct_interface,
    tct_union,
    tct_struct,
    tct_any
}tc_class_type;

/**
 * Class: TClassStruct
 * Purpose: 
 */
class TC_DATA_STRUCT TClassStruct :
    public TVariableType
{
public:
    TClassStruct();
    bool AddAttribute(const TClassAttribute& att, bool doOverride = false);
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

