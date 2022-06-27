#pragma once
#include <TString.h>
#include <TVariable.h>
#include <TDataMap.h>


class TcType
{
public:
	/**
	 * since types fall into multiple categories, identify the types available
	 */
	typedef enum class type_type {
		t_primitive,	// type is a primitive
		t_class,		// Type is a class (or similar)
		t_array,		// Type is an array
		t_procedure		// Type i a procedure/function
	}type_type;

	virtual type_type GetTypeType() = 0;
};

class TcPrimitiveType :
	public TcType
{
public:
	typedef enum class primitive_type {
		pt_uint,
		pt_int,
		pt_float,
		pt_char,
		pt_bool
	} primitive_type;


	TcPrimitiveType(UCHAR size, primitive_type cat);
	TcPrimitiveType();
	TcPrimitiveType(const TcPrimitiveType& copy);

	virtual type_type GetTypeType() override;

	UCHAR size;
	primitive_type type;
};

class TcArrayType : public TcType
{
public:
	UINT elementCount;
	TrecPointer<TcType> type;
	TcArrayType();
	TcArrayType(const TcArrayType& copy);
	virtual type_type GetTypeType() override;
};

class TcClassType : public TcType
{
public:
	virtual type_type GetTypeType() override;

	typedef enum class Protection
	{
		p_public,			// Accessible from anywhere in the code
		p_class,			// Accessible from class code alone
		p_class_nested,		// Accessible from class and nested class code
		p_class_nested_sub,	// Accessible from class, nested class, and subclasses
		p_class_sub,		// Accessible from class and subclasses
		p_module,			// Accessible from all code within the same Module
		P_module_sub,		// Accessible from module and subclasses declared outside the module
	} Protection;

	typedef enum class Mutability
	{
		m_immutable,	// Must be assigned during construction
		m_regular,		// changeable if the parent object is changeable
		m_mutable		// changeable, even if the parent object is immutable
	}Mutability;


	class Attribute
	{
	public:
		TString name;							// Name of the Attribute
		TrecPointer<TcType> type;				// Type of the Attribute
		TrecPointer<TVariable> defaultValue;	// Default value to assign (if provided)
		Protection getAccessLevel, setAccessLevel;					// Access level (from where can the attribute be accessed)
		bool isStatic;
		Mutability mutability;

		Attribute();
		Attribute(const Attribute& copy);
	};

	TDataArray<Attribute> attributes;
	TDataMap<TDataArray<TrecPointer<TcType>>> parents;
};