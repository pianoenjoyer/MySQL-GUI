#pragma once
#include "afxdialogex.h"

class CTableStructureTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTableStructureTab)

public:
	CTableStructureTab(CWnd* pParent = nullptr);
	virtual ~CTableStructureTab();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_STRUCTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
