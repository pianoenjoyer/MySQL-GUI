
// MFCDataBaseInterFace.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCDataBaseInterFaceApp:
// See MFCDataBaseInterFace.cpp for the implementation of this class
//

class CMFCDataBaseInterFaceApp : public CWinApp
{
public:
	CMFCDataBaseInterFaceApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCDataBaseInterFaceApp theApp;
