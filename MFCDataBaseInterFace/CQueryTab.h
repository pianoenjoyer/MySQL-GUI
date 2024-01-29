#pragma once

#include "CDBConnection.h"
#include "CQueryHistoryDlg.h"
// CQueryTab dialog

class CQueryTab : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryTab)
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CQueryTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CQueryTab();
	CQueryHistoryDlg m_historyDlg;
	void SetSQLEditText(const CString& text);
	void SynchronizeFontProperties();
	BOOL SetDefaultFontSize(); 
	BOOL SetDefaultFont();
	void PopulateFontSizesDropdown();
	void PopulateFontNamesDropdown();
	void CQueryTab::AddQueryToHistoryFile(const CStringW& query);
	int CALLBACK EnumFontsProc(const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam);
	CComboBox m_comboTables;
	std::shared_ptr<CDBConnection> db;
	CString errorString;
	sql::ResultSet* m_resultSet;
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	void ExecuteQueryMainDlg(CStringW sqlText);
	void SendMessageToConsole(CString msg, COLORREF color);
	void ExecuteQueryMainDlg();
	void PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values);
	void PopulateColumnsList();
	void ExecuteQueryMainDlg(sql::SQLString queryText);
	bool FillTableDropdown();
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnGo();
	void UpdateStringCounter();
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnnSelect();
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdaterecord();
	afx_msg void OnBnClickedBtnDeleterecord();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnRefactor();
	afx_msg void OnBnClickedBtnClearmsg();
	afx_msg void OnCbnSelchangeSelTable();
	afx_msg void OnBnClickedBtnForward();
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnSchema();
	afx_msg void OnLbnDblclkListColumns();
	afx_msg void OnEnChangeEditQuery();
	afx_msg void OnEnVscrollEditQuery();
	afx_msg void OnEnVscrollStringcounter();
	afx_msg void OnEnChangeRichSql();
	afx_msg void OnEnVscrollRichSql();
	afx_msg void OnCbnSelchangeFontSize();
	afx_msg void OnCbnSelchangeFontcombo();
	afx_msg void OnBnClickedColorFont();
	void ApplyFontSize(int nSize);
	void ApplyFontType(const CString& strFontType);
	void ApplyFontColor(COLORREF color);
	afx_msg void OnBnClickedBtnHistory();
};
