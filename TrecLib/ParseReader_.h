#pragma once
#include "Parser_.h"
#include "TObject.h"
#include "TFile.h"

/**
 * Class: ParseReader_
 * Purpose: Base Class for all Markup-language parsers
 * 
 * SuperClass: TObject - allows it to be used by Environment Scripts
 */
class _TREC_LIB_DLL ParseReader_ : public TObject
{
public:

	/*
	* Method: ParseReader_::ParseReader_
	* Purpose: Constructor
	* Parameters: TrecPointer<TFile> tf - the TFile to use
	*			TrecPointer<Parser_> par - the parser to call upon
	* Returns: void
	*/
	ParseReader_(TrecPointer<TFile>, TrecPointer<Parser_>);

	/*
	* Method: ParseReader_::~ParseReader_
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~ParseReader_();

	/*
	* Method: ParseReader_::read
	* Purpose: Reads the file in a predefined format
	* Parameters: int* - the line number an error occurs
	* Returns: bool - success
	* 
	* Attributes: virtual
	*/
	virtual bool read(int*);

	/*
	* Method: ParseReader_::GetAnaGameType
	* Purpose: Retieves the AnaGame type
	* Parameters: void
	* Returns: UCHAR* - the AnaGame type ID format
	*
	* Attribtues: deprecated
	*/
	virtual UCHAR* GetAnaGameType();


	/**
	 * Method: ParseReader_::GetType()
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

protected:

	/**
	 * File to read from
	 */
	TrecPointer<TFile> tReader;
	/**
	 * Object to document the resources the reader collects
	 */
	TrecPointer<Parser_> respond;

	/**
	 * whether the program has both of the resources needed to do it's job
	 */
	bool materials;
	/**
	 * deprecated - used when Reader was still able to use MFC's CArchive
	 */
	bool usingTFile;
};