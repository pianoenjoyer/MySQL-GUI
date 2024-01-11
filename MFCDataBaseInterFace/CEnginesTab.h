#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

// CEnginesTab dialog

class CEnginesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEnginesTab)

public:
	CEnginesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEnginesTab();
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENGINES };
#endif

protected:
	std::shared_ptr<CDBConnection> db;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void PopulateEnginesList();
	BOOL OnInitDialog();
	void UpdateListFilter();
	void AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditEnginessearch();
	afx_msg void OnBnClickedBtnRefreshEngines();
};
