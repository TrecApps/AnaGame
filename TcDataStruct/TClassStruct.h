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
     * Purpose: Constructor
     * Parameters: const TString& n - name of attribute
     *              const TString& t - type of attribute
     * Returns: new Attribute object
     */
    TClassAttribute(const TString& n, const TString& t);
    /**
     * Method: TClassAttribute::TClassAttribute
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Blank Atribute object
     */
    TClassAttribute();
    /**
     * Method: TClassAttribute::TClassAttribute
     * Purpose: Copy Constructor
     * Parameters: const TClassAttribute& copy - original object to copy from
     * Returns: new Atribute object based off of existing one
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
 * Purpose: Represents an entire Class
 * 
 * SuperClass: TVariableType - represents a variable type
 */
class TC_DATA_STRUCT TClassStruct :
    public TVariableType
{
public:
    /**
     * Method: TClassStruct::TClassStruct
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Class blueprint
     */
    TClassStruct();
    /**
     * Method: TClassStruct::AddAttribute
     * Purpose: Adds an attribute to the class
     * Parameters: const TClassAttribute& att - adds a new attribute to the 
     *              bool doOverride = false - whether to replace an attribute if the name is already provided
     * Returns: bool - whether it was added or not
     */
    bool AddAttribute(const TClassAttribute& att, bool doOverride = false);
    /**
     * Method: TClassStruct::GetAttributeByName
     * Purpose: Returns the attribute by the name
     * Parameters: const TString& name
     * Returns: TClassAttribute - the attribute collected (check the name of this object to assess validity)
     */
    TClassAttribute GetAttributeByName(const TString& name);

    /**
     * Method: TClassStruct::GetAttributeByIndex
     * Purpose: Gets an attribute by index of stored attributes
     * Parameters: UINT index - the location in the attributes repo 
     *              TClassAttribute& att - the attribute to collect
     * Returns: bool - whether there was an attribute (if true, att should be valid)
     */
    bool GetAttributeByIndex(UINT index, TClassAttribute& att);

    /**
     * Method: TClassStruct::SetCaseInsensitive
     * Purpose: Makes this class refer to attributes without regard to case
     * Parameters: void
     * Returns: void
     */
    void SetCaseInsensitive();

    /**
     * Method: TClassStruct::GetVarGategory
     * Purpose: Reports to code with a TVariableType reference what class this actually is
     * Parameters: void
     * Returns: var_category - the type category (class version)
     */
    virtual var_category GetVarGategory() override;

    /**
     * Method: TClassStruct::SetClassType
     * Purpose: Sets the class type
     * Parameters: tc_class_type classType - the way to treat this class
     * Returns: void
     */
    void SetClassType(tc_class_type classType);
    /**
     * Method: TClassStruct::GetClassType
     * Purpose: Reports the way to treat this class
     * Parameters: void
     * Returns: tc_class_type - the way to treat this class
     */
    tc_class_type GetClassType();

    /**
     * Method: TClassStruct::AddParentClass
     * Purpose: Adds the Parent Class
     * Parameters: const TString& className - the name of the parent class
     *              tc_class_type cType = tc_class_type::tct_class - not used
     * Returns: UINT - 0 if added, 1 if already present
     */
    UINT AddParentClass(const TString& className, tc_class_type cType = tc_class_type::tct_class);

    /**
     * Method: TClassStruct::GetParentClass
     * Purpose: Reports the Parent class stored at the present 
     * Parameters: UINT index - the index of the parent class
     *              TString& className - holder of the class
     * Returns: bool - true if className holds the name of the parent class, false if index is out of bounds
     */
    bool GetParentClass(UINT index, TString& className);

protected:
    /**
     * Holds list of parent classes
     */
    TDataArray<TString> parentClasses;
    /**
     * The nature of the class (regular class, struct, union, interface, etc.)
     */
    tc_class_type classType;
    /**
     * List of attributes
     */
    TDataArray<TClassAttribute> attributes;
    /**
     * Constructors for this interpretor
     */
    TDataArray<TrecSubPointer<TVariable, TInterpretor>> constructors;
    /**
     * Whether names are case senstive
     */
    bool caseSensitive;
};

