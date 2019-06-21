#pragma once
#include "stdafx.h"
#include "TString.h"
#include "TFile.h"

/*
* enum CentralDirectories
* Represents possible directory types AnaGame and its taps might be interested in
*/
typedef enum CentralDirectories
{
	cd_Executable,	// 0
	cd_AppData,		// 1
	cd_Desktop,		// 2
	cd_Documents,	// 3
	cd_Music,		// 4
	cd_Pictures,	// 5
	cd_Videos,		// 6
	cd_Downloads,	// 7
	cd_User			// 8
}CentralDirectories;

void  ForgeDirectory(TString& dir);


/*
* Function: GetDirectory
* Purpose: Gets the directory specified
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetDirectory(CentralDirectories cd);

/*
* Function: GetDirectoryWithSlash
* Purpose: Gets the directory specified with a slash at the end
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetDirectoryWithSlash(CentralDirectories cd);

/*
* Function: GetShadowDirectory
* Purpose: Gets AnaGame's "shadow" version of the specified directory
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetShadowDirectory(CentralDirectories cd);

/*
* Function: GetShadowDirectoryWithSlash
* Purpose: Gets AnaGame's "shadow" version of the specified directory
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetShadowDirectoryWithSlash(CentralDirectories cd);

/*
* Function: GetShadowFilePath
* Purpose: Sets up the directory and returns the path for the Shadow version of the provided file
* Parameters: TFile& f - the file to get s shadow File's path for
* Returns: TString - the Path of the intended Shadow file
* Note: The provided File has to be open AND it has to be found in an AnaGame approved directory
*/
TString GetShadowFilePath(TFile& f);
