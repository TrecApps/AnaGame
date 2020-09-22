#pragma once
#include <Windows.h>
#include <new>
#include <memory>
#include "TrecLib.h"

#include "AnagameDef.h"


extern UCHAR TArrayType[];
extern UCHAR TDataArrayType[];
extern UCHAR TMapType[];

class TString;

/*
* Class: TObject
* Purpose: Serves as the base object for the AnaGame platform
*/
class _TREC_LIB_DLL TObject 
{
public:
	/*
	* Method: TObject::TObject
	* Purpose: Default constructor for the TObject
	* Parameters: void
	* Returns: void
	*/
	TObject();
	/*
	* Method: TObject::~TObject
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TObject();

	/*
	* Method: TObject::GetAnaGameType
	* Purpose: Retrieves the AnaGame type
	* Parameters: void
	* Returns: UCHAR* - the AnaGame type represenation
	*
	* Note: DEPRICATED
	*/
	virtual UCHAR* GetAnaGameType();
	/**
	 * Method: TObject::getVariableValueStr
	 * Purpose: Returns the String value of a TObjects given variable, meant for databinding
	 * Parameters: TString& varName - the name of the variable to seek
	 * Returns: TString - the variable value in String form, or an empty string if variable does not exist
	 * 
	 * Attributes: virtual
	 */
	virtual TString getVariableValueStr(const TString& varName);
	/**
	 * Method: TObject::toString
	 * Purpose: Returns a string representation of this object
	 * Parameters: void
	 * Returns: TString - repreetnation of this object
	 * 
	 * Attributes: virtual
	 */
	virtual TString toString();

	/**
	 * Method: TObject::GetType()
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 * 
	 * Note: This method is provided to allow interpretors to allow scripts to access methods of Anagame Objects.
	 *   subclasses should report their type first, then the parent clss type and seperate it with a SemiColon
	 * 
	 * Attributes: virtual
	 */
	virtual TString GetType();

protected:
	
};






/*
* Function: boolToString
* Purpose: returns string representations of the bool
* Parameters: bool val -  the bool to convert
* Returns: WCHAR* string representations
*/
 _TREC_LIB_DLL WCHAR* boolToString(bool);
