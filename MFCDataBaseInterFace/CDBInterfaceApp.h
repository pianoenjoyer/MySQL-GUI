
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"


class CDBInterfaceApp : public CWinApp
{
public:
	CDBInterfaceApp();
public:
	virtual BOOL InitInstance();
	CString m_language;

	DECLARE_MESSAGE_MAP()
};

extern CDBInterfaceApp theApp;
