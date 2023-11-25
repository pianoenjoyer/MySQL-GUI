#pragma once

#include "CResultTab.h"
#include "CDBConnection.h"
#include "CExportDlg.h"
#include "CQueryTab.h"
#include "CTableCreationDlg.h"
#include "CProceduresDlg.h"
#include "CHomeTab.h"

class CMainDlg : public CDialogEx
{

	struct ListItem 
	{
		CString mainItem;
		std::vector<CString> subItems;
	};

	DECLARE_DYNAMIC(CMainDlg)
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
	CTableCreationDlg m_tableTab;
	CProceduresDlg m_proceduresTab;
	CHomeTab m_homeTab;

	CDBExportDlg m_exportTab;
	std::vector<ListItem> m_AllItems;
	CDBExportDlg exportWindow;
	HICON m_hIcon;
	std::vector<CString> history;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	CString m_pathToFile;
	CString m_SQLtextQuery;
	CString m_titleDatabaseName;
	void CMainDlg::OnSize(UINT nType, int cx, int cy);
	sql::ResultSet* m_resultSet;
	void CMainDlg::SetProgressBarPosition(int position);
	inline sql::ResultSet* GetResultSet() { return m_resultSet; }
	inline void SetResultSet(sql::ResultSet* ptr) { m_resultSet = ptr; }

	CMainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainDlg();
	virtual BOOL OnInitDialog();
	bool CMainDlg::FillTreeControl();
	void CMainDlg::SwitchTabByName(const CString& tabName);
	void CMainDlg::BuildDatabaseTree(CTreeCtrl& treeCtrl);
	void CMainDlg::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	CString ReadFileContent();
	void CMainDlg::SetCurDataBase();
	afx_msg void OnBnClickedBtnBrowse();
	bool CMainDlg::FillDatabaseDropdown();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBtnCollapse();
	afx_msg void OnBnClickedBtnExpand();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnMenuOpen();
	afx_msg void OnConnectionDisconnect();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnConnectionCheckconnection();
	afx_msg void OnNMClickSyslinkServerinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileExit();
	afx_msg void OnFileExport();

	afx_msg void OnEditSelectall();
	afx_msg void OnHelpMysqldocumentation();
	afx_msg void OnAboutVersionandcredits();
	afx_msg void OnHelpServerinfo();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnTvnSelchangedTreeStructure(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTreeStructure(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeMaintab(NMHDR* pNMHDR, LRESULT* pResult);
	CTabCtrl m_mainTabCtrl;
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnCbnSelchangeCmbSelDb();
};

