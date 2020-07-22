#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include "TString.h"

class TTypeMetadata;	// Used to hold metadata on various resources. Use for languages like Typescript decorators and Java annotations
class TAttribute;		// Used to hold detils about a class or structs attributes
class TProcedure;		// Used to hold procedures, such as functions

/**
 * enum class code_type_cat
 * Purpose: the category of type, meant to be used by compilers/interpretors to help
 *	organize types provided by a language/library/code whether the type is a primitive type, object,type, json, etc
 */
typedef enum class code_type_cat
{
	code_type_cat_prim,		// Primitive data types
	code_type_cat_object,	// Basic object
	code_type_cat_json		// Up in the air
}code_type_cat;

/**
 * class TType
 * Purpose: meant to hold information about a data-type - intended to be used by Anagame-based
 * compilers and interpretors keeping track of types provided by a langauge or a code file
 */
class TType :
	public TObject
{
public:

	/**
	 * Method: TType::TType
	 * Purpose: Constructor
	 * Parameters: TString& name - name of the type
	 * Returns: New TType object
	 */
	TType(TString& name);


	/**
	 * Method: TType::GetByteSize
	 * Purpose: Reports the number of bytes an instance of the type this TType specifies is expected to take up
	 * Parameters: void
	 * Returns:	int - the bytes an instance is expected to consume
	 */
	virtual int GetByteSize() = 0;


	/**
	 * Method: TType::GetTypeCategory
	 * Purpose: Reports the proper category of a given type
	 * Parameters: void
	 * Returns: code_type_cat - the category of the type 
	 */
	virtual code_type_cat GetTypeCategory() = 0;

	/**
	 * Method: TType::GetMetadata
	 * Purpose: Provided metadata on the type (provided to support language features such as Java Annotations
	 * Parameters: void
	 * Returns: TrecPointer<TTypeMetadata> - the metadata to provide
	 */
	virtual TrecPointer<TTypeMetadata> GetMetadata() = 0;

	/**
	 * Method: TType::GetAttributeByName
	 * Purpose: Provides the Attribute by name if possible
	 * Parameters: TString& - the name of the attribute
	 * Returns: TrecPointer<TAttribute> - the attribute to return
	 */
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&) = 0;

	/**
	 * Method: TType::GetProcedureByName
	 * Purpose: Provides the Procedure by name if possible
	 * Parameters: TString& - the name of the procedure
	 * Returns: TrecPointer<TProcedure> - the Procedure to return
	 */
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&) = 0;

	/**
	 * Method: TType::GetName
	 * Purpose: Reports the name of the type
	 * Parameters: void
	 * Returns: TString - name of the type
	 */
	virtual TString GetName();

protected:
	/**
	 * the name of the TType
	 */
	TString name;
};

