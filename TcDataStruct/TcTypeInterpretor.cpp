#include "TcTypeInterpretor.h"

TcTypeInterpretor::TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env):
    TcInterpretor(parentInterpretor, env)
{
	isAsync = false;
}

bool TcTypeInterpretor::SubmitClassType(const TString& className, TClassStruct& classStruct, bool updating)
{
	if (!updating)
	{
		TClassStruct s;
		if (classes.retrieveEntry(className, s))
			return false;

		classes.addEntry(className, classStruct);
		return true;
	}
	else
	{
		TClassStruct s;
		if (classes.retrieveEntry(className, s))
		{
			classes.setEntry(className, classStruct);
			return true;
		}
		return dynamic_cast<TcTypeInterpretor*>(parent.Get()) && 
			dynamic_cast<TcTypeInterpretor*>(parent.Get())->SubmitClassType(className, classStruct, updating);
	}
}

bool TcTypeInterpretor::GetClass(const TString& className, TClassStruct& classStruct)
{
	if (classes.retrieveEntry(className, classStruct))
		return true;

	TrecSubPointer<TVariable, TcTypeInterpretor> typeParent = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcTypeInterpretor>(
		TrecPointerKey::GetTrecPointerFromSoft<>(parent));
	if (typeParent.Get())
		typeParent->GetClass(className, classStruct);
	return false;
}

TrecPointer<TVariable> TcTypeInterpretor::PrepAsyncCall()
{
	return TrecPointer<TVariable>();
}

bool TcTypeInterpretor::IsAsync()
{
	return isAsync;
}
