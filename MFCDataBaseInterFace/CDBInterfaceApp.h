
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class CDBInterfaceApp : public CWinApp
{
public:
	CDBInterfaceApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CString m_language;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDBInterfaceApp theApp;
