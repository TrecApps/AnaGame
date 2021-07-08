#include "TcTypeInterpretor.h"

/**
 * Method: TcTypeInterpretor::TcTypeInterpretor
 * Purpose: Constructor
 * Parameters: TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env
 * Returns: New TcTypeInterpretor object
 */
TcTypeInterpretor::TcTypeInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env):
    TcInterpretor(parentInterpretor, env)
{
	isAsync = false;
}
/**
 * Method: TcTypeInterpretor::SubmitClassType
 * Purpose: Adds a type to be held by the Interpretor
 * Parameters: const TString& className - Name of the class to add
 *              TClassStruct& classStruct - detals on the class
 *              bool updating - whether this is an addition operation (false) or updating an existing class (true)
 * Returns: bool - whether operation was successful. If updating, then the type must already be registered. Otherwise, it must not
 */
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
/**
 * Method: TcTypeInterpretor::GetClass
 * Purpose: Retrieves an existing class
 * Parameters: const TString& className - the name of the class to seek
 *              TClassStruct& classStruct - holder of the class structure
 * Returns: bool - whether the class was actually found
 */
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

/**
 * Method: TcTypeInterpretor::PrepAsyncCall
 * Purpose: Returns an Async Variable that will run a copy of this function
 * Parameters: void
 * Returns: TrecPointer<TVariable> - The Asynce Variable that will run this function
 */
TrecPointer<TVariable> TcTypeInterpretor::PrepAsyncCall()
{
	return TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAsyncVariable>(GetCurrentThreadId(), 
		TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(this->Clone()));
}

/**
 * Method: TcTypeInterpretor::PrepAsyncCall
 * Purpose: Whether this interpretor is meant to run asynchronously
 * Parameters: void
 * Returns: bool
 */
bool TcTypeInterpretor::IsAsync()
{
	return isAsync;
}
