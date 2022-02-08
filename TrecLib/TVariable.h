#pragma once
#include "TrecReference.h"
#include "TString.h"

/**
 * Enum Class: var_type
 * Purpose: Tracks the type of variable being held, whether primitive, collection, native, etc.
 */
typedef enum class var_type
{
    expression,          // Can be Operated upon to generate a new variable
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
     * Method: TVariable::GetIterator
     * Purpose: for type that support it, retrieves an Iterator Variable of this Variable
     * Parameters: void
     * Return: TrecPointer<TVariable> - for some variable types, this would be null, but other variable types woudld support iteration
     * 
     * Attributes: virtual
     */
    virtual TrecPointer<TVariable> GetIterator();


    /**
     * Method: TVariable::GetString
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
     * Method: TVariable::GetString
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: const TString& detail - 
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     *
     * Attributes: abstract
     */
    virtual TString GetString(const TString& detail);

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
    virtual void SetSelf(TrecPointer<TVariable> vSelf);

protected:
    /**
     * Self Reference
     */
    TrecPointerSoft<TVariable> vSelf;
};


/**
 * Enum Class: return_mode
 * Purpose: How Returning works
 */
typedef enum class return_mode
{
    rm_regular,     // Nothing special
    rm_break,       // Break was detected
    rm_continue,    // Continue was detected
    rm_return,       // Return was detected
    rm_super_return, // Return was detected, and the caller should also return
    rm_yield        // Similar to Return, but function is expected to resume from the yield point
}return_mode;

/**
 * Class: ReturnObject
 * Purpose: Represents information returned from an operation
 */
class _TREC_LIB_DLL ReturnObject
{
public:
    TString errorMessage;               // String Information
    TrecPointer<TVariable> errorObject; // A Variable Generated
    TrecPointer<TVariable> errorObject2;// Backup Variable
    USHORT returnCode;                  // 0 if working, error otherwise
    TDataArray<TString> stackTrace;     // stack trace info
    return_mode mode;                   // Mode, the context in which an operation ended
    USHORT nextCount;                   // How many Statements were part of a single Expression (statement and next statement)


    /**
     * Mehtod: ReturnObject::ReturnObject
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: New Return Object
     */
    ReturnObject();

    /**
     * Mehtod: ReturnObject::ReturnObject
     * Purpose: Copy Constructor
     * Parameters: const ReutnrObject& copy - the object to copy
     * Returns: new Return Object
     */
    ReturnObject(const ReturnObject& copy);

    void operator=(const ReturnObject& copy);

    const static USHORT ERR_NO_ERROR = 0;
    const static USHORT ERR_BRACKET_MISMATCH = 1;
    const static USHORT ERR_PARENTH_MISMATCH = 2;
    const static USHORT ERR_NOT_NUMBER = 3;
    const static USHORT ERR_TOO_FEW_PARMS = 4;
    const static USHORT ERR_EXISTING_VAR = 5;
    const static USHORT ERR_UNDEFINED = 6;
    const static USHORT ERR_BROKEN_REF = 7;
    const static USHORT ERR_INCOMPLETE_STATEMENT = 8;
    const static USHORT ERR_IMPROPER_TYPE = 9;
    const static USHORT ERR_IMPROPER_NAME = 10;
    const static USHORT ERR_UNEXPECTED_TOK = 11;
    const static USHORT ERR_UNSUPPORTED_OP = 12;
    const static USHORT ERR_INVALID_FILE_PARAM = 13;

    const static USHORT ERR_THROWN = 19;
    const static USHORT ERR_GENERIC_ERROR = 20;
    const static USHORT ERR_INTERNAL = 21;          // Indicates an Issue that the Interpretor has detected witin itself

};

/**
 * Class: TVariableIterator
 * Purpose: Iterates through the variable (if the variable supports it
 */
class _TREC_LIB_DLL TVariableIterator : public TVariable
{
public:
    /**
     * Method: TVariableIterator::SetReverse
     * Purpose: enables iterators to go through backwards
     * Parameters: bool doReverse - if true, the iterator will now go in reverse (if supported)
     *              bool reset - if true, will point to the beginning or end of the target variable
     * Returns: bool - whether the operation was supported
     *
     * Attributes: abstract
     */
    virtual bool SetReverse(bool doReverse, bool reset) = 0;


    /**
     * Method: TVariable::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;

    /**
     * Method: TVariableIterator::Traverse
     * Purpose: Goues through the variable
     * Parameters: UINT& currentIndex - the index of the retrieved variable
     *              TString& currentName - the name of the retrieved variable
     *              TrecPointer<TVariable>& value - the retireved variable
     * Returns: bool - whether the variable was retrieved
     * 
     * Attributes: abstract
     */
    virtual bool Traverse(UINT& currentIndex, TString& currentName, TrecPointer<TVariable>& value) = 0;



    /**
     * Method: TVariableIterator::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetVType() override;
    /**
     * Method: TVariableIterator::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TString - The TString referered by the variable (empty if not a string)
     *
     * Note:  Call "GetVarType" first and make sure that it returns "var_type::string" first
     *
     * Attributes: override
     */
    virtual TString GetString() override;

    /**
     * Method: TVariableIterator::Get4Value
     * Purpose: Returns the value held by the variable assuming four bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: UINT - The value held as a UINT (0 if not a primitive type)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::primitive" first
     *
     * Attributes: override
     */
    virtual UINT Get4Value() override;



    /**
     * Method: TVariableIterator::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;

    /**
     * Method: TVariableIterator::GetErrorInfo
     * Purpose: Allows interpretors using an iterator to see if an 'error' has occured, relevent for functions
     * Parameters: void
     * Returns: ReturnObject - error information (likely blank, but if a function was called, error information might be contained)
     * 
     * Attributes: virtual
     */
    virtual ReturnObject GetErrorInfo();

};


/**
 * Class: TVObject
 * Purpose: Adds TVariable integration to the TObject
 */
class _TREC_LIB_DLL TVObject : public TObject
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