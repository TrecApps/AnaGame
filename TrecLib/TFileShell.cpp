#include "TFileShell.h"
#include "TDirectory.h"
#include <PathCch.h>
#include <strsafe.h>
#include <intsafe.h>

/**
 * Method: TBlankNode::TFileShell
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TFileShell::GetType()
{
	return TString(L"TFileShell;") + TObject::GetType();
}
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
	AG_THREAD_LOCK
		bool ret =path.GetSize() && !deleted;
	RETURN_THREAD_UNLOCK ret;
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
	AG_THREAD_LOCK
		TString ret(path);
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::GetName
* Purpose: Reports the name of the file minus the path
* Parameters: void
* Returns: TString - the name of the file
*/
TString TFileShell::GetName()
{
	AG_THREAD_LOCK
	int slashLoc = path.FindLastOneOf(TString(L"/\\"));
	TString ret((slashLoc == -1 ? path : path.SubString(slashLoc + 1)));
	RETURN_THREAD_UNLOCK ret;
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
	TString newPath(path);

	newPath.Replace(L'/', L'\\');

	// Make sure path is canonical
	WCHAR newPathBuff[300];
	for (UINT Rust = 0; Rust < 300; Rust++)
	{
		newPathBuff[Rust] = L'\0';
	}

	if (FAILED(PathCchCanonicalizeEx(newPathBuff, 300, newPath.GetConstantBuffer().getBuffer(), 1)))
		return TrecPointer<TFileShell>();

	newPath.Set(newPathBuff);

	DWORD ftyp = GetFileAttributesW(newPath.GetConstantBuffer().getBuffer());

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
	AG_THREAD_LOCK
	Refresh();
	FILETIME ret = fileInfo.ftCreationTime;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::GetLastAccessed
* Purpose: Reports the time that the file was last accessed
* Parameters: void
* Returns: FILETIME - the structure representing the last access time
*/
FILETIME TFileShell::GetLastAccessed()
{
	AG_THREAD_LOCK
	Refresh();
	FILETIME ret = fileInfo.ftLastAccessTime;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::GetLastWritten
* Purpose: Reports the time that the file was last updated
* Parameters: void
* Returns: FILETIME - the structure representing the last update time
*/
FILETIME TFileShell::GetLastWritten()
{
	AG_THREAD_LOCK
	Refresh();
	FILETIME ret = fileInfo.ftLastWriteTime;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::GetSize
* Purpose: Reports the current size of the file
* Parameters: void
* Returns: ULONG64 - the size of the file
*/
ULONG64 TFileShell::GetSize()
{
	AG_THREAD_LOCK
	Refresh();
	ULONG64 ret = (static_cast<ULONG64>(fileInfo.nFileSizeHigh) << 32) + static_cast<ULONG64>(fileInfo.nFileSizeLow);
	RETURN_THREAD_UNLOCK ret;
}

/**
 * Method: TFileShell::GetRelativePath
 * Purpose: Retrieves the Relative file path for the Provided directory
 * Parameters: TString& relativePath - the relative path provided if successful
 *				TrecPointer<TFileShell> directory - the directory to check (must be valid and a TDirectory)
 *				bool allowOutside - if true, than this file can be outside of the provided directory
 * Returns: bool - true if the method worked, false otherwise
 */
bool TFileShell::GetRelativePath(TString& relativePath, TrecPointer<TFileShell> directory, bool allowOutside)
{
	if (!directory.Get() || !directory->IsDirectory())
		return false;

	TString d(directory->GetPath());
	if (!d.EndsWith(L'\\'))
		d.AppendChar(L'\\');
	TString f(path);

	bool done = false;

	if (f.Find(d) == 0)
	{
		relativePath.Set(f.SubString(d.GetSize()));
		done = true;
	}

	if (!done && allowOutside)
	{
		int index = 0; 

		while (index != -1)
		{
			int dIndex = d.Find(L'\\', index);

			int fIndex = f.Find(L'\\', index);

			if(dIndex == fIndex && dIndex != -1 && d.SubString(0, dIndex).CompareNoCase(f.SubString(0, fIndex)))
				index = dIndex;
			else break;
		}

		TString remainingDirectory(d.SubString(index + 1));

		relativePath.Empty();

		UINT slashCount = remainingDirectory.CountFinds(L'\\');

		while (slashCount--)
		{
			relativePath.Append(L"..\\");
		}

		relativePath.Append(f.SubString(index + 1));
		done = true;
	}

	return done;
}

/*
* Method: TFileShell::IsArchive
* Purpose: Reports the file represents an archive
* Parameters: void
* Returns: bool - whether the file is an archive
*/
bool TFileShell::IsArchive()
{
	AG_THREAD_LOCK
	Refresh();
	bool ret = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::IsEncrypted
* Purpose: Reports the file is encrypted
* Parameters: void
* Returns: bool - whether the file is encrypted
*/
bool TFileShell::IsEncrypted()
{
	AG_THREAD_LOCK
	Refresh();
	bool ret = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::IsHidden
* Purpose: Reports the file is hidden
* Parameters: void
* Returns: bool - whether the file is hidden
*/
bool TFileShell::IsHidden()
{
	AG_THREAD_LOCK
	Refresh();
	bool ret =  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TFileShell::IsReadOnly
* Purpose: Reports the file is read-only
* Parameters: void
* Returns: bool - whether the file is read-only
*/
bool TFileShell::IsReadOnly()
{
	AG_THREAD_LOCK
	Refresh();
	bool ret = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
	RETURN_THREAD_UNLOCK ret;
}


/*
* Method: TFileShell::TFileShell
* Purpose: Creates the intended file attribute object
* Parameters: const TString& path -  the path of the intended file
* Returns: TFIleShell requested
*/
TFileShell::TFileShell(const TString& path)
{
	AG_THREAD_LOCK
	TString newPath(path);
	if (GetFileAttributesExW(newPath.GetConstantBuffer().getBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
		this->path.Set(path);


	deleted = false;
	RETURN_THREAD_UNLOCK;
}

/*
* Method: TFileShell::Refresh
* Purpose: Refreshes the information about the file after object creation time, in case changes are made -> allows AnaGame to update file info in real-time
* Parameters: void
* Returns: void
*/
void TFileShell::Refresh()
{
	AG_THREAD_LOCK
	if (!GetFileAttributesExW(path.GetConstantBuffer().getBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
	{
		if (path.GetSize())
			deleted = true;
	}
	RETURN_THREAD_UNLOCK;
}