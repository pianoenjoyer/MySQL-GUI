#pragma once
#include "afxdialogex.h"
#include <memory>
#include "CDBConnection.h"

// CVariablesTab dialog

class CVariablesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CVariablesTab)

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	bool PopulateVariablesList();
	void UpdateListFilter();
	void AddVariableToList(CListCtrl* pListCtrl, const CString& variable, const CString& value);
	bool PopulateVariablesCombo();
	DECLARE_MESSAGE_MAP()
public:
	CVariablesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVariablesTab();
	std::shared_ptr<CDBConnection> db;
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	afx_msg void OnEnChangeEditVarsearch();
	afx_msg void OnBnClickedBtnRefreshVar();
	afx_msg void OnCbnSelchangeCmbVars();
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnLvnItemchangedListVars(NMHDR* pNMHDR, LRESULT* pResult);
};
