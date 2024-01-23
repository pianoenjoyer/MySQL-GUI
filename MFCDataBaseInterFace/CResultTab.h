#pragma once
#include "pch.h"
#include "CDBConnection.h"
#include "Convertions.h"


// CResultTab dialog
class CMainDlg;

class CResultTab : public CDialogEx
{
	DECLARE_DYNAMIC(CResultTab)
	struct ListItem {
		CString mainItem;
		std::vector<CString> subItems;
	};

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CResultTab(CWnd* pParent = nullptr);   // standard constructor
	CResultTab(int DIALOG_ID, CWnd* pParent /*=nullptr*/);
	virtual ~CResultTab();
	std::vector<ListItem> m_AllItems;
	std::shared_ptr<CDBConnection> db;
	CListCtrl m_resultList;
	CComboBox m_comboLimit;
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	void SetPagesState();
	BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnClroutput();
	void SendMessageToQueryInfo(CString msg, COLORREF color);
	afx_msg void OnBnClickedCheckShowall();
	afx_msg void OnEnChangeListSearch();
	afx_msg void OnCbnSelchangeComboNmbOfRows();
	afx_msg void OnBnClickedBtnFirstpage();
	afx_msg void OnBnClickedBtnPrevpage();
	CMainDlg* m_pMainDlg;
	bool FillLimitDropdown();
	void BeginPageState();
	int BuildResultList(sql::ResultSet* resultSet, int offset);
	int BuildResultList(sql::ResultSet* resultSet);
	void SaveOriginalListState();
	int RebuildResultList();
	afx_msg void OnEnChangeEditCurrentpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnLastpage();
	afx_msg void OnBnClickedCheckShowbinary();
};
