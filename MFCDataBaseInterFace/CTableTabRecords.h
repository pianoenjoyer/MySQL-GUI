#pragma once
#include "afxdialogex.h"
#include "CResultTab.h"

// CTableTabRecords dialog

class CTableTabRecords : public CResultTab
{
	DECLARE_DYNAMIC(CTableTabRecords)

public:
	CTableTabRecords(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTableTabRecords();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT_SHORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
