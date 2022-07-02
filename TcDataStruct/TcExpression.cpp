#include "TcExpression.h"
#include "TStringVariable.h"
#include "TSpecialVariable.h"
#include "TContainerVariable.h"
#include "TObjectVariable.h"


void TcStringExpression::AppendExpression(TrecPointer<TcExpression> exp)
{
	expressions.push_back(exp);
}

tc_exp_type TcStringExpression::GetExpressionType()
{
	return expressions.Size() ? tc_exp_type::string_exp : tc_exp_type::string;
}

bool TcStringExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	this->value = value.Get() ? value->GetString() : L"null";
	return true;
}

bool TcStringExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	TString ret(value);
	bool process = expressions.Size() > 0;
	for (UINT Rust = 0; Rust < expressions.Size(); Rust++)
	{
		TString replacement(L"null");

		auto exp = expressions[Rust];
		if (exp.Get())
		{
			if (exp->ProcessExpression(ret, messages, helperVariable))
				replacement.Set(ret.errorObject.Get() ? ret.errorObject->GetString() : L"null");
			else return false;
		}

		process = value.Replace(L"${}", replacement, false) > 0;
	}
	ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(value);
	return true;
}

tc_exp_type TcNumberExpression::GetExpressionType()
{
	return tc_exp_type::number;
}

bool TcNumberExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	this->value = DoubleLong::GetValueFromPrimitive(value);
	return this->value.type != double_long::dl_invalid;
}

bool TcNumberExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	ret.errorObject.Nullify();
	switch (value.type)
	{
	case double_long::dl_double:
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(value.value.d);
		break;
	case double_long::dl_sign:
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(value.value.s);
		break;
	case double_long::dl_unsign:
		ret.errorObject = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TPrimitiveVariable>(value.value.u);
		break;
	}
	return ret.errorObject.Get() != nullptr;
}


void TcExpression::AppendExpression(TrecPointer<TcExpression> exp)
{
}

bool TcExpression::AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable)
{
	return false;
}

bool TcVariableExpression::SetObject(TrecPointer<TVariable> var)
{
	this->object = var;
	return true;
}

bool TcVariableExpression::AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable)
{
	if (dynamic_cast<TcInterpretor*>(helperVariable.Get()))
	{
		return dynamic_cast<TcInterpretor*>(helperVariable.Get())->UpdateVariable(this->varName, value) == 0;
	}
	return false;
}

tc_exp_type TcVariableExpression::GetExpressionType()
{
	return tc_exp_type::variable;
}

bool TcVariableExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	this->varName = value->GetString();
	return true;
}

bool TcVariableExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	if (dynamic_cast<TContainerVariable*>(object.Get()))
	{
		bool pres;
		ret.errorObject = dynamic_cast<TContainerVariable*>(object.Get())->GetValue(varName, pres);
		if (!pres)
		{
			ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);

			
		}
		return true;
	}
	if (dynamic_cast<TObjectVariable*>(object.Get()))
	{
		TrecObjectPointer obj = dynamic_cast<TObjectVariable*>(object.Get())->GetObjectW();
		if (dynamic_cast<TVObject*>(obj.Get()))
		{
			if(!dynamic_cast<TVObject*>(obj.Get())->GetVariable(varName, ret.errorObject))
				ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
		}
		else
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			return false;
		}
	}

	if (dynamic_cast<TcInterpretor*>(helperVariable.Get()))
	{
		bool pres;
		ret.errorObject = dynamic_cast<TcInterpretor*>(helperVariable.Get())->GetVariable(varName, pres);
		if (!pres)
		{
			ret.errorObject = TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined);
		}
		return true;
	}
	return false;
}

bool TcFunctionCallExpression::SetObject(TrecPointer<TVariable> var)
{
	object = var;
	return true;
}

tc_exp_type TcFunctionCallExpression::GetExpressionType()
{
	return tc_exp_type::function_call;
}

bool TcFunctionCallExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	this->functionName = value->GetString();
	return true;
}

bool TcFunctionCallExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	TrecPointer<TVariable> function;
	if (dynamic_cast<TContainerVariable*>(object.Get()))
	{
		bool pres;
		function = dynamic_cast<TContainerVariable*>(object.Get())->GetValue(functionName, pres);
	}
	else if (dynamic_cast<TObjectVariable*>(object.Get()))
	{
		TrecObjectPointer obj = dynamic_cast<TObjectVariable*>(object.Get())->GetObjectW();
		if (dynamic_cast<TVObject*>(obj.Get()))
		{
			dynamic_cast<TVObject*>(obj.Get())->GetVariable(functionName, function);
		}
		else
		{
			ret.returnCode = ret.ERR_UNSUPPORTED_OP;
			return false;
		}
	}
	else if (dynamic_cast<TcInterpretor*>(helperVariable.Get()))
	{
		bool pres;
		function = dynamic_cast<TcInterpretor*>(helperVariable.Get())->GetVariable(functionName, pres);
	}

	if (!dynamic_cast<TcRunner*>(function.Get()))
	{
		ret.returnCode = ret.ERR_BROKEN_REF;
		ret.errorMessage.Format(L"Could not Find procedure '%ws'!", functionName.GetConstantBuffer().getBuffer());
		return false;
	}

	TDataArray<TrecPointer<TVariable>> params;
	bool worked = true;
	for (UINT Rust = 0; Rust < this->paramValues.Size(); Rust++)
	{
		ret.errorObject.Nullify();
		if (paramValues[Rust].Get())
		{
			worked &= paramValues[Rust]->ProcessExpression(ret, messages, helperVariable);
		}
		params.push_back(ret.errorObject);

	}

	if (!worked)
		return false;

	dynamic_cast<TcRunner*>(function.Get())->SetIntialVariables(params);
	ret = dynamic_cast<TcRunner*>(function.Get())->Run();
	

	return ret.returnCode == 0;;
}

void TcArrayExpression::AppendExpression(TrecPointer<TcExpression> exp)
{
	this->paramValues.push_back(exp);
}

bool TcArrayExpression::AttemptAssign(TrecPointer<TVariable> value, TrecPointer<TVariable> helperVariable)
{
	if (paramValues.Size() == 1 && paramValues[0].Get() && paramValues[0]->GetExpressionType() == tc_exp_type::variable)
		return paramValues[0]->AttemptAssign(value, helperVariable);

	if(!paramValues.Size())
		return false;

	TrecSubPointer<TVariable, TContainerVariable> values = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TContainerVariable>(value);

	bool worked = true;
	for (UINT Rust = 0; Rust < paramValues.Size(); Rust++)
	{
		if (!paramValues[Rust].Get() && paramValues[Rust]->GetExpressionType() != tc_exp_type::variable)
			return false;
		if (values.Get())
			worked &= paramValues[Rust]->AttemptAssign(Rust < values->GetSize() ?
				values->GetValueAt(Rust) : TSpecialVariable::GetSpecialVariable(SpecialVar::sp_undefined), helperVariable);
		else
			worked &= paramValues[Rust]->AttemptAssign(value, helperVariable);
	}
	return worked;
}

tc_exp_type TcArrayExpression::GetExpressionType()
{
	return tc_exp_type::array_exp;
}

bool TcArrayExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	return false;
}

bool TcArrayExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	TrecSubPointer<TVariable, TContainerVariable> newArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
	

	for (UINT Rust = 0; Rust < paramValues.Size(); Rust++)
	{
		ret.errorObject.Nullify();
		if (paramValues[Rust].Get())
		{
			if (!paramValues[Rust]->ProcessExpression(ret, messages, helperVariable))
				return false;
		}
		newArray->AppendValue(ret.errorObject);
	}

	ret.errorObject = TrecSubToTrec(newArray);

	return true;
}

void TcGroupExpression::SetOperators(TDataArray<TrecPointer<TcOperator>>& ops)
{
	operatorLibrary = ops;
}

void TcGroupExpression::AppendExpression(TrecPointer<TcExpression> exp)
{
	expressions.push_back(exp);
}

void TcGroupExpression::AppendOperator(operator_type opType)
{
	operators.push_back(opType);
}

tc_exp_type TcGroupExpression::GetExpressionType()
{
	return tc_exp_type::exp_collection;
}

bool TcGroupExpression::SetValue(TrecPointer<TVariable> value, TDataArray<CompileMessage>& messages)
{
	return false;
}

bool TcGroupExpression::ProcessExpression(ReturnObject& ret, TDataArray<CompileMessage>& messages, TrecPointer<TVariable> helperVariable)
{
	TDataArray<TrecPointer<TVariable>> expressions;

	for (UINT Rust = 0; Rust < this->expressions.Size(); Rust++)
	{
		ret.errorObject.Nullify();

		if (this->expressions[Rust].Get() && !this->expressions[Rust]->ProcessExpression(ret, messages, helperVariable))
			return false;
		expressions.push_back(ret.errorObject);
	}

	TDataArray<operator_type> operators = this->operators;

	for (UINT Rust = 0; Rust < operatorLibrary.Size() && operators.Size(); Rust++)
	{
		bool rightToLeft = operatorLibrary[Rust]->FromRightToLeft();

		for (UINT C = rightToLeft ? operators.Size() - 1 : 0; C < operators.Size(); rightToLeft ? C-- : C++)
		{
			if (C + 1 >= expressions.Size())
				return false;

			if (!operatorLibrary[Rust]->HasOp(operators[C]))
				continue;
			TrecPointer<TVariable> var1 = expressions[C], var2 = expressions[C + 1];
			
			operatorLibrary[Rust]->process(operators[C], ret, var1, var2);

			if (ret.returnCode)
				return false;

			expressions[C] = ret.errorObject;
			expressions.RemoveAt(C + 1);
			operators.RemoveAt(C);

			rightToLeft ? C++ : C--;
		}
	}


	if(operators.Size() || expressions.Size( ) != 1)
		return false;

	ret.errorObject = expressions[0];
	return true;
}


UniOpExpression::UniOpExpression(uni_op op, TrecPointer<TcExpression> exp)
{
	this->op = op;
	subExpression = exp;
}
