// AnagameCompiler.h : main header file for the AnagameCompiler DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAnagameCompilerApp
// See AnagameCompiler.cpp for the implementation of this class
//

class CAnagameCompilerApp : public CWinApp
{
public:
	CAnagameCompilerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
