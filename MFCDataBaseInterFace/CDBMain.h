#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBExport.h"
// CDBMain dialog

class CDBMain : public CDialogEx
{

	struct ListItem {
		CString mainItem;
		std::vector<CString> subItems;
	};

	DECLARE_DYNAMIC(CDBMain)
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::vector<ListItem> m_AllItems;
	CDBExport exportWindow;
	HICON m_hIcon;
	std::vector<CString> history;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	CString m_pathToFile;
	CString m_SQLtextQuery;
	CString m_titleDatabaseName;

	CDBMain(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBMain();
	virtual BOOL OnInitDialog();
	void CDBMain::fillEditText();
	void CDBMain::SaveOriginalListState();
	void CDBMain::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl);
	int CDBMain::FillListControl(sql::ResultSet* resultSet);
	void CDBMain::PopulateDropdown(const std::vector<std::string>& values);
	CString ReadFileContent();
	void CDBMain::SendMessageToConsole(CString msg, COLORREF color);
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnGo();
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
};
