#pragma once
#include "afxdialogex.h"
#include <memory>
#include "CDBConnection.h"

// CVariablesTab dialog

class CVariablesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CVariablesTab)

public:
	CVariablesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVariablesTab();
	std::shared_ptr<CDBConnection> db;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CVariablesTab::OnInitDialog();
	void CVariablesTab::PopulateVariablesList();
	void CVariablesTab::UpdateListFilter();
	void CVariablesTab::AddVariableToList(CListCtrl* pListCtrl, const CString& variable, const CString& value);
	DECLARE_MESSAGE_MAP()
public:
	inline void CVariablesTab::SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> CVariablesTab::GetDatabaseObject() { return db; };
	afx_msg void OnEnChangeVarsearch();
	afx_msg void OnEnChangeEditVarsearch();
};
