#pragma once
#include "CDBConnection.h"
#include <memory>

// CResultTab dialog
class CMainDlg;

class CResultTab : public CDialogEx
{
	DECLARE_DYNAMIC(CResultTab)
	struct ListItem {
		CString mainItem;
		std::vector<CString> subItems;
	};

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
	std::vector<ListItem> m_AllItems;
	std::shared_ptr<CDBConnection> db;
	CListCtrl m_resultList;
	CComboBox m_comboLimit;

	BOOL CResultTab::OnInitDialog();
	afx_msg void OnBnClickedBtnClroutput();
	void CResultTab::SendMessageToQueryInfo(CString msg, COLORREF color);
	afx_msg void OnBnClickedCheckShowall();
	afx_msg void OnEnChangeListSearch();
	afx_msg void OnCbnSelchangeComboNmbOfRows();
	afx_msg void OnBnClickedBtnFirstpage();
	afx_msg void OnBnClickedBtnPrevpage();
	CMainDlg* m_pMainDlg;
	bool CResultTab::FillLimitDropdown();
	void CResultTab::BeginPageState();
	int CResultTab::BuildResultList(sql::ResultSet* resultSet, int offset);
	int CResultTab::BuildResultList(sql::ResultSet* resultSet);
	void CResultTab::SaveOriginalListState();
	int CResultTab::RebuildResultList();
	afx_msg void OnEnChangeEditCurrentpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnLastpage();
	afx_msg void OnBnClickedCheckShowbinary();
};
