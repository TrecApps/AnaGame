
#include "Parser_.h"

UCHAR Parser_Type[] = { 2, 0b10000000, 4 };

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
* Method: Parser_::Obj
* Purpose: Takes in a String that represents an Object title
* Parameters: TString* v - the name of a class
* Returns: bool - false for the base class
*/
bool Parser_::Obj(TString* v)
{
	return false;
}

/*
* Method: Parser_::Attribute
* Purpose: Sets up attributes for the current object being analyzed
* Parameters: TString* v - the value to parse in an attribute
*			TString e - the attribute name
* Returns: bool - success result
*/
bool Parser_::Attribute(TString* v, TString e)
{
	return false;
}

/*
* Method: Parser_
* Purpose: Sets up attributes for the current object being analyzed
* Parameters: TrecPointer<TString> v - the value to parse in an attribute
*			TString& e - the attribute name
* Returns: bool - success result
*/
bool Parser_::Attribute(TrecPointer<TString> v, TString& e)
{
	return false;
}

/*
* Method: Parser_::submitType
* Purpose: For a parser to process the type it is parsing, incase the information is incompatible with its purpose
* Parameters: TString v - the Parse type to check
* Returns: bool - whether the parser type is the correct type
*/
bool Parser_::submitType(TString v)
{
	return false;
}

/*
* Method: Parser_::submitEdition
* Purpose: Version of the Parse type, to handle incompatabilities between version
* Parameters: TString v - the version string
* Returns: bool - whether or not a version is compatible
*/
bool Parser_::submitEdition(TString v)
{
	return false;
}

/*
* Method: Parser_::goChild
* Purpose: Objects submitted will be children of the current object
* Parameters: void
* Returns: bool
*/
bool Parser_::goChild()
{
	return false;
}

/*
* Method: Parser_::goParent
* Purpose: Go up a node in a parsed object tree
* Parameters: void
* Returns: void
*/
void Parser_::goParent()
{
}

/*
* Method: Parser_::GetAnaGameType
* Purpose: Retrieves the Version type
* Parameters: void
* Returns: UCHAR* - AnaGame representation of the objects type
*
* Note: DEPRICATED
*/
UCHAR * Parser_::GetAnaGameType()
{
	return Parser_Type;
}
