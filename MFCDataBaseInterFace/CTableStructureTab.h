#pragma once
#include "afxdialogex.h"


// CTableStructureTab dialog

class CTableStructureTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTableStructureTab)

public:
	CTableStructureTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTableStructureTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_STRUCTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
