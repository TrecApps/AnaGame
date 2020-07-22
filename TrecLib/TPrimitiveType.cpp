#include "TPrimitiveType.h"

/**
 * Method: TPrimitiveType::TPrimitiveType
 * Purpose: Sets up a Primitive Type
 * Parameters: int size - size in byte needed to hold this type
 *              BYTE type - the type of primitive type this is
 *              TString& name - name of the type
 * Returns: TPrimitiveType - structure representing  primitive type in a given language/environment
 */
TPrimitiveType::TPrimitiveType(int size, BYTE type, TString& name): TType(name)
{
	this->size = size;
	this->type = type;
}

/**
 * Method: TPrimitiveType::GetByteSize
 * Purpose: Reports the number of bytes this primitive type would take up in a program
 * Parameters: void
 * Returns: int - the number of bytes the primitive type would hold
 */
int TPrimitiveType::GetByteSize()
{
	return size;
}

/**
 * Method: TPrimitiveType::GetTypeCategory
 * Purpose: Reports that this type represents a primitive type
 * Parameters: void
 * Returns: code_type_cat - code_type_cat_prim for this class
 */
code_type_cat TPrimitiveType::GetTypeCategory()
{
	return code_type_cat::code_type_cat_prim;
}

/**
 * Method: TPrimitiveType::GetType
 * Purpose: Returns what this type is supposed to represent
 * Parameters: void
 * Returns: BYTE - the Primitive type type in BYTE form
 */
BYTE TPrimitiveType::GetType()
{
	return type;
}

/**
 * Method: TPrimitiveType::GetMetadata
 * Purpose: blank implementation not used by Primitive types
 * Parameters: TString& - not used by this class
 * Returns: TrecPointer<TProcedure> - null pointer
 */
TrecPointer<TTypeMetadata> TPrimitiveType::GetMetadata()
{
	return TrecPointer<TTypeMetadata>();
}

/**
 * Method: TPrimitiveType::GetAttributeByName
 * Purpose: blank implementation not used by Primitive types
 * Parameters: TString& - not used by this class
 * Returns: TrecPointer<TAttribute> - null pointer
 */
TrecPointer<TAttribute> TPrimitiveType::GetAttributeByName(TString&)
{
	return TrecPointer<TAttribute>();
}

/**
 * Method: TPrimitiveType::GetProcedureByName
 * Purpose: blank implementation not used by Primitive types
 * Parameters: TString& - not used by this class
 * Returns: TrecPointer<TProcedure> - null pointer
 */
TrecPointer<TProcedure> TPrimitiveType::GetProcedureByName(TString&)
{
	return TrecPointer<TProcedure>();
}
