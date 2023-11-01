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
	void CQueryTab::SendMessageToConsole(CString msg, COLORREF color);
	void CQueryTab::ExecuteQueryMainDlg();
	CString errorString;

	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnnSelect();
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdaterecord();
	afx_msg void OnBnClickedBtnDeleterecord();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnRefactor();
};
