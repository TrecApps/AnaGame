#include "TcExpression.h"

TcExpression::ExpressionComponent::ExpressionComponent()
{
}

TcExpression::ExpressionComponent::ExpressionComponent(const ExpressionComponent& copy)
{
}

TcExpression::ExpressionComponent::ExpressionComponent(TrecSubPointer<TVariable, TcExpression> ex)
{
}

TcExpression::ExpressionComponent::ExpressionComponent(tc_int_op o)
{
}

TcExpression::TcExpression()
{
}

bool TcExpression::AppendString(const TString& str, bool ex)
{
	return false;
}

bool TcExpression::AppendNumber(TrecPointer<TVariable> num)
{
	return false;
}

bool TcExpression::AppendVariable(const TString& str)
{
	return false;
}

bool TcExpression::AppendExpression(TrecSubPointer<TVariable, TcExpression> exp)
{
	return false;
}

bool TcExpression::AppendOperator(tc_int_op op)
{
	return false;
}

void TcExpression::Process(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret)
{
}

var_type TcExpression::GetVarType()
{
	return var_type::expression;
}

TString TcExpression::GetString()
{
	return TString();
}

UINT TcExpression::Get4Value()
{
	return 0;
}

ULONG64 TcExpression::Get8Value()
{
	return ULONG64();
}

UINT TcExpression::GetSize()
{
	return 0;
}

UINT TcExpression::GetVType()
{
	return 0;
}
