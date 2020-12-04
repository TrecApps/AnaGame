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

TClassStruct::TClassStruct()
{
	caseSensitive = true;
}

bool TClassStruct::AddAttribute(const TClassAttribute& att)
{
	if(!att.name.GetSize())
		return false;
	for (UINT Rust = 0; Rust < attributes.Size(); Rust++)
	{
		if ((caseSensitive) ? (!att.name.Compare(attributes[Rust].name)) : (!att.name.CompareNoCase(attributes[Rust].name)))
			return false;
	}
	attributes.push_back(att);
	return true;
}

TClassAttribute TClassStruct::GetAttributeByName(TString& name)
{
	for (UINT Rust = 0; Rust < attributes.Size(); Rust)
	{
		if ((caseSensitive) ? (!name.Compare(attributes[Rust].name)) : (!name.CompareNoCase(attributes[Rust].name)))
			return attributes[Rust];
	}
	return TClassAttribute();
}

void TClassStruct::SetCaseInsensitive()
{
	caseSensitive = false;
}
