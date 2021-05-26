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
	ThreadLock();
	if (!att.name.GetSize())
	{
		ThreadRelease();
		return false;
	}
	for (UINT Rust = 0; Rust < attributes.Size(); Rust++)
	{
		if ((caseSensitive) ? (!att.name.Compare(attributes[Rust].name)) : (!att.name.CompareNoCase(attributes[Rust].name)))
		{
			bool ret = false;
			if (doOverride)
			{
				ret = true;
				attributes[Rust] = att;
			}
			ThreadRelease();
			return ret;
		}
	}
	attributes.push_back(att);
	ThreadRelease();
	return true;
}

TClassAttribute TClassStruct::GetAttributeByName(const TString& name)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < attributes.Size(); Rust++)
	{
		if ((caseSensitive) ? (!name.Compare(attributes[Rust].name)) : (!name.CompareNoCase(attributes[Rust].name)))
		{
			ThreadRelease();
			return attributes[Rust];
		}
	}
	ThreadRelease();
	return TClassAttribute();
}

bool TClassStruct::GetAttributeByIndex(UINT index, TClassAttribute& att)
{
	ThreadLock();
	if (index >= this->attributes.Size())
	{
		ThreadRelease();
		return false;
	}
	att = attributes[index];
	ThreadRelease();
	return true;
}

void TClassStruct::SetCaseInsensitive()
{
	ThreadLock();
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
	ThreadLock();
	auto ret = classType;
	ThreadRelease();
	return ret;
}

UINT TClassStruct::AddParentClass(const TString& className, tc_class_type cType)
{
	ThreadLock();
	for (UINT Rust = 0; Rust < parentClasses.Size(); Rust++)
	{
		if (!className.Compare(parentClasses[Rust]))
		{
			ThreadRelease();
			return 1;
		}
	}
	parentClasses.push_back(className);
	ThreadRelease();
	return 0;
}

bool TClassStruct::GetParentClass(UINT index, TString& className)
{
	ThreadLock();
	bool ret = false;
	if (index < parentClasses.Size())
	{
		className.Set(parentClasses[index]);
		ret = true;
	}
	ThreadRelease();
	return ret;
}
