
#include "TcType.h"

TcPrimitiveType::TcPrimitiveType(UCHAR size, primitive_type cat)
{
	this->size = size;
	this->type = type;
}

TcPrimitiveType::TcPrimitiveType()
{
	type = primitive_type::pt_int;
	size = 4;
}

TcPrimitiveType::TcPrimitiveType(const TcPrimitiveType& copy)
{
	size = copy.size;
	type = copy.type;
}

TcType::type_type TcPrimitiveType::GetTypeType()
{
	return type_type::t_primitive;
}

TcClassType::Attribute::Attribute()
{
	getAccessLevel = setAccessLevel = Protection::p_public;
	isStatic = false;
	mutability = Mutability::m_regular;
}

TcClassType::Attribute::Attribute(const Attribute& copy)
{
	getAccessLevel = copy.getAccessLevel;
	setAccessLevel = copy.setAccessLevel;
	isStatic = copy.isStatic;
	mutability = copy.mutability;
	type = copy.type;
	name.Set(copy.name);
	defaultValue = copy.defaultValue;
}

TcArrayType::TcArrayType()
{
	elementCount = 0;
}

TcArrayType::TcArrayType(const TcArrayType& copy)
{
	elementCount = copy.elementCount;
	type = copy.type;
}

TcType::type_type TcArrayType::GetTypeType()
{
	return type_type::t_array;
}

TcType::type_type TcClassType::GetTypeType()
{
	return type_type::t_class;
}
