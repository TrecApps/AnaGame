
#include "TFile.h"
#include "TString.h"

UCHAR TFileType[] = { 2, 0b10000000, 2 };

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
	fileEncode = FileEncodingType::fet_unknown;

	UINT readWrite = 0, sharing = 0, atts = 0;
	ConvertFlags(nOpenFlags, readWrite, sharing, atts);

	// If no attribute for opening is specified, use the value most likely to succeed
	if (!atts)
		atts = OPEN_ALWAYS;
	fileHandle = CreateFileW(lpszFileName.GetConstantBuffer(), readWrite, sharing, nullptr, atts, FILE_ATTRIBUTE_NORMAL, nullptr);

	
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		fileHandle = 0;
		int err = GetLastError();
		return false;
	}

	// Here, the file is open, try to deduce the file type
	fileEncode = DeduceEncodingType();
	filePath.Set(lpszFileName);
	return true;
}





/**
 * Method: TFile::ReadString
 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line
 * Parameters: TString& rString - the String to read into
 * Returns: bool - success of reading
 */
BOOL TFile::ReadString(TString & rString)
{
	bool success = false;
	rString.Empty();
	char letter[1];
	UCHAR letter2[2];
	WCHAR cLetter;
	UCHAR temp;
	WCHAR wLetter;
	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		
		while (Read(&letter[0], 1))
		{
			if (letter[0] == '\n')
				break;
			rString.AppendChar(ReturnWCharType(letter[0]));
			success = true;
		}
		
		break;
	case FileEncodingType::fet_unicode:
		
		while (Read(letter2, 2))
		{
			temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			if (cLetter == L'\n')
				break;
			rString.AppendChar(cLetter);
			success = true;
		}

		break;
	case FileEncodingType::fet_unicode_little:
		
		while (Read(&wLetter, 2))
		{
			if (wLetter == L'\n')
				break;
			rString.AppendChar(wLetter);
			success = true;
		}

	}
	return success;
}


/**
 * Method: TFile::ReadString
 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line 
 *			Or when the specificed number of characters are read
 * Parameters: TString& rString - the String to read into
 *				UINT nMax - max number of characters to read
 * Returns: bool - success of reading
 */
UINT TFile::ReadString(TString & rString, UINT nMax)
{
	rString.Empty();
	UINT rust = 0;

	if (fileEncode == FileEncodingType::fet_unknown)
		DeduceEncodingType();

	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
		char letter[1];
		for ( ; rust <= nMax && Read(&letter, 1); rust++)
		{
			rString.AppendChar(ReturnWCharType(letter[0]));
		}

		break;
	case FileEncodingType::fet_unicode:
		UCHAR letter2[2];
		
		for (; rust <= nMax && Read(&letter2, 2); rust++)
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
		
		for( ; rust <= nMax && Read(&wLetter, 2); rust++ )
		{
			rString.AppendChar(wLetter);
		}
		
	}
	return rust;
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
	bool success = false;
	rString.Empty();
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

	}
	return rString.GetSize();
}

/*
* Method: TFile::WriteString
* Purpose: Wrties a string to the file
* Parameters: LPCTSTR lpsz - the Stringt to write
* Returns: void
*/
void TFile::WriteString(const TString& lpsz)
{
	UINT size = 0;
	CHAR* acsiiText = nullptr;
	UINT wBytes = 0;
	WCHAR cLetter = L'\0';
	UCHAR bytes[2];
	UCHAR temp = 0;
	if (fileEncode == FileEncodingType::fet_unknown)
		fileEncode = FileEncodingType::fet_unicode_little;
	switch (fileEncode)
	{
	case FileEncodingType::fet_acsii:
	case FileEncodingType::fet_unicode8:
		size = lpsz.GetSize();
		acsiiText = new CHAR[size * 2 + 1];
		wBytes = WideCharToMultiByte(CP_ACP,
			0, lpsz.GetConstantBuffer(), -1,
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
		
		Write(lpsz.GetConstantBuffer(), lpsz.GetSize() * sizeof(WCHAR));
		
	}

}

/*
* Method: TFile::IsOpen
* Purpose: Reports whether the File is open or not
* Parameters: void
* Returns: bool - is the file open
*/
bool TFile::IsOpen()
{
	return fileHandle != 0;
}

/*
* Method: TFile::SetEncoding
* Purpose: Sets the encoding of the File (if not already set)
* Parameters: FileEncodingType fet - the Encoding type to do
* Returns: bool - whether the file encoding was set
*/
bool TFile::SetEncoding(FileEncodingType fet)
{
	if (fileEncode == FileEncodingType::fet_unknown && fet != FileEncodingType::fet_unknown)
	{
		fileEncode = fet;
		return true;
	}
	return false;
}

/*
* Method: TFile::GetFileDirectory
* Purpose: Retrieves the directory of the file
* Parameters: void
* Returns: TString - the directory path of the file
*/
TString TFile::GetFileDirectory()
{
	TString sep(L"/\\");
	int seperate = filePath.FindLastOneOf(sep);

	if (seperate == -1)
		return TString();

	return filePath.SubString(0, seperate + 1);
}

/*
* Method: TFile::GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type 
* 
* Note: DEPRICATED
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
	TString ext = GetFileName();
	if(ext.Find(L'.') == -1)
		return ext;
	for (int c = ext.GetSize() - 1; c >= 0; c--)
	{
		if (ext[c] == L'.')
		{
			return ext.SubString(c + 1);
		}
	}
	return ext;
}
/**
 * Method: TFile::Close
 * Purpose: Closes the file
 * Parameters: void
 * Returns: void
 */
void TFile::Close()
{
	if (fileHandle)
		CloseHandle((HANDLE)fileHandle);
	fileHandle = 0;
}
/**
 * Method: TFile::Flush
 * Purpose: Flushes the file's internal buffer
 * Parameters: void
 * Returns: void
 */
void TFile::Flush()
{
	if (!fileHandle)
		return;
	FlushFileBuffers((HANDLE)fileHandle);
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
	if (!fileHandle)
		return;
	LPDWORD resCount = new DWORD;
	LPDWORD resCount2 = resCount;
	LPOVERLAPPED lap = new OVERLAPPED;
	LPOVERLAPPED lap2 = lap;
	BOOL res = WriteFile((HANDLE)fileHandle, buffer, count, resCount, nullptr);

	if (!res)
	{
		int err = GetLastError();
		return;
	}
	position += *resCount;

	delete resCount2;
	delete lap2;
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
				return FileEncodingType::fet_unicode7;

			// Now check for UTF-8
			if (twoBytes[0] == 0xef &&
				twoBytes[1] == 0xbb &&
				twoBytes[2] == 0xbf)
				return FileEncodingType::fet_unicode8;

			// Check for big endian unicode
			if (twoBytes[0] == 0xfe &&
				twoBytes[1] == 0xff)
				return FileEncodingType::fet_unicode;

			// Check for little-endian unicode
			if (twoBytes[0] == 0xff &&
				twoBytes[1] == 0xfe)
				return FileEncodingType::fet_unicode_little;
			

			// There is no BOM to tell us the encoding, need to guess
			// Check to see if it is big-endian the old fashioned way
			if (twoBytes[0] == 0x00 &&
				twoBytes[2] == 0x00 &&
				twoBytes[4] == 0x00)
				return FileEncodingType::fet_unicode;

			if (twoBytes[1] == 0x00 &&
				twoBytes[3] == 0x00)
				return FileEncodingType::fet_unicode_little;

			return FileEncodingType::fet_acsii;
		}
		else
			return FileEncodingType::fet_unknown;
	}
	else
	{
		if (value & IS_TEXT_UNICODE_STATISTICS ||
			value & IS_TEXT_UNICODE_CONTROLS ||
			value & IS_TEXT_UNICODE_ASCII16)
			return FileEncodingType::fet_unicode_little;

		if (value & IS_TEXT_UNICODE_REVERSE_STATISTICS ||
			value & IS_TEXT_UNICODE_REVERSE_CONTROLS ||
			value & IS_TEXT_UNICODE_REVERSE_ASCII16)
			return FileEncodingType::fet_unicode;

		if (value & IS_TEXT_UNICODE_NOT_UNICODE_MASK)
			return FileEncodingType::fet_acsii;
	}
	return FileEncodingType::fet_unknown;
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
/**
 * Method: TFile::GetFileName
 * Purpose: Gets the name of the file
 * Parameters: void
 * Returns: TString - the name of the file (minus the path)
 */
TString TFile::GetFileName()
{
	TString sep(L"/\\");
	int seperate = filePath.FindLastOneOf(sep);

	if(seperate == -1)
		return TString();

	return filePath.SubString(seperate + 1);
}
/**
 * Method: TFile::GetFilePath
 * Purpose: Retrievs the fill path of the file
 * Parameters: void
 * Returns: TString - the path of the file
 */
TString TFile::GetFilePath()
{
	return filePath;
}
/**
 * Method: TFile::GetFileTitle
 * Purpose: Retirevs the "title" of the file
 * Parameters: void
 * Returns: TString - the title of the file
 */
TString TFile::GetFileTitle()
{
	WCHAR* cTitle = new WCHAR[filePath.GetSize() + 1];
	ZeroMemory(cTitle, sizeof(WCHAR) * (filePath.GetSize() + 1));

	TString ret;
	if (!::GetFileTitleW(filePath.GetConstantBuffer(), cTitle, filePath.GetSize()))
	{
		ret.Set(cTitle);
	}

	delete[] cTitle;
	return ret;
}
/**
 * Method: TFile::GetLength
 * Purpose: Retirevs the current size of the file
 * Parameters: void
 * Returns: ULONGLONG - length of the file
 */
ULONGLONG TFile::GetLength()
{
	LARGE_INTEGER  len_li;
	GetFileSizeEx((HANDLE)fileHandle, &len_li);
	LONGLONG  len_ll = len_li.QuadPart;
	return len_ll;
}
/**
 * Method: TFile::GetPosition
 * Purpose: Retrievs the current position of the file pointer
 * Parameters: void 
 * Returns: ULONGLONG -  the current position of the file pointer
 */
ULONGLONG TFile::GetPosition()
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
	if(!fileHandle)
		return 0;
	LPDWORD resCount = new DWORD;
	LPDWORD resCount2 = resCount;
	LPOVERLAPPED lap = new _OVERLAPPED;
	LPOVERLAPPED lap2 = lap;
	BOOL res = ReadFile((HANDLE)fileHandle, buffer, count, resCount, nullptr);

	if (!res)
		return 0;
	position += *resCount;

	DWORD stackResCount = *resCount;

	delete resCount;
	delete lap2;
	return stackResCount;
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
	if (!fileHandle) return 0;
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
	return position;
}
/**
 * Method: TFile::SeekToBegin
 * Purpose: Sets the files seek point to the beginning
 * Parameters: void
 * Returns: void
 */
void TFile::SeekToBegin()
{
	if (!fileHandle) return;
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
	if (!fileHandle) return 0;
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

	return position;
}
