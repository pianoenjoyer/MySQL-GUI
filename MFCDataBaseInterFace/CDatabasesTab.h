#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include <memory>

// CDatabasesTab dialog

class CDatabasesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabasesTab)

public:
	CDatabasesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDatabasesTab();
	std::shared_ptr<CDBConnection> db;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CDatabasesTab::OnInitDialog();
	void CDatabasesTab::PopulateDatabaseList();
	CString CDatabasesTab::GetDatabaseCreatedDate(const CString& databaseName);
	CString CDatabasesTab::GetDatabaseOwner(const CString& databaseName);
	void CDatabasesTab::AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation);
	CString CDatabasesTab::GetDatabaseCollation(const CString& databaseName);
	void CDatabasesTab::AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation,
		const CString& size);
	void CDatabasesTab::PopulateCharacterSetDropdown();
	CString CDatabasesTab::GetDatabaseSize(const CString& databaseName);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreatedb();
	afx_msg void OnBnClickedBtnDeletedb();
};
