#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

// CCharsetsTab dialog

class CCharsetsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCharsetsTab)

public:
	CCharsetsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCharsetsTab();
	std::shared_ptr<CDBConnection> db;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSETS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void PopulateCharacterSetsList();
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	void AddCharacterSetToList(CListCtrl* pListCtrl, const CString& characterSet, const CString& defaultCollation);
	DECLARE_MESSAGE_MAP()
	void UpdateListFilter();
public:
	afx_msg void OnEnChangeEditCharsetsearch();
	afx_msg void OnBnClickedBtnRefreshCharsets();
};
