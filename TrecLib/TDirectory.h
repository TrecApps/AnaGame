#pragma once
#include "TFileShell.h"

/*
 * class TDirectory
 * Purpose: Represents a Folder in the file system
 */
class _TREC_LIB_DLL TDirectory :
	public TFileShell
{
	friend class TrecPointerKey;
protected:
	/*
	 * Method: TDirectory::Constructor
	 * Purpose: Creates the TDirectory (by creating the TFileShell)
	 * Parameters: TString& path - path of the directory
	 * Returns: void
	 */
	TDirectory(const TString& path);
public:

	/*
	* Method: TDirectory::IsDirectory
	* Purpose: Reports whether this object represents a directory as opposed to a regular file
	* Parameters: void
	* Returns: bool - true as this is a directory (overrides the TFileShell method that returns false)
	*/
	bool IsDirectory()override;

	/*
	* Method: TDirectory::GetFileListing
	* Purpose: Retrieves the list of Files held in the directory
	* Parameters: void
	* Returns: TDataArray<TrecPointer<TFileShell>> - List of Directories
	*/
	TDataArray<TrecPointer<TFileShell>> GetFileListing();

	/*
	* Method: TDirectory::GetFileListing
	* Purpose: Retrieves the list of Files held in the directory
	* Parameters: TDataArray<TrecPointer<TFileShell>>& files - reference that holds the results
	* Returns: void
	*/
	void GetFileListing(TDataArray<TrecPointer<TFileShell>>& files);
};

