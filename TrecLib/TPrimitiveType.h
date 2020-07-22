#pragma once
#include "TType.h"

#define PRIMITIVE_TYPE BYTE
#define PT_BOOLEAN		0
#define PT_INTEGER		1
#define PT_UNSIGNED_INT	2
#define PT_DECIMAL		3
#define PT_ACSII		4
#define PT_UNICODE		5

/**
 * class TPrimitiveType
 * Purpose: Provide data on a given type, focus is on Primitive type
 */
class TPrimitiveType :	public TType
{
public:
	/**
	 * Method: TPrimitiveType::TPrimitiveType
	 * Purpose: Sets up a Primitive Type
	 * Parameters: int size - size in byte needed to hold this type
     *              BYTE type - the type of primitive type this is
     *              TString& name - name of the type
	 * Returns: TPrimitiveType - structure representing  primitive type in a given language/environment
	 */
	TPrimitiveType(int size, BYTE type, TString& name);

	/**
	 * Method: TPrimitiveType::GetByteSize
     * Purpose: Reports the number of bytes this primitive type would take up in a program
     * Parameters: void
     * Returns: int - the number of bytes the primitive type would hold
     */
	virtual int GetByteSize();

	/**
     * Method: TPrimitiveType::GetTypeCategory
     * Purpose: Reports that this type represents a primitive type
     * Parameters: void
     * Returns: code_type_cat - code_type_cat_prim for this class
     */
	virtual code_type_cat GetTypeCategory();

	/**
     * Method: TPrimitiveType::GetType
     * Purpose: Returns what this type is supposed to represent
     * Parameters: void
     * Returns: BYTE - the Primitive type type in BYTE form
     */
	BYTE GetType();

	/**
     * Method: TPrimitiveType::GetMetadata
     * Purpose: blank implementation not used by Primitive types
     * Parameters: TString& - not used by this class
     * Returns: TrecPointer<TProcedure> - null pointer
     */
	virtual TrecPointer<TTypeMetadata> GetMetadata();

	/**
     * Method: TPrimitiveType::GetAttributeByName
     * Purpose: blank implementation not used by Primitive types
     * Parameters: TString& - not used by this class
     * Returns: TrecPointer<TAttribute> - null pointer
     */
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&);

    /**
     * Method: TPrimitiveType::GetProcedureByName
     * Purpose: blank implementation not used by Primitive types
     * Parameters: TString& - not used by this class
     * Returns: TrecPointer<TProcedure> - null pointer
     */
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&);

private:
	int size;
	BYTE type;
};

