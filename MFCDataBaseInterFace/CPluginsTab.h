#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

class CPluginsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CPluginsTab)

public:
	CPluginsTab(CWnd* pParent = nullptr);
	virtual ~CPluginsTab();
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLUGINS };
#endif

protected:
	std::shared_ptr<CDBConnection> db;
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	void UpdateListFilter();
	void PopulatePluginsList();
	void AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditPluginsearch();
	afx_msg void OnBnClickedBtnRefreshPlugins();
};
