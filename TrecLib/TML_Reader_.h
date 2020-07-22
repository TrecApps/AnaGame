#pragma once
#include "ParseReader_.h"

/*
* Class: TML_Reader_
* Purpose: Reads TML Files
*/
class _TREC_LIB_DLL TML_Reader_ :
	public ParseReader_
{
public:
	/**
	 * Method: TML_Reader_::TML_Reader_
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TFile> tf - the TFile to use
	 *			TrecPointer<Parser_> par - the parser to call upon
	 * Returns: new TML Reader Object
	 */
	TML_Reader_(TrecPointer<TFile>, TrecPointer<Parser_>);
	/**
	 * Method: TML_Reader_::~TML_Reader_
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TML_Reader_();

	/**
	 * Method: TML_Reader_::read
	 * Purpose: Reads the file anticipating a TML format
	 * Parameters: int* - the line number an error occurs
	 * Returns: bool - success
	 */
	bool read(int*);

private:
	/**
	 * Method: TML_Reader_::ReadString
	 * Purpose: Reads a single line in the file
	 * Parameters: TString & line - the line to read
	 * Returns: bool - whether materials are set
	 */
	bool ReadString(TString& line);
};