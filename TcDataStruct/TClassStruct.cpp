#include "TClassStruct.h"

/**
 * Method: TClassAttribute::
 * Purpose: Constructor
 * Parameters: const TString& n - name of attribute
 *              const TString& t - type of attribute
 * Returns: new Attribute object
 */
TClassAttribute::TClassAttribute(const TString& n, const TString& t)
{
	name.Set(n);
	type.Set(t);
	access = other = 0;
}

/**
 * Method: TClassAttribute::TClassAttribute
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new Blank Atribute object
 */
TClassAttribute::TClassAttribute()
{
	access = other = 0;
}

/**
 * Method: TClassAttribute::TClassAttribute
 * Purpose: Copy Constructor
 * Parameters: const TClassAttribute& copy - original object to copy from
 * Returns: new Atribute object based off of existing one
 */
TClassAttribute::TClassAttribute(const TClassAttribute& copy)
{
	name.Set(copy.name);
	type.Set(copy.type);
	access = copy.access;
	other = copy.other;

	def = copy.def;
}

/**
 * Method: TClassStruct::TClassStruct
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Class blueprint
 */
TClassStruct::TClassStruct()
{
	caseSensitive = true;
	classType = tc_class_type::tct_class;
}

/**
 * Method: TClassStruct::AddAttribute
 * Purpose: Adds an attribute to the class
 * Parameters: const TClassAttribute& att - adds a new attribute to the
 *              bool doOverride = false - whether to replace an attribute if the name is already provided
 * Returns: bool - whether it was added or not
 */
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

/**
 * Method: TClassStruct::GetAttributeByName
 * Purpose: Returns the attribute by the name
 * Parameters: const TString& name
 * Returns: TClassAttribute - the attribute collected (check the name of this object to assess validity)
 */
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

/**
 * Method: TClassStruct::GetAttributeByIndex
 * Purpose: Gets an attribute by index of stored attributes
 * Parameters: UINT index - the location in the attributes repo
 *              TClassAttribute& att - the attribute to collect
 * Returns: bool - whether there was an attribute (if true, att should be valid)
 */
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

/**
 * Method: TClassStruct::SetCaseInsensitive
 * Purpose: Makes this class refer to attributes without regard to case
 * Parameters: void
 * Returns: void
 */
void TClassStruct::SetCaseInsensitive()
{
	ThreadLock();
	caseSensitive = false;
	ThreadRelease();
}

/**
 * Method: TClassStruct::GetVarGategory
 * Purpose: Reports to code with a TVariableType reference what class this actually is
 * Parameters: void
 * Returns: var_category - the type category (class version)
 */
var_category TClassStruct::GetVarGategory()
{
	return var_category::vc_class;
}

/**
 * Method: TClassStruct::SetClassType
 * Purpose: Sets the class type
 * Parameters: tc_class_type classType - the way to treat this class
 * Returns: void
 */
void TClassStruct::SetClassType(tc_class_type classType)
{
	this->classType = classType;
}

/**
 * Method: TClassStruct::GetClassType
 * Purpose: Reports the way to treat this class
 * Parameters: void
 * Returns: tc_class_type - the way to treat this class
 */
tc_class_type TClassStruct::GetClassType()
{
	ThreadLock();
	auto ret = classType;
	ThreadRelease();
	return ret;
}

/**
 * Method: TClassStruct::AddParentClass
 * Purpose: Adds the Parent Class
 * Parameters: const TString& className - the name of the parent class
 *              tc_class_type cType = tc_class_type::tct_class - not used
 * Returns: UINT - 0 if added, 1 if already present
 */
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

/**
 * Method: TClassStruct::GetParentClass
 * Purpose: Reports the Parent class stored at the present
 * Parameters: UINT index - the index of the parent class
 *              TString& className - holder of the class
 * Returns: bool - true if className holds the name of the parent class, false if index is out of bounds
 */
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
