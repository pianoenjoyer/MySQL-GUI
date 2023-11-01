#pragma once
#include "afxdialogex.h"
#include "memory"
#include "CDBConnection.h"
// CQueryTab dialog

class CQueryTab : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryTab)

public:
	CQueryTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CQueryTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboTables;
	std::shared_ptr<CDBConnection> db;
	void CQueryTab::ExecuteQueryMainDlg(CStringW sqlText);



	afx_msg void OnBnClickedBtnGo();
};
