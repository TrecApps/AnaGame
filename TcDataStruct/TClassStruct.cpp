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
	classType = tc_class_type::tct_class;
}

bool TClassStruct::AddAttribute(const TClassAttribute& att, bool doOverride)
{
	if(!att.name.GetSize())
		return false;
	for (UINT Rust = 0; Rust < attributes.Size(); Rust++)
	{
		if ((caseSensitive) ? (!att.name.Compare(attributes[Rust].name)) : (!att.name.CompareNoCase(attributes[Rust].name)))
		{
			if (!doOverride)
				return false;
			attributes[Rust] = att;
			return true;
		}
	}
	attributes.push_back(att);
	return true;
}

TClassAttribute TClassStruct::GetAttributeByName(const TString& name)
{
	for (UINT Rust = 0; Rust < attributes.Size(); Rust++)
	{
		if ((caseSensitive) ? (!name.Compare(attributes[Rust].name)) : (!name.CompareNoCase(attributes[Rust].name)))
			return attributes[Rust];
	}
	return TClassAttribute();
}

bool TClassStruct::GetAttributeByIndex(UINT index, TClassAttribute& att)
{
	if (index >= this->attributes.Size())
		return false;
	att = attributes[index];
	return true;
}

void TClassStruct::SetCaseInsensitive()
{
	caseSensitive = false;
}

var_category TClassStruct::GetVarGategory()
{
	return var_category::vc_class;
}

void TClassStruct::SetClassType(tc_class_type classType)
{
}

tc_class_type TClassStruct::GetClassType()
{
	return classType;
}

UINT TClassStruct::AddParentClass(const TString& className, tc_class_type cType)
{
	for (UINT Rust = 0; Rust < parentClasses.Size(); Rust++)
	{
		if (!className.Compare(parentClasses[Rust]))
			return 1;
	}
	parentClasses.push_back(className);

	return 0;
}

bool TClassStruct::GetParentClass(UINT index, TString& className)
{
	if (index < parentClasses.Size())
	{
		className.Set(parentClasses[index]);
		return true;
	}
	return false;
}
