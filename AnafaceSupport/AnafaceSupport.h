// AnafaceSupport.h : main header file for the AnafaceSupport DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAnafaceSupportApp
// See AnafaceSupport.cpp for the implementation of this class
//

class CAnafaceSupportApp : public CWinApp
{
public:
	CAnafaceSupportApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
