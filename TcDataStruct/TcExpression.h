#pragma once
#include "TcDataStruct.h"
#include <TVariable.h>
#include <TcInterpretor.h>
#include "TcOperator.h"

/**
 * Enum Class: tc_exp_type
 * Purpose: Helps label the type of Expression we are dealing with
 */
typedef enum class tc_exp_type
{
	expression,
	function,
	number,
	string,
	string_exp,
	variable
}tc_exp_type;



class TC_DATA_STRUCT TcExpression : public TVariable
{
public:
    class TC_DATA_STRUCT ExpressionComponent
    {
    public: 
        ExpressionComponent();
        ExpressionComponent(const ExpressionComponent& copy);
        ExpressionComponent(TrecSubPointer<TVariable, TcExpression> ex);
        ExpressionComponent(tc_int_op o);
        TrecSubPointer<TVariable, TcExpression> expression;
        tc_int_op op;   // If None, then this is an expression
    };
protected:

    /**
     * List of Components in an expression
     */
    TDataArray<ExpressionComponent> components;
    /**
     * String expression or the variable
     */
    TString str;

    /**
     * Variable involved
     */
    TrecPointer<TVariable> var;

    /**
     * type of expression
     */
    tc_exp_type type;

    void ProcessExpression(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators,
        ReturnObject& ret, const TDataArray<ExpressionComponent>& comp);

    void ProcessOperators(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators,
        ReturnObject& ret, TDataArray<ExpressionComponent>& comp, UINT opIndex);

    void ProcessOperator(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators,
        ReturnObject& ret, TDataArray<ExpressionComponent>& comp, UINT expIndex, TrecPointer<TcOperator> op);

    bool InspectContainers(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators,
        ReturnObject& ret, TDataArray<TrecSubPointer<TVariable, TcExpression>>& comps, TDataArray<tc_int_op>& opList);

    static TrecSubPointer<TVariable, TcExpression> GenerateNumberExpression(TrecPointer<TVariable> v);
    static TrecSubPointer<TVariable, TcExpression> GenerateExpression(TrecPointer<TVariable> v);


public:



    /**
     * Method: TcExpression::TcExpression
     * Purpose: Default Constructor
     * Parameters:
     * Returns: New Expression
     */
    TcExpression();

    /**
     * Method: TcExpression::AppendString
     * Purpose: Adds a String expression to the Tree
     * Parameters: const TString& str - the string to add
     *              bool ex - whether the string contians expressions that need to be processed
     * Returns: bool - whether a string could be added
     */
    TrecSubPointer<TVariable, TcExpression> AppendString(const TString& str, bool ex);


    /**
     * Method: TcExpression::AppendNumber
     * Purpose: Adds a String expression to the Tree
     * Parameters: const TString& str - the string to add
     *              bool ex - whether the string contians expressions that need to be processed
     * Returns: bool - whether a Number could be added
     */
    bool AppendNumber(TrecPointer<TVariable> num);


    /**
     * Method: TcExpression::AppendVariable
     * Purpose: Adds a Variable expression to the Tree
     * Parameters: const TString& str - the variable to add
     * Returns: bool - whether a variable could be added
     */
    bool AppendVariable(const TString& str);


    /**
     * Method: TcExpression::AppendExpression
     * Purpose: Adds an expression to the Tree
     * Parameters: TrecSubPointer<TVariable, TcExpression> exp - the expression to add
     * Returns: bool - whether a Number could be added
     */
    bool AppendExpression(TrecSubPointer<TVariable, TcExpression> exp);

    /**
     * Method: TcExpression::AppendOperator
     * Purpose: Adds an Operator to the expression
     * Parameters: tc_int_op op - the operator to add
     * Returns bool - whether an operator can be added
     */
    bool AppendOperator(tc_int_op op);

    /**
     * Method: TcExpression::Process
     * Purpose: Processed the Operation
     * Parameters: TrecSubPointer<TVariable, TcInterpretor> processor - the interpretor caling the operations (needed to resolve variables)
     *              TDataArray<TcOperatorGroup>& operators - the operators the Processor has to work with
     *              ReturnObject& ret - the means of recieving information and returning information
     * Returns: void
     */
    void Process(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret);

    /**
     * Method: TcExpression::GetVarType
     * Purpose: Reports the type of varible that this object represents
     * Parameters: void
     * Returns: var_type - the type of variable this represents
     *
     * Attributes: override
     */
    virtual var_type GetVarType() override;


    /**
     * Method: TcExpression::GetObject
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
     * Method: TcExpression::Get4Value
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
     * Method: TcExpression::Get8Value
     * Purpose: Returns the value held by the variable assuming eight bytes (it is up to the interpretor to determine if conversion needs to be done)
     * Parameters: void
     * Returns: ULONG64 - The value held as an 8 bit integer (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual ULONG64 Get8Value() override;


    /**
     * Method: TcExpression::GetSize
     * Purpose: Returns the estimated size of the value held
     * Parameters: void
     * Returns: UINT - The estimated size in bytes of the data
     *
     * Attributes: override
     */
    virtual UINT GetSize() override;


    /**
     * Method: TcExpression::GetType
     * Purpose: Returns the basic type of the object
     * Parameters: void
     * Returns: UCHAR - The value held as a UINT (0 if not a primitive type)
     *
     * Attributes: override
     */
    virtual UINT GetVType() override;
};