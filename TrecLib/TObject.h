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
* Class TObject
* Serves as the base object for the AnaGame platform
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
	 */
	virtual TString getVariableValueStr(const TString& varName);
	/**
	 * Method: TObject::toString
	 * Purpose: Returns a string representation of this object
	 * Parameters: void
	 * Returns: TString - repreetnation of this object
	 */
	virtual TString toString();

protected:
	
};






/*
* Function: boolToString
* Purpose: returns string representations of the bool
* Parameters: bool val -  the bool to convert
* Returns: WCHAR* string representations
*/
 _TREC_LIB_DLL WCHAR* boolToString(bool);
