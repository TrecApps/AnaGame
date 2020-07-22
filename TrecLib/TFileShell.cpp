#include "TFileShell.h"
#include "TDirectory.h"

/*
* Method: TFileShell::IsDirectory
* Purpose: Reports whether this file represents a directory
* Parameters: void
* Returns: bool - false if object is the base class, true, if object is a TDirectory
*/
bool TFileShell::IsDirectory()
{
	return false;
}

/*
* Method: TFileShell::IsValid
* Purpose: Reports whether the object represents an existing file
* Parameters: void
* Returns: bool - true if the file exists and have not yet been deleted, false otherwise
*/
bool TFileShell::IsValid()
{
	return path.GetSize() && !deleted;
}

/*
* Method: TFileShell::GetPath
* Purpose: Reports the path that leads to the file
* Parameters: void
* Returns: TString - the path of the file
*
* Note: if the path is an empty string, you can assume that the TFileShell is invalid
*/
TString TFileShell::GetPath()
{
	return path;
}

/*
* Method: TFileShell::GetName
* Purpose: Reports the name of the file minus the path
* Parameters: void
* Returns: TString - the name of the file
*/
TString TFileShell::GetName()
{
	int slashLoc = path.FindLastOneOf(TString(L"/\\"));
	if (slashLoc == -1)
		return path;

	return path.SubString(slashLoc+1);
}

/*
* Method: static TFileShell::GetFileInfo
* Purpose:Returns an Object representing the file of the specified path
* Parameters: TString& path - the path of the file
* Returns: TrecPointer<TFileShell> - the file information regarding the path
*
* Note: If the file does not exist, the pointer will be null. If the file is a directory,
*	the pointer will contain a TDirectory (which extends the TFileShell)
*/
TrecPointer<TFileShell> TFileShell::GetFileInfo(const TString& path)
{
	DWORD ftyp = GetFileAttributesW(path.GetConstantBuffer());

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return TrecPointer<TFileShell>(); // if Invalid, simply return a Null


	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return TrecPointerKey::GetNewTrecPointerAlt<TFileShell, TDirectory>(path);
	else
		return TrecPointerKey::GetNewTrecPointer<TFileShell>(path);
}

/*
* Method: TFileShell::GetCreationDate
* Purpose: Reports the time that the file was created
* Parameters: void
* Returns: FILETIME - the structure representing the file creation time
*/
FILETIME TFileShell::GetCreationDate()
{
	Refresh();
	return fileInfo.ftCreationTime;
}

/*
* Method: TFileShell::GetLastAccessed
* Purpose: Reports the time that the file was last accessed
* Parameters: void
* Returns: FILETIME - the structure representing the last access time
*/
FILETIME TFileShell::GetLastAccessed()
{
	Refresh();
	return fileInfo.ftLastAccessTime;
}

/*
* Method: TFileShell::GetLastWritten
* Purpose: Reports the time that the file was last updated
* Parameters: void
* Returns: FILETIME - the structure representing the last update time
*/
FILETIME TFileShell::GetLastWritten()
{
	Refresh();
	return fileInfo.ftLastWriteTime;
}

/*
* Method: TFileShell::GetSize
* Purpose: Reports the current size of the file
* Parameters: void
* Returns: ULONG64 - the size of the file
*/
ULONG64 TFileShell::GetSize()
{
	Refresh();
	return (static_cast<ULONG64>(fileInfo.nFileSizeHigh) << 32) + static_cast<ULONG64>(fileInfo.nFileSizeLow);
}

/*
* Method: TFileShell::IsArchive
* Purpose: Reports the file represents an archive
* Parameters: void
* Returns: bool - whether the file is an archive
*/
bool TFileShell::IsArchive()
{
	Refresh();
	return fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

/*
* Method: TFileShell::IsEncrypted
* Purpose: Reports the file is encrypted
* Parameters: void
* Returns: bool - whether the file is encrypted
*/
bool TFileShell::IsEncrypted()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED;
}

/*
* Method: TFileShell::IsHidden
* Purpose: Reports the file is hidden
* Parameters: void
* Returns: bool - whether the file is hidden
*/
bool TFileShell::IsHidden()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
}

/*
* Method: TFileShell::IsReadOnly
* Purpose: Reports the file is read-only
* Parameters: void
* Returns: bool - whether the file is read-only
*/
bool TFileShell::IsReadOnly()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
}


/*
* Method: TFileShell::TFileShell
* Purpose: Creates the intended file attribute object
* Parameters: const TString& path -  the path of the intended file
* Returns: TFIleShell requested
*/
TFileShell::TFileShell(const TString& path)
{
	if (GetFileAttributesExW(path.GetConstantBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
		this->path.Set(path);


	deleted = false;
}

/*
* Method: TFileShell::Refresh
* Purpose: Refreshes the information about the file after object creation time, in case changes are made -> allows AnaGame to update file info in real-time
* Parameters: void
* Returns: void
*/
void TFileShell::Refresh()
{
	if (!GetFileAttributesExW(path.GetConstantBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
	{
		if (path.GetSize())
			deleted = true;
	}
}