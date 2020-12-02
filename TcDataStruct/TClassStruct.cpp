#include "TClassStruct.h"

TClassAttribute::TClassAttribute(const TString& n, const TString& t)
{
	name.Set(n);
	type.Set(t);
	access = other = 0;
}

TClassAttribute::TClassAttribute()
{
	access = other = 0;
}

TClassAttribute::TClassAttribute(const TClassAttribute& copy)
{
	name.Set(copy.name);
	type.Set(copy.type);
	access = copy.access;
	other = copy.other;

	def = copy.def;
}
