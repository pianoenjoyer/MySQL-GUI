#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include <memory>

class CHomeTab : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeTab)

public:
	CHomeTab(CWnd* pParent = nullptr);
	virtual ~CHomeTab();
	std::shared_ptr<CDBConnection> db;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	virtual void				DoDataExchange(CDataExchange* pDX);
	bool						AddGeneralInfoItem(CListCtrl* pListCtrl, LPCTSTR lpszProperty, LPCTSTR lpszValue);
	void						PopulateGeneralInfo();
	BOOL						OnInitDialog();
	bool						SetConnectionCollation(const CString& collation);
	void						AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment);
	void AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license);
	void PopulatePluginsList();
	bool PopulateConnectionCollationDropdown();
	void PopulateEnginesList();
	CString GetCurrentConnectionCollation();
	DECLARE_MESSAGE_MAP()
public:

	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	afx_msg void OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeConncoll();
};
