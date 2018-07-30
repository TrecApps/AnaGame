
// AnaGame_Builder.h : main header file for the AnaGame_Builder application
//
#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <TString.h>
#include <TDataArray.h>
#include "resource.h"       // main symbols
#include <DirectoryInterface.h>

// CAnaGameBuilderApp:
// See AnaGame_Builder.cpp for the implementation of this class
//


/*typedef enum CentralDirectories
{
	cd_Executable,
	cd_AppData,
	cd_Desktop,
	cd_Documents,
	cd_Music,
	cd_Pictures,
	cd_Videos,
	cd_Downloads
}CentralDirectories;*/

class CAnaGameBuilderApp : public CWinApp
{
public:
	CAnaGameBuilderApp();

	//TString GetDefaultDirectory(CentralDirectories cd);

private:
	//TDataArray<TString> defaultDirectories;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnaGameBuilderApp theApp;
