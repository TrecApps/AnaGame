#pragma once
#include "TType.h"
#include "TMap.h"
#include "TTypeMetadata.h"

/**
 * Method: TAttribute::
 * Purpose:
 * Parameters:
 * Return:
 */


/**
 * class TAttribute
 * Purpose: Provides base level support for managing a class/object attribute, written with Anagame
 *	compilers in minds
 */
class TAttribute :
	public TType
{
public:
	/**
	 * Method: TAttribute::TAttribute
	 * Purpose: Sets up the attribute
	 * Parameters: TSTring& name - the name of the attribute
	 * Return: Object of type TAttribute
	 */
	TAttribute(TString& name);

	/**
     * Method: TAttribute::GetDetails
     * Purpose: The basic attributes about this type (such as permissions, static, mutability, and language specific features
     * Parameters: void
     * Return: short - token representing attributes of the attribute
     */
	virtual short GetDetails() = 0;

	/**
     * Method: TAttribute::GetType
     * Purpose: Returns the type of the attribute
     * Parameters: void
     * Return: TrecPointer<TType> -  the type involved
     */
	virtual TrecPointer<TType> GetType() = 0;

	/**
     * Method: TAttribute::GetAttributeByName
     * Purpose: Returns the Attribute by name
     * Parameters: TString& - the name of the attribute
     * Return: TrecPointer<TAttribute> - the attribute requested 
     */
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&)override;

    /**
     * Method: TAttribute::GetProcedureByName
     * Purpose: Returns the Procedure held by the class/object
     * Parameters: TString& the name of the procedure
     * Return: TrecPointer<TProcedure> -  the procedure requested
     */
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&)override;

protected:
	TMap<TTypeMetadata> metadata;
};

