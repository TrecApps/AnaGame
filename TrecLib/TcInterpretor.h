#pragma once
#include "TcRunner.h"
#include "TDirectory.h"
#include "TEnvironment.h"
#include "CodeStatement.h"
#include "ObjectOperator.h"
#include "TFileModifier.h"

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
 * 
 */
typedef enum class tc_int_op
{
    none, // There is no special operation involved
    // Basic Arithmetic
    add,
    sub,
    mult,
    div,
    mod,
    pow,
    add_assign,
    sub_assign,
    mul_assign,
    div_assign,
    mod_assign,
    pow_assign,
    pre_inc,
    post_inc,
    pre_dec,
    post_dec,
    // Logical Operators
    and_l,
    or_l,
    xor_l,
    not_l,
    and_assign,
    or_assign,
    xor_assign,
    // equality
    eq,
    eq_t,
    gt,
    //gt_t,
    gte,
    //gte_t,
    lt,
    //lt_t,
    lte,
    //lte_t,
    not_e,
    not_e_t,
    // expansion
    basic_exp,
    point_exp,
    // Bitwise
    and_b,
    or_b,
    xor_b,
    left_b,
    right_b,
    b_and_assign,
    b_or_assign,
    b_xor_assign,
    b_left_assign,
    b_right_assign,
    // Misclanous
    reg_assign,
    conditional,
    separator
}tc_int_op;



/**
 * Class: TcInterpretor
 * Purpose: Represents Code that can be run (and possibly compiled)
 * 
 * SuperClass: TVariable
 */
class _TREC_LIB_DLL TcInterpretor :
    public TcRunner
{
public:

    /**
     * Method: TcInterpretor::UpdateVariable
     * Purpose: Updates an existing Variable
     * Parameters: const TString& name - the name to update
     *              TrecPointer<TVariable> value - value to update it with
     *              bool addLocally - If true, then tf the variable is not found, go ahead and add it to 'this' interpretor (false by default)
     *              bool makeConst - whether the variable added should be const or not (ignored if 'addLocally' is false) (false by Default)
     * Returns: UINT - error code (0 for no error, 1 for doesn't exist, 2 for value is immutable)
     */
    virtual UINT UpdateVariable(const TString& name, TrecPointer<TVariable> value, bool addLocally = false, bool makeConst = false);


    /**
     * Method: TVariable::GetIterator
     * Purpose: for type that support it, retrieves an Iterator Variable of this Variable
     * Parameters: void
     * Return: TrecPointer<TVariable> - for some variable types, this would be null, but other variable types woudld support iteration
     *
     * Attributes: virtual
     */
    virtual TrecPointer<TVariable> GetIterator();

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






/// Interpretor Functionality

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
     * Method: TcInterpretor::CheckVarName
     * Purpose: Method Interpreters can use to inspect the variable name to make sure it is valid
     * Parameters: const  TString& varname - the variable name to check
     *              ReportObject& ro - the Object to modify based off of the findings
     * Returns: void
     */
    static void CheckVarName(const TString& varname, ReturnObject& ro);

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


    /**
     * Method: TcInterpretor::GetVariable
     * Purpose: Retrieves the variable specified
     * Parameters: TString& varName - the name of the variable requested
     *				bool& present - whether the variable was present or not (used to distinguish between 'null' and 'undefined')
     *              bool currentScope - whether to only check the current scope (don't check parent interpretors), default of false
     * Returns: TrecPointer<TVariable> - the variable requested (null if not found)
     */
    virtual TrecPointer<TVariable> GetVariable(TString& varName, bool& present, bool currentScope = false);

    /**
     * Method: TcInterprtor::PrepReturn
     * Purpose: Refactored Solution for setting an error message to return
     * Parameters: ReturnObject& ret - the object to operate on
     *              const TString& mess - the message to send
     *				const TString& stackMess - the message to prepend to the stack message
     *              USHORT code - the code to add
     *              LONG64 line - the line Number (use a negative value to ignore the stack trace)
     * Returns: void
     */
    void PrepReturn(ReturnObject& ret, const TString& mess, const TString& stackMess, USHORT code, LONG64 line);

    /**
     * Method: TcInterpretor::SetParent
     * Purpose: Allows a parent Interpretor to be configured after creation
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> parent - the parent to set
     *              bool replace - whether to set the parent even if interpretor currently has one
     * void: bool - whether the parent was set
     */
    bool SetParent(TrecSubPointer<TVariable, TcInterpretor> parent, bool replace);

    virtual TrecPointer<TVariable> Clone() override;

    UCHAR GetVarStatus(TString& varName);

    TDataMap<TcVariableHolder> GetVariables();
protected:

    /**
     * Method: TcInterpretor::AddModifiers
     * Purpose: Configures the file Modifiers before processing the file
     * Parameters: void
     * Returns: void
     * 
     * Attributes: virtual
     */
    virtual void AddModifiers();


    TDataArray<TrecPointer<TFileModifier>> fileModifiers; // Easy means through which Source Files can be Altered

    /**
     * Self Reference
     */
    TrecSubPointerSoft<TVariable, TcInterpretor> self;
    TrecPointerSoft<TVariable> parent;
    TrecPointer<TEnvironment> environment;

    TrecPointer<TVariable> methodObject;            // 'this' or 'self' object to act on
    TDataArray<TrecPointer<TVariable>> defaultVars; // List of Default Values to use


    TDataMap<TcVariableHolder> strictVariables; // List of Variables to hold and Generate prior to runtime (must not be removed)
    
    TString selfWord;

};

