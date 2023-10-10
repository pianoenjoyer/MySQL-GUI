#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBExportDlg.h"
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
	std::vector<ListItem> m_AllItems;
	CDBExportDlg exportWindow;
	HICON m_hIcon;
	std::vector<CString> history;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	CString m_pathToFile;
	CString m_SQLtextQuery;
	CString m_titleDatabaseName;

	CDBMainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBMainDlg();
	virtual BOOL OnInitDialog();
	bool CDBMainDlg::FillTreeControl();
	void CDBMainDlg::fillEditText();
	void CDBMainDlg::SaveOriginalListState();
	void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl);
	int CDBMainDlg::FillListControl(sql::ResultSet* resultSet);
	void CDBMainDlg::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	CString ReadFileContent();
	void CDBMainDlg::SendMessageToConsole(CString msg, COLORREF color);
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnGo();
	bool CDBMainDlg::FillTableDropdown();
	bool CDBMainDlg::FillLimitDropdown();
	bool CDBMainDlg::FillDatabaseDropdown();
	afx_msg void OnEnChangeEditQtext();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnPrinttable();
	afx_msg void OnBnClickedBtnClroutput();
	afx_msg void OnBnClickedBtnUndo();
	afx_msg void OnBnClickedBtnDeletetable();
	afx_msg void OnBnClickedCheckShowAll();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBtnCollapse();
	afx_msg void OnBnClickedBtnExpand();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnEnChangeListSearch();
	afx_msg void OnBnClickedBtnClearmsg();

	afx_msg void OnMenuOpen();
	afx_msg void OnConnectionDisconnect();
	afx_msg void OnCbnSelchangeCmbSelDb();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnConnectionCheckconnection();
};
