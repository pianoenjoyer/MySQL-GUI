#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBExportDlg.h"
#include "CResultTab.h"
#include "CQueryTab.h"
// CDBMainDlg dialog

class CDBMainDlg : public CDialogEx
{

	struct ListItem {
		CString mainItem;
		std::vector<CString> subItems;
	};

	DECLARE_DYNAMIC(CDBMainDlg)
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CResultTab m_resultTab;
	CQueryTab m_queryTab;
	std::vector<ListItem> m_AllItems;
	CDBExportDlg exportWindow;
	HICON m_hIcon;
	std::vector<CString> history;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	CString m_pathToFile;
	CString m_SQLtextQuery;
	CString m_titleDatabaseName;

	sql::ResultSet* m_resultSet;

	inline sql::ResultSet* GetResultSet() { return m_resultSet; }
	inline void SetResultSet(sql::ResultSet* ptr) { m_resultSet = ptr; }

	CDBMainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBMainDlg();
	virtual BOOL OnInitDialog();
	bool CDBMainDlg::FillTreeControl();
	void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl);
	void CDBMainDlg::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	CString ReadFileContent();
	afx_msg void OnBnClickedBtnBrowse();
	bool CDBMainDlg::FillTableDropdown();
	bool CDBMainDlg::FillDatabaseDropdown();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBtnCollapse();
	afx_msg void OnBnClickedBtnExpand();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnMenuOpen();
	afx_msg void OnConnectionDisconnect();
	afx_msg void OnCbnSelchangeCmbSelDb();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnConnectionCheckconnection();
	afx_msg void OnNMClickSyslinkServerinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileExit();
	afx_msg void OnFileExport();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectall();
	afx_msg void OnHelpMysqldocumentation();
	afx_msg void OnAboutVersionandcredits();
	afx_msg void OnHelpServerinfo();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnTvnSelchangedTreeStructure(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTreeStructure(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeMaintab(NMHDR* pNMHDR, LRESULT* pResult);
	CTabCtrl m_mainTabCtrl;
};

inline sql::SQLString CStringToSQLString(const CString& cstr);
