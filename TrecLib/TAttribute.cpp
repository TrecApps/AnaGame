#include "TAttribute.h"

TAttribute::TAttribute(TString& name):TType(name)
{
}

TrecPointer<TAttribute> TAttribute::GetAttributeByName(TString&)
{
	return TrecPointer<TAttribute>();
}

TrecPointer<TProcedure> TAttribute::GetProcedureByName(TString&)
{
	return TrecPointer<TProcedure>();
}
