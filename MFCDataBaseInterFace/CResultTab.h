#pragma once
#include "afxdialogex.h"


// CResultTab dialog

class CResultTab : public CDialogEx
{
	DECLARE_DYNAMIC(CResultTab)

public:
	CResultTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CResultTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::shared_ptr<CDBConnection> db;
	CListCtrl m_resultList;
	CComboBox m_comboLimit;
};
