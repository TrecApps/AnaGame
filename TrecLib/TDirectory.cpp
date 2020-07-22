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
	TDataArray<TrecPointer<TFileShell>> ret;

	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	fileBrowser = FindFirstFileExW(path.GetConstantBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
		return ret;

	TString newPath;

	do
	{
		newPath.Set(this->path + data.cFileName);
		ret.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));

	FindClose(fileBrowser);
	fileBrowser = 0;

	return ret;
}

/*
* Method: TDirectory::GetFileListing
* Purpose: Retrieves the list of Files held in the directory
* Parameters: TDataArray<TrecPointer<TFileShell>>& files - reference that holds the results
* Returns: void
*/
void TDirectory::GetFileListing(TDataArray<TrecPointer<TFileShell>>& files)
{
	files.RemoveAll();
	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	TString searchPath(path + L"\\*");

	fileBrowser = FindFirstFileExW(searchPath.GetConstantBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
	{
		int e = GetLastError();
		return;
	}
		

	TString newPath;

	do
	{
		if (!this->GetName().Compare(data.cFileName))
			continue;
		newPath.Set(this->path + L"\\" + data.cFileName);
		files.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));
	int e = GetLastError();

	FindClose(fileBrowser);
	fileBrowser = 0;
}