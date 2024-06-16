#pragma once
#include "afxdialogex.h"
#include "CResultTab.h"

class CTableTabRecords : public CResultTab
{
	DECLARE_DYNAMIC(CTableTabRecords)

public:
	CTableTabRecords(CWnd* pParent = nullptr);
	virtual ~CTableTabRecords();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT_SHORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
