#include "TType.h"

TType::TType(TString& name)
{
	this->name.Set(name);
}

TString TType::GetName()
{
	return name;
}
