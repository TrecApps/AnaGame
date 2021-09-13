#pragma once
#include "TrecReference.h"
#include "TString.h"

/**
 * Enum Class: var_type
 * Purpose: Tracks the type of variable being held, whether primitive, collection, native, etc.
 */
typedef enum class var_type
{
    primitive,           // Primitive variable is being held
    primitive_formatted, // Primitive variable with formatting data
    collection,          // A collection type variable is being held, whether an array, JavaScript style object, tuple, dictionary, etc.
    native_object,       // Variable references a C++ level native Anagame object
    string,              // References a String
    interpretor,         // References an Interpretor (i.e. procedures/functions that are stored as variables
    interpretor_gen,     // Generates new Interpretors
    accessor,            // Type of variable that holds a Getter and Setter method
    special_value,       // Holds a special value that doesn't fit in any of the other categories
    iterator,            // Serves as an iterator
    async                // Serves as a brdge between threads
}var_type;




/**
 * Class: TVariable
 * Purpose: Represents a given variable in the system, abstract to support both Objects and raw data types
 */
class _TREC_LIB_DLL TVariable: public TObject
{
public:

    virtual TrecPointer<TVariable> Clone();

    /**
     * Method: TVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: abstract
     */
    virtual var_type GetVarType() = 0;


    /**
     * Method: TVariable::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     *
     * Attributes: abstract
     */
    virtual TString GetString() = 0;

    /**
     * Method: TVariable::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     *
     * Attributes: abstract
     */
    virtual UINT Get4Value() = 0;



    /**
     * Method: TVariable::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual ULONG64 Get8Value() = 0;


    /**
     * Method: TVariable::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: abstract
     */
    virtual UINT GetSize() = 0;


    /**
     * Method: TVariable::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: abstract
     */
    virtual UINT GetVType() = 0;

    /**
     * Method: TVariable::SetSelf
     * Purpose: Allows the Variable to have a reference to itself
     * Parameters: TrecPointer<TVariable> - reference to assign
     * Returns: void
     */
    virtual void SetSelf(TrecPointer<TVariable>);
};


/**
 * Class: TVObject
 * Purpose: Adds TVariable integration to the TObject
 */
class TVObject : public TObject
{
public:

    /**
     * Method: TVObject::HasVariableSupport
     * Purpose: Reports whether this TObject has TVariable support (since TVariables expect TObjects to exist and thus Raw TObjects cannot support it)
     * Parameters: void
     * Returns: bool - false for base object, true for any object that extends TVObject
     */
    virtual bool HasVariableSupport();

    /**
     * Method: TVObject::GetVariable
     * Purpose: Revtrieves a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
     * Returns: bool - whether the variable was valid or not
     * 
     * Attributes: abstract
     */
    virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) = 0;

    /**
     * Method: TVObject::SetVariable
     * Purpose: Sets a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable> var - the value to set
     * Returns: bool - whether the variable was valid or not
     *
     * Attributes: abstract
     */
    virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var) = 0;
};



namespace VarFunction
{
    /**
     * Function: VarFunction::IsTrue
     * Purpose: Reports to the interpretor whether the provided value is "true"
     * Parameters: TrecPointer<TVariable> var - the variable to analyze
     *              bool& result - whether the variable amounts to "true" or not (reliable if the return value is true)
     *              UCHAR def = 0 - token indicating what types to refer to as true or not (0 means only booleans are checked)
     * Returns: bool - whether or not the variable could be analyzed in a boolean fashion based off of the token provided
     *
     * Note: if this function returns false, the interpretor should consider the line to be an error and handle it accordingly.
     */
    _TREC_LIB_DLL bool IsTrue(TrecPointer<TVariable> var, bool& result, UCHAR def = 0);

}

/**
 * Class: TVariableMarker
 * Purpose: holdes a TVariable reference as well as whether the variable is marked as mutable or not
 */
class _TREC_LIB_DLL TVariableMarker
{
private:
    /*
     * whether the variable is mutable or not
     */
    bool mutableVar;

    /**
     * The variable itself
     */
    TrecPointer<TVariable> var;

public:

    /**
     * Method: TVariableMarker::TVariableMarker
     * Purpose: Constructor
     * Parameters: bool makeMutable - whether this variable should be able to be changed later
     *              TrecPointer<TVariable> var - the new variable
     * Returns: New Variable marker
     */
    TVariableMarker(bool makeMutable, TrecPointer<TVariable> var);


    /**
     * Method: TVariableMarker::TVariableMarker
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Variable marker
     */
    TVariableMarker();


    /**
     * Method: TVariableMarker::TVariableMarker
     * Purpose: Copy Constructor
     * Parameters: const TVariableMarker& orig -  the marker to copy from
     * Returns: New Variable marker
     */
    TVariableMarker(const TVariableMarker& orig);


    /**
     * Method: TVariableMarker::operator=
     * Purpose: Overloads assignment operator for the class
     * Parameters: const TVariableMarker& orig -  the marker to copy from
     * Returns: void
     */
    void operator=(const TVariableMarker& orig);


    /**
     * Method: TVariableMarker::SetVariable
     * Purpose: Sets the Variable if allowed
     * Parameters: TrecPointer<TVariable> var - the new variable to assign to
     * Returns: bool - whether the new variable was set
     */
    bool SetVariable(TrecPointer<TVariable> var);

    /**
     * Method: TVariableMarker::GetVariable
     * Purpose: Reports the Variable being held
     * Parameters: void
     * Returns: TrecPointer<TVariable> var - the variable being held
     */
    TrecPointer<TVariable> GetVariable() const;

    /**
     * Method: TVariableMarker::IsMutable
     * Purpose: Reports whether the variable is mutable (can be changed) or not
     * Parameters: void
     * Returns: bool - mutability status
     */
    bool IsMutable() const;
};