
#include "TFile.h"
#include "TString.h"

UCHAR TFileType[] = { 2, 0b10000000, 2 };

/*
* Method: (TFile) (Constructor) 
* Purpose: Default constrictor for the TFile class
* Parameters: void
* Returns: void
*/
TFile::TFile()
{
	fileEncode = fet_unknown;
	fileHandle = 0;
	position = 0ULL;
}

/*
* Method: (TFile) (Constructor)
* Purpose: Creates a file that presumably will be opened
* Parameters: LPCTSTR lpszFileName - the File name to open
*			UINT nOpenFlags - flags that specify the open status of the file
* Returns: void
*/
TFile::TFile(TString& lpszFileName, UINT nOpenFlags) 
{
	Open(lpszFileName, nOpenFlags);
}


/*
* Method: (TFile) (Destructor) 
* Purpose: Cleans up TFile resources
* Parameters: void
* Returns: void
*/
TFile::~TFile()
{
	Close();
}

/*
* Method: TFile - Open
* Purpose: Opens the File
* Parameters: LPCTSTR lpszFileName - The File to open
*			UINT nOpenFlags - Flags to open the file
*			CFileException * pError - Error information
* Returns: BOOL - success or failure to open file
*/
bool TFile::Open(TString& lpszFileName, UINT nOpenFlags)
{
	fileEncode = fet_unknown;

	std::string fName = lpszFileName.GetRegString();
	OFSTRUCT structure;
	fileHandle = OpenFile(fName.c_str(), &structure, nOpenFlags);

	if (fileHandle == HFILE_ERROR)
	{
		fileHandle = 0;
		return false;
	}

	// Here, the file is open, try to deduce the file type
	fileEncode = DeduceEncodingType();
	return true;
}





/*
* Method: TFile - ReadString
* Purpose: Reads a line in a file into a String, taking into account the file encoding
* Parameters: CString& rString - the String to read into
* Returns: BOOL - success of reading
*/
BOOL TFile::ReadString(TString & rString)
{
	bool success = false;
	rString.Empty();
	switch (fileEncode)
	{
	case fet_acsii:
		char letter[1];
		while (Read(&letter, 1))
		{
			if (letter[0] == '\n')
				break;
			rString += ReturnWCharType(letter[0]);
			success = true;
		}
		
		break;
	case fet_unicode:
		UCHAR letter2[2];
		while (Read(&letter2, 2))
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			if (cLetter == L'\n')
				break;
			rString += cLetter;
			success = true;
		}

		break;
	case fet_unicode_little:
		WCHAR wLetter;
		while (Read(&wLetter, 2))
		{
			if (wLetter == L'\n')
				break;
			rString += wLetter;
			success = true;
		}

	}
	return success;
}

UINT TFile::ReadString(TString & rString, UINT nMax)
{
	rString.Empty();
	UINT rust = 0;
	switch (fileEncode)
	{
	case fet_acsii:
		char letter[1];
		for ( ; rust <= nMax && Read(&letter, 1); rust++)
		{
			rString += ReturnWCharType(letter[0]);
		}

		break;
	case fet_unicode:
		UCHAR letter2[2];
		
		for (; rust <= nMax && Read(&letter2, 2); rust++)
		{
			WCHAR cLetter;
			UCHAR temp = letter2[0];
			letter2[0] = letter2[1];
			letter2[1] = temp;
			memcpy(&cLetter, letter2, 2);
			
			rString += cLetter;
		}

		break;
	case fet_unicode_little:
		WCHAR wLetter;
		
		for( ; rust <= nMax && Read(&wLetter, 2); rust++ )
		{
			rString += wLetter;
		}
		
	}
	return rust;
}


UINT TFile::ReadString(TString & rString, WCHAR chara)
{
	bool success = false;
	rString.Empty();
	switch (fileEncode)
	{
	case fet_acsii:
		char letter[1];
		while (Read(&letter, 1))
		{
			if (letter[0] == chara)
				break;
			rString += ReturnWCharType(letter[0]);
			success = true;
		}

		break;
	case fet_unicode:
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
			rString += cLetter;
			success = true;
		}

		break;
	case fet_unicode_little:
		WCHAR wLetter;
		while (Read(&wLetter, 2))
		{
			if (wLetter == chara)
				break;
			rString += wLetter;
			success = true;
		}

	}
	return rString.GetSize();
}

/*
* Method: TFile - WriteString
* Purpose: Wrties a string to the file
* Parameters: LPCTSTR lpsz - the Stringt to write
* Returns: void
*/
void TFile::WriteString(TString& lpsz)
{
	UINT size = 0;
	CHAR* acsiiText = nullptr;
	UINT wBytes = 0;
	WCHAR cLetter = L'\0';
	UCHAR bytes[2];
	UCHAR temp = 0;
	WCHAR* bufferString = lpsz.GetBufferCopy();
	if (fileEncode == fet_unknown)
		fileEncode = fet_unicode_little;
	switch (fileEncode)
	{
	case fet_acsii:
	case fet_unicode8:
		size = lpsz.GetSize();
		acsiiText = new CHAR[size * 2 + 1];
		wBytes = WideCharToMultiByte(CP_ACP,
			0, bufferString, -1,
			acsiiText, size * 2, NULL,
			NULL);
		Write(acsiiText, wBytes);
		break;
	case fet_unicode:
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
	case fet_unicode_little:
		for (UINT c = 0; lpsz[c] != L'\0'; c++)
			Write(&bufferString[c], 2);
	}

	delete[] bufferString;
}

/*
* Method: TFile - IsOpen
* Purpose: Reports whether the File is open or not
* Parameters: void
* Returns: bool - is the file open
*/
bool TFile::IsOpen()
{
	return fileHandle != 0;
}

/*
* Method: TFile - SetEncoding
* Purpose: Sets the encoding of the File (if not already set)
* Parameters: FileEncodingType fet - the Encoding type to do
* Returns: bool - whether the file encoding was set
*/
bool TFile::SetEncoding(FileEncodingType fet)
{
	if (fileEncode == fet_unknown && fet != fet_unknown)
	{
		fileEncode = fet;
		return true;
	}
	return false;
}

/*
* Method: TFile - GetFileDirectory
* Purpose: Retrieves the directory of the file
* Parameters: void
* Returns: TString - the directory path of the file
*/
TString TFile::GetFileDirectory()
{
	if(!IsOpen())
		return TString();
	TString directory = GetFilePath();
	TString fileName = GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetSize());
	return directory;
}

/*
* Method: TFile - GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type 
*/
UCHAR * TFile::GetAnaGameType()
{
	return TFileType;
}

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

void TFile::Close()
{
	if (fileHandle)
		CloseHandle((HANDLE)fileHandle);
	fileHandle = 0;
}

void TFile::Write(const void* buffer, UINT count)
{
	if (!fileHandle)
		return;
	LPDWORD resCount;
	LPOVERLAPPED lap;
	BOOL res = WriteFile((HANDLE)fileHandle, buffer, count, resCount, lap);

	if (!res)
		return;
	position += *resCount;
}

/*
* Method: TFile - DeduceEncodingType
* Purpose: Attempts to determine the encoding type of the file if possible
* Parameters: void
* Returns: FileEncodingType - the File Encoding type deduced
*/
FileEncodingType TFile::DeduceEncodingType()
{
	if (!IsOpen())
		return fet_unknown;
	if (GetLength() < 2)
		return fet_unknown;
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
				return fet_unicode7;

			// Now check for UTF-8
			if (twoBytes[0] == 0xef &&
				twoBytes[1] == 0xbb &&
				twoBytes[2] == 0xbf)
				return fet_unicode8;

			// Check for big endian unicode
			if (twoBytes[0] == 0xfe &&
				twoBytes[1] == 0xff)
				return fet_unicode;

			// Check for little-endian unicode
			if (twoBytes[0] == 0xff &&
				twoBytes[1] == 0xfe)
				return fet_unicode_little;
			

			// There is no BOM to tell us the encoding, need to guess
			// Check to see if it is big-endian the old fashioned way
			if (twoBytes[0] == 0x00 &&
				twoBytes[2] == 0x00 &&
				twoBytes[4] == 0x00)
				return fet_unicode;

			if (twoBytes[1] == 0x00 &&
				twoBytes[3] == 0x00)
				return fet_unicode_little;

			return fet_acsii;
		}
		else
			return fet_unknown;
	}
	else
	{
		if (value & IS_TEXT_UNICODE_STATISTICS ||
			value & IS_TEXT_UNICODE_CONTROLS ||
			value & IS_TEXT_UNICODE_ASCII16)
			return fet_unicode_little;

		if (value & IS_TEXT_UNICODE_REVERSE_STATISTICS ||
			value & IS_TEXT_UNICODE_REVERSE_CONTROLS ||
			value & IS_TEXT_UNICODE_REVERSE_ASCII16)
			return fet_unicode;

		if (value & IS_TEXT_UNICODE_NOT_UNICODE_MASK)
			return fet_acsii;
	}
	return fet_unknown;
}

ULONGLONG TFile::GetLength()
{
	LARGE_INTEGER  len_li;
	GetFileSizeEx((HANDLE)fileHandle, &len_li);
	LONGLONG  len_ll = len_li.QuadPart;
	return len_ll;
}

ULONGLONG TFile::GetPosition()
{
	return position;
}

UINT TFile::Read(void* buffer, UINT count)
{
	if(!fileHandle)
		return 0;
	LPDWORD resCount;
	LPOVERLAPPED lap;
	BOOL res = ReadFile((HANDLE)fileHandle, buffer, count, resCount, lap);

	if (!res)
		return 0;
	position += *resCount;
	return *resCount;
}

ULONGLONG TFile::Seek(LONGLONG offset, UINT from)
{
	if (!fileHandle) return;
	PLONG hZero;
	*hZero = 0;
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
	return position;
}

void TFile::SeekToBegin()
{
	if (!fileHandle) return;
	PLONG hZero;
	*hZero = 0;
	DWORD res = SetFilePointer((HANDLE)fileHandle, 0, hZero, FILE_BEGIN);

	if (res != INVALID_SET_FILE_POINTER)
	{
		position = 0;
	}
	else
	{
		// To-Do:
	}
}

ULONGLONG TFile::SeekToEnd()
{
	if (!fileHandle) return;
	PLONG hZero;
	*hZero = 0;
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
}
