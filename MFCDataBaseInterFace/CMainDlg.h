#pragma once

#include "CResultTab.h"
#include "CDBConnection.h"
#include "CExportDlg.h"
#include "CQueryTab.h"
#include "CTableCreationDlg.h"
#include "CProceduresDlg.h"
#include "CHomeTab.h"
#include "CVariablesTab.h"
#include "CCharsetsTab.h"
#include "CDatabasesTab.h"
#include "CMonitorTab.h"
#include "CEnginesTab.h"
#include "CPluginsTab.h"
#include "CLoadingDlg.h"

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

	CMainDlg(CWnd* pParent = nullptr);  // Standard constructor
	CMainDlg(std::shared_ptr<CDBConnection> db, CWnd* pParent = nullptr);


	virtual ~CMainDlg();
	//tabs
	CResultTab m_resultTab;
	CQueryTab m_queryTab;
	CTableCreationDlg m_tableTab;
	CProceduresDlg m_proceduresTab;
	CHomeTab m_homeTab;
	CVariablesTab m_varsTab;
	CCharsetsTab m_charsetsTab;
	CDatabasesTab m_databasesTab;
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CDBExportDlg m_exportTab;
	CMonitorTab m_monitorTab;
	CEnginesTab m_enginesTab;
	CPluginsTab m_pluginsTab;

	std::vector<ListItem> m_AllItems;
	CDBExportDlg exportWindow;
	HICON m_hIcon;
	std::vector<CString> history;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	CString m_pathToFile;
	CString m_SQLtextQuery;
	CString m_titleDatabaseName;
	void OnSize(UINT nType, int cx, int cy);
	sql::ResultSet* m_resultSet;
	void SetProgressBarPosition(int position);
	inline sql::ResultSet* GetResultSet() { return m_resultSet; }
	inline void SetResultSet(sql::ResultSet* ptr) { m_resultSet = ptr; }

	virtual BOOL OnInitDialog();
	bool FillTreeControl();
	void SwitchTabByName(const CString& tabName);
	void BuildDatabaseTree(CTreeCtrl& treeCtrl);
	void PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	CString ReadFileContent();
	void SetCurDataBase();
	afx_msg void OnBnClickedBtnBrowse();
	bool FillDatabaseDropdown();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBtnCollapse();
	afx_msg void OnBnClickedBtnExpand();
	void SetDlgStyle(int style);
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
	afx_msg void OnBnClickedBtnUnsel();
};

