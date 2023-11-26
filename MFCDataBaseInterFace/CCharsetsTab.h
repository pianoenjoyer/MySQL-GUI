#pragma once
#include "afxdialogex.h"


// CCharsetsTab dialog

class CCharsetsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCharsetsTab)

public:
	CCharsetsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCharsetsTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSETS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
