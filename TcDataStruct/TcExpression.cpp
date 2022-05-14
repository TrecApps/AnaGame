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

void TcStringExpression::CompileExpression(TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, TDataArray<CompileMessage>& messages)
{
	TrecSubPointer<TVariable, AnagameRunner> runner;
	if (!runners.retrieveEntry(currentRunner, runner) || !runner.Get())
	{
		CompileMessage m;
		m.isError = true;
		m.message.Format("Internal Error! Attempt to compile Procedure %ws failed. Proc not found!", currentRunner.GetConstantBuffer().getBuffer());
		messages.push_back(m);
		return;
	}

	runner->AddOp(this->stringExpression);

	for (UINT Rust = 0; Rust < subExpressions.Size(); Rust++)
	{
		if (subExpressions[Rust]->IsCompileConfirmed())
		{
			TrecPointer<TVariable> value;
			subExpressions[Rust]->GetValue(value);

			runner->AddOp(value.Get() ? value->GetString() : L"null");
		}
		else
		{
			subExpressions[Rust]->CompileExpression(vars, runners, currentRunner, messages);
		}
	}

	runner->AddOp(AnagameRunner::RunnerCode(runner_op_code::str_cond, subExpressions.Size()));
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

VariableContiainer::Variable::Variable()
{
	stackLoc = 0;
	useObject = true;
	rawData = 0;
}

VariableContiainer::Variable::Variable(const Variable& copy)
{
	stackLoc = copy.stackLoc;
	useObject = copy.useObject;
	rawData = copy.rawData;
	objData = copy.objData;
	type = copy.type;
}

void TcExpression::CompileExpression(TDataArray<VariableContiainer>& vars, TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, const TString& currentRunner, TDataArray<CompileMessage>& messages)
{
}
