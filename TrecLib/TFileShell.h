#pragma once
#include "TObject.h"
#include "TString.h"

/*
 * class TFileShell
 * Purpose: Represents a File that exists on the file System.
 *
 * Note: this class does not open a file for reading or writing, merely provides information about the file in question
 */
class _TREC_LIB_DLL TFileShell :
	public TObject
{
	friend class TrecPointerKey;
public:

	/*
	* Method: TFileShell::IsDirectory
	* Purpose: Reports whether this file represents a directory
	* Parameters: void
	* Returns: bool - false if object is the base class, true, if object is a TDirectory
	*/
	virtual bool IsDirectory();

	/*
	* Method: TFileShell::IsValid
	* Purpose: Reports whether the object represents an existing file
	* Parameters: void
	* Returns: bool - true if the file exists and have not yet been deleted, false otherwise
	*/
	bool IsValid();

	/*
	* Method: TFileShell::GetPath
	* Purpose: Reports the path that leads to the file
	* Parameters: void
	* Returns: TString - the path of the file
	*
	* Note: if the path is an empty string, you can assume that the TFileShell is invalid
	*/
	TString GetPath();


	/*
	* Method: TFileShell::GetName
	* Purpose: Reports the name of the file minus the path
	* Parameters: void
	* Returns: TString - the name of the file
	*/
	TString GetName();


	/*
	* Method: static TFileShell::GetFileInfo
	* Purpose:Returns an Object representing the file of the specified path
	* Parameters: TString& path - the path of the file
	* Returns: TrecPointer<TFileShell> - the file information regarding the path
	*
	* Note: If the file does not exist, the pointer will be null. If the file is a directory,
	*	the pointer will contain a TDirectory (which extends the TFileShell)
	*/
	TrecPointer<TFileShell> static GetFileInfo(const TString& path);


	/*
	* Method: TFileShell::GetCreationDate
	* Purpose: Reports the time that the file was created
	* Parameters: void
	* Returns: FILETIME - the structure representing the file creation time
	*/
	FILETIME GetCreationDate();

	/*
	* Method: TFileShell::GetLastAccessed
	* Purpose: Reports the time that the file was last accessed
	* Parameters: void
	* Returns: FILETIME - the structure representing the last access time
	*/
	FILETIME GetLastAccessed();

	/*
	* Method: TFileShell::GetLastWritten
	* Purpose: Reports the time that the file was last updated
	* Parameters: void
	* Returns: FILETIME - the structure representing the last update time
	*/
	FILETIME GetLastWritten();


	/*
	* Method: TFileShell::GetSize
	* Purpose: Reports the current size of the file
	* Parameters: void
	* Returns: ULONG64 - the size of the file
	*/
	ULONG64 GetSize();


	/*
	* Method: TFileShell::IsArchive
	* Purpose: Reports the file represents an archive
	* Parameters: void
	* Returns: bool - whether the file is an archive
	*/
	bool IsArchive();

	/*
	* Method: TFileShell::IsEncrypted
	* Purpose: Reports the file is encrypted
	* Parameters: void
	* Returns: bool - whether the file is encrypted
	*/
	bool IsEncrypted();

	/*
	* Method: TFileShell::IsHidden
	* Purpose: Reports the file is hidden
	* Parameters: void
	* Returns: bool - whether the file is hidden
	*/
	bool IsHidden();

	/*
	* Method: TFileShell::IsReadOnly
	* Purpose: Reports the file is read-only
	* Parameters: void
	* Returns: bool - whether the file is read-only
	*/
	bool IsReadOnly();

protected:
	/*
	* Method: TFileShell::TFileShell
	* Purpose: Creates the intended file attribute object
	* Parameters: const TString& path -  the path of the intended file
	* Returns: TFIleShell requested
	*/
	TFileShell(const TString& path);

	/*
	* Method: TFileShell::Refresh
	* Purpose: Refreshes the information about the file after object creation time, in case changes are made -> allows AnaGame to update file info in real-time
	* Parameters: void
	* Returns: void
	*/
	void Refresh();

	/**
	 * whether the relevant file has been deleted since the object was created
	 */
	bool deleted;

	/**
	 * the Path of the target file
	 */
	TString path;

	/**
	 * information about the file
	 */
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
};

