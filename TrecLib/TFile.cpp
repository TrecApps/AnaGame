
#include "TFile.h"
#include "TString.h"

UCHAR TFileType[] = { 2, 0b10000000, 2 };


/**
 * Method: TFile::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TFile::GetType() 
{
	return TString(L"TFile;") + TObject::GetType();
}

/*
* Method: TFile::TFile 
* Purpose: Default Constrictor
* Parameters: void
* Returns: void
*/

TFile::TFile()
{
	fileEncode = FileEncodingType::fet_unknown;
	fileHandle = 0;
	position = 0ULL;
}

/*
* Method: TFile::TFile
* Purpose: Constructor
* Parameters: LPCTSTR lpszFileName - the File name to open
*			UINT nOpenFlags - flags that specify the open status of the file
* Returns: void
*/
TFile::TFile(const TString& lpszFileName, UINT nOpenFlags) 
{
	Open(lpszFileName, nOpenFlags);
}


/*
* Method: TFile::~TFile
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TFile::~TFile()
{
	Close();
}

/*
* Method: TFile::Open
* Purpose: Opens the File
* Parameters: LPCTSTR lpszFileName - The File to open
*			UINT nOpenFlags - Flags to open the file
* Returns: bool - success or failure to open file
*/
bool TFile::Open(const TString& lpszFileName, UINT nOpenFlags)
{
	AG_THREAD_LOCK
	fileEncode = FileEncodingType::fet_unknown;

	UINT readWrite = 0, sharing = 0, atts = 0;
	ConvertFlags(nOpenFlags, readWrite, sharing, atts);

	// If no attribute for opening is specified, use the value most likely to succeed
	if (!atts)
		atts = OPEN_ALWAYS;
	TString newFileName(lpszFileName);
	fileHandle = CreateFileW(newFileName.GetConstantBuffer().getBuffer(), readWrite, sharing, nullptr, atts, FILE_ATTRIBUTE_NORMAL, nullptr);

	
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		fileHandle = 0;
		int err = GetLastError();
		RETURN_THREAD_UNLOCK false;
	}

	// Here, the file is open, try to deduce the file type
	fileEncode = DeduceEncodingType();
	filePath.Set(lpszFileName);
	RETURN_THREAD_UNLOCK true;
}





/**
 * Method: TFile::ReadString
 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line
 * Parameters: TString& rString - the String to read into
 * Returns: bool - success of reading
 */
BOOL TFile::ReadString(TString & rString)
{
	AG_THREAD_LOCK
	bool success = false;
	rString.Empty();
	char letter[1];
	UCHAR letter2[2];
	WCHAR cLetter;
	UCHAR temp;
	WCHAR wLetter;

	char uni8[4];
	UCHAR bytes;

	WCHAR* wideString = nullptr;

	std::string charHolder;

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		
		while (Read(&letter[0], 1))
		{
			success = true;
			if (letter[0] == '\n')
				break;
			rString.AppendChar(ReturnWCharType(letter[0]));
		}
		
		break;
	case FileEncodingType::fet_unicode:
		
		while (Read(letter2, 2))
		{
			success = true;
			temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			if (cLetter == L'\n')
				break;
			rString.AppendChar(cLetter);
		}

		break;
	case FileEncodingType::fet_unicode_little:
		
		while (Read(&wLetter, 2))
		{
			success = true;
			if (wLetter == L'\n')
				break;
			rString.AppendChar(wLetter);
		}
		break;
	case FileEncodingType::fet_unicode8:
		ZeroMemory(uni8, sizeof(char) * 4);
		while (bytes = ReadUnicode8Char(uni8))
		{
			success = true;
			if (uni8[0] == '\n')
				break;

			for (UINT Rust = 0; bytes; bytes--, Rust++)
			{
				charHolder.push_back(uni8[Rust]);
				uni8[Rust] = '\0';
			}
		}

		wideString = new WCHAR[charHolder.size() * 2];
		ZeroMemory(wideString, sizeof(WCHAR) * charHolder.size() * 2);

		MultiByteToWideChar(CP_UTF8, 0, charHolder.c_str(), charHolder.size(), wideString, charHolder.size() * 2);
		rString.Set(wideString);
		delete[] wideString;
		wideString = nullptr;
	}
	RETURN_THREAD_UNLOCK success;
}


/**
 * Method: TFile::ReadString
 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line 
 *			Or when the specificed number of characters are read
 * Parameters: TString& rString - the String to read into
 *				ULONGLONG nMax - max number of characters to read
 * Returns: ULONGLONG - How many characgers were read
 */
ULONGLONG TFile::ReadString(TString & rString, ULONGLONG nMax)
{
	AG_THREAD_LOCK
	rString.Empty();
	ULONGLONG rust = 0;

	if (fileEncode == FileEncodingType::fet_unknown)
		DeduceEncodingType();

	char uni8[4];
	UCHAR bytes;

	WCHAR* wideString = nullptr;

	std::string charHolder;

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		char letter[1];
		for ( ; rust <= nMax && Read(&letter, 1); rust+=1)
		{
			rString.AppendChar(ReturnWCharType(letter[0]));
		}

		break;
	case FileEncodingType::fet_unicode:
		UCHAR letter2[2];
		
		for (; rust <= nMax && Read(&letter2, 2); rust+=2)
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			
			rString.AppendChar(cLetter);
		}

		break;
	case FileEncodingType::fet_unicode_little:
		WCHAR wLetter;
		
		for( ; rust <= nMax && Read(&wLetter, 2); rust+=2 )
		{
			rString.AppendChar(wLetter);
		}
		break;
	case FileEncodingType::fet_unicode8:
		ZeroMemory(uni8, sizeof(char) * 4);
		while (bytes = ReadUnicode8Char(uni8) && rust <= nMax)
		{
			rust += bytes;

			for (UINT Rust = 0; bytes; bytes--, Rust++)
			{
				charHolder.push_back(uni8[Rust]);
				uni8[Rust] = '\0';
			}
		}

		wideString = new WCHAR[charHolder.size() * 2];
		ZeroMemory(wideString, sizeof(WCHAR) * charHolder.size() * 2);

		MultiByteToWideChar(CP_UTF8, 0, charHolder.c_str(), charHolder.size(), wideString, charHolder.size() * 2);
		rString.Set(wideString);
		delete[] wideString;
		wideString = nullptr;
	}
	RETURN_THREAD_UNLOCK rust;
}


ULONGLONG TFile::ReadStringLine(TString& rString, ULONGLONG nMax)
{
	AG_THREAD_LOCK
	rString.Empty();
	ULONGLONG rust = 0;

	if (fileEncode == FileEncodingType::fet_unknown)
		DeduceEncodingType();

	char uni8[4];
	UCHAR bytes;

	WCHAR* wideString = nullptr;

	std::string charHolder;

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		char letter[1];
		for (; rust <= nMax && Read(&letter, 1) && letter[0] != '\n'; rust++)
		{
			rString.AppendChar(ReturnWCharType(letter[0]));
		}

		break;
	case FileEncodingType::fet_unicode:
		UCHAR letter2[2];

		for (; rust <= nMax && Read(&letter2, 2); rust+=2)
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);

			if (cLetter == L'\n')
			{
				rust += 2;
				break;
			}
			rString.AppendChar(cLetter);
		}

		break;
	case FileEncodingType::fet_unicode_little:
		WCHAR wLetter;

		for (; rust < nMax && Read(&wLetter, 2); rust+=2)
		{
			if (wLetter == L'\n')
			{
				rust += 2;
				break;
			}
			rString.AppendChar(wLetter);
		}
		break;
	case FileEncodingType::fet_unicode8:
		ZeroMemory(uni8, sizeof(char) * 4);
		while (bytes = ReadUnicode8Char(uni8) && rust <= nMax)
		{
			rust += bytes;
			if (uni8[0] == '\n')
				break;

			for (UINT Rust = 0; bytes; bytes--, Rust++)
			{
				charHolder.push_back(uni8[Rust]);
				uni8[Rust] = '\0';
			}
		}

		wideString = new WCHAR[charHolder.size() * 2];
		ZeroMemory(wideString, sizeof(WCHAR) * charHolder.size() * 2);

		MultiByteToWideChar(CP_UTF8, 0, charHolder.c_str(), charHolder.size(), wideString, charHolder.size() * 2);
		rString.Set(wideString);
		delete[] wideString;
		wideString = nullptr;
	}
	RETURN_THREAD_UNLOCK rust;
}

/**
 * Method: TFile::ReadString
 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the specified chara
 * Parameters: TString& rString - the String to read into
 *				WCHAR chara - the character to stop at
 * Returns: bool - success of reading
 */
UINT TFile::ReadString(TString & rString, WCHAR chara)
{
	AG_THREAD_LOCK
	bool success = false;
	rString.Empty();

	char uni8[4];
	UCHAR bytes;

	WCHAR* wideString = nullptr;

	WCHAR charaComp;

	std::string charHolder;

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		char letter[1];
		while (Read(&letter, 1))
		{
			if (letter[0] == chara)
				break;
			rString.AppendChar(ReturnWCharType(letter[0]));
			success = true;
		}

		break;
	case FileEncodingType::fet_unicode:
		UCHAR letter2[2];
		while (Read(&letter2, 2))
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			if (cLetter == chara)
				break;
			rString.AppendChar( cLetter);
			success = true;
		}

		break;
	case FileEncodingType::fet_unicode_little:
		WCHAR wLetter;
		while (Read(&wLetter, 2))
		{
			if (wLetter == chara)
				break;
			rString.AppendChar(wLetter);
			success = true;
		}
		break;
	case FileEncodingType::fet_unicode8:
		ZeroMemory(uni8, sizeof(char) * 4);
		while (bytes = ReadUnicode8Char(uni8))
		{
			
			if (MultiByteToWideChar(CP_UTF8, 0, uni8, bytes, &charaComp, 1) && chara == charaComp)
				break;

			for (UINT Rust = 0; bytes; bytes--, Rust++)
			{
				charHolder.push_back(uni8[Rust]);
				uni8[Rust] = '\0';
			}
		}

		wideString = new WCHAR[charHolder.size() * 2];
		ZeroMemory(wideString, sizeof(WCHAR) * charHolder.size() * 2);

		MultiByteToWideChar(CP_UTF8, 0, charHolder.c_str(), charHolder.size(), wideString, charHolder.size() * 2);
		rString.Set(wideString);
		delete[] wideString;
		wideString = nullptr;
	}
	RETURN_THREAD_UNLOCK rString.GetSize();
}

/**
 * Method: TFile::ReadString
 * Purpose: Reads the file up to one of the provided characters
 * Parameters: TString& rString - the string to retun
 *				TString& chars - the characters to stop at
 *				UCHAR flags - flags influence the behavior of this method
 *				UINT max - max number of bytes to read (0 for no maximum)
 * Returns: UINT - the size of the resulting string
 *
 * Note: Written with Source code interpretation in mind
 * Flags variable values:
 *		0b00000001 - TFile::include_end - include the terminating character in the return String
 *      0b00000010 - TFile::out_of_quotes - makesure that when we do find the characters, they are outside of quotes
 *      0booooo100 - TFile::watch_backslash - factor backslashes in handling the other flags
 */
UINT TFile::ReadString(TString& rString, const TString& chars, UCHAR flags, UINT max)
{
	AG_THREAD_LOCK
	bool success = false;
	rString.Empty();
	char letter[1] = { '\0' };

	WCHAR quote = L'\0';

	UINT backslashes = 0;

	bool maxSet = max > 0;
	WCHAR charaComp;

	char uni8[4];
	UCHAR bytes;

	WCHAR* wideString = nullptr;


	std::string charHolder;

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		
		while (Read(&letter, 1))
		{
			// If we care about backslashes and we encounter one on this character, increase the count
			if (flags & 0b00000100)
			{
				if (letter[0] == '\\')
					backslashes++;
				// else backslashes = 0;
			}

			// If we don't care about backslashes or we have an od number of them, they they don't bother us for these operations
			if ((flags & 0b00000100) == 0 || backslashes % 2 == 0)
			{
				// If we care about making sure the final character is out of quotes, check to make sure we are, in fact, out of them
				if (flags & 0b00000010)
				{
					if (letter[0] == '\'')
					{
						if (quote == L'\'')
							quote = 0;
						else if (!quote)
							quote = L'\'';
					}
					else if (letter[0] == '\"')
					{
						if (quote == L'\"')
							quote = 0;
						else if (!quote)
							quote = L'\"';
					}
				}

				// if we don't care about quotes or we are out of them, then check to see if we reached a terminating character
				if ((!(flags & 0b00000010) || !quote) && chars.Find(letter[0]) != 1)
				{
					// If we want to add the terminating character, then do so here
					if (flags & 0b00000001)
						rString.AppendChar(ReturnWCharType(letter[0]));
					break;
				}
			}

			// Odd # of backslashes will affect the first non-backslash character, so process it first before resetting the backslash count
			if (flags & 0b00000100)
			{
				if (letter[0] != '\\')
					backslashes = 0;
			}

			rString.AppendChar(ReturnWCharType(letter[0]));
			success = true;

			if (maxSet && !(--max))
			{
				break;
			}

		}

		break;
	case FileEncodingType::fet_unicode:
		UCHAR letter2[2];
		while (Read(&letter2, 2))
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);



			// If we care about backslashes and we encounter one on this character, increase the count
			if (flags & 0b00000100)
			{
				if (cLetter == '\\')
					backslashes++;
				// else backslashes = 0;
			}

			// If we don't care about backslashes or we have an od number of them, they they don't bother us for these operations
			if ((flags & 0b00000100) == 0 || backslashes % 2 == 0)
			{
				// If we care about making sure the final character is out of quotes, check to make sure we are, in fact, out of them
				if (flags & 0b00000010)
				{
					if (cLetter == '\'')
					{
						if (quote == L'\'')
							quote = 0;
						else if (!quote)
							quote = L'\'';
					}
					else if (cLetter == '\"')
					{
						if (quote == L'\"')
							quote = 0;
						else if (!quote)
							quote = L'\"';
					}
				}

				// if we don't care about quotes or we are out of them, then check to see if we reached a terminating character
				if ((!(flags & 0b00000010) || !quote) && chars.Find(cLetter) != 1)
				{



					if (chars.Find(cLetter) != -1)
					{
						if (flags & 0b00000001)
							rString.AppendChar(cLetter);
						break;
					}
				}
			}

			// Odd # of backslashes will affect the first non-backslash character, so process it first before resetting the backslash count
			if (flags & 0b00000100)
			{
				if (cLetter != '\\')
					backslashes = 0;
			}

			rString.AppendChar(cLetter);
			success = true;

			if (maxSet)
			{
				max -= 2;
				if (!max) break;
			}

		}

		break;
	case FileEncodingType::fet_unicode_little:
		WCHAR wLetter;
		while (Read(&wLetter, 2))
		{
			// If we care about backslashes and we encounter one on this character, increase the count
			if (flags & 0b00000100)
			{
				if (wLetter == '\\')
					backslashes++;
				// else backslashes = 0;
			}

			// If we don't care about backslashes or we have an od number of them, they they don't bother us for these operations
			if ((flags & 0b00000100) == 0 || backslashes % 2 == 0)
			{
				// If we care about making sure the final character is out of quotes, check to make sure we are, in fact, out of them
				if (flags & 0b00000010)
				{
					if (wLetter == '\'')
					{
						if (quote == L'\'')
							quote = 0;
						else if (!quote)
							quote = L'\'';
					}
					else if (wLetter == '\"')
					{
						if (quote == L'\"')
							quote = 0;
						else if (!quote)
							quote = L'\"';
					}
				}

				// if we don't care about quotes or we are out of them, then check to see if we reached a terminating character
				if ((!(flags & 0b00000010) || quote==0) && chars.Find(wLetter) != -1)
				{


					if (chars.Find(wLetter) != -1)
					{
						if (flags & 0b00000001)
							rString.AppendChar(wLetter);
						break;
					}
				}
			}

			// Odd # of backslashes will affect the first non-backslash character, so process it first before resetting the backslash count
			if (flags & 0b00000100)
			{
				if (wLetter != '\\')
					backslashes = 0;
			}
				
			rString.AppendChar(wLetter);
			success = true;
			if (maxSet)
			{
				max -= 2;
				if (!max || max == UINT32_MAX) break;
			}
		}
		break;
	case FileEncodingType::fet_unicode8:
		ZeroMemory(uni8, sizeof(char) * 4);
		while (bytes = ReadUnicode8Char(uni8))
		{

			UINT worked = MultiByteToWideChar(CP_UTF8, 0, uni8, bytes, &charaComp, 1);
			
			// If we care about backslashes and we encounter one on this character, increase the count
			if (worked )
			{
				if(flags & 0b00000100)
				{
					if (charaComp == '\\')
					backslashes++;
				// else backslashes = 0;
				}

				// If we don't care about backslashes or we have an od number of them, they they don't bother us for these operations
				if ((flags & 0b00000100) == 0 || backslashes % 2 == 0)
				{
					// If we care about making sure the final character is out of quotes, check to make sure we are, in fact, out of them
					if (flags & 0b00000010)
					{
						if (charaComp == '\'')
						{
							if (quote == L'\'')
								quote = 0;
							else if (!quote)
								quote = L'\'';
						}
						else if (charaComp == '\"')
						{
							if (quote == L'\"')
								quote = 0;
							else if (!quote)
								quote = L'\"';
						}
					}

					// if we don't care about quotes or we are out of them, then check to see if we reached a terminating character
					if ((!(flags & 0b00000010) || quote == 0) && chars.Find(charaComp) != -1)
					{


						if (chars.Find(charaComp) != -1)
						{
							if (flags & 0b00000001)
								for (UINT Rust = 0; bytes; bytes--, Rust++)
									charHolder.push_back(uni8[Rust]);
							break;
						}
					}
				}

				// Odd # of backslashes will affect the first non-backslash character, so process it first before resetting the backslash count
				if (flags & 0b00000100)
				{
					if (charaComp != '\\')
						backslashes = 0;
				}

			} // if worked
			success = true;

			bool reachedMax = false;

			if (maxSet)
			{
				max -= bytes;
				if (!max || max == UINT32_MAX)reachedMax = true;
			}

			for (UINT Rust = 0; bytes; bytes--, Rust++)
			{
				charHolder.push_back(uni8[Rust]);
				uni8[Rust] = '\0';
			}

			if (reachedMax)
				break;
		}

		wideString = new WCHAR[charHolder.size() * 2];
		ZeroMemory(wideString, sizeof(WCHAR)* charHolder.size() * 2);

		MultiByteToWideChar(CP_UTF8, 0, charHolder.c_str(), charHolder.size(), wideString, charHolder.size() * 2);
		rString.Set(wideString);
		delete[] wideString;
		wideString = nullptr;

	}
	RETURN_THREAD_UNLOCK rString.GetSize();
}

/*
* Method: TFile::WriteString
* Purpose: Wrties a string to the file
* Parameters: LPCTSTR lpsz - the Stringt to write
* Returns: void
*/
void TFile::WriteString(const TString& lpsz)
{
	AG_THREAD_LOCK
	UINT size = 0;
	CHAR* acsiiText = nullptr;
	UINT wBytes = 0;
	WCHAR cLetter = L'\0';
	UCHAR bytes[2];
	UCHAR temp = 0;

	TString newParam(lpsz);

	if (fileEncode == FileEncodingType::fet_unknown)
		fileEncode = FileEncodingType::fet_unicode_little;
	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
	case FileEncodingType::fet_unicode8:
		size = lpsz.GetSize();
		acsiiText = new CHAR[size * 2 + 1];
		wBytes = WideCharToMultiByte(CP_ACP,
			0, newParam.GetConstantBuffer().getBuffer(), -1,
			acsiiText, size * 2, NULL,
			NULL);
		Write(acsiiText, wBytes);
		break;
	case FileEncodingType::fet_unicode:
		for (UINT c = 0; lpsz[c] != L'\0'; c++)
		{
			cLetter = lpsz[c];
			
			memcpy(bytes, &cLetter, 2);
			temp = bytes[0];
			bytes[0] = bytes[1];
			bytes[1] = temp;
			Write(bytes, 2);
		}
		break;
	case FileEncodingType::fet_unicode_little:
		
		Write(newParam.GetConstantBuffer().getBuffer(), lpsz.GetSize() * sizeof(WCHAR));
		
	}
	RETURN_THREAD_UNLOCK;
}

/*
* Method: TFile::IsOpen
* Purpose: Reports whether the File is open or not
* Parameters: void
* Returns: bool - is the file open
*/
bool TFile::IsOpen() const
{
	AG_THREAD_LOCK
	bool ret = fileHandle != 0;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFile::SetEncoding
* Purpose: Sets the encoding of the File (if not already set)
* Parameters: FileEncodingType fet - the Encoding type to do
* Returns: bool - whether the file encoding was set
*/
bool TFile::SetEncoding(FileEncodingType fet)
{
	AG_THREAD_LOCK
	if (fileEncode == FileEncodingType::fet_unknown && fet != FileEncodingType::fet_unknown)
	{
		fileEncode = fet;
		RETURN_THREAD_UNLOCK true;
	}
	RETURN_THREAD_UNLOCK false;
}

/*
* Method: TFile::GetFileDirectory
* Purpose: Retrieves the directory of the file
* Parameters: void
* Returns: TString - the directory path of the file
*/
TString TFile::GetFileDirectory()
{
	AG_THREAD_LOCK
	TString sep(L"/\\");
	int seperate = filePath.FindLastOneOf(sep);

	if (seperate == -1)
	{
		RETURN_THREAD_UNLOCK TString();
	}
	RETURN_THREAD_UNLOCK filePath.SubString(0, seperate + 1);
}

/*
* Method: TFile::GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type 
* 
* Note: deprecated
*/
UCHAR * TFile::GetAnaGameType()
{
	return TFileType;
}


/**
 * Method: TFile::GetFileExtension
 * Purpose: Retrievs the extension of the file
 * Parameters: voif
 * Returns: TString - the file extension detected
 */
TString TFile::GetFileExtension()
{
	AG_THREAD_LOCK
	TString ext = GetFileName();
	if (ext.Find(L'.') == -1)
	{
		RETURN_THREAD_UNLOCK ext;
	}
	for (int c = ext.GetSize() - 1; c >= 0; c--)
	{
		if (ext[c] == L'.')
		{
			RETURN_THREAD_UNLOCK ext.SubString(c + 1);
		}
	}
	RETURN_THREAD_UNLOCK ext;
}
/**
 * Method: TFile::Close
 * Purpose: Closes the file
 * Parameters: void
 * Returns: void
 */
void TFile::Close()
{
	AG_THREAD_LOCK
	if (fileHandle)
		CloseHandle((HANDLE)fileHandle);
	fileHandle = 0;
	RETURN_THREAD_UNLOCK;
}
/**
 * Method: TFile::Flush
 * Purpose: Flushes the file's internal buffer
 * Parameters: void
 * Returns: void
 */
void TFile::Flush()
{
	AG_THREAD_LOCK
	if (!fileHandle)
	{
		RETURN_THREAD_UNLOCK;
	}
	FlushFileBuffers((HANDLE)fileHandle);
	RETURN_THREAD_UNLOCK;
}
/**
 * Method: TFile::Write
 * Purpose: Writes a set of bytes to the file
 * Parameters: const void* buffer - pointer to bytes
 *				UINT count - the number of bytes to write
 * Returns: void
 */
void TFile::Write(const void* buffer, UINT count)
{
	AG_THREAD_LOCK
	if (!fileHandle)
	{
		RETURN_THREAD_UNLOCK;
	}
	LPDWORD resCount = new DWORD;
	LPDWORD resCount2 = resCount;
	LPOVERLAPPED lap = new OVERLAPPED;
	LPOVERLAPPED lap2 = lap;
	BOOL res = WriteFile((HANDLE)fileHandle, buffer, count, resCount, nullptr);

	if (!res)
	{
		int err = GetLastError();
		delete resCount2;
		delete lap2;
		RETURN_THREAD_UNLOCK;
	}
	position += *resCount;

	delete resCount2;
	delete lap2;
	RETURN_THREAD_UNLOCK;
}
/**
 * Method: TFile::GetEncodingType
 * Purpose: Retriev the file encoding mode
 * Parameters: void
 * Returns: FileEncodingType - the type of encoding the file is set to use
 */
FileEncodingType TFile::GetEncodingType()
{
	return fileEncode;
}

/*
* Method: TFile::DeduceEncodingType
* Purpose: Attempts to determine the encoding type of the file if possible
* Parameters: void
* Returns: FileEncodingType - the File Encoding type deduced
*/
FileEncodingType TFile::DeduceEncodingType()
{
	if (!IsOpen())
		return FileEncodingType::fet_unknown;
	if (GetLength() < 2)
		return FileEncodingType::fet_unknown;
	AG_THREAD_LOCK
	UCHAR twoBytes[30];
	UINT bytes = Read(&twoBytes, 30);

	SeekToBegin();

	int value = IS_TEXT_UNICODE_UNICODE_MASK |
		IS_TEXT_UNICODE_REVERSE_MASK |
		IS_TEXT_UNICODE_NOT_UNICODE_MASK |
		IS_TEXT_UNICODE_NOT_ASCII_MASK;


	if (!IsTextUnicode(&twoBytes, bytes, &value))
	{
		// Tests have failed, so we have to rely on our own assessment

		if (bytes >= 5)
		{
			// Test for UTF-7
			if (twoBytes[0] == 0x2b &&
				twoBytes[1] == 0x2f &&
				twoBytes[2] == 0x76 &&
				twoBytes[3] == 0x38 &&
				twoBytes[4] == 0x2d)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode7;
			}
			// Now check for UTF-8
			if (twoBytes[0] == 0xef &&
				twoBytes[1] == 0xbb &&
				twoBytes[2] == 0xbf)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode8;
			}
			// Check for big endian unicode
			if (twoBytes[0] == 0xfe &&
				twoBytes[1] == 0xff)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode;
			}
			// Check for little-endian unicode
			if (twoBytes[0] == 0xff &&
				twoBytes[1] == 0xfe)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode_little;
			}

			// There is no BOM to tell us the encoding, need to guess
			// Check to see if it is big-endian the old fashioned way
			if (twoBytes[0] == 0x00 &&
				twoBytes[2] == 0x00 &&
				twoBytes[4] == 0x00)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode;
			}
			if (twoBytes[1] == 0x00 &&
				twoBytes[3] == 0x00)
			{
				RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode_little;
			}
			RETURN_THREAD_UNLOCK FileEncodingType::fet_acsii;
		}
		else
		{
			RETURN_THREAD_UNLOCK FileEncodingType::fet_unknown;
		}
	}
	else
	{
		if (value & IS_TEXT_UNICODE_STATISTICS ||
			value & IS_TEXT_UNICODE_CONTROLS ||
			value & IS_TEXT_UNICODE_ASCII16)
		{
			RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode_little;
		}
		if (value & IS_TEXT_UNICODE_REVERSE_STATISTICS ||
			value & IS_TEXT_UNICODE_REVERSE_CONTROLS ||
			value & IS_TEXT_UNICODE_REVERSE_ASCII16)
		{
			RETURN_THREAD_UNLOCK FileEncodingType::fet_unicode;
		}
		if (value & IS_TEXT_UNICODE_NOT_UNICODE_MASK)
		{
			RETURN_THREAD_UNLOCK FileEncodingType::fet_acsii;
		}
	}
	RETURN_THREAD_UNLOCK FileEncodingType::fet_unknown;
}
/**
 * Method: TFile::ConvertFlags
 * Purpose: Used internally by the class to convert TFile:: flags into Windows Crreate flags
 * Parameters: UINT& input - the flags to convert
 *				 UINT& open - basic open flags to send to Windows
 *				 UINT& security - share attributes to send to windows
 *				 UINT& creation - create instruction flags for Windows
 * Returns: void 
 */
void TFile::ConvertFlags(UINT& input, UINT& open, UINT& security, UINT& creation)
{
	open = input & 0xff000000;

	security = (input >> 8) & 0x000000ff;
	creation = (input >> 16) & 0x000000ff;
}

UCHAR TFile::ReadUnicode8Char(char* seq4)
{
	if(!Read(seq4,1))
	return 0;
	AG_THREAD_LOCK
	if ((seq4[0] & 0b11110000) == 0b11110000)
	{
		// we are dealing with a 4 byte sequence in UTF-8
		// Already have the first byte, now get the other three
		RETURN_THREAD_UNLOCK 1 + Read(&seq4[1], 3);
	}

	if ((seq4[0] & 0b11100000) == 0b11100000)
	{
		// At this point, we are dealing with a 3 byte sequence,
		// Get the other two
		RETURN_THREAD_UNLOCK 1 + Read(&seq4[1], 2);
	}

	if ((seq4[0] & 0b11000000) == 0b11000000)
	{
		// Just a two byte sequence
		// Get the second byte
		RETURN_THREAD_UNLOCK 1 + Read(&seq4[1], 1);
	}
	// just one byte
	RETURN_THREAD_UNLOCK 1;
}

/**
 * Method: TFile::GetFileName
 * Purpose: Gets the name of the file
 * Parameters: void
 * Returns: TString - the name of the file (minus the path)
 */
TString TFile::GetFileName() const
{
	AG_THREAD_LOCK
	TString sep(L"/\\");
	int seperate = filePath.FindLastOneOf(sep);

	if (seperate == -1)
	{
		RETURN_THREAD_UNLOCK TString();
	}
	RETURN_THREAD_UNLOCK filePath.SubString(seperate + 1);
}

/**
 * Method: TFile::GetFilePath
 * Purpose: Retrievs the fill path of the file
 * Parameters: void
 * Returns: TString - the path of the file
 */
TString TFile::GetFilePath() const
{
	return filePath;
}
/**
 * Method: TFile::GetFileTitle
 * Purpose: Retirevs the "title" of the file
 * Parameters: void
 * Returns: TString - the title of the file
 */
TString TFile::GetFileTitle() const
{
	AG_THREAD_LOCK
	WCHAR* cTitle = new WCHAR[filePath.GetSize() + 1];
	ZeroMemory(cTitle, sizeof(WCHAR) * (filePath.GetSize() + 1));

	TString ret;
	TString newPath(filePath);
	if (!::GetFileTitleW(newPath.GetConstantBuffer().getBuffer(), cTitle, filePath.GetSize()))
	{
		ret.Set(cTitle);
	}

	delete[] cTitle;
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TFile::GetLength
 * Purpose: Retirevs the current size of the file
 * Parameters: void
 * Returns: ULONGLONG - length of the file
 */
ULONGLONG TFile::GetLength() const
{
	AG_THREAD_LOCK
	LARGE_INTEGER  len_li;
	GetFileSizeEx((HANDLE)fileHandle, &len_li);
	LONGLONG  len_ll = len_li.QuadPart;
	RETURN_THREAD_UNLOCK len_ll;
}

/**
 * Method: TFile::GetPosition
 * Purpose: Retrievs the current position of the file pointer
 * Parameters: void 
 * Returns: ULONGLONG -  the current position of the file pointer
 */
ULONGLONG TFile::GetPosition() const
{
	return position;
}

/**
 * Method: TFile::Read
 * Purpose: Reads a given number of bytes
 * Parameters: void* buffer - pointer to the buffer 
 *				UINT count - number of bytes to read
 * Returns: UINT - number of bytes read
 */
UINT TFile::Read(void* buffer, UINT count)
{
	AG_THREAD_LOCK
	if (!fileHandle)
	{
		RETURN_THREAD_UNLOCK 0;
	}
	LPDWORD resCount = new DWORD;
	LPDWORD resCount2 = resCount;
	LPOVERLAPPED lap = new _OVERLAPPED;
	LPOVERLAPPED lap2 = lap;
	BOOL res = ReadFile((HANDLE)fileHandle, buffer, count, resCount, nullptr);

	if (!res)
	{
		int e = GetLastError();
		delete resCount;
		delete lap2;
		RETURN_THREAD_UNLOCK 0;
	}
	position += *resCount;

	DWORD stackResCount = *resCount;

	delete resCount;
	delete lap2;
	RETURN_THREAD_UNLOCK stackResCount;
}
/**
 * Method: TFile::Seek
 * Purpose: Sets the file's seek point according to the users specification
 * Parameters: LONGLONG offset - the offset for the seek point
 *				 UINT from - ofset from where? (0 for beginning, 1 for current point, 2 for end)
 * Returns: ULONGLONG -  the position of the file's seek point
 */
ULONGLONG TFile::Seek(LONGLONG offset, UINT from)
{
	AG_THREAD_LOCK
	if (!fileHandle) {
		RETURN_THREAD_UNLOCK 0;
	}
	PLONG hZero = new LONG;
	*hZero = 0;
	PLONG store = hZero;
	DWORD res = SetFilePointer((HANDLE)fileHandle, offset, hZero, from);

	if (res != INVALID_SET_FILE_POINTER)
	{
		ULONGLONG upper = static_cast<ULONGLONG>(*hZero) << 32;

		position = upper + static_cast<ULONGLONG>(res);
	}
	else
	{
		// To-Do:
	}
	delete store;
	RETURN_THREAD_UNLOCK position;
}
/**
 * Method: TFile::SeekToBegin
 * Purpose: Sets the files seek point to the beginning
 * Parameters: void
 * Returns: void
 */
void TFile::SeekToBegin()
{
	AG_THREAD_LOCK
	if (!fileHandle)
	{
		RETURN_THREAD_UNLOCK;
	}
	PLONG hZero = new LONG;
	*hZero = 0;
	PLONG store = hZero;
	DWORD res = SetFilePointer((HANDLE)fileHandle, 0, hZero, FILE_BEGIN);

	if (res != INVALID_SET_FILE_POINTER)
	{
		position = 0;
	}
	else
	{
		// To-Do:
	}

	delete store;
	RETURN_THREAD_UNLOCK;
	//return position;
}

/**
 * Method: TFile::SeekToEnd
 * Purpose: Sets the seek to the end of the file
 * Parameters: void
 * Returns: ULONGLONG - the current seek of the file
 */
ULONGLONG TFile::SeekToEnd()
{
	AG_THREAD_LOCK
	if (!fileHandle)
	{
		RETURN_THREAD_UNLOCK 0;
	}
	PLONG hZero = new LONG;
	*hZero = 0;
	PLONG store = hZero;
	DWORD res = SetFilePointer((HANDLE)fileHandle, 0, hZero, FILE_END);

	if (res != INVALID_SET_FILE_POINTER)
	{
		ULONGLONG upper = static_cast<ULONGLONG>(*hZero) << 32;

		position = upper + static_cast<ULONGLONG>(res);
	}
	else
	{
		// To-Do:
	}

	delete store;

	RETURN_THREAD_UNLOCK position;
}
