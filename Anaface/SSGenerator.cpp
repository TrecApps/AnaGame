
#include "SSGenerator.h"



void ClearCSSComment(TString& text)
{
	while (true)
	{
		int beginLoc = text.Find(L"/*");
		int endLoc = text.Find(L"*/");

		if (beginLoc == -1 || endLoc == -1)
			return;
		if (endLoc < beginLoc)
			return;
		text.Delete(beginLoc, (endLoc + 2) - beginLoc);
	}
}

/*
* Method: (CSSGenerator) (Constructor) 
* Purpose: Sets up a CSSGenerator with a file to read
* Parameters: CArchive& a - the file to read
* Returns: void
*/
CSSGenerator::CSSGenerator(TFile& a)
{
	Arch = &a;
	styleList = TrecPointerKey::GetNewTrecPointer<TArray<styleTable>>();
}

/*
* Method: (CSSGenerator) (Constructor)
* Purpose: Sets up a CSSGenerator with a string to parse
* Parameters: TString t - the string to parse
* Returns: void
*/
CSSGenerator::CSSGenerator(TString t)
{
	Arch = nullptr;
	parsable = t;
	styleList = TrecPointerKey::GetNewTrecPointer<TArray<styleTable>>();
}

/*
* Method: (CSSGenerator) (Destructor)
* Purpose: Cleans up the Generator
* Parameters: void
* Returns: void
*/
CSSGenerator::~CSSGenerator()
{

}

/*
* Method: CSSGenerator - Parse
* Purpose: Initialites the Parsing process
* Parameters: void
* Returns: bool - true
*/
bool CSSGenerator::Parse()
{
	if (Arch)
		return ParseArchive();
	else
		return ParseString();

}


/*
* Method: CSSGenerator - ReturnWCharType
* Purpose: Retrieves the WCHAR version of an ACSII character
* Parameters: char c - the character to convert
* Returns: WCHAR - the resulting CHAR
*/
WCHAR CSSGenerator::ReturnWCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}

/*
* Method: CSSGenerator - isWhiteSpace
* Purpose: Reports whether a character is a whitespace character
* Parameters: WCHAR c - the character to test
* Returns: bool - whether or not character is whitespace
*/
bool CSSGenerator::isWhiteSpace(WCHAR c)
{
	return c == L' ' || c == L'\n' || c == L'\t';
}

/*
* Method: CSSGenerator - GetStyles
* Purpose: Retrieves the syle list retrieved from the CSS document
* Parameters: void
* Returns: TrecPointer<TArray<styleTable>> - the style list generated from the CSS document
*/
TrecPointer<TArray<styleTable>> CSSGenerator::GetStyles()
{
	return styleList;
}

/*
* Method: CSSGenerator - GetAnaGameType
* Purpose: Retireves the AnaGame type for the CSSGenerator
* Parameters: void
* Returns: UCHAR* - the AnaGame represenation for the CSSGenerator class
*/
UCHAR * CSSGenerator::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: CSSGenerator - ParseArchive
* Purpose: Builds a string if provided an archive
* Parameters: void
* Returns: bool - whether a string could have been built
*/
bool CSSGenerator::ParseArchive()
{
	if(!Arch)
		return false;
	while (Arch->ReadString(piece, L'}'))
	{
		if (!ParseGroup())
			return false;
	}


	return true;
}

/*
* Method: CSSGenerator - ParseString
* Purpose: Splits the CSS file into groups
* Parameters: void
* Returns: bool - whether there are groups
*/
bool CSSGenerator::ParseString()
{
	TrecPointer<TDataArray<TString>> groups = parsable.split(TString(L"}"));
	if (!groups.Get())
		return false;
	for (int c = 0; c < groups->Size(); c++)
	{
		piece = groups->at(c);
		ParseGroup();
	}
	return true;
}

/*
* Method: CSSGenerator - ParseGroup
* Purpose: Parses a series of styles within a single class/id/etc
* Parameters: void
* Returns: bool - whether the string provided can be parsed
*/
bool CSSGenerator::ParseGroup()
{
	ClearCSSComment(piece);
	TrecPointer<styleTable> st = TrecPointerKey::GetNewTrecPointer<styleTable>();

	

	TrecPointer<TDataArray<TString>> class_details = piece.split(TString(L"{"));
	if(!class_details.Get() || class_details->Size() != 2)
		return false;
	if (!class_details->at(1).GetSize() || !class_details->at(0).GetSize())
		return false;
	TrecPointer<TDataArray<TString>> details = class_details->at(1).split(TString(L";"));
	if (!details.Get() || !details->Size())
		return false;
	class_details->at(0).Trim();
	st->style = class_details->at(0);

	for (int c = 0; c < details->Size(); c++)
	{
		if (!details->at(c).GetSize())
			continue;
		TrecPointer<TDataArray<TString>> detail = details->at(c).split(TString(L":"));
		if (!detail.Get() || detail->Size() != 2)
			continue;
		if (!detail->at(0).GetSize() || !detail->at(1).GetSize())
			continue;
		TString t = detail->at(0);
		t.Trim();
		detail->at(1).Trim();
		st->names.addEntry(t, TrecPointerKey::GetNewTrecPointer<TString>(detail->at(1)));
		
	}

	styleList->Add(st);
	return true;
}

/*
* Method: CSSGenerator - BuildString
* Purpose: Adds String to parse later in the process
* Parameters: WCHAR w - the character to add until closing character is detected
* Returns: bool - true
*/
bool CSSGenerator::BuildString(WCHAR w)
{
	//WCHAR w_char = L'\0';
		
		if (w == L'}')
		{
			ParseGroup();
			piece.Empty();
		}
		else
		piece += w;	
	
	return true;
}

