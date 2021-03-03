#pragma once
#include "TVariable.h"
#include "TDirectory.h"
#include "TEnvironment.h"
#include "TDataMap.h"

#ifndef TC_INTERPRETOR_COMPILE
#define TC_INTERPRETOR_COMPILE

typedef USHORT COMPILE_TYPE;

const COMPILE_TYPE NO_COMPILE =  0x0000;  // Compilation by Interpretor is not supported, Statements are 
const COMPILE_TYPE ALL_OBJECTS = 0x0001;  // All Variables are treated as Anagame Objects (use for type implicit languages like JS, Python)
const COMPILE_TYPE MIXED_TYPES = 0x0002;  // Variables known to be primitive will be treated as such and not automatically wrapped in an Object, Other objects used
const COMPILE_TYPE ALL_PRIM =    0x0004;  // All Variables are treated as primitive, the Language is expected to determine how objects are managed
const COMPILE_TYPE USE_REGISTR = 0x0008;  // Code that can support Registers. Markers will be set to int at assemblers which registers to use

#endif // !TC_INTERPRETOR_COMPILE


/**
 * Class: TcVariableHolder
 * Purpose: Holds Variables, including value, type and mutbility status
 */
class _TREC_LIB_DLL TcVariableHolder
{
public:
    bool mut;                       // Is this variable mutable
    TString type;                   // The type of Variable this should be (empty string means anything)
    TrecPointer<TVariable> value;   // The Value held by the Variable

    /**
     * Mehtod: TcVariableHolder::TcVariableHolder
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Variable Holder
     */
    TcVariableHolder();

    /**
     * Mehtod: TcVariableHolder::TcVariableHolder
     * Purpose: Copy Constructor
     * Parameters: const TcVariableHolder& copy - Variable Holder to copy
     * Returns: new Variable Holder
     */
    TcVariableHolder(const TcVariableHolder& copy);
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
    rm_return       // Return was detected
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
    USHORT returnCode;                  // 0 if working, error otherwise
    TDataArray<TString> stackTrace;     // stack trace info
    return_mode mode;                   // Mode, the context in which an operation ended


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


    const static USHORT ERR_GENERIC_ERROR = 20;

};

/**
 * Class: TcInterpretor
 * Purpose: Represents Code that can be run (and possibly compiled)
 * 
 * SuperClass: TVariable
 */
class _TREC_LIB_DLL TcInterpretor :
    public TVariable
{
public:

    /// TVariable Methods
    /**
     * Method: TcInterpretor::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TcInterpretor::GetObject
     * Purpose: Returns the Object held by the variable, or null if variable is a raw data type
     * Parameters: void
     * Returns: TrecObjectPointer - The Object referered by the variable (or null if not an object)
     *
     * Note: Call "GetVarType" first and make sure that it returns "var_type::native_object" first
     *
     * Attributes: override
     */
    virtual TrecObjectPointer GetObject() override;


    /**
     * Method: TcInterpretor::GetObject
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
     * Method: TcInterpretor::Get4Value
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
     * Method: TcInterpretor::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TcInterpretor::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TcInterpretor::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetType() override;

/// Interpretor Functionality


    /**
     * Method: TcInterpreotr::CanCompile
     * Purpose: Whether the Compiler is capable of compiling it's statements into a more compact form
     * Parameters: void
     * Returns: COMPILE_TYPE - marker indicating the Compilation the Interpretor can do
     * 
     * Attributes: abstract
     * 
     * Note: If the Returned Value is 0, then the Interpretor will not Attempt to Compile and is expected to run the Statements AS IS
     */
    virtual COMPILE_TYPE CanCompile() = 0;

    /**
     * Method: TcInterpretor::Compile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret) = 0;

    /**
     * Method: TcInterpretor::SetFile
     * Purpose: Sets the File Where Sourcecode is expected to be read
     * Parameters: TrecPointer<TFileShell> codeFile - file holding the intitial source code
     *              ReturnObject& ret - allows method to report any error information encountered
     *              bool isFirst - whether this is the first Interpretor called (true by default)
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst = true) = 0;

    /**
     * Method: TcInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() = 0;

    /**
     * Method: TcInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) = 0;

    /**
     * Method: TCInterpretor::SetParamNames
     * Purpose: Allows Parameter names to be set
     * Parameters: TDataArray<TString>& paramNames - the Names of Initial parameters
     * Returns: void
     */
    void SetParamNames(TDataArray<TString>& paramNames);


    /**
     * Method: TCInterpretor::SetParamTypes
     * Purpose: Allows Parameter types to be set
     * Parameters: TDataArray<TString>& paramTypes - the Types of Initial parameters
     * Returns: void
     */
    void SetParamTypes(TDataArray<TString>& paramTypes);

    /**
     * Method: TCInterpretor::SetDefaultValues
     * Purpose: Allows Parameter Defaults to be set
     * Parameters: TDataArray<TrecPointer<TVariable>>& paramTypes - the Types of Initial parameters
     * Returns: void
     */
    void SetParamDefaults(TDataArray<TrecPointer<TVariable>>& paramDefaults);

    /**
     * Method: TcInterpretor::SetActiveObject
     * Purpose: Sets the Active Object, enabling this Interpreotr to act as a method in an existing object
     * Parameters: TrecPointer<TVariable> var - the object to act on
     * Returns: void
     */
    void SetActiveObject(TrecPointer<TVariable> var);

    /**
     * Method: TcInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     * Returns: void
     */
    virtual void PreProcess(ReturnObject& ret) = 0;


    /**
     * Method: TcInterpretor::TcInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TcInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);

    /**
     * Method: TcInterpretor::SetSelf
     * Purpose: Allows the Variable to have a reference to itself
     * Parameters: TrecPointer<TVariable> - reference to assign
     * Returns: void
     *
     * Attributes: override
     */
    virtual void SetSelf(TrecPointer<TVariable>) override;

    /**
     * Method: TcInterpretor::ProcessIndividualStatement
     * Purpose: Allows an Interpretor to process a statement 
     * Parameters: const TString& statement - the statemnt to process
     *              ReturnObject& ret - offers return information
     * Returns: void
     * 
     * Attributes: abstract
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret) = 0;
protected:

    /**
     * Self Reference
     */
    TrecSubPointerSoft<TVariable, TcInterpretor> self;
    TrecPointerSoft<TVariable> parent;
    TrecPointer<TEnvironment> environment;

    TrecPointer<TVariable> methodObject;            // 'this' or 'self' object to act on
    TDataArray<TString> paramNames, paramTypes;     // Parameter Names and Types
    TDataArray<TrecPointer<TVariable>> defaultVars; // List of Default Values to use

    TDataMap<TcVariableHolder> variables;       // List of Variables to Hold
    TDataMap<TcVariableHolder> strictVariables; // List of Variables to hold and Generate prior to runtime (must not be removed)
    
};

