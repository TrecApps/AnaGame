
#include "SSGenerator.h"

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
	charDeduced = false;
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
	charDeduced = true;
	usingWide = true;
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
	{
		UCHAR bytes[2];
		Arch->Read(bytes, 2);

		DeduceCharType(bytes);
		ParseArchive();
	}
	else
		ParseString();
	return true;
}

/*
* Method: CSSGenerator - DeduceCharType
* Purpose: Deduces the type of character encoding between Unicode and char
* Parameters: unsigned char * bytes - the characters to analyze
* Returns: void
*/
void CSSGenerator::DeduceCharType(unsigned char * bytes)
{
	if (!bytes)
		return;
	WCHAR w_char;
	memcpy(&w_char, bytes, 2);
	char s_char = static_cast<char>(*bytes);

	if (w_char == L'<' || isWhiteSpace(w_char) || (w_char >= L'a' && w_char <= L'z') 
		|| (w_char >= L'A' && w_char <= L'Z'))
	{
		charDeduced = true;
		usingWide = true;
		BuildString(w_char);
	}
	else if (s_char == ' ' || s_char == '<' || s_char == '\n' || s_char == '\t'
		|| (s_char >= 'a' && s_char <= 'z')
		|| (s_char >= 'A' && s_char <= 'Z'))
	{
		charDeduced = true;
		usingWide = false;
		BuildString(ReturnWCharType(s_char));
	}
	else
	{
		charDeduced = false;
	}
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
	if (!charDeduced)
		return false;
	if (usingWide)
	{
		WCHAR w_char = L'\0';
		while (Arch->Read(&w_char, 2) == 2)
		{
			BuildString(w_char);
		}
	}
	else
	{
		char s_char = 0;
		WCHAR w_char = 0;
		while (Arch->Read(&s_char, 1))
		{
			w_char = ReturnWCharType(s_char);
			BuildString(w_char);
		}
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

