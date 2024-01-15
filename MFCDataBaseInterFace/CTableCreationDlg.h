#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CTableRecordsTab.h"
#include "CTableStructureTab.h"
// CTableCreationDlg dialog

class CTableCreationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTableCreationDlg)

public:
	CTableCreationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTableCreationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLES };
#endif

protected:
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CTableRecordsTab m_recordsTab;
	CTableStructureTab m_structureTab;
	DECLARE_MESSAGE_MAP()
public:void PopulateStorageEngineDropdown();
	void PopulateCharacterSetDropdown();
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	CListCtrl m_ListCreateTable;
	CRichEditCtrl m_editTableName;
	// m_comboCollation
	CComboBox m_cmbCollation;
	std::shared_ptr<CDBConnection> db;
	CRichEditCtrl m_editComments;
	afx_msg void OnBnClickedGo();
	afx_msg void OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTabTables(NMHDR* pNMHDR, LRESULT* pResult);
};
