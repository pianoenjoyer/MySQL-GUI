#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CTableRecordsTab.h"
#include "CTableStructureTab.h"
#include "CTableTabRecords.h"
#include "CResultPlus.h"

class CTableCreationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTableCreationDlg)

public:
	CTableCreationDlg(CWnd* pParent = nullptr);
	virtual ~CTableCreationDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLES };
#endif

protected:
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	CTableTabRecords m_recordsTab;
	CTableStructureTab m_structureTab;
	CResultTab m_resultTab;
	CResultPlus m_resultPlus;
	sql::ResultSet* m_pCurResultSet;
	DECLARE_MESSAGE_MAP()
public:void PopulateStorageEngineDropdown();
	void PopulateCharacterSetDropdown();
	bool UpdateStructureList();
	bool UpdateRecorldsList();
	bool PopulateDatabaseDropdown();
	bool PopulateTableDropdown();
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	CListCtrl m_ListCreateTable;
	CRichEditCtrl m_editTableName;
	CComboBox m_cmbCollation;
	std::shared_ptr<CDBConnection> db;
	CRichEditCtrl m_editComments;
	afx_msg void OnBnClickedGo();
	afx_msg void OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTabTables(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeCmbDbs();
	afx_msg void OnCbnSelchangeCmbTbs();
	afx_msg void OnBnClickedBtnRefdb();
	afx_msg void OnBnClickedBtnReftbl();
};
