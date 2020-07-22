#pragma once
#include "ParseReader_.h"


/*
* Class: HTML_Reader
* Parser Reader optimized for HTML files
*/
class _TREC_LIB_DLL HTML_Reader :	public ParseReader_
{
	friend class HTMLParser;
public:

	/*
	* Method: HTML_Reader::HTML_Reader
	* Purpose: Constructor
	* Parameters: TrecPointer<TFile> tf - the TFile to use
	 *			TrecPointer<Parser_> par - the parser to call upon
	* Returns: void
	* Note: Could be depreciated - CArchive is unique to MFC and CArchive could be replaced with TFile
	*/
	HTML_Reader(TrecPointer<TFile>, TrecPointer<Parser_>);

	/**
	 * Method: HTML_Reader::~HTML_Reader
	 * Purpose: Cleans up the parser
	 * Parameters: void
	 * Returns: void
	 */
	~HTML_Reader();

	/*
	* Method: HTML_Reader::read
	* Purpose: Reads the file anticipating the HTML format
	* Parameters: int* - the line number an error occurs
	* Returns: bool - success
	*/
	bool read(int*)override;

	/**
	 * Method: HTML_Reader::GetAnaGameType
	 * Purpose: Retieves the AnaGame type
	 * Parameters: void
	 * Returns: UCHAR* - the AnaGame type ID format
	 *
	 * Note: DEPRICATED
	 */
	virtual UCHAR* GetAnaGameType();

private:
	/**
	 * Whether a character was determined
	 */
	bool charDeduced;
	/**
	 * Whether the file read is using wide characters
	 */
	bool usingWide;

	/*
	* Method: HTML_Reader::DeduceCharType
	* Purpose: Uses the first character to decude whether it is reading Unicode or ACSII
	* Parameters: unsigned char* bytes - the bytes to analyze
	* Returns: void
	*/
	void DeduceCharType(unsigned char*);
	/*
	* Method: HTML_Reader::ReturnWCharType
	* Purpose: Converts a char to a WCHAR
	* Parameters: char c - the char to convert
	* Returns: WCHAR - the wide char version of c
	* Note: Functionality is generic enough that method should be moved to a function
	*/
	WCHAR ReturnWCharType(char);
	/*
	* Method: HTML_Reader::isWhiteSpace
	* Purpose: Determines whether a character is white space or not
	* Parameters: WCHAR c - the character to analyze
	* Returns: bool - whether wchar is whitespace
	*/
	bool isWhiteSpace(WCHAR);
	/*
	* Method: HTML_Reader::DeduceToken
	* Purpose: Attempts to determine what type of HTML tokens it was given
	* Parameters: TString& t - the string detected
	* Returns: bool - whether or not the string was processed
	*/
	bool DeduceToken(TString& t);
	bool openTaken;
	/*
	* Method: HTML_Reader::SubmitToken
	* Purpose: Deduces the token
	* Parameters: TString t - the string to look into
	* Returns: void
	*/
	void SubmitToken(TString);

	/*
	* Method: HTML_Reader::parseQuoteTokens
	* Purpose: Manages tokens placed within Quotation marks
	* Parameters: TrecPointer<TArray<TString>>& tokens - the tokens to parse
	* Returns: bool - success result
	*/
	bool parseQuoteTokens(TrecPointer<TDataArray<TString>>& tokens);

	/*
	* Method: HTML_Reader::endContentMode
	* Purpose: Signifies that the content between HTML tages is over
	* Parameters: void
	* Returns: void
	*/
	void endContentMode();

	/**
	 * whether the string being built is for content or not
	 */
	bool contentMode;

	/**
	 * 
	 */
	bool contentSpace;
	/**
	 * the string to hold content in
	 */
	TrecPointer<TString> contentS;
};

