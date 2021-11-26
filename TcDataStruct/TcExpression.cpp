#include "TcExpression.h"
#include "TStringVariable.h"
#include "TSpecialVariable.h"

TcExpression::ExpressionComponent::ExpressionComponent()
{
	op = tc_int_op::none;
}

TcExpression::ExpressionComponent::ExpressionComponent(const ExpressionComponent& copy)
{
	op = copy.op;
	expression = copy.expression;
}

TcExpression::ExpressionComponent::ExpressionComponent(TrecSubPointer<TVariable, TcExpression> ex)
{
	op = tc_int_op::none;
	expression = ex;
}

TcExpression::ExpressionComponent::ExpressionComponent(tc_int_op o)
{
	op = o;
}

void TcExpression::ProcessExpression(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret, const TDataArray<ExpressionComponent>& comp)
{
	auto tempComp = comp;

	for (UINT Rust = 0; Rust < operators.Size(); Rust++)
	{
		ProcessOperators(processor, operators, ret,tempComp, Rust);
	}

}

void TcExpression::ProcessOperators(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret, TDataArray<ExpressionComponent>& comp, UINT opIndex)
{
	scan:
	if (operators[opIndex].rightToLeft)
	{
		
		for (UINT Rust = 0; Rust < comp.Size(); Rust++)
		{
			if (comp[Rust].op != tc_int_op::none)
			{
				for (UINT C = 0; C < operators[opIndex].operators.Size(); C++)
				{
					if (operators[opIndex].operators[C]->GetOpType() == comp[Rust].op)
					{
						ProcessOperator(processor, operators, ret, comp, Rust, operators[opIndex].operators[C]);
						if (ret.returnCode)
							return;
						goto scan; // Since comp can be reduced
					}
				}
			}
		}
	}
	else
	{
		for (UINT Rust = comp.Size() - 1; Rust < comp.Size(); Rust--)
		{
			if (comp[Rust].op != tc_int_op::none)
			{
				for (UINT C = 0; C < operators[opIndex].operators.Size(); C++)
				{
					if (operators[opIndex].operators[C]->GetOpType() == comp[Rust].op)
					{
						ProcessOperator(processor, operators, ret, comp, Rust, operators[opIndex].operators[C]);
						if (ret.returnCode)
							return;
						goto scan; // Since comp can be reduced
					}
				}
			}
		}
	}
}

void TcExpression::ProcessOperator(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret, TDataArray<ExpressionComponent>& comp, UINT expIndex, TrecPointer<TcOperator> op)
{
	TrecPointer<TVariable> left, right;
	TDataArray<TrecPointer<TVariable>> params;
	switch (op->GetOpType())
	{
	case tc_int_op::separator:
		return;

	case tc_int_op::conditional:
		if(expIndex + 3 >= comp.Size() || !expIndex)
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}

		if (comp[expIndex + 2].op != tc_int_op::separator)
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}

		if(!comp[expIndex -1].expression.Get() || !comp[expIndex + 1].expression.Get() || !comp[expIndex + 3].expression.Get())
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}

		comp[expIndex - 1].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		ret.errorObject.Nullify();

		comp[expIndex + 1].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		ret.errorObject.Nullify();

		comp[expIndex +3].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		ret.errorObject.Nullify();

		op->PerformOperation(params, ret);
		if (!ret.returnCode)
		{
			comp[expIndex - 1].expression = GenerateExpression(ret.errorObject);
			for (UINT Rust = 0; Rust < 4; Rust++)
			{
				comp.RemoveAt(expIndex);
			}
		}
		


		return;
	case tc_int_op::pre_inc:
	case tc_int_op::post_inc:
	case tc_int_op::pre_dec:
	case tc_int_op::post_dec:

		break;
	case tc_int_op::not_l:
		if (expIndex < comp.Size() || !comp[expIndex].expression.Get())
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}
		comp[expIndex].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		op->PerformOperation(params, ret);
		if (!ret.returnCode)
		{
			comp[expIndex].expression = TcExpression::GenerateNumberExpression(ret.errorObject);
			comp.RemoveAt(expIndex);
			break;
		}
	default:
		if (expIndex + 1 >= comp.Size() || !expIndex)
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}

		if (!comp[expIndex - 1].expression.Get() || !comp[expIndex + 1].expression.Get())
		{
			ret.returnCode = ret.ERR_UNEXPECTED_TOK;
			ret.errorMessage.Set("Exprected Expression after __not__ operator");
			return;
		}
		comp[expIndex - 1].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		ret.errorObject.Nullify();

		comp[expIndex + 1].expression->Process(processor, operators, ret);
		if (ret.returnCode) return;
		params.push_back(ret.errorObject);
		ret.errorObject.Nullify();

		op->PerformOperation(params, ret);
		if (!ret.returnCode)
		{
			if (op->IsAssign())
			{
				// To-Do: Manage Assign
			}
			comp[expIndex - 1].expression = GenerateExpression(ret.errorObject);
			for (UINT Rust = 0; Rust < 2; Rust++)
			{
				comp.RemoveAt(expIndex);
			}
		}

	}
}

bool TcExpression::InspectContainers(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators,
	ReturnObject& ret, TDataArray<TrecSubPointer<TVariable, TcExpression>>& expressions, TDataArray<tc_int_op>& ops)
{
	if (!ops.Size() && expressions.Size() == 1)
	{
		expressions[0]->Process(processor, operators, ret);
		return true;
	}

	if (!ops.Size() && expressions.Size())
	{
		ret.returnCode = ret.ERR_UNEXPECTED_TOK;
		ret.errorMessage.Format(L"Found %d expressions without an operator to combine them!", expressions.Size());
		return true;
	}

	if (ops.Size() && !expressions.Size())
	{
		ret.returnCode = ret.ERR_UNEXPECTED_TOK;
		ret.errorMessage.Format(L"Found %d operators without an expression to operate on!", ops.Size());
		return true;
	}
	return false;
}

TrecSubPointer<TVariable, TcExpression> TcExpression::GenerateNumberExpression(TrecPointer<TVariable> num)
{
	TrecSubPointer<TVariable, TcExpression> ret;
	if (!num.Get() || num->GetVarType() != var_type::primitive)
		return ret;
	ret = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcExpression>();

	ret->type = tc_exp_type::number;
	ret->var = num;
	return ret;
}

TcExpression::TcExpression()
{
	type = tc_exp_type::expression;
}

TrecSubPointer<TVariable, TcExpression> TcExpression::AppendString(const TString& str, bool ex)
{
	if (type != tc_exp_type::expression)
		return TrecSubPointer<TVariable, TcExpression>();
	TrecSubPointer<TVariable, TcExpression> e = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcExpression>();

	e->type = ex ? tc_exp_type::string_exp : tc_exp_type::string;

	e->str.Set(str);
	components.push_back(e);

	return e;
}

bool TcExpression::AppendNumber(TrecPointer<TVariable> num)
{
	if(!num.Get() || num->GetVarType() != var_type::primitive || type != tc_exp_type::expression)
		return false;
	TrecSubPointer<TVariable, TcExpression> e = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcExpression>();

	e->type = tc_exp_type::number;
	e->var = num;
	components.push_back(e);
	return true;
}

bool TcExpression::AppendVariable(const TString& str)
{
	if (type != tc_exp_type::expression)
		return false;
	TrecSubPointer<TVariable, TcExpression> e = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TcExpression>();

	e->type = tc_exp_type::variable;
	e->str.Set(str);
	components.push_back(e);
	return true;
}

bool TcExpression::AppendExpression(TrecSubPointer<TVariable, TcExpression> exp)
{
	if (type != tc_exp_type::expression && type != tc_exp_type::string_exp && type != tc_exp_type::function)
		return false;

	if (type == tc_exp_type::string_exp)
	{
		int start = 0, i = -1, ei = -1;
		for (UINT Rust = 0; Rust < components.Size(); Rust++)
		{
			start = str.Find(L"${}", start);
			if (start == -1)
				return true;
		}
		i = str.Find(L"${", start);
		ei = str.Find(L"}", i);
		if (i == -1 || ei == -1)
			return true;

		if (ei == (i + 2))
		{
			if (exp.Get())
				components.push_back(TrecSubPointer<TVariable, TcExpression>());
		}
		else
			str.Delete(i + 1, ei - (i + 1));
	}
	
	components.push_back(exp);
	return true;
}

bool TcExpression::AppendOperator(tc_int_op op)
{
	if (type != tc_exp_type::expression)
		return false;
	components.push_back(op);
	return true;
}

void TcExpression::Process(TrecSubPointer<TVariable, TcInterpretor> processor, TDataArray<TcOperatorGroup>& operators, ReturnObject& ret)
{
	if (!processor.Get())
	{
		ret.returnCode = ret.ERR_INTERNAL;
		ret.errorMessage.Set("Interpretor needed for variable resolution in Expression Processing");
		return;
	}
	TString r(str);

	bool worked;
	TDataArray<TrecPointer<TVariable>> tda;

	switch (type)
	{
	case tc_exp_type::string_exp:
		for (UINT Rust = 0; components.Size(); Rust++)
		{
			if (components[Rust].op != tc_int_op::none)
			{
				ret.returnCode = ret.ERR_UNEXPECTED_TOK;
				ret.errorMessage.Set(L"Unexpected Operator in String Expression!");
				return;
			}
			TString rep;
			if (!components[Rust].expression.Get())
				rep.Set(L"null");
			else
			{
				components[Rust].expression->Process(processor, operators, ret);
				if (ret.returnCode)
					return;
				rep.Set(ret.errorObject.Get() ? ret.errorObject->GetString() : L"null");
				ret.errorObject.Nullify();
			}

			if (!r.Replace(L"${}", rep, false))
				break;
		}
	case tc_exp_type::string:
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(r);
		return;
	case tc_exp_type::number:
		ret.errorObject = var;
		return;
	case tc_exp_type::variable:
		ret.errorObject = processor->GetVariable(str, worked);
		if (!worked)
			ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
		return;
	case tc_exp_type::function:
		for (UINT Rust = 0; Rust < components.Size(); Rust++)
		{
			if (components[Rust].op != tc_int_op::none)
			{
				ret.returnCode = ret.ERR_UNEXPECTED_TOK;
				ret.errorMessage.Set(L"Unexpected Operator in String Expression!");
				return;
			}
			if (!components[Rust].expression.Get())
				tda.push_back(TrecPointer<TVariable>());
			else
			{
				components[Rust].expression->Process(processor, operators, ret);
				if (ret.returnCode)
					return;
				tda.push_back(ret.errorObject);
			}
		}
		dynamic_cast<TcInterpretor*>(var.Get())->SetIntialVariables(tda);
		ret = dynamic_cast<TcInterpretor*>(var.Get())->Run();
		return;

	case tc_exp_type::expression:

		ProcessExpression(processor, operators, ret, components);


	}
}

var_type TcExpression::GetVarType()
{
	return var_type::expression;
}

TString TcExpression::GetString()
{
	return str;
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
