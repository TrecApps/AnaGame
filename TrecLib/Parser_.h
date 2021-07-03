#pragma once
#include "TString.h"


/**
 * Class: Parser_
 * Purpose: Base class for parsing data into actual objects
 * 
 * SuperClass: TObject - allows it to be used by Environment Scripts
 */
class _TREC_LIB_DLL Parser_ : public TObject
{
public:


	/**
	 * Method: Parser_::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/*
	* Method: Parser_::Parser_
	* Purpose: Constructor
	* Parameters: void
	* Returns: void
	*/
	Parser_();
	/*
	* Method: Parser_::~Parser_
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~Parser_();

	// for the initial object type
	/*
	 * Method: Parser_::Obj
	 * Purpose: Takes in a String that represents an Object title
	 * Parameters: TString* v - the name of a class
	 * Returns: bool - false for the base class
	 * 
	 * Attributes: virtual
	 */
	virtual bool Obj(TString& v) = 0;
	// for the attribute name

	/*
	 * Method: Parser_::Attribute
	 * Purpose: Sets up attributes for the current object being analyzed
	 * Parameters: TString* v - the value to parse in an attribute
	 *			TString e - the attribute name
	 * Returns: bool - success result
	 * 
	 * Attributes: virtual
	 */
	virtual bool Attribute(TString& v, TString e)=0;


	/*
	* Method: Parser_
	* Purpose: Sets up attributes for the current object being analyzed
	* Parameters: TrecPointer<TString> v - the value to parse in an attribute
	*			TString& e - the attribute name
	* Returns: bool - success result
	 * 
	 * Attributes: virtual
	*/
	virtual bool Attribute(TrecPointer<TString> v, TString& e)=0;
	// for the attribute value (here it may be good that TStrings are used)


	/*
	 * Method: Parser_::submitType
	 * Purpose: For a parser to process the type it is parsing, incase the information is incompatible with its purpose
	 * Parameters: TString v - the Parse type to check
	 * Returns: bool - whether the parser type is the correct type
	 * 
	 * Attributes: virtual
	 */
	virtual bool submitType(TString v)=0;
	/**
	 * Method: Parser_::submitEdition
	 * Purpose: Version of the Parse type, to handle incompatabilities between version
	 * Parameters: TString v - the version string
	 * Returns: bool - whether or not a version is compatible
	 * 
	 * Attributes: virtual
	 */
	virtual bool submitEdition(TString v)=0;

	/**
	 * Method: Parser_::goChild
	 * Purpose: Objects submitted will be children of the current object
	 * Parameters: void
	 * Returns: bool
	 * 
	 * Attributes: virtual
	 */
	virtual bool goChild()=0;
	/**
	 * Method: Parser_::goParent
	 * Purpose: Go up a node in a parsed object tree
	 * Parameters: void
	 * Returns: void
	 * 
	 * Attributes: virtual
	 */
	virtual void goParent()=0;

	/*
	* Method: Parser_::GetAnaGameType
	* Purpose: Retrieves the Version type
	* Parameters: void
	* Returns: UCHAR* - AnaGame representation of the objects type
	*
	* Attributes: deprecated
	*/
	virtual UCHAR* GetAnaGameType();
};

