#pragma once
#include "afxdialogex.h"
#include <memory>
#include "CDBConnection.h"

// CResultTab dialog

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
	afx_msg void OnBnClickedBtnClroutput();
	afx_msg void OnBnClickedCheckShowall();
	afx_msg void OnEnChangeListSearch();
	afx_msg void OnCbnSelchangeComboNmbOfRows();
	afx_msg void OnBnClickedBtnFirstpage();
	afx_msg void OnBnClickedBtnPrevpage();
	bool CResultTab::FillLimitDropdown();
	int CResultTab::FillListControl(sql::ResultSet* resultSet, int offset);
	int CResultTab::FillListControl(sql::ResultSet* resultSet);
	void CResultTab::SaveOriginalListState();
	afx_msg void OnEnChangeEditCurrentpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnLastpage();
};
