#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

class CCharsetsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCharsetsTab)

public:
	CCharsetsTab(CWnd* pParent = nullptr);
	virtual ~CCharsetsTab();
	std::shared_ptr<CDBConnection> db;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSETS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
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
