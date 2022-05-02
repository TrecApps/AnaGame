#include "TcExpression.h"
#include "TStringVariable.h"
#include "TSpecialVariable.h"

TcStringExpression::TcStringExpression(const TString& exp)
{
	stringExpression.Set(exp);
}

int TcStringExpression::GetSubExpression(UINT start, TString& expression, const TString& begin, const TString& end)
{
	int bIndex = stringExpression.Find(begin, start);
	if (bIndex == -1)
		return -1;

	int eIndex = stringExpression.Find(end, bIndex + 1);
	if (eIndex == -1)
		return -1;

	expression.Set(stringExpression.SubString(bIndex + begin.GetSize(), eIndex));
	return eIndex + end.GetSize();
}

bool TcStringExpression::IsCompileConfirmed()
{
	return subExpressions.Size() == 0;
}

tc_exp_type TcStringExpression::GetExpressionType()
{
	return subExpressions.Size() ? tc_exp_type::string_exp : tc_exp_type::string;
}

bool TcStringExpression::GetValue(TrecPointer<TVariable>& value)
{
	value = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(stringExpression);
	return true;
}

TcNumberExpression::TcNumberExpression(TrecSubPointer<TVariable, TPrimitiveVariable> v)
{
	assert(v.Get());
}

bool TcNumberExpression::IsCompileConfirmed()
{
	// This holds a raw value
	return true;
}

tc_exp_type TcNumberExpression::GetExpressionType()
{
	return tc_exp_type::number;
}

bool TcNumberExpression::GetValue(TrecPointer<TVariable>& value)
{
	return false;
}
