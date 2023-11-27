#pragma once

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
	CString errorString;
	sql::ResultSet* m_resultSet;

	void CQueryTab::ExecuteQueryMainDlg(CStringW sqlText);
	void CQueryTab::SendMessageToConsole(CString msg, COLORREF color);
	void CQueryTab::ExecuteQueryMainDlg();
	void CQueryTab::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	void CQueryTab::PopulateColumnsList();
	void CQueryTab::ExecuteQueryMainDlg(sql::SQLString queryText);
	bool CQueryTab::FillTableDropdown();
	BOOL CQueryTab::OnInitDialog();
	afx_msg void OnBnClickedBtnGo();
	void CQueryTab::UpdateStringCounter();
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnnSelect();
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdaterecord();
	afx_msg void OnBnClickedBtnDeleterecord();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnRefactor();
	afx_msg void OnBnClickedBtnClearmsg();
	afx_msg void OnCbnSelchangeSelTable();
	afx_msg void OnBnClickedBtnForward();
	void CQueryTab::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnSchema();
	afx_msg void OnLbnDblclkListColumns();
	afx_msg void OnEnChangeEditQuery();
	afx_msg void OnEnVscrollEditQuery();
	afx_msg void OnEnVscrollStringcounter();
};
