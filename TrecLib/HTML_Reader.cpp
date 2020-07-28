
#include "HTML_Reader.h"

UCHAR HTML_ReaderType[] = { 3, 0b10000000, 3, 1 };

/*
* Method: HTML_Reader::HTML_Reader
* Purpose: Constructor
* Parameters: TrecPointer<TFile> tf - the TFile to use
*			TrecPointer<Parser_> par - the parser to call upon
* Returns: void
* Note: Could be depreciated - CArchive is unique to MFC and CArchive could be replaced with TFile
*/
HTML_Reader::HTML_Reader(TrecPointer<TFile> ta, TrecPointer<Parser_> p):ParseReader_(ta,p)
{
	charDeduced = false;
	openTaken = false;
	contentMode = false;
	contentSpace = false;
	contentS = TrecPointerKey::GetNewTrecPointer<TString>();
	usingWide = false;
}

/*
 * Method: HTML_Reader::~HTML_Reader
 * Purpose: Cleans up the parser
 * Parameters: void
 * Returns: void
 */
HTML_Reader::~HTML_Reader()
{
}

/*
* Method: HTML_Reader::read
* Purpose: Reads the file anticipating the HTML format
* Parameters: int* - the line number an error occurs
* Returns: bool - success
*/
bool HTML_Reader::read(int * i)
{
	if(!materials)
		return false;

	unsigned char char2[2];
	if (!tReader.Get())
		return false;
	tReader->Read(char2, 2);
	DeduceCharType(char2);

	if (!charDeduced)
	{
		i = 0;
		return false;
	}

	if (usingWide)
	{
		WCHAR w;
		memcpy(&w, char2, 2);
		TString t;
		if (!isWhiteSpace(w))
			t.AppendChar(w);
		//*i = 0;
		int ri = 0;
		while (tReader->Read(char2, 2) == 2)
		{
			memcpy(&w, char2, 2);
			if (!isWhiteSpace(w))
				t.AppendChar(w);
			else
			{
				ri++;
				*i = ri;
				if (!DeduceToken(t))
					return false;
				if (openTaken)
					t.Set(L'<');
				openTaken = false;
			}
		}
	}
	else
	{
		char charFir = char2[0];
		char charSec = char2[1];
		WCHAR w1 = ReturnWCharType(charFir);
		WCHAR w2 = ReturnWCharType(charSec);
		TString t;

		if (!isWhiteSpace(w1))
			t.AppendChar( w1);
		if (!isWhiteSpace(w2))
			t.AppendChar(w2);
		int ri = 0;
		while (tReader->Read(&charFir, 1) == 1)
		{
			w1 = ReturnWCharType(charFir);

			if (contentMode)
			{
				if (w1 == L'<')
				{
					// Time to end ContentMode
					endContentMode();
					t.Set(w1);
				}
				else
				{
					//if (!isWhiteSpace(w1))
					//	contentSpace = true;
					//if (contentSpace)
						contentS.Get()->AppendChar(w1);

				}
			}
			else
			{
				t.AppendChar(w1);
				if (w1 == L'>')
				{

					DeduceToken(t);
					contentMode = true;
				}

				
			}

			/**/
		}

	}
	return true;
}

/**
 * Method: HTML_Reader::GetAnaGameType
 * Purpose: Retieves the AnaGame type
 * Parameters: void
 * Returns: UCHAR* - the AnaGame type ID format
 *
 * Note: DEPRICATED
 */
UCHAR * HTML_Reader::GetAnaGameType()
{
	return HTML_ReaderType;
}

/*
* Method: HTML_Reader::DeduceCharType
* Purpose: Uses the first character to decude whether it is reading Unicode or ACSII
* Parameters: unsigned char* bytes - the bytes to analyze
* Returns: void
*/
void HTML_Reader::DeduceCharType(unsigned char * bytes)
{
	if (!bytes)
		return;
	WCHAR w_char;
	memcpy(&w_char, bytes, 2);
	char s_char = static_cast<char>(*bytes);

	if (w_char == L'<' || isWhiteSpace(w_char))
	{
		charDeduced = true;
		usingWide = true;
	}
	else if (s_char == ' ' || s_char == '<' || s_char == '\n' || s_char == '\t')
	{
		charDeduced = true;
		usingWide = false;
	}
	else
	{
		charDeduced = false;
	}
}

/*
* Method: HTML_Reader::ReturnWCharType
* Purpose: Converts a char to a WCHAR
* Parameters: char c - the char to convert
* Returns: WCHAR - the wide char version of c
* Note: Functionality is generic enough that method should be moved to a function
*/
WCHAR HTML_Reader::ReturnWCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}

/*
* Method: HTML_Reader::isWhiteSpace
* Purpose: Determines whether a character is white space or not
* Parameters: WCHAR c - the character to analyze
* Returns: bool - whether wchar is whitespace
*/
bool HTML_Reader::isWhiteSpace(WCHAR c)
{
	return c == L'\r' || c == L' ' || c == L'\n' || c == L'\t';
}

/*
* Method: HTML_Reader::DeduceToken
* Purpose: Attempts to determine what type of HTML tokens it was given
* Parameters: TString& t - the string detected
* Returns: bool - whether or not the string was processed
*/
bool HTML_Reader::DeduceToken(TString & t)
{
	if (!t.GetSize())
		return true;

	TrecPointer<TDataArray<TString>> tokens = t.split(L"<>");

	if (!tokens.Get())
		return false;

	// We expect the Size to at least be 1
	if (tokens->Size())
	{
		auto tokens2 = tokens->at(0).split(L"\"");
		if (tokens2.Get() && tokens2->Size())
		{
			if (!parseQuoteTokens(tokens2))
				return false;
		}
	}

	/*if (t[0] == L'<')
	{
		bool contentNeeded = false;
		TString content;
		int endToken = t.Find(L'>');
		if (endToken == t.GetLength() - 1 && t.Find(L'/') == t.GetLength() - 2)
		{
			if (t == TString(L"<b/>") || t == TString(L"<i/>") || t == TString(L"<p/>"))
				respond->Obj(&t);
			else
			{
				respond->goParent();
			}
			contentMode = true;
		}
		else if (endToken == t.GetLength() - 1)
		{
			contentMode = true;
			/*contentNeeded = true;
			content = t.SubString(endToken);

			if (usingWide)
			{
				WCHAR w_char = L'\0';
				while (reader->Read(&w_char, 2) == 2 && (w_char != L'<' || w_char != L'>'))
				{
					content += w_char;
				}
				if (w_char == L'<')
					openTaken = true;
			}//* /
		}
		else
			respond->goChild();
		respond->Obj(&t);


	}
	else if (t.Find(L">") == t.GetLength() - 1)
	{
		contentMode = true;
	}
	else if (t.Find(L'=') != -1)
	{
		TrecPointer<TArray<TString>> peices = t.split(TString(L"="));
		if (peices->Size() == 2)
		{
			respond->Attribute(peices->at(1), peices->at(0)->GetBuffer());
			peices->at(0)->GetBuffer();
		}
	}
	*/
	t.Set(L"");
	return true;
}


/*
* Method: HTML_Reader::SubmitToken
* Purpose: Deduces the token
* Parameters: TString t - the string to look into
* Returns: void
*/
void HTML_Reader::SubmitToken(TString t)
{
	DeduceToken(t);
}

/*
* Method: HTML_Reader::parseQuoteTokens
* Purpose: Manages tokens placed within Quotation marks
* Parameters: TrecPointer<TArray<TString>>& tokens - the tokens to parse
* Returns: bool - success result
*/
bool HTML_Reader::parseQuoteTokens(TrecPointer<TDataArray<TString>>& tokens)
{
	if(!tokens.Get() || !tokens->Size())
		return false;

	auto tokens2 = tokens->at(0).split(L" \n\t\r\v\f");
	respond->Obj(tokens2->at(0));
	




	if (tokens2->Size() > 1)
	{
		if (tokens->Size() > 1)
		{
			tokens2->at(tokens2->Size() - 1).Append(tokens->at(1));
			auto tokens3 = tokens2->at(1).split(L"=");
			if (tokens3->Size() > 1)
			{
				respond->Attribute(tokens3->at(1), tokens3->at(0));
				//tokens3->at(1)->ReleaseBuffer();
			}

			// To-Do: Implement support for quotation marks
			for (UINT c = 3; c < tokens->Size(); c += 2)
			{
				tokens2 = tokens->at(c-1).split(L" \n\t\r\v\f");
				tokens2->at(tokens2->Size() - 1).Append(tokens->at(c));
				tokens3 = tokens2->at(0).split(L"=");
				if (tokens3->Size() > 1)
				{
					respond->Attribute(tokens3->at(1), tokens3->at(0));
					//tokens3->at(1)->ReleaseBuffer();
				}
				else if (tokens3->Size() == 1)
				{
					respond->Obj(tokens3->at(0));
				}
			}
		}
		else
		{
			for (UINT c = 1; c < tokens2->Size(); c++)
			{
				auto tokens3 = tokens2->at(c).split(L"=");
				if (tokens3->Size() > 1)
				{
					respond->Attribute(tokens3->at(0), tokens3->at(1));
					//tokens3->at(1)->ReleaseBuffer();
				}
				else if (tokens3->Size() == 1)
				{
					respond->Obj(tokens3->at(0));
				}
			}
		}
	}

	return true;
}

/*
* Method: HTML_Reader::endContentMode
* Purpose: Signifies that the content between HTML tages is over
* Parameters: void
* Returns: void
*/
void HTML_Reader::endContentMode()
{
	TString cap(L"caption");
	respond->Attribute(contentS, cap);
	contentMode = false;
	contentSpace = false;
	contentS->Empty();
}
