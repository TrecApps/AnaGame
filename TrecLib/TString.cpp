#include "TString.h"
#include "TDataArray.h"
#include <cassert>

UCHAR TStringType[] = { 2, 0b10000000, 1 };

static TDataArray<WCHAR> whiteChar;


void fillWhiteChar()
{
	if (whiteChar.Size())
		return;

	whiteChar.push_back(L' ');
	// whiteChar.push_back(L'\s');
	whiteChar.push_back(L'\n');
	whiteChar.push_back(L'\r');
	whiteChar.push_back(L'\t');

}


/*
* Method: TString::Constructor
* Purpose: Default Constructor for the TString Class
* Parameters: void
* Returns: void
*/

TString::TString()
{
	string = nullptr;
	Empty();
	fillWhiteChar();
}


bool ConvertTStringToBinary(const TString& string, UINT& val)
{
	if (string.GetSize() > 32)
		return false;
	UINT tempVal = 0;
	for (UINT Rust = string.GetSize() - 1, mark = 0x00000001; Rust < string.GetSize() && mark; Rust--, mark = mark << 1)
	{
		WCHAR ch = string.GetAt(Rust);
		switch (ch)
		{
		case L'0':
			break;
		case L'1':
			tempVal = tempVal | mark;
			break;
		default:
			return false;
		}
	}
	val = tempVal;
	return true;
}

bool ConvertTStringToOctal(const TString& string, UINT& val)
{
	UINT stringSize = string.GetSize();
	
	// Max number is 37777777777.
	if (stringSize > 11)
		return false;
	if (stringSize == 11)
	{
		WCHAR ch = string.GetAt(0);
		if (ch > L'3' || ch < L'0')
			return false;
	}

	UINT tempVal = 0, shift = 0;

	for (UINT Rust = string.GetSize() - 1; Rust < stringSize; Rust--, shift += 3)
	{
		WCHAR ch = string.GetAt(Rust);
		if (ch > L'7' || ch < L'0')
			return false;
		UINT charVal = ch - L'0';

		tempVal += (charVal << shift);
	}
	val = tempVal;
	return true;
}

bool ConvertTStringToHex(const TString& string, UINT& val)
{
	if (string.GetSize() > 8)
		return false;
	UINT tempVal = 0, shift = 0;

	for (UINT Rust = string.GetSize() - 1; Rust < string.GetSize(); Rust--, shift += 4)
	{
		WCHAR ch = string.GetAt(Rust);
		UINT charVal = 0;

		if (ch <= L'9' && ch >= L'0')
			charVal = ch - L'0';
		else if (ch <= L'f' && ch >= L'a')
			charVal = ch - L'a' + 10;
		else
			return false;

		tempVal += (charVal << shift);
	}
	val = tempVal;
	return true;
}

/**
 * Method: TString::ConvertStringToUint
 * Purpose: Allows TString class to convert itself into a numeral representation
 * Parameters: const TString& string - the string to convert
 *				UINT& num - the number to hold the data in
 *				number_base base - the base to assume
 * Returns: bool - whether the string could be converted into an int
 *
 * Note: if the String starts with a number specifier (0x or 0b), then the base param must
 *		match the specifier or be 'generic'. Otherwise it will fail and false is returned.
 *		Also, if 'generic' is set and string has no specifier, then decimal will be considered followed by hex.
 *		Finally, if you dont want octal, then add a binary/hex specifier tot he string, remove all leading zeros, or
 *		don't use 'generic' as the base parameter
 */
bool TString::ConvertStringToUint(const TString& string, UINT& num, number_base base)
{
	number_base assumeBase = number_base::nb_generic;
	
	// Easier to just deal with lowercase than worry about both lower and upper case
	TString tempString(string.GetLower().GetTrim());
	if (tempString.StartsWith(L"0x", true) || tempString.StartsWith(L"x", true))
	{
		assumeBase = number_base::nb_hexadecimal;
		tempString.Delete(0, tempString.Find(L'x') + 1);
	}
	else if (tempString.StartsWith(L"0b", true) || tempString.StartsWith(L"b", true))
	{
		assumeBase = number_base::nb_binary;
		tempString.Delete(0, tempString.Find(L'b') + 1);
	}
	else if (tempString.StartsWith(L'0'))
	{
		assumeBase = number_base::nb_octal;
	}
	

	// Now make sure that there is no inconsistancy in what the user is asking for and what the string provides
	bool works = false;
	switch (base)
	{
	case number_base::nb_generic:	// User has no preference, use the assumed
		works = true;
		break;
	case number_base::nb_binary:	// User has a preference. Make sure our assumption is generic or octal or matches the user preference
	case number_base::nb_decimal:
	case number_base::nb_hexadecimal:
	case number_base::nb_octal:
		if (assumeBase == number_base::nb_generic || assumeBase == number_base::nb_octal)
			assumeBase = base;
		works = (assumeBase == base);
	}

	if (!works)
		return false;
	int iNum = 0;

	switch (assumeBase)
	{
	case number_base::nb_binary:
		works = ConvertTStringToBinary(tempString, num);
		break;
	case number_base::nb_decimal:
		works = (tempString.ConvertToInt(iNum) == 0);
		if (works)
			num = static_cast<UINT>(iNum);
		break;
	case number_base::nb_hexadecimal:
		works = ConvertTStringToHex(tempString, num);
		break;
	case number_base::nb_octal:
		works = ConvertTStringToOctal(tempString, num);
		break;
	default: // i.e. Generic
		works = (tempString.ConvertToInt(iNum) == 0);
		if (works)
			num = static_cast<UINT>(iNum);
		else
			works = ConvertTStringToHex(tempString, num);
	}

	return works;
}

/**
 * Method: TString::GetType
 * Purpose: returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */

TString TString::GetType()
{
	return TString(L"TString;") + TObject::GetType();
}

/*
* Method: TString::Destructor
* Purpose: Cleans up the TString
* Parameters: void
* Returns: void
*/
TString::~TString()
{
	TObjectLocker threadLock(&thread);
	if (string)
		delete[] string;
	return;
}

TString::TString(UINT num)
{
	this->capacity = this->size = 0;
	this->string = nullptr;
	Format(L"%d", num);
}

TString::TString(float num)
{
	this->capacity = this->size = 0;
	this->string = nullptr;
	Format(L"%f", num);
}


/*
* Method: TString::Constructor 
* Purpose: Uses a pointer to another TString to build this one
* Parameters: TString* orig - pointer to TString to copy
* Returns: void
*/
TString::TString(const TString * orig)
{
	fillWhiteChar();
	if (!orig)
	{
		size = capacity = 0;
		string = nullptr;
		return;
	}

	capacity = orig->capacity;
	size = orig->size;

	string = new WCHAR[capacity];
	memcpy(string, (orig->string), capacity * sizeof(WCHAR));
	string[size] = L'\0';
}

/*
* Method: TString::Constructor 
* Purpose: Uses a C-String  to construct TString
* Parameters: const char* cps - C-String to copy
* Returns: void
*/
TString::TString(const char* cps)
{
	fillWhiteChar();
	if(!cps)
	{
		string = nullptr;
		Empty();
		return;
	}

	capacity = strlen(cps) + 1;
	string = new WCHAR[capacity];
	size = capacity - 1;

	MultiByteToWideChar(CP_ACP, 0, cps, size, string, size);
	string[capacity - 1] = L'\0';
}

/*
* Method: TString::Constructor 
* Purpose: Builds a TString off of a wide char array
* Parameters: const WCHAR* wcps - Wide Character array to copy
* Returns: void 
*/
TString::TString(const WCHAR * wcps)
{
	fillWhiteChar();
	if (!wcps)
	{
		string = nullptr;
		Empty();
		return;
	}

	size = lstrlenW(wcps);
	capacity = size + 1;
	string = new WCHAR[capacity];
	memcpy(string, wcps, size * sizeof(WCHAR));
	string[capacity - 1] = L'\0';
}


/*
* Method: TString::Constructor
* Purpose: Uses a reference to another TString to build this one
* Parameters: TString* orig - reference to TString to copy
* Returns: void
*/
TString::TString(const TString & c)
{
	fillWhiteChar();
	size = c.size;
	capacity = c.capacity;

	string = new WCHAR[capacity];
	memcpy(string, c.string, capacity * sizeof(WCHAR));
	string[capacity - 1] = L'\0';
}

/*
* Method: TString::Constructor
* Purpose: Uses a reference to an existing C++ String to build the TString
* Parameters: CString* cps - reference to C++ String to copy
* Returns: void
*/
TString::TString(std::string & str)
{
	fillWhiteChar();

	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	int e = 0;
	if (!sizeNeeded)
	{
		e = GetLastError();
	}

	assert(sizeNeeded > 0);

	capacity = sizeNeeded + 1;

	string = new WCHAR[capacity];

	string[sizeNeeded] = string[sizeNeeded - 1] = '\0';

	size = str.size();

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, string, capacity);
}

/*
* Method: TString::Constructor
* Purpose: Uses a single WCHAR to build the TString
* Parameters: WCHAR c - the Character to set the TString equal to
* Returns: void
*/
TString::TString(WCHAR c)
{
	fillWhiteChar();
	size = 1;
	capacity = 5;
	string = new WCHAR[capacity];
	memset(string, 0, capacity * sizeof(WCHAR));
	string[0] = c;
}

/*
* Method: TString::ConvertToInt
* Purpose: Converts a Compliant TString into an integer
* Parameters: int* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToInt(int& value)const
{
	TObjectLocker threadLock(&thread);
	if (!size)
	{
		return NOT_NUMB;
	}int temp = value;

	value = 0;
	int hold;
	bool positive = true;

	for (UINT c = 0; c < size; c++)
	{
		if (convertToNumber(this->GetAt(c), &hold))
		{
			value = (value * 10) + hold;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			value = temp;
			return NOT_NUMB;
		}
	}
	if (positive)
		value = abs(value);
	else
		value = -(abs(value));
	return T_NO_ERROR;
}

/*
* Method: TString::ConvertToInt
* Purpose: Converts a Compliant TString into an integer
* Parameters: long* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToInt(long& value)const
{
	TObjectLocker threadLock(&thread);
	if (!size)
	{
		return NOT_NUMB;
	}
	int temp = value;

	value = 0;
	int hold;
	bool positive = true;

	for (UINT c = 0; c < size; c++)
	{
		if (convertToNumber(this->GetAt(c), &hold))
		{
			value = (value * 10) + hold;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			value = temp;
			return NOT_NUMB;
		}
	}
	if (positive)
		value = abs(value);
	else
		value = -(abs(value));
	return T_NO_ERROR;
}

/*
* Method: TString::ConvertToLong
* Purpose: Converts a Compliant TString into a long integer
* Parameters: long* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToLong(long long& value)const
{
	TObjectLocker threadLock(&thread);
	if (!size)
	{
		return NOT_NUMB;
	}
	long long temp = value;

	value = 0;
	int hold;

	bool positive = true;

	for (UINT c = 0; c < size; c++)
	{
		if (convertToNumber(this->GetAt(c), &hold))
		{
			value = (value * 10) + hold;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			value = temp;
			return NOT_NUMB;
		}
	}
	if (positive)
		value = abs(value);
	else
		value = -(abs(value));
	return T_NO_ERROR;

}

/*
* Method: TString::ConvertToDouble
* Purpose: Converts a Compliant TString into a double floating point number
* Parameters: double* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToDouble(double& value)const
{
	TObjectLocker threadLock(&thread);
	if (!size)
	{
		return NOT_NUMB;
	}
	double temp = value;

	value = 0;
	double dec = 0.1;
	bool fullInt = true;
	int hold;

	bool positive = true;

	for (UINT c = 0; c < size; c++)
	{
		if (fullInt && convertToNumber(this->GetAt(c), &hold))
		{
			value = (value * 10) + hold;
		}
		else if (convertToNumber(this->GetAt(c), &hold)) // moved to decimal portion
		{
			value = value + (double)hold * dec;
			dec = dec / 10;
		}
		else if (this->GetAt(c) == L'.')
		{
			fullInt = false;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			value = temp;
			return NOT_NUMB;
		}

	}
	if (positive)
		value = abs(value);
	else
		value = -(abs(value));

	return T_NO_ERROR;
}

/*
* Method: TString::ConvertToFloat
* Purpose: Converts a Compliant TString into a float
* Parameters: float* value - the value to store
* Returns: short - 0 if successful, error code otherwise
*/
short TString::ConvertToFloat(float& value)const
{
	TObjectLocker threadLock(&thread);
	if (!size)
	{
		return NOT_NUMB;
	}
	float temp = value;

	value = 0;
	float dec = 0.1f;
	bool fullInt = true;
	int hold;
	bool positive = true;

	for (UINT c = 0; c < size; c++)
	{
		if (fullInt && convertToNumber(this->GetAt(c), &hold))
		{
			value = (value * 10) + hold;
		}
		else if (convertToNumber(this->GetAt(c), &hold)) // moved to decimal portion
		{
			value = value + (float)hold * dec;
			dec = dec / 10;
		}
		else if (this->GetAt(c) == L'.')
		{
			fullInt = false;
		}
		else if (!c && this->GetAt(c) == L'-')
		{
			positive = false;
		}
		else
		{
			value = temp;
			return NOT_NUMB;
		}

	}

	if (positive)
		value = abs(value);
	else
		value = -(abs(value));

	return T_NO_ERROR;
}

/*
* Method: TString::split
* Purpose: Splits a String by the provided deliniators
* Parameters: TString str - the TString holding deliniators
*			UCHAR flags - flags to use to control the behavior of this method
			WCHAR exitQuote - quote to look for if String is believed to begin in quotes
* Returns: TrecPointer<TArray<TString>> - Array of TStrings holding tokens
*
* flags: 0b00000001 - t_file_check_back_slash - ignore a hit if odd number of backslashes are present
*		 0b00000010 - t_file_out_of_quotes	  - ignore hits found within a quotation string
*		 0b00000100 - t_file_starts_in_quote  - assume that String starts within a quote
*        0b00001000 - t_file_include_empty
*/
TrecPointer<TDataArray<TString>> TString::split(TString str, UCHAR flags, WCHAR exitQuote) const 
{
	return splitn(str, 0, flags, exitQuote);
}

/*
* Method: TString::splitn
* Purpose: Splits a String by the provided deliniators, providing a limit as to how many times the string can be split
* Parameters: TString str - the TString holding deliniators
*			UINT elements - the max number of elements to split by (0 for no limit)
*			UCHAR flags - flags to use to control the behavior of this method
			WCHAR exitQuote - quote to look for if String is believed to begin in quotes
* Returns: TrecPointer<TArray<TString>> - Array of TStrings holding tokens
*
* flags: 0b00000001 - t_file_check_back_slash - ignore a hit if odd number of backslashes are present
*		 0b00000010 - t_file_out_of_quotes	  - ignore hits found within a quotation string
*		 0b00000100 - t_file_starts_in_quote  - assume that String starts within a quote
*        0b00001000 - t_file_include_empty
*
* Attributes: const
*/
TrecPointer<TDataArray<TString>> TString::splitn(TString str, UINT elements, UCHAR flags, WCHAR exitQuote) const
{
	TObjectLocker threadLock(&thread);
	TrecPointer<TDataArray<TString>> ret;
	ret = TrecPointerKey::GetNewTrecPointer<TDataArray<TString>>();

	if (!this->GetSize())
	{
		return ret;
	}


	if (elements == 1)
	{
		ret->push_back(TString(this));
		return ret;
	}
	bool hasLimit = elements > 0;

	TString tok;

	int pos = ((flags & 0b00000110) == 0b00000110) ? Find(exitQuote) : 0;

	bool insertEmpty = flags & 0b00001000;

	int begPos = pos;
	tok.Set(this->Tokenize(str, pos, flags & 0b00000010));
	while ((!tok.IsEmpty() || (insertEmpty && pos != -1)) && begPos != -1 && (!hasLimit || elements--))
	{
		if (flags & 0b00000001)
		{

			while (tok.IsBackSlashChar(tok.GetSize()))
			{
				tok.Set(this->Tokenize(str, pos, flags & 0b00000010));
				tok.Set(this->SubString(begPos, pos));
			}
		}

		for (UINT Rust = 0; Rust < str.size; Rust++)
		{
			
			tok.Remove(str[Rust], flags & 0b00000010);
		}

		(ret).Get()->push_back(tok);
		begPos = pos;
		if (hasLimit && elements == 1)
		{
			while (pos == ((flags & 2) ? FindOneOf(str, pos) : FindOneOfOutOfQuotes(str, pos)))
				pos++;
			ret->push_back(SubString(pos));
			break;
		}
		else
			tok.Set(this->Tokenize(str, pos, flags & 0b00000010));
	}

	return ret;
}

bool TString::IsBackSlashChar(UINT index)
{
	TObjectLocker threadLock(&thread);
	if (index > size)
	{
		return false;
	}
	UINT comp = 0;

	for (int C = index - 1; C >= 0 && this->string[C] == L'\\'; C--)
		comp++;

	return comp % 2 == 1;
}

/*
* Method: TString::GetBufferCopy
* Purpose: Gets a copy of the Raw Buffer
* Parameters: void
* Returns: WCHAR* - the copy of the TStrings buffer
*
* Note: the data returned was initialized via new[] and thereforem you need to call delete[] on it. It is recommended you
*		use "GetConstantBuffer()" wherever possible
*/
WCHAR * TString::GetBufferCopy() const 
{
	TObjectLocker threadLock(&thread);
	WCHAR* returnable = new WCHAR[size+1];
	for (UINT c = 0; c < GetSize(); c++)
		returnable[c] = string[c];
	returnable[size] = L'\0';
	return returnable;
}

/*
 * Mehtod: TString::GetConstantBuffer
 * Purpose: returns the underlying String 
 * Parameters: void
 * Returns: const WCHAR* - a constant pointer of the underlying string buffer
 */
TString::TConstBuffer TString::GetConstantBuffer() const
{
	// TObject::ThreadLock();
	return TConstBuffer(this);
}

/*
* Method: TString::SubString
* Purpose: returns a substring specified 
* Parameters: UINT beginningIndex - the index to start at
*			int endIndex - exclusive index to end (use negative value to go to end)
* Returns: TString::the Substring generated
*/
TString TString::SubString(UINT beginningIndex, int endIndex) const
{
	TObjectLocker threadLock(&thread);

	TString returnable;
	if (endIndex == -1)
	{
		for (UINT c = beginningIndex; c < GetSize(); c++)
			returnable.AppendChar(GetAt(c));
	}
	else
	{
		endIndex = abs(endIndex);
		if (static_cast<UINT>(endIndex) > GetSize())
			endIndex = GetSize();
		for (UINT c = beginningIndex; c < static_cast<UINT>(endIndex); c++)
			returnable.AppendChar(GetAt(c));
	}
	return returnable;
}

/*
* Method: TString::Trim
* Purpose: Removes Whitespace on both sides
* Parameters: void
* Returns: void
*/
void TString::Trim()
{
	TObjectLocker threadLock(&thread);
	TrimRight();
	TrimLeft();
	return;
}

/**
 * Method: TString::GetTrim
 * Purpose: Retrieves acopy of the TString with whitespace trimmed. It does not alter the original String
 * Parameters: void
 * Returns: TString::Trimmed copy of the String
 */
TString TString::GetTrim() const
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.Trim();
	return ret;
}

/**
 * Method: TString::TrimRight
 * Purpose: Performs an in-place trimming of whitespace towards the end
 * Parameters: void
 * Returns: void
 */
void TString::TrimRight()
{
	TObjectLocker threadLock(&thread);
	int end = size - 1;
	while (end >= 0 && iswspace(string[end]))
	{
		end--;
	}
	end++;
	size = end;
	string[size] = L'\0';
	return;
}

/**
 * Method: TString::GetTrimRight
 * Purpose: Retrieves a version of the string with no whitespace on the end
 * Parameters: void
 * Returns: TString::the String with the TrimRight operation performed
 */
TString TString::GetTrimRight()
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.TrimRight();
	return ret;
}

/**
 * Method: TString::TrimLeft
 * Purpose: Performs an in-place trimming of whitespace at the beginning
 * Parameters: void
 * Returns: void
 */
void TString::TrimLeft()
{
	TObjectLocker threadLock(&thread);
	UINT end = 0;
	while (end < size && iswspace(string[end])) end++;
	UINT sEnd = end;
	for (UINT Rust = 0; end < size; Rust++, end++)
	{
		string[Rust] = string[end];
	}
	size -= sEnd;
	string[size] = L'\0';
	return;
}

/**
 * Method: TString::GetTrimLeft
 * Purpose: Retrieves a version of the string with no whitespace towards the beginning
 * Parameters: void
 * Returns: TString::the String with the TrimLeft  operation performed
 */
TString TString::GetTrimLeft()
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.TrimLeft();
	return ret;
}

/**
 * Method: TString::Empty
 * Purpose: Dumps the contents of the string so it is empty
 * Parameters: void
 * Returns: void
 */
void TString::Empty()
{
	TObjectLocker threadLock(&thread);
	if (string)
		delete[] string;
	string = nullptr;
	size = capacity = 0;

	string = new WCHAR[3];
	capacity = 3;
	string[0] = string[1] = string[2] = L'\0';
	return;
}

/**
 * Method: TString::IsEmpty
 * Purpose: Reports whether the string is empty
 * Parameters: void
 * Returns: bool - whether the string is empty or not
 */
bool TString::IsEmpty()
{
	return size == 0;;
}

/**
 * Method: TString::GetRegString
 * Purpose: Retrieves a version of the string in ASCII, using the std::string as the return type
 * Parameters: void
 * Returns: std::string - the TString with ASCII content
 */
std::string TString::GetRegString()
{
	TObjectLocker threadLock(&thread);
	std::string reg;
	char* regString = new char[size + 1];
	BOOL bFlag = FALSE;
	/*int convert =*/ WideCharToMultiByte(CP_ACP, 0, string, size, regString, size, NULL, &bFlag);

	regString[size] = '\0';
	reg = regString;
	delete[] regString;
	return reg;
}

/*
* Method: TString::ConvertToColor
* Purpose: Converts the string into a Direct2D color representation
* Parameters: D2D1_COLOR_F & color - the color to generate
*			ColorFrmat& cf - the Color Format detected in the string
* Returns: bool - success if true, false if TString doesn't hold a color
*/
bool TString::ConvertToColor(D2D1_COLOR_F & color, ColorFormat& cf)
{
	TObjectLocker threadLock(&thread);
	if (!GetSize())
	{
		return false;
	}// First Check for a Hex Format
	if (GetAt(0) == L'#' && GetSize() == 7 || GetSize() == 9)
	{
		UINT value = 0;
		UCHAR c = 0,shift;
		for (c = 1, shift = 24; c < GetSize(); c += 2, shift -= 8)
		{
			UINT v1, v2;
			if (!convertToNumberHex(GetAt(c), v1) || convertToNumberHex(GetAt(c + 1), v2))
			{
				return false;
			}
			v1 *= 16;
			v1 += v2;

			value += (v1 << shift);
		}

		if (c == 7) // we have cform_hex
		{
			cf = ColorFormat::cform_hex;
			value = value | 255; // Make alpha completly transparent
		}
		else if (c == 9) // we have cform_hexa
		{
			cf = ColorFormat::cform_hexa;
		}
		else // Supposed to be hex, but invalid length encountered
		{
			return false;
		}
		UCHAR r = static_cast<UCHAR>(value >> 24);
		UCHAR g = static_cast<UCHAR>(value >> 16);
		UCHAR b = static_cast<UCHAR>(value >> 8);
		UCHAR a = static_cast<UCHAR>(value);

		color.r = static_cast<float>(r) / 255.0f;
		color.g = static_cast<float>(g) / 255.0f;
		color.b = static_cast<float>(b) / 255.0f;
		color.a = static_cast<float>(a) / 255.0f;
		return true;
	}

	// This wasn't a hex, try AnaGames format
	TrecPointer<TDataArray<TString>> values = split(TString(L", "));
	if (values.Get() && values->Size() > 2)
	{
		bool works = true;
		D2D1_COLOR_F tempColor = { 0.0f,0.0f,0.0f,0.0f };
		if (!works || !values->at(0).GetSize() || values->at(0).ConvertToFloat(tempColor.r))
			works = false;

		if (!works || !values->at(1).GetSize() || values->at(1).ConvertToFloat(tempColor.g))
			works = false;

		if (!works || !values->at(2).GetSize() || values->at(2).ConvertToFloat(tempColor.b))
			works = false;

		if (works && values->Size() > 3)
		{
			if (!values->at(3).GetSize() || values->at(3).ConvertToFloat(tempColor.a))
				works = false;
			else
				cf = ColorFormat::cform_ana_a;
		}
		else if (works)
		{
			tempColor.a = 1.0f;
			cf = ColorFormat::cform_ana;
		}

		if (works)
		{
			color = tempColor;
			return true;
		}
	}

	// Okay, check for rbg[a] or hsl[a]
	values = split(TString(L", ()"));
	if (!values.Get() || values->Size() < 4) // We don't have enough data for either format
	{
		return false;
	}

	values->at(0).SetLower();

	if (!values->at(0).Compare(L"rgb") || !values->at(0).Compare(L"rgba"))
	{
		// Okay, it seems we have an rgb[a] format

	

		//D2D1_COLOR_F tempColor = { 0.0f,0.0f,0.0f,0.0f };
		int tempColor[4];
		if (values->at(1).ConvertToInt(tempColor[0]) ||
			values->at(2).ConvertToInt(tempColor[1]) ||
			values->at(3).ConvertToInt(tempColor[2]))
		{
			return false;
		}
		// Okay, we have legitimate values now, time to convert
		color.r = static_cast<float>(tempColor[0]) / 255.0f;
		color.g = static_cast<float>(tempColor[1]) / 255.0f;
		color.b = static_cast<float>(tempColor[2]) / 255.0f;

		// Try to see if alpha is available
		if (values->Size() > 4 && values->at(4).GetSize() && !values->at(4).ConvertToFloat(color.a))
		{
			cf = ColorFormat::cform_rgba;
			return true;
		}

		// Alpha didn't work, set to default of 1.0f
		cf = ColorFormat::cform_rgb;
		color.a = 1.0f;
		return true;

	}

	if (!values->at(0).Compare(L"hsl") || !values->at(0).Compare(L"hsla"))
	{
		// Whoa! We have the more complex hsl format

		// Try getting the hue
		int h = 0;
		if (values->at(1).ConvertToInt(h))
		{
			return false;
		}
		h = abs(h) % 360;


		values->at(2).Remove(L'%');
		values->at(3).Remove(L'%');

		float s, l;
		if (values->at(2).ConvertToFloat(s) || values->at(2).ConvertToFloat(l))
		{
			return false;
		}
		if (s > 100.0f || l > 100.0f)
		{
			return false;
		}
		// We now have legitimate values for hsl and now we need to convert it to rgb
		// The algorithm is based of of the JavaScript code for the conversion by a Garry Tan 
		// was posted at StackOverFlow at this url: https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
		if (!s)
		{
			color.r = color.g = color.b = 0.0f;
		}
		else
		{
			l /= 100.f;
			s /= 100.f;

			float q = l < 0.5 ? l * (100.0f + s) : l + s - l * s;
			float p = 2 * l - q;

			int r_h = h + 120;
			int g_h = h;
			int b_h = h - 120;

			// Target the r value
			color.r = ConvertHueToRGB(p, q, r_h);
			color.g = ConvertHueToRGB(p, q, g_h);
			color.b = ConvertHueToRGB(p, q, b_h);


		}

		// Try to see if alpha is available
		if (values->Size() > 4 && !values->at(4).ConvertToFloat(color.a))
		{
			cf = ColorFormat::cform_hsla;
			return true;
		}

		// Alpha didn't work, set to default of 1.0f
		cf = ColorFormat::cform_hsl;
		color.a = 1.0f;
		return true;
	}

	// No regonizable format was used, return false;
	return false;
}

/**
 * Method: TString::FindOutOfQuotes
 * Purpose: Performs the find operation while ignoring hits found within quotation marks
 * Parameters: const TString& subString - the substring to search for
 *				 int start - location to start the search
 * Returns: int - the index of the substring found (-1 if not found)
 */
int TString::FindOutOfQuotes(const TString& subString, int start, bool ignoreEscape) const 
{
	TObjectLocker threadLock(&thread);
	TDataArray<int> possibleIndeces;
	while (start != -1) 
	{
		start = Find(subString, start, ignoreEscape);
		if (start != -1)
		{
			possibleIndeces.push_back(start);
			start++;
		}
	}

	TDataArray<IndexRange> quotes;
	WCHAR quoteMode = 0;
	IndexRange range{ -1,-1 };
	for (UINT rust = 0; rust < GetSize() && rust != -1; rust++)
	{
		switch (quoteMode)
		{
		case L'\'':
			range.start = rust - 1;
			rust = Find(L'\'', rust);
			if (rust != -1)
			{
				range.end = rust;
				quotes.push_back(range);
				quoteMode = 0;
			}
			break;
		case L'\"':
			range.start = rust - 1;
			rust = Find(L'\"', rust);
			if (rust != -1)
			{
				range.end = rust;
				quotes.push_back(range);
				quoteMode = 0;
			}
			break;
		default: // should be 0
			if (GetAt(rust) == L'\'')
				quoteMode = L'\'';
			else if (GetAt(rust) == L'\"')
				quoteMode = L'\"';
		}
	}

	for (UINT c = 0; c < possibleIndeces.Size(); c++)
	{
		int ind = possibleIndeces[c];
		bool works = true;
		for (UINT rust = 0; rust < quotes.Size(); rust++) 
		{
			if (ind > quotes[rust].start && ind < quotes[rust].end)
			{
				works = false;
				continue;
			}
		}
		if (works) { return ind; }
	}
	return -1;
}

int TString::FindOneOfOutOfQuotes(const TString& chars, int start) const
{
	TObjectLocker threadLock(&thread);
	int min = INT32_MAX;

	bool found = false;

	for (UINT Rust = 0; Rust < chars.GetSize(); Rust++)
	{
		int end = FindOutOfQuotes(chars[Rust], start, false);

		if (end != -1)
		{
			found = true;
			if (end < min)
				min = end;
		}
	}

	return found ? min : -1;
}


/**
 * Method: TString::Set
 * Purpose: Sets the contents of the string to the specified string
 * Parameters: const TString& t - the string to set to
 * Returns: void
 */
void TString::Set(const TString& t)
{
	TObjectLocker threadLock(&thread);
	Set(&t);
	return;
}

/**
 * Method: TString::Set
 * Purpose: Sets the contents of the string to the specified raw string
 * Parameters: const TString* s - the raw string to set to
 * Returns: void
 */
void TString::Set(const TString* s)
{
	TObjectLocker threadLock(&thread);
	if (s)
	{
		if (string)
			delete[] string;
		string = new WCHAR[s->capacity];
		capacity = s->capacity;
		size = s->size;

		for (UINT c = 0; c < size; c++)
			string[c] = s->string[c];
		string[size] = L'\0';
	}
	return;
}


/**
 * Method: TString::Set
 * Purpose: Sets the contents of the string to the specified raw string
 * Parameters: const WCHAR* w - the raw string to set to
 * Returns: void
 */
void TString::Set(const WCHAR* w)
{
	TObjectLocker threadLock(&thread);
	if (w)
	{
		TString str(w);
		Set(&str);
	}
	return;
}

/**
 * Method: TString::Set
 * Purpose: Sets the contents of the string to the specified character
 * Parameters: WCHAR w - the character to set to
 * Returns: void
 */
void TString::Set(WCHAR w)
{
	TObjectLocker threadLock(&thread);
	this->Empty();
	this->AppendChar(w);
	return;
}

/*
* Method: TString::operator=
* Purpose: Assigns an Existing TString to this TString
* Parameters: TString& t - the TString to copy
* Returns: void 
*/
TString TString::operator=(const TString &t)
{
	TObjectLocker threadLock(&thread);
	Set(t);
	return this;
}



/*
* Method: TString::operator=
* Purpose: Assigns an Existing TString to this TString
* Parameters: TString* s - the TString to copy
* Returns: void
*/
TString TString::operator=(const TString * s)
{
	TObjectLocker threadLock(&thread);
	Set(s);
	return this;
}



/*
* Method: TString::operator=
* Purpose: Assigns an existing wide string to this TString
* Parameters: WCHAR* w - the wide string to copy
* Returns: void
*/
TString TString::operator=(const WCHAR * w)
{
	TObjectLocker threadLock(&thread);
	Set(w);
	return this;
}

/*
* Method: TString::operator=
* Purpose: Assigns a WCHAR to this TString
* Parameters: WCHAR w - the Character to copy
* Returns: void
*/
TString TString::operator=(WCHAR w)
{
	TObjectLocker threadLock(&thread);
	Set(w);
	return this;
}

/*
* Method: TString::operator+
* Purpose: Adds the contents of an existing TString to this one 
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+(const TString & t)const
{
	TObjectLocker threadLock(&thread);
	TString returnstring = this;
	for (UINT c = 0; c < t.GetSize();c++)
		returnstring.AppendChar(t.GetAt(c));
	return returnstring;
}



/*
* Method: TString::operator+
* Purpose: Adds the contents of an existing TString to this one
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+(const TString *t) const
{
	TObjectLocker threadLock(&thread);
	if (t)
	{
		TString returnstring = this;
		for (UINT c = 0; c < t->GetSize(); c++)
			returnstring.AppendChar(t->GetAt(c));
		return returnstring;
	}
	return this;
}



/*
* Method: TString::operator+
* Purpose: Adds the contents of an existing wide string to this one
* Parameters: WCHAR* w - the wide string to append
* Returns: void
*/
TString TString::operator+(const WCHAR * w)const 
{
	TObjectLocker threadLock(&thread);
	if (w)
	{
		TString returnstring = this;
		for (int c = 0; w[c] != L'\0'; c++)
			returnstring.AppendChar(w[c]);
		return returnstring;
	}
	return this; // return this string since there was nothing to add
}

/**
 * Method: TString::operator+
 * Purpose: returns a version of this string with the character appended
 * Parameters: WCHAR w -the character to append
 * Returns: TString version of the string with the appended character
 */
TString TString::operator+(WCHAR w) const
{
	TObjectLocker threadLock(&thread);
	TString returnstring = this;
	returnstring.AppendChar(w);
	return returnstring;
}

/*
* Method: TString::operator+=
* Purpose: Adds the contents of an existing TString to this one
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+=(const TString& t)
{
	TObjectLocker threadLock(&thread);
	for (UINT c = 0; c < t.GetSize(); c++)
		AppendChar(t.GetAt(c));
	return this;
}


/*
* Method: TString::operator+=
* Purpose: Adds the contents of an existing TString to this one
* Parameters: TString& t - the TString to append
* Returns: void
*/
TString TString::operator+=(const TString* t)
{
	TObjectLocker threadLock(&thread);
	if (t)
	{
		for (UINT c = 0; c < t->GetSize(); c++)
			AppendChar(t->GetAt(c));
	}
	return this;
}



/*
* Method: TString::operator+=
* Purpose: Adds the contents of an existing wide string to this one
* Parameters: WCHAR* w - the wide string to append
* Returns: void
*/
TString TString::operator+=(const WCHAR* w)
{
	TObjectLocker threadLock(&thread);
	if (w)
	{
		for (int c = 0; w[c] != L'\0'; c++)
			this->AppendChar(w[c]);
	}
	return this;
}

/**
 * Method: TString::operator+=
 * Purpose: Appends the WCHAR to the string
 * Parameters: WCHAR w
 * Returns: TString::the result of the append operation
 */
TString TString::operator+=(WCHAR w)
{
	TObjectLocker threadLock(&thread);
	AppendChar(w);
	return this;
}

/*
* Method: TString::operator==
* Purpose: Conpares the TString with a TString and returns true if contents are equal
* Parameters: TString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(TString & s)
{
	TObjectLocker threadLock(&thread);
	if (GetSize() != s.GetSize())
	{
		return false;
	}
	for (UINT c = 0; c < GetSize(); c++)
		if (GetAt(c) != s[c])
		{
			return false;
		}
	return true;
}



/*
* Method: TString::operator==
* Purpose: Conpares the TString with a TString and returns true if contents are equal
* Parameters: TString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(TString * s)
{
	TObjectLocker threadLock(&thread);
	if (!s)
	{
		return false;
	}
	if (GetSize() != s->GetSize())
	{
		return false;
	}
	for (UINT c = 0; c < GetSize(); c++)
		if (GetAt(c) != s->GetAt(c))
		{
			return false;
		}
	return true;
}


/*
* Method: TString::operator==
* Purpose: Conpares the TString with a wide string and returns true if contents are equal
* Parameters: WCHAR* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator==(WCHAR * s)
{
	TObjectLocker threadLock(&thread);
	if (!s)
	{
		return false;
	}
	for (UINT c = 0; c < GetSize(); c++, s++)
	{
		if (*s == L'\0')
		{
			return false;
		}
		if (*s != GetAt(c))
		{
			return false;
		}
	}
	return *s == L'\0';
}

/*
* Method: TString::operator!=
* Purpose: Conpares the TString with a TString and returns true if contents are unequal
* Parameters: TString& s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(TString & s)
{
	TObjectLocker threadLock(&thread);
	return !(*this == s);
}



/*
* Method: TString::operator!=
* Purpose: Conpares the TString with a TString and returns true if contents are unequal
* Parameters: TString* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(TString * s)
{
	TObjectLocker threadLock(&thread);
	return !(*this == s);
}



/*
* Method: TString::operator!=
* Purpose: Conpares the TString with a wide string and returns true if contents are unequal
* Parameters: WCHAR* s - the string to compare
* Returns: bool - result of comparison
*/
bool TString::operator!=(WCHAR * s)
{
	TObjectLocker threadLock(&thread);
	return !(*this == s);
}

/**
 * Method: TString::operator[]
 * Purpose: Retrieves the character at the provided index
 * Parameters: UINT loc - the index of the character
 * Returns: WCHAR - the character at the index, '\0' if index is out of bounds
 */
WCHAR& TString::operator[](UINT loc) const 
{
	ThreadLock();
	if (loc > size)
	{
		ThreadRelease();
		throw L"IndexOutOfBounds";
	}
	ThreadRelease();
	return string[loc];
}

/*
* Method: TString::GetAnaGameType
* Purpose: Retrieves the Class type for the AnaGame Virtual Machine
* Parameters: void
* Returns: UCHAR* - The AnaGAme type ID
*/
UCHAR * TString::GetAnaGameType()
{
	TObjectLocker threadLock(&thread);
	return TStringType;
}

/**
 * Method: TString::GetSize
 * Purpose: Retrieves the size of the string
 * Parameters: void
 * Returns: UINT - the current size of the string
 */
UINT TString::GetSize() const
{
	return size;
}

/**
 * Method: TString::GetAt
 * Purpose: Retrieves the character at the provided index
 * Parameters: UINT c - the index of the character
 * Returns: WCHAR - the character at the index, '\0' if index is out of bounds
 */
WCHAR TString::GetAt(UINT c) const
{
	TObjectLocker threadLock(&thread);
	if (c < size)
	{
		return string[c];
	}
	return L'\0';
}

/**
 * Method: TString::AppendFormat
 * Purpose: Appends the provided format to this string
 * Parameters: const WCHAR* format - the template
 *				... - the series of variables to add
 * Returns: void
 */
void TString::AppendFormat(const WCHAR* format, ...)
{
	
	if (!format)
		return;
	va_list vList;
	va_start(vList, format);
	TObjectLocker threadLock(&thread);
	TString tFormat(format);

	WCHAR* formatedString = new WCHAR[tFormat.capacity * 2 + 100];

	int result = vswprintf(formatedString, (tFormat.capacity * 2) + 99, tFormat.string, vList);

	va_end(vList);


	//TString ret;
	if (result > -1)
	{
		formatedString[result] = L'\0';
		Append(formatedString);
	}

	delete[] formatedString;
	return;
}

/**
 * Method: TString::AppendFormat
 * Purpose: Appends the provided format to this string
 * Parameters: const TString format - the template
 *				... - the series of variables to add
 * Returns: void
 */
void TString::AppendFormat(const TString format, ...)
{
	va_list vList;
	va_start(vList, format);
	TObjectLocker threadLock(&thread);;
	WCHAR* formatedString = new WCHAR[format.capacity * 2 + 100];
	int result = vswprintf(formatedString, (format.capacity * 2) + 99, format.string, vList);

	va_end(vList);


	TString ret;
	if (result > -1)
	{
		formatedString[result] = L'\0';
		Append(formatedString);
	}

	delete[] formatedString;
	return;
}

/**
 * Method: TString::Format
 * Purpose: Sets this string to the provided format
 * Parameters: const WCHAR* format - the template
 *				... - the series of variables to add
 * Returns: void
 */
void TString::Format(const WCHAR* format, ...)
{
	
	if (!format)
		return;
	va_list vList;
	va_start(vList, format);
	TObjectLocker threadLock(&thread);
	TString tFormat(format);

	WCHAR* formatedString = new WCHAR[tFormat.capacity * 2 + 100];


	int result = vswprintf(formatedString, (tFormat.capacity * 2) + 99, tFormat.string, vList);

	va_end(vList);

	TString ret;
	if (result > -1)
	{
		formatedString[result] = L'\0';
		Set(formatedString);
	}

	delete[] formatedString;
	return;
}

/**
 * Method: TString::Format
 * Purpose: Sets this string to the provided format
 * Parameters: const TString format - the template
 *				... - the series of variables to add
 * Returns: void
 */
void TString::Format(const TString format, ...)
{
	va_list vList;
	va_start(vList, format);
	TObjectLocker threadLock(&thread);
	WCHAR* formatedString = new WCHAR[format.capacity * 2 + 100];



	int result = vswprintf(formatedString, (format.capacity * 2) + 99, format.string, vList);

	va_end(vList);


	TString ret;
	if (result > -1)
	{
		formatedString[result] = L'\0';
		Set(formatedString);
	}

	delete[] formatedString;
	return;
}

/**
 * Method: TString::AppendChar
 * Purpose: Appends the provided character to this string
 * Parameters: WCHAR ch - the character to append
 * Returns: void
 */
void TString::AppendChar(WCHAR ch)
{
	TObjectLocker threadLock(&thread);
	if (size + 1 == capacity)
	{
		WCHAR* newString = new WCHAR[++capacity];

		memcpy(newString, string, (capacity - 1) * sizeof(WCHAR));
		delete[] string;
		string = newString;
	}

	string[size++] = ch;
	string[size] = L'\0'; return;
}

/**
 * Method: TString::Append
 * Purpose: Appends the provided string to this string
 * Parameters: const TString& app - the string to append
 * Returns: void
 */
void TString::Append(const TString& app)
{
	TObjectLocker threadLock(&thread);
	capacity += app.capacity;

	WCHAR* newString = new WCHAR[capacity];

	for (UINT c = 0; c < size; c++)
	{
		newString[c] = string[c];
	}

	UINT newSize = size + app.size;

	for (UINT c = size, rust = 0; c < newSize && rust < app.size; c++, rust++)
	{
		newString[c] = app[rust];
	}
	size = newSize;
	newString[size] = L'\0';
	delete[] string;
	string = newString;
	return;
}

bool IsWhitespace(WCHAR ch)
{
	fillWhiteChar();

	for (UINT Rust = 0; Rust < whiteChar.Size(); Rust++)
	{
		if (whiteChar[Rust] == ch)
			return true;
	}

	return false;
}

/*
* Function: convertToNumber
* Purpose: Converts a character into a number
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumber(TCHAR c, int* i)
{
	switch (c)
	{
	case '0':
		*i = 0;
		return true;
	case '1':
		*i = 1;
		return true;
	case '2':
		*i = 2;
		return true;
	case '3':
		*i = 3;
		return true;
	case '4':
		*i = 4;
		return true;
	case '5':
		*i = 5;
		return true;
	case '6':
		*i = 6;
		return true;
	case '7':
		*i = 7;
		return true;
	case '8':
		*i = 8;
		return true;
	case '9':
		*i = 9;
		return true;
	default:
		return false;
	}
}

/*
* Function: convertToNumber
* Purpose: Converts a character into a number, allowing for hew values
* Parameters: TCHAR c - the character to convert
*			int* i - the new number
* Returns: bool - whether character was a number
*/
bool convertToNumberHex(TCHAR c, UINT & i)
{
	switch (c)
	{
	case L'0':
		i = 0;
		return true;
	case L'1':
		i = 1;
		return true;
	case L'2':
		i = 2;
		return true;
	case L'3':
		i = 3;
		return true;
	case L'4':
		i = 4;
		return true;
	case L'5':
		i = 5;
		return true;
	case L'6':
		i = 6;
		return true;
	case L'7':
		i = 7;
		return true;
	case L'8':
		i = 8;
		return true;
	case L'9':
		i = 9;
		return true;
	case L'a':
	case L'A':
		i = 10;
		return true;
	case L'b':
	case L'B':
		i = 11;
		return true;
	case L'c':
	case L'C':
		i = 12;
		return true;
	case L'd':
	case L'D':
		i = 13;
		return true;
	case L'e':
	case L'E':
		i = 14;
		return true;
	case L'f':
	case L'F':
		i = 15;
		return true;
	}
	return false;
}

/*
* Function: convertHueToRGB
* Purpose: Converts an hsv value to a comparble RGB format
* Parameters: float p - the p of the new color
*			float q - the q of the new color
*			int hue - the hue of the color
* Returns: float - the color in an RGB channel
*/
float ConvertHueToRGB(float p, float q, int hue)
{
	float javaScriptHue = static_cast<float>(hue) / 360.0f;

	if (javaScriptHue < 1.0f / 6.0f)
		return p + (q - p) * 6 * javaScriptHue;
	if (javaScriptHue < 0.5f)
		return q;
	if (javaScriptHue < 2.0f / 3.0f)
		return p + (q - p)*(2.0f / 3.0f - javaScriptHue) * 6.0f;
	return p;

}

/*
* Function: ReturnCharType
* Purpose: returns the WCHAR representation of the char
* Parameters: char c - the chaacter to convert
* Returns: WCHAR - the wide version of the character
*/
WCHAR ReturnCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}

/*
* Function: ReturnWCharType
* Purpose: Returns the WCHAR representation of the char
* Parameters: char c - the chaacter to convert
* Returns: WCHAR - the wide version of the character
*/
WCHAR ReturnWCharType(char c)
{
	WCHAR w[] = { L'0',L'\0' };
	size_t conv = 0;
	char charTo[] = { c, '\0' };
	mbstowcs_s(&conv, w, 2, charTo, 1);
	return w[0];
}

bool _TREC_LIB_DLL IndexComesFirst(int focusIndex, int checkIndex)
{
	if(focusIndex == -1)
		return false;
	return checkIndex == -1 || focusIndex < checkIndex;
}

TString formatSpecifiers(L"diuoxXfFeEgGaAcCsSpT");

/*
TString TString::GetFormattedString(const TString& format, va_list& data)
{
	WCHAR* formatedString = new WCHAR[format.capacity * 2 + 100];
	WCHAR* cop = format.GetBufferCopy();

	va_start(data, cop);

	int result = vswprintf(formatedString, (format.capacity * 2) + 99, format.string, data);

	va_end(data);
	delete[] cop;

	TString ret;
	if (result > -1)
	{
		formatedString[result] = L'\0';
		ret.Set(formatedString);
	}

	delete[] formatedString;
	
	return ret;
}*/


/**
 * Method: TString::Compare
 * Purpose: Compares this string with a provided string
 * Parameters: const TString& other - the string to compare this string to
 * Returns: int - 0 if they are the same
 */
int TString::Compare(const TString& other) const
{
	return Compare(other.string);
}

/**
 * Method: TString::Compare
 * Purpose: Compares this string with a provided raw string
 * Parameters: const WCHAR* other - the string to compare this string to
 * Returns: int - 0 if they are the same
 */
int TString::Compare(const WCHAR* other) const
{
	TObjectLocker threadLock(&thread);
	int min = min(size, lstrlenW(other));
	for (int c = 0; c < min; c++)
	{
		if (string[c] < other[c])
		{
			return - (c + 1);
		}
		if (string[c] > other[c])
		{
			return c + 1;
		}
	}

	if (size == lstrlenW(other)) { return 0; }
	if (size > lstrlenW(other)) { return size; }
	return static_cast<int>(-1);
}

/**
 * Method: TString::CompareNoCase
 * Purpose: Compares this string with a provided string, ignoring case
 * Parameters: const TString& other - the string to compare this string to
 * Returns: int - 0 if they are the same
 */
int TString::CompareNoCase(const TString& other) const
{
	TObjectLocker threadLock(&thread);
	TString _this(this);
	TString _other(other);
	_this.SetLower();
	_other.SetLower();
	return _this.Compare(_other);
}

/**
 * static Method: TString::Compare
 * Purpose: Compares two strings for equality
 * Parameters: const TString& str1 - the main string to compare
 *				const TString& str2 - the second string to compare to
 * Returns: int - 0 if they are the same
 */
int TString::Compare(const TString& str1, const TString& str2)
{
	return str1.Compare(str2);
}

/**
 * Method: TString::Delete
 * Purpose: Deletes a section of the string in -place
 * Parameters: int index - the index of the first character to remove
 *				int count - the number of characters to remove
 * Returns: int - the new size of the string
 */
int TString::Delete(UINT index, int count)
{
	TObjectLocker threadLock(&thread);
	if (count < 1)
	{
		return size;
	}
	if (index < 0 || index > size)
	{
		return size;
	}
	WCHAR* newString = new WCHAR[capacity];
	for (int c = 0; c < index; c++)
	{
		newString[c] = static_cast<WCHAR>(string[c]);
	}

	for (int c = index, rust = index + count; rust < capacity; c++, rust++)
	{
		newString[c] = string[rust];
	}

	delete[] string;
	string = newString;

	size -= count;
	string[size] = L'\0';
	return size;
}

/**
 * Method: TString::GetDelete
 * Purpose: Retrieves a TString with a section deleted
 * Parameters: int& ret - the size of the new string
 *				int index - the index of the first character to remove
 *				int count - the number of characters to remove
 * Returns: TString::the String with the Delete operation applied
 */
TString TString::GetDelete(int& ret, int index, int count)
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Delete(index, count);
	return retStr;
}

/**
 * Method: TString::StartsWith
 * Purpose: deduces whether the String starts with a given sequence
 * Parameters: const TString& seq - the sequence to check
 *				bool ignoreCase - whether to ignore case when doing the analysis
 *				bool whitespace - if true, only return true if there is whitespace at the character after 'seq'
 * Returns: bool
 *
 * Note: Will return false if seq is longer than 'this' string
 */
bool TString::StartsWith(const TString& seq, bool ignoreCase, bool whitespace) const
{
	TObjectLocker threadLock(&thread);
	if (seq.GetSize() > size)
	{
		return false;
	}
	if (ignoreCase)
	{
		TString lSeq(seq.GetLower());
		TString lThis(this->GetLower());

		for (UINT Rust = 0; Rust < lSeq.GetSize(); Rust++)
		{
			if (lThis[Rust] != lSeq[Rust]) { return false; }
		}
	}
	else
	{
		for (UINT Rust = 0; Rust < seq.GetSize(); Rust++)
		{
			if (string[Rust] != seq[Rust]) { return false; }
		}
	}

	if (whitespace)
	{
		if (seq.GetSize() == size)
		{
			return false;
		}
		WCHAR ch = this->string[seq.GetSize()];

		for (UINT Rust = 0; Rust < whiteChar.Size(); Rust++)
		{
			if (whiteChar[Rust] == ch)
			{
				return true;
			}
		}

		return false;
	}

	return true;
}


/**
 * Method: TString::EndsWith
 * Purpose: deduces whether the String ends with a given sequence
 * Parameters: const TString& seq - the sequence to check
 *				bool ignoreCase - whether to ignore case when doing the analysis (false by default)
 * Returns: bool
 *
 * Note: Will return false if seq is longer than 'this' string
 */
bool TString::EndsWith(const TString& seq, bool ignoreCase)const 
{
	TObjectLocker threadLock(&thread);
	if (seq.GetSize() > size)
	{
		return false;
	}
	if (ignoreCase)
	{
		TString lSeq(seq.GetLower());
		TString lThis(this->GetLower());
		for (UINT C = lThis.GetSize() - 1, Rust = lSeq.GetSize() - 1; C < size && Rust < size; C--, Rust--)
		{
			if (lThis[C] != lSeq[Rust])
			{
				return false;
			}
		}
	}
	else
	{
		for (UINT C = size - 1, Rust = seq.GetSize() - 1; C < size && Rust < size; C--, Rust--)
		{
			if (string[C] != seq[Rust])
			{
				return false;
			}
		}
	}
	return true;
}

/**
 * Method: TString::Find
 * Purpose: Finds the last instance of the specified string
 * Parameters: const TString& sub - the string to search for
 *				int start - the index to begin the search from 
 *				bool ignoreEscape - whether to ignore the presence of an escape character infront of a possible hit
	 *				bool notAlphaNum - false if you don't care if entry is surrounded by alpha-numberic characters, true if you want it isolated from alphanumeric characters
 * Returns: int - the index of the string found
 */
int TString::Find(const TString& sub, int start, bool ignoreEscape, bool notAlphaNum) const
{
	TObjectLocker threadLock(&thread);
	int indexStart = start;

	while ((indexStart = Find(sub[0], indexStart, ignoreEscape)) != -1)
	{
		bool works = true;
		for (int c = 0, rust = indexStart; c < sub.GetSize() || rust < size; c++, rust++)
		{
			if (c == sub.GetSize())
			{
				works = true;
				break;
			}
			if (rust == size)
			{
				works = false;
				break;
			}
			if (sub[c] != string[rust])
			{
				works = false;
				break;
			}
		}

		if (works)
		{
			if(!notAlphaNum) // If we don't care about what surrounds our entry, go ahead and return it
			{
				return indexStart;
			}// Otherwise, check to see if it is isolated from other alpha numeric characters
			// Use this to track the ends. 1 = clear from beginning, 2 means clear from end, need three to return
			UCHAR uWorks = 0;
			if (indexStart == 0 || !isalnum(string[indexStart - 1]))
				uWorks += 1;
			int indexEnd = indexStart + sub.GetSize();
			if (indexEnd >= size || !isalnum(string[indexEnd]))
				uWorks += 2;
			if (uWorks == 3)
			{
				return indexStart;
			}
		}
		indexStart++;
	}

	return -1;
}

/**
 * Method: TString::Find
 * Purpose: Finds the first instance of the specified character
 * Parameters: WCHAR sub - the character to search for
 *				int start - the index to begin the search from
 *				bool ignoreEscape - whether to ignore the presence of an escape character infront of a possible hit
 * Returns: int - the index of the character found
 */
int TString::Find(WCHAR sub, int start, bool ignoreEscape) const 
{
	TObjectLocker threadLock(&thread);
	if (start < 0)
	{
		return - 1;
	}
	for (int c = start; c < size; c++)
	{
		if (string[c] == sub)
		{
			bool cont = false;
			if (!ignoreEscape && c > 0 && string[c - 1] == L'\\')
			{
				UINT slashCount = 1;
				for (int Rust = c - 2; Rust > -1 && string[Rust] == L'\\'; Rust--)
					slashCount++;
				cont = slashCount % 2 != 0;
			}
			if(cont)
				continue;
			return c;
		}
	}
	return -1;
}

/**
 * Method: TString::FindOneOf
 * Purpose: Searches for one of the specified characters
 * Parameters: const TString& chars -  the list of characters to search for
 *				int start - the index to begin the search from 
 * Returns: int - the index of the character found
 */
int TString::FindOneOf(const TString& chars, int start) const
{
	TObjectLocker threadLock(&thread);
	if (start < 0)
	{
		return - 1;
	}
	for (int c = start; c < size; c++)
	{
		for (UINT rust = 0; rust < chars.GetSize(); rust++)
		{
			if (string[c] == chars[rust])
			{
				return c;
			}
		}
	}


	return -1;
}

/**
 * Method: TString::FindLast
 * Purpose: Finds the last instance of the specified string
 * Parameters: const TString& sub - the string to search for
 *				int start - the index to begin the search from (searches backwards)
 * Returns: int - the index of the string found
 */
int TString::FindLast(const TString& sub, int start) const
{
	TObjectLocker threadLock(&thread);
	if (start >= static_cast<int>(size))
	{
		return - 1;
	}
	if (start < 0)
		start = size - 1;

	for (int c = start; c >= 0; c--)
	{
		if (sub[0] == string[c])
		{
			bool works = true;
			for (int rust = c, C = 0; rust < size && C < sub.size; rust++, C++)
			{
				if (string[rust] != sub[C])
				{
					works = false;
					break;
				}
			}
			if (works)
			{
				return c;
			}
		}
	}
	return -1;
}

/**
 * Method: TString::FindLast
 * Purpose: Finds the last instance of the specified character
 * Parameters: WCHAR sub - the character to search for
 *				int start - the index to begin the search from (searches backwards)
 * Returns: int - the index of the character found
 */
int TString::FindLast(WCHAR sub, int start) const 
{
	TObjectLocker threadLock(&thread);
	if (start >= static_cast<int>(size))
	{
		return - 1;
	}
	for (int rust = ((start == -1) ? size - 1 : start); rust >= 0; rust--)
	{
		if (string[rust] == sub)
		{
			return rust;
		}
	}
	return -1;
}

/**
 * Method: TString::FindLastOneOf
 * Purpose: Searches backwards for one of the specified characters
 * Parameters: const TString& chars -  the list of characters to search for
 *				int start - the index to begin the search from (searches backwards)
 * Returns: int - the index of the character found
 */
int TString::FindLastOneOf(const TString& chars, int start) const 
{
	TObjectLocker threadLock(&thread);
	if (start >= static_cast<int>(size))
	{
		return - 1;	
	}
	if (start < 0)
		start =  size-1;

	for (int c = start; c >= 0; c--)
	{
		for (UINT rust = 0; rust < chars.GetSize(); rust++)
		{
			if (string[c] == chars[rust])
			{
				return c;
			}
		}
	}

	return -1;
}

/**
 * Method: TString::SetAsEnvironmentVariable
 * Purpose:
 * Parameters: TString& var - the name of the Enviroment variable to set this string to
 * Returns: bool - whether the operation was successful or not
 */
bool TString::SetAsEnvironmentVariable(TString& var)
{
	TObjectLocker threadLock(&thread);
	WCHAR* newString = new WCHAR[1000];

	int newSize = GetEnvironmentVariableW(var.string, newString, 999);

	if (!newSize)
	{
		return false;
	}
	WCHAR* newString2 = new WCHAR[newSize + 1];
	for (int c = 0; c < newSize + 1; c++)
	{
		newString2[c] = newString[c];
	}

	if (string)
		delete[] string;
	string = newString2;
	string[newSize] = L'\0';

	size = newSize;
	capacity = size + 1;

	delete[] newString;

	return true;
}


/**
 * Method: TString::CountFinds
 * Purpose: Counts the number of times the provided string appears in this string
 * Parameters: const TString& query - the string to search for
 * Returns: UINT - the number of times the query string appears in this string
 * 
 * Attributes: const
 */
UINT TString::CountFinds(const TString& query, int stop) const
{
	TObjectLocker threadLock(&thread);
	UINT ret = 0;

	int start = 0;
	if (stop < 0)
		stop = size;

	while ((start = Find(query, start)) != -1 && start < stop)
	{
		ret++;
		start++;
	}

	return ret;
}

/**
 * Method: TString::CountFinds
 * Purpose: Counts the number of times the provided character appears in this string
 * Parameters: WCHAR ch - the string to search for
 * Returns: UINT - the number of times the query string appears in this string
 * 
 * Attributes: const
 */
UINT TString::CountFinds(WCHAR ch, int stop) const
{
	TObjectLocker threadLock(&thread);
	UINT ret = 0;

	int start = 0;
	if (stop < 0)
		stop = size;

	while ((start = Find(ch, start)) != -1 && start < stop)
	{
		ret++;
		start++;
	}

	return ret;
}

/**
 * Method: TString::CountOneOfFinds
 * Purpose: Counts the number of times one of the characters in the provided string appears in this string
 * Parameters: const TString& query - the collection of characters to search for
 * Returns: UINT - the number of times the query string appears in this string
 * 
 * Attributes: const
 */
UINT TString::CountOneOfFinds(const TString& query) const
{
	TObjectLocker threadLock(&thread);
	UINT ret = 0;

	int start = 0;

	while ((start = FindOneOf(query, start)) != -1)
	{
		ret++;
		start++;
	}

	return ret;
}

/**
 * Method: TString::Insert
 * Purpose: Inserts a sub-string in-place
 * Parameters: int index - the index to insert the subStr at
 *				TString& subStr - the string to insert
 * Returns: int - the size of the new string
 */
int TString::Insert(int index, const TString& subStr)
{
	TObjectLocker threadLock(&thread);
	if (index < 0 || index > size)
	{
		return size;
	}
	WCHAR* newString = new WCHAR[capacity + subStr.capacity];

	
	for (int c = 0; c < index; c++)
	{
		newString[c] = string[c];
	}

	for (int c = index, rust = 0; rust < subStr.GetSize(); c++, rust++)
	{
		newString[c] = subStr.GetAt(rust);
	}

	for (int c = index + subStr.GetSize(), rust = index; rust < size; c++, rust++)
	{
		newString[c] = string[rust];
	}

	delete[]string;
	string = newString;
	size = size + subStr.GetSize();

	capacity += subStr.capacity;
	string[size] = L'\0';
	return size;
}

/**
 * Method: TString::GetInsert
 * Purpose: Retrieves a String with a another string inserted
 * Parameters: int& ret - reference to the size of the string
 *				int index - the index to insert the character at
 *				TString& subStr - the string to insert
 * Returns: TString::The copy of the string with the parameter string inserted
 */
TString TString::GetInsert(int& ret, int index, TString& subStr)
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Insert(index, subStr);
	return retStr;
}

/**
 * Method: TString::Insert
 * Purpose: Inserts a character in-place
 * Parameters: int index - the index to insert the character at 
 *				WCHAR ch - the character to insert
 * Returns: int - the size of the new string
 */
int TString::Insert(int index, WCHAR ch)
{
	TObjectLocker threadLock(&thread);
	TString sub(ch);
	return Insert(index,sub);
}

/**
 * Method: TString::GetInsert
 * Purpose: Retrieves a String with a character inserted
 * Parameters: int& ret - reference to the size of the string
 *				int index - the index to insert the character at 
 *				WCHAR ch - the character to insert
 * Returns: TString::The copy of the string with the character inserted
 */
TString TString::GetInsert(int& ret, int index, WCHAR ch)
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Insert(index, ch);
	return retStr;
}

/**
 * Method: TString::SetLower
 * Purpose: Sets this string to have all lowercase in-place
 * Parameters: void
 * Returns: void
 */
void TString::SetLower()
{
	TObjectLocker threadLock(&thread);
	for (UINT c = 0; c < size; c++)
	{
		string[c] = towlower(string[c]);
	}
}

/**
 * Method: TString::GetLower
 * Purpose: Retrieves a version of the String with all lowercase
 * Parameters: void
 * Returns: TString::the lowercase version of the String
 */
TString TString::GetLower() const
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.SetLower();
	return ret;
}

/**
 * Method: TString::SetUpper
 * Purpose: Sets this string to have all caps in-place
 * Parameters: void
 * Returns: void
 */
void TString::SetUpper()
{
	TObjectLocker threadLock(&thread);
	for (UINT c = 0; c < size; c++)
	{
		string[c] = towupper(string[c]);
	}
}

/**
 * Method: TString::GetUpper
 * Purpose: Retrieves a version of the String with all caps
 * Parameters: void
 * Returns: TString::the Uppercase version of the String
 */
TString TString::GetUpper()const
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.SetUpper();
	return ret;
}

/**
 * Method: TString::SetReverse
 * Purpose: Reverses the contents of the string in-place
 * Parameters: void
 * Returns: void
 */
void TString::SetReverse()
{
	TObjectLocker threadLock(&thread);
	for (UINT Rust = 0; Rust < size / 2; Rust++)
	{
		WCHAR temp = string[Rust];
		string[Rust] = string[size - (Rust + 1)];
		string[size - (Rust + 1)] = temp;
	}
}

/**
 * Method: TString::GetReverse
 * Purpose: Retrieves a TString that has the contents reversed
 * Parameters: void
 * Returns: TString::the copy with the characters reversed
 */
TString TString::GetReverse()
{
	TObjectLocker threadLock(&thread);
	TString ret(this);
	ret.SetReverse();
	return ret;
}

/**
 * Method: TString::Remove
 * Purpose: Removes the character from the TSTring in-place
 * Parameters: WCHAR c - the character to remove
 * Returns: int - The number of times the WCHAR was found in the string
 */
int TString::Remove(WCHAR c, bool outOfQuotes)
{
	TObjectLocker threadLock(&thread);
	WCHAR* newString = new WCHAR[capacity];

	int newCount = 0;

	WCHAR q = L'\0';

	for (int rust = 0; rust < size; rust++)
	{
		if (outOfQuotes)
		{
			if (!q && (string[rust] == L'\'' || string[rust] == L'\"'))
				q = string[rust];
			else if (q == string[rust])
				q = L'\0';
		}

		if (!q && string[rust] == c)
			continue;
		newString[newCount++] = string[rust];
	}
	size = newCount;
	delete[] string;
	string = newString;
	return size;
}

/**
 * Method: TString::GetRemove
 * Purpose: returns a TString with a given character removed
 * Parameters: int& ret - The number of times the WCHAR was found in the string
 *				WCHAR c - the character to remove
 * Returns: TString::copy with the specified character removed
 */
TString TString::GetRemove(int& ret, WCHAR c, bool outOfQuotes)
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Remove(c, outOfQuotes);
	return retStr;
}

/**
 * Method: TString::Replace
 * Purpose: Applies the Replace operation via TStrings as parameters
 * Parameters: const TString& oldStr - the String to replace
 *				const TString& newStr - the String to replace the old string with
 * Returns: int - the number of instances where the replacement operaiton was applied
 */
int TString::Replace(const TString& oldStr, const TString& newStr, bool doAll)
{
	TObjectLocker threadLock(&thread);
	TDataArray<int> indices;

	int index = 0;
	while (true)
	{
		index = Find(oldStr, index);
		if (index == -1)
			break;
		indices.push_back(index);
		index+= oldStr.GetSize();
		if (!doAll)
			break;
	}

	// If the old string does not appear, make no changes
	if (!indices.Size())
	{
		return 0;
	}
	int difference = (static_cast<int>(newStr.size) - static_cast<int>(oldStr.size)) * indices.Size();
	int newSize = size + difference;

	WCHAR* newString = new WCHAR[capacity + difference];

	int currentIndex = indices[0];
	int indexIndex = 1;
	for (int rust = 0, c = 0; rust < size && c < newSize; rust++, c++)
	{
		if (rust == currentIndex)
		{
			for (UINT C = 0; C < newStr.size && c < newSize; C++, c++)
			{
				newString[c] = newStr[C];
			}
			c--; // So then the outer for loop increments again, it will be where it should
			if (indexIndex >= indices.Size())
			{
				currentIndex = -1;
			}
			else
			{
				currentIndex = indices[indexIndex++];
			}
			rust += (oldStr.size - 1);
			continue;
		}

		newString[c] = string[rust];
	}

	size = newSize;
	newString[size] = L'\0';
	delete[] string;
	string = newString;
	capacity += difference;

	return indices.Size();
}

/**
 * Method: TString::GetReplace
 * Purpose: returns a copy of the TString with the old String replaced by the new one
 * Parameters: int& ret - reference of the replacemnt counter
 *				const TString& oldStr - the String to replace
 *				const TString& newStr - the String to replace the old string with
 * Returns: TString::The Copy with the Replace operation applied
 */
TString TString::GetReplace(int& ret, const TString& oldStr, const TString& newStr, bool doAll) const
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Replace(oldStr, newStr, doAll);
	return retStr;
}

/**
 * Method: TString::Replace
 * Purpose: In-place character replacement operation
 * Parameters: WCHAR& oldStr - the character to replace
 *				 WCHAR& newStr - the character to replace the old one with
 * Returns: The number of characters in the string replaced
 */
int TString::Replace(WCHAR& oldStr, WCHAR& newStr)
{
	TObjectLocker threadLock(&thread);
	int ret = 0;

	for (UINT Rust = 0; Rust < size; Rust++)
	{
		if (string[Rust] == oldStr)
		{
			ret++;
			string[Rust] = newStr;
		}
	}

	return ret;
}

/**
 * Method: TString::GetReplace
 * Purpose: returns a copy of the TString with the old character replaced by the new one
 * Parameters: int& ret - reference to the return value, see "Returns:" for the Replace method
 *				 WCHAR& oldStr - the character to replace
 *				 WCHAR& newStr - the character to replace the old one with
 * Returns: TString::copy of the string with the replace operation applied
 */
TString TString::GetReplace(int& ret, WCHAR& oldStr, WCHAR& newStr)
{
	TObjectLocker threadLock(&thread);
	TString retStr(this);
	ret = retStr.Replace(oldStr, newStr);
	return retStr;
}

/**
 * Method: TString::Tokenize
 * Purpose: Allows the Traditional Tokenization routine to be performed in the TString
 * Parameters: TString& tokens - characters to split by
 *				 int& start - the location to begin at
 * Returns:
 */
TString TString::Tokenize(TString& tokens, int& start, bool outOfQuotes) const
{
	TObjectLocker threadLock(&thread);
	int end;
	if (!outOfQuotes)
	{
		while ((end = FindOneOf(tokens, start)) == start)
		{
			start++;
		}
	}
	else
	{
		while ((end = FindOneOfOutOfQuotes(tokens, start)) == start)
			start++;
	}

	TString ret = SubString(start, end);

	start = end;


	return ret;
}

TString::TConstBuffer::TConstBuffer(const TString* string)
{
	if (!string)
		throw L"Null Pointer Exception";
	this->string = string;
	string->ThreadLock();
}

TString::TConstBuffer::TConstBuffer(const TConstBuffer& buff)
{
	this->string = buff.string;
	string->ThreadLock();
}

TString::TConstBuffer::~TConstBuffer()
{
	string->ThreadRelease();
}

const WCHAR* TString::TConstBuffer::getBuffer()
{
	return string->string;
}
