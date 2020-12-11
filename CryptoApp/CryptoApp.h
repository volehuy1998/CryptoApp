
// CryptoApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CryptoApp:
// See CryptoApp.cpp for the implementation of this class
//

class CryptoApp : public CWinApp
{
public:
	CryptoApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CryptoApp theApp;
