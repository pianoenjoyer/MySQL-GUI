#pragma once
#include "afxdialogex.h"


// CTableRecordsTab dialog

class CTableRecordsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTableRecordsTab)

public:
	CTableRecordsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTableRecordsTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_RECORDS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
