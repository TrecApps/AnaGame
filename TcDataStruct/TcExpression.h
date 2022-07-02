#pragma once
#include "TcDataStruct.h"
#include <TVariable.h>
#include <TcInterpretor.h>
#include "TcOperator.h"
#include "TPrimitiveVariable.h"
#include "TcType.h"
#include "AnagameRunners.h"
#include "TcCompiler.h"
#include <TMap.h>
#include "DefaultObjectOperator.h"
#include "Operator.h"
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
	variable,		// Holds a variable Reference
	function_call,	// Calls a function
	array_exp,		// An Array
	exp_collection
}tc_exp_type;


class TC_DATA_STRUCT TcExpression
{
public:

	virtual void AppendExpression(TrecPointer<TcExpression> exp);
	virtual bool AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable);

	virtual tc_exp_type GetExpressionType() = 0;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) = 0;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) = 0;
};

class TC_DATA_STRUCT TcStringExpression: public TcExpression
{
protected:
	TString value;
	TDataArray<TrecPointer<TcExpression>> expressions;
public:
	virtual void AppendExpression(TrecPointer<TcExpression> exp);
	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT TcNumberExpression : public TcExpression
{
protected:
	DoubleLong value;
public:

	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT TcVariableExpression : public TcExpression
{
protected:
	TString varName;
	TrecPointer<TVariable> object;		// In case this variable is an attribute
public:
	bool SetObject(TrecPointer<TVariable> var);
	virtual bool AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable)override;
	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT TcFunctionCallExpression : public TcExpression
{
protected:
	TString functionName;
	TDataArray<TrecPointer<TcExpression>> paramValues;
	TrecPointer<TVariable> object;			// Used in case this is a method
public:

	bool SetObject(TrecPointer<TVariable> var);

	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT TcArrayExpression : public TcExpression
{
protected:
	TDataArray<TrecPointer<TcExpression>> paramValues;
public:
	virtual void AppendExpression(TrecPointer<TcExpression> exp) override;
	virtual bool AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable)override;

	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT TcGroupExpression : public TcExpression
{
protected:
	TDataArray<TrecPointer<TcExpression>> expressions;
	TDataArray<TrecPointer<TcOperator>> operatorLibrary;
	TDataArray<operator_type> operators;
public:

	void SetOperators(TDataArray<TrecPointer<TcOperator>>&);

	virtual void AppendExpression(TrecPointer<TcExpression> exp)override;
	void AppendOperator(operator_type opType);


	virtual tc_exp_type GetExpressionType() override;
	virtual bool SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages) override;

	virtual bool ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable) override;
};

class TC_DATA_STRUCT UniOpExpression : public TcExpression
{
public:
	typedef enum class uni_op
	{
		op_negate,
		op_pre_inc,
		op_pre_dec,
		op_post_inc,
		op_post_dec
	}uni_op;

	UniOpExpression(uni_op op, TrecPointer<TcExpression> exp);

protected:
	uni_op op;
	TrecPointer<TcExpression> subExpression;
};