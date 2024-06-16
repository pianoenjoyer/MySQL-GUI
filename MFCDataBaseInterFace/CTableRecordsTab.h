#pragma once
#include "afxdialogex.h"

class CTableRecordsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTableRecordsTab)

public:
	CTableRecordsTab(CWnd* pParent = nullptr);
	virtual ~CTableRecordsTab();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_RECORDS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
