#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include <memory>
// CHomeTab dialog

class CHomeTab : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeTab)

public:
	CHomeTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHomeTab();
	std::shared_ptr<CDBConnection> db;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool CHomeTab::AddGeneralInfoItem(CListCtrl* pListCtrl, LPCTSTR lpszProperty, LPCTSTR lpszValue);
	void CHomeTab::PopulateGeneralInfo();
	BOOL CHomeTab::OnInitDialog();
	bool CHomeTab::SetConnectionCollation(const CString& collation);
	void CHomeTab::AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment);
	void CHomeTab::AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license);
	void CHomeTab::PopulatePluginsList();
	bool CHomeTab::PopulateConnectionCollationDropdown();
	void CHomeTab::PopulateEnginesList();
	CString CHomeTab::GetCurrentConnectionCollation();
	DECLARE_MESSAGE_MAP()
public:

	inline void CHomeTab::SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> CHomeTab::GetDatabaseObject() { return db; };
	afx_msg void OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeConncoll();
};
