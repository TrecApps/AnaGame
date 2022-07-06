#include "TcObjectInterpretor.h"
#include "TStringVariable.h"


void TcObjectInterpretor::TcObjectStatement::SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next)
{
}

void TcObjectInterpretor::TcObjectStatement::SetNextStatement(TrecPointer<TcObjectStatement> next)
{
}

void TcObjectInterpretor::TcObjectStatement::SetExpression(TrecPointer<TcExpression> exp)
{
	this->expression = exp;
}

void TcObjectInterpretor::TcObjectStatement::Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret)
{
	if (expression.Get())
	{
		TDataArray<CompileMessage> message;
		auto var = TrecSubToTrec(runner);
		if (expression->ProcessExpression(ret, message, var) && !ret.returnCode)
		{
			ret.returnCode = ret.ERR_GENERIC_ERROR;
		}
	}
}

TcObjectInterpretor::TcBlockStatement::TcBlockStatement(const TString& name, bool conditional, bool repeat, bool runFirst, bool negate)
{
	this->conditional = conditional;
	this->repeat = repeat;
	this->runFirst = runFirst;
	this->negate = negate;
	this->name.Set(name);
}

void TcObjectInterpretor::TcBlockStatement::SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next)
{
	this->block = next;
}

void TcObjectInterpretor::TcBlockStatement::SetNextStatement(TrecPointer<TcObjectStatement> next)
{
	this->next = next;
}

void TcObjectInterpretor::TcBlockStatement::Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret)
{
	TcObjectStatement::Run(runner, ret);
	if (!block.Get() || ret.returnCode)
	{
		return;
	}
	if (!conditional)
	{
		ret = block->Run();
	}
	else {
		if (!repeat)
		{
			if (runner->IsTruthful(ret.errorObject) != negate)
				ret =block->Run();
			else if (next.Get())
				next->Run(runner, ret);
		}
		else
		{
			if (runFirst)
			{
				ret = block->Run();
				if (ret.returnCode)
					return;
				TcObjectStatement::Run(runner, ret);
			}

			while((runner->IsTruthful(ret.errorObject) != negate) && !ret.returnCode)
			{
				block->Run();
				if (ret.returnCode)
					return;

				// For loop is responsible for handling break and for
				if (ret.mode != return_mode::rm_regular)
				{
					if (ret.mode == return_mode::rm_return || ret.mode == return_mode::rm_yield)
						return;
					// here, we either break or continue
					return_mode m = ret.mode;

					// If this is to break out of or continue in a specific a specific block, only reset if this shile loop is said block
					if(ret.errorMessage.GetSize() && !name.Compare(ret.errorMessage))
						ret.mode = return_mode::rm_regular;
					if (m == return_mode::rm_break)
						break;
				}

				TcObjectStatement::Run(runner, ret);

			}

			if (next.Get())
				next->Run(runner, ret);
		}
	}
}

void TcObjectInterpretor::TcExceptionStatement::SetExpression(TrecPointer<TcExpression> exp)
{
}

void TcObjectInterpretor::TcExceptionStatement::SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next)
{
	TcBlockStatement::SetSubStatements(next);
}

void TcObjectInterpretor::TcExceptionStatement::SetNextStatement(TrecPointer<TcObjectStatement> next)
{
	TcBlockStatement::SetNextStatement(next);
}

void TcObjectInterpretor::TcExceptionStatement::Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret)
{
	if (isTry)
	{
		if (block.Get())
		{
			ret = block->Run();
		}
		if (next.Get())
			next->Run(runner, ret);
	}
	else if (this->errorName.GetSize())
	{
		if (ret.returnCode)
		{
			
			TDataArray<TString> n;
			n.push_back(errorName);
			block->SetParamNames(n);

			TDataArray<TrecPointer<TVariable>> vars;
			vars.push_back(ret.errorObject.Get() ? ret.errorObject : TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TStringVariable>(ret.errorMessage));
			block->SetIntialVariables(vars);

			ret = block->Run();
		}
		if (next.Get())
			next->Run(runner, ret);
	}
	else if(block.Get())
	{
		auto r = block->Run();
		if (!ret.returnCode)
			ret = r;
	}
}

void TcObjectInterpretor::TcForStatement::SetExpression(TrecPointer<TcExpression> exp)
{
}

void TcObjectInterpretor::TcForStatement::SetSubStatements(TrecSubPointer<TVariable, TcObjectInterpretor> next)
{
}

void TcObjectInterpretor::TcForStatement::SetNextStatement(TrecPointer<TcObjectStatement> next)
{
}

void TcObjectInterpretor::TcForStatement::Run(TrecSubPointer<TVariable, TcObjectInterpretor> runner, ReturnObject& ret)
{
}

void TcObjectInterpretor::SetSelf(TrecPointer<TVariable> self)
{
	TVariable::SetSelf(self);
	if (self.Get() != this)
		throw L"";
	this->self = TrecPointerKey::GetSoftSubPointerFromSub<> (TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcObjectInterpretor>(self));
}

TcObjectInterpretor::TcObjectInterpretor(TrecPointerSoft<TEnvironment> environment, TrecSubPointer<TVariable, TcObjectInterpretor> parent)
{
	this->parent = TrecPointerKey::GetSoftSubPointerFromSub<>(parent);
	this->environment = environment;
	yeildIndex = 0;
}

void TcObjectInterpretor::SetTruthDetector(TrecSubPointer<TVariable, TcRunner> isTrue)
{
	this->isTrue = isTrue;
}

void TcObjectInterpretor::SetStatements(TDataArray<TrecPointer<TcObjectStatement>> statements)
{
	this->statements;
}

ReturnObject TcObjectInterpretor::Run()
{
	TrecSubPointer<TVariable, TcObjectInterpretor> thisInterpretor = TrecPointerKey::GetSubPointerFromSoft<>(self);
	ReturnObject ret;

	UINT tempIndex = yeildIndex;
	yeildIndex = 0;
	for (UINT Rust = tempIndex; Rust < statements.Size(); Rust++)
	{
		statements[Rust]->Run(thisInterpretor, ret);
		if (ret.returnCode || (ret.mode != return_mode::rm_regular))
		{
			if (ret.mode == return_mode::rm_yield || ret.mode == return_mode::rm_block)
			{
				yeildIndex = Rust++;
			}
			break;
		}
	}
	return ret;
}

void TcObjectInterpretor::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
	yeildIndex = 0;
	TString finalName;
	TrecSubPointer<TVariable, TContainerVariable> finalArray;
	for (UINT Rust = 0; Rust < params.Size(); Rust++)
	{
		TcVariableHolder holder(true, L"", params[Rust]);

		if (Rust == paramNames.Size() - 1 && paramNames[Rust - 1].StartsWith(L"#..#"))
		{
			finalName.Set(paramNames[Rust - 1].SubString(4));
			finalArray = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
		}
		if (finalArray.Get())
			finalArray->AppendValue(params[Rust]);
		else if (Rust < paramNames.Size())
			variables.addEntry(paramNames[Rust], holder);
	}

	if (finalArray.Get())
		variables.addEntry(finalName, TcVariableHolder(true, L"", TrecSubToTrec(finalArray)));
}

bool TcObjectInterpretor::IsTruthful(TrecPointer<TVariable> var)
{
	if (this->isTrue.Get())
	{
		
		TDataArray<TrecPointer<TVariable>> value;
		value.push_back(var);
		isTrue->SetIntialVariables(value);
		ReturnObject ret = isTrue->Run();
		return ret.errorObject.Get() ? ret.errorObject->Get4Value() > 0 : false;
	}
	return false;
}
