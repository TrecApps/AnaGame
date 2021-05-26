
#include "Parser_.h"

UCHAR Parser_Type[] = { 2, 0b10000000, 4 };

/**
 * Method: Parser_::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString Parser_::GetType()
{
	return TString(L"Parser_;") + TObject::GetType();
}



/*
* Method: Parser_::Parser_
* Purpose: Constructor
* Parameters: void
* Returns: void
*/
Parser_::Parser_()
{
}

/*
* Method: Parser_::~Parser_
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
Parser_::~Parser_()
{
}


/*
* Method: Parser_::GetAnaGameType
* Purpose: Retrieves the Version type
* Parameters: void
* Returns: UCHAR* - AnaGame representation of the objects type
*
* Note: deprecated
*/
UCHAR * Parser_::GetAnaGameType()
{
	return Parser_Type;
}
