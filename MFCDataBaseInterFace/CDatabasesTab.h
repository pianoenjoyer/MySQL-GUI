#pragma once
#include "afxdialogex.h"


// CDatabasesTab dialog

class CDatabasesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabasesTab)

public:
	CDatabasesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDatabasesTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
