#pragma once
#include "TVariable.h"

#include "TDataMap.h"

/**
 * Class: BinaryStack
 * Purpose: Represents a Binary Stack
 */
class _TREC_LIB_DLL BinaryStack
{
protected:
    TDataArray<UINT> binaries;
    ReturnObject retObj;
public:
    bool IsError();
    ReturnObject GetError(bool reset);

    UINT GetSize();
    UINT GetSizeInBytes();

    BinaryStack();
    BinaryStack(const BinaryStack& copy);

    void Push(UINT data);
    void Push(ULONG64 data);

    UINT Pop1();
    ULONG64 Pop2();

    bool GetValueFromTop(UINT loc, UINT& val);

};

/**
 * Class: BinaryStack
 * Purpose: Represents a Binary Stack
 */
class _TREC_LIB_DLL ObjectStack
{
protected:
    TDataArray<TrecPointer<TVariable>> objects;
    ReturnObject retObj;
public:
    bool IsError();
    ReturnObject GetError(bool reset);

    UINT GetSize();

    ObjectStack();
    ObjectStack(const ObjectStack& copy);

    void Push(TrecPointer<TVariable> var);

    TrecPointer<TVariable> Pop();

    bool GetValueFromTop(UINT loc,TrecPointer<TVariable>& val);
};

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

    /**
     * Method: TcVariableHolder::TcVariableHolder
     * Purpose: Param Constructor
     * Parameters: bool mut - whether this variable is mutable or not
     *              const TString& type - the type of variable this is (empty for 'any')
     *              TrecPointer<TVariable> value - the actual value held
     * Returns: New variable holder
     */
    TcVariableHolder(bool mut, const TString& type, TrecPointer<TVariable> value);
};

class _TREC_LIB_DLL TcRunner :
    public TVariable
{
public:

    virtual var_type GetVarType() override;

    /**
     * Method: TcRunner::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() = 0;

    /**
     * Method: TcRunner::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) = 0;

    /**
     * Method: TcRunner::SetParamNames
     * Purpose: Allows Parameter names to be set
     * Parameters: TDataArray<TString>& paramNames - the Names of Initial parameters
     * Returns: void
     */
    void SetParamNames(TDataArray<TString>& paramNames);


    /**
     * Method: TcRunner::SetParamTypes
     * Purpose: Allows Parameter types to be set
     * Parameters: TDataArray<TString>& paramTypes - the Types of Initial parameters
     * Returns: void
     */
    void SetParamTypes(TDataArray<TString>& paramTypes);

    /**
     * Mehtod: TcRunner::SetCurrentStack
     * Purpose: Sets the binary Stack for the runner (meant for Anagame Binary and Hybrid Runners)
     * Parameters: TrecPointer<BinaryStack> stack - binary Stack
     * Returns: void
     */
    virtual void SetCurrentStack(TrecPointer<BinaryStack>& stack);
    

    /**
     * Mehtod: TcRunner::SetCurrentStack
     * Purpose: Sets the binary Stack for the runner (meant for Anagame Binary and Hybrid Runners)
     * Parameters: TrecPointer<BinaryStack> stack - binary Stack
     * Returns: void
     */
    virtual void SetCurrentStack(TrecPointer<ObjectStack>& stack);


/// <summary>
///  TVariable Methods
/// 
/// </summary>

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
    virtual UINT GetVType() override;


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

    virtual bool IsTruthful(TrecPointer<TVariable> var);
protected:
    TDataArray<TString> paramNames, paramTypes;     // Parameter Names and Types
    TDataMap<TcVariableHolder> variables;       // List of Variables to Hold
};

typedef void (*NativeTcRunner1)(TDataMap<TcVariableHolder> variables, ReturnObject& ret);

class _TREC_LIB_DLL TcNativeRunner1 :
    public TcRunner
{
private:
    NativeTcRunner1 function;
public:
    /**
     * Method: TcNativeRunner1::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run() override;

    /**
     * Method: TcNativeRunner1::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params) override;

    /**
     * Method: TcNaticeRunner1::TcNativeRunner1
     * Purpose: Constructor
     * Parameters: NativeTcRunner1 function
     * Returns: New TcNativeRunner1 object
     */
    TcNativeRunner1(NativeTcRunner1 function);
};