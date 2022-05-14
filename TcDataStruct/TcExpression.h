#pragma once
#include "TcDataStruct.h"
#include <TVariable.h>
#include <TcInterpretor.h>
#include "TcOperator.h"
#include "TPrimitiveVariable.h"
#include "TcType.h"
#include "AnagameRunners.h"
#include "TcCompiler.h"

/**
 * Enum Class: tc_exp_type
 * Purpose: Helps label the type of Expression we are dealing with
 */
typedef enum class tc_exp_type
{
	expression,		// Holds an Operation
	function,		// Holds a Function
	number,			// Holds a number
	string,			// Holds a Confirmed String
	string_exp,		// Holds a String with Expressions within
	variable		// Holds a variable Reference
}tc_exp_type;


class VariableContiainer
{
public:
	class Variable
	{
	public:
		UINT stackLoc;
		bool useObject;
		ULONG64 rawData;
		TrecPointer<TVariable> objData;

		TrecPointer<TcType> type;

		Variable();
		Variable(const Variable& copy);
	};

	TMap<Variable> variables;
};



class TC_DATA_STRUCT TcExpression
{
public:

	/**
	 * Method: TcExpression::IsCompileConfirmed
	 * Purpose: Reports whether the Value of the expression can be confirmed at compile time. If so, simply calulate the value at compile time
	 *	instead of having the Runners calculate it each time the statement in question is run
	 * Parameters: void
	 * Returns: bool - whether the value is available at run time
	 */
	virtual bool IsCompileConfirmed() = 0;

	/**
	 * Method: TcExpression::GetExpressionType
	 * Purpose: Reports the type of expression being handled here
	 * Parameters: void
	 * Returns: tc_exp_type - the expression type this Expression Represents
	 */
	virtual tc_exp_type GetExpressionType() = 0;

	/**
	 * Method: TcExpression::GetValue
	 * Purpose: Returns a value that the Expression Represents
	 * Parameters: TrecPointer<TVariable>& value - reference to the Variable to return
	 * Returns: bool - whether the Expression returned a valid variable
	 */
	virtual bool GetValue(TrecPointer<TVariable>& value) = 0;

	/**
	 * Method: TcExpression::CompileExpression
	 * Purpose: Compiles the Expression
	 * Parameters: TDataArray<VariableContiainer>& vars - variables available
	 *					TMap<TrecSubPointer<TVariable, AnagameRunner>>& runners - runners available
	 *					const TString& currentRunner - name of the runner to look at
	 *					TDataArray<CompileMessage>& messages - output info
	 */
	virtual void CompileExpression(TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, TDataArray<CompileMessage>& messages);
};

class TcStringExpression : public TcExpression
{
	friend class BNFString;
protected:
	TDataArray<TrecPointer<TcExpression>> subExpressions;
	TString stringExpression;
	
public:
	/**
	 * Method: TcStringExpression::TcStringExpression
	 * Purpose: Constructor
	 * Parameters: void
	 * Returns: New String Expression Object
	 */
	TcStringExpression(const TString& exp);

	/**
	 * Mehtod: TcStringExpression::GetSubExpression
	 * Purpose: Retrieves the expression Strings within the Template String (managed by BNF tags)
	 * Parameters: UINT start - index of the string to start looking for
	 *				TString& expression - the expression extracted
	 *				const TString& begin - the beginning of the expression
	 *				const TString& end - the ending tag
	 * Returns: int - the index to begin the next search (-1 if no more sub-expressions are available)
	 */
	int GetSubExpression(UINT start, TString& expression, const TString& begin, const TString& end);

	/**
	 * Method: TcStringExpression::IsCompileConfirmed
	 * Purpose: Reports whether the Value of the expression can be confirmed at compile time. If so, simply calulate the value at compile time
	 *	instead of having the Runners calculate it each time the statement in question is run
	 * Parameters: void
	 * Returns: bool - whether the value is available at run time
	 */
	virtual bool IsCompileConfirmed()override;

	/**
	 * Method: TcStringExpression::GetExpressionType
	 * Purpose: Reports the type of expression being handled here
	 * Parameters: void
	 * Returns: tc_exp_type - the expression type this Expression Represents
	 */
	virtual tc_exp_type GetExpressionType()override;

	/**
	 * Method: TcStringExpression::GetValue
	 * Purpose: Returns a value that the Expression Represents
	 * Parameters: TrecPointer<TVariable>& value - reference to the Variable to return
	 * Returns: bool - whether the Expression returned a valid variable
	 */
	virtual bool GetValue(TrecPointer<TVariable>& value) override;

	/**
	 * Method: TcStringExpression::CompileExpression
	 * Purpose: Compiles the Expression
	 * Parameters: TDataArray<VariableContiainer>& vars - variables available
	 *					TMap<TrecSubPointer<TVariable, AnagameRunner>>& runners - runners available
	 *					const TString& currentRunner - name of the runner to look at
	 *					TDataArray<CompileMessage>& messages - output info
	 */
	virtual void CompileExpression(TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, TDataArray<CompileMessage>& messages)override;
};

class TcNumberExpression : public TcExpression
{
protected:
	TrecSubPointer<TVariable, TPrimitiveVariable> variable;
public:
	/**
	 * Method: TcNumberExpression::TcNumberExpression
	 * Purpose: Constructor
	 * Parameters: TrecSubPointer<TVariable, TPrimitiveVariable> v
	 * Returns: New Number Expression Object
	 */
	TcNumberExpression(TrecSubPointer<TVariable, TPrimitiveVariable> v);

	
	/**
	 * Method: TcNumberExpression::IsCompileConfirmed
	 * Purpose: Reports whether the Value of the expression can be confirmed at compile time. If so, simply calulate the value at compile time
	 *	instead of having the Runners calculate it each time the statement in question is run
	 * Parameters: void
	 * Returns: bool - whether the value is available at run time
	 */
	virtual bool IsCompileConfirmed()override;

	/**
	 * Method: TcNumberExpression::GetExpressionType
	 * Purpose: Reports the type of expression being handled here
	 * Parameters: void
	 * Returns: tc_exp_type - the expression type this Expression Represents
	 */
	virtual tc_exp_type GetExpressionType()override;

	/**
	 * Method: TcNumberExpression::GetValue
	 * Purpose: Returns a value that the Expression Represents
	 * Parameters: TrecPointer<TVariable>& value - reference to the Variable to return
	 * Returns: bool - whether the Expression returned a valid variable
	 */
	virtual bool GetValue(TrecPointer<TVariable>& value) override;
};

class TcVariableExpression : public TcExpression
{
protected:
	TString variableName;

public:

	TcVariableExpression(const TString& variableName);


};