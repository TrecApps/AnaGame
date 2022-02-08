#include "TDirectory.h"

/*
* Method: TDirectory::Constructor
* Purpose: Creates the TDirectory (by creating the TFileShell)
* Parameters: TString& path - path of the directory
* Returns: void
*/
TDirectory::TDirectory(const TString& path) : TFileShell(path)
{

}

/**
 * Method: TDirectory::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString TDirectory::GetType()
{
	return TString(L"TDirectory;") + TFileShell::GetType();
} 

/*
* Method: TDirectory::IsDirectory
* Purpose: Reports whether this object represents a directory as opposed to a regular file
* Parameters: void
* Returns: bool - true as this is a directory (overrides the TFileShell method that returns false)
*/

bool TDirectory::IsDirectory()
{
	return true;
}

/*
* Method: TDirectory::GetFileListing
* Purpose: Retrieves the list of Files held in the directory
* Parameters: void
* Returns: TDataArray<TrecPointer<TFileShell>> - List of Directories
*/
TDataArray<TrecPointer<TFileShell>> TDirectory::GetFileListing()
{
	AG_THREAD_LOCK
	TDataArray<TrecPointer<TFileShell>> ret;

	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	fileBrowser = FindFirstFileExW(path.GetConstantBuffer().getBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
	{
		RETURN_THREAD_UNLOCK ret;
	}
	TString newPath;

	do
	{
		newPath.Set(this->path + data.cFileName);
		ret.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));

	FindClose(fileBrowser);
	fileBrowser = 0;

	RETURN_THREAD_UNLOCK ret;
}

/*
* Method: TDirectory::GetFileListing
* Purpose: Retrieves the list of Files held in the directory
* Parameters: TDataArray<TrecPointer<TFileShell>>& files - reference that holds the results
* Returns: void
*/
void TDirectory::GetFileListing(TDataArray<TrecPointer<TFileShell>>& files)
{
	AG_THREAD_LOCK
	files.RemoveAll();
	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	TString searchPath(path + L"\\*");

	fileBrowser = FindFirstFileExW(searchPath.GetConstantBuffer().getBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
	{
		//int e = GetLastError();
		RETURN_THREAD_UNLOCK;
	}
		

	TString newPath;

	do
	{
		if (!this->GetName().Compare(data.cFileName))
			continue;
		newPath.Set(this->path + L"\\" + data.cFileName);
		files.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));
	//int e = GetLastError();

	FindClose(fileBrowser);
	fileBrowser = 0;
	RETURN_THREAD_UNLOCK;
}