#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

// CPluginsTab dialog

class CPluginsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CPluginsTab)

public:
	CPluginsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPluginsTab();
	inline void CPluginsTab::SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLUGINS };
#endif

protected:
	std::shared_ptr<CDBConnection> db;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CPluginsTab::OnInitDialog();
	void CPluginsTab::UpdateListFilter();
	void CPluginsTab::PopulatePluginsList();
	void CPluginsTab::AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditPluginsearch();
	afx_msg void OnBnClickedBtnRefreshPlugins();
};
