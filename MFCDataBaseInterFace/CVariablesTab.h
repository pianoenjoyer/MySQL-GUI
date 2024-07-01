#pragma once

#include <memory>

class CDBConnection;

class CVariablesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CVariablesTab)

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLES };
#endif

private:
	std::shared_ptr<CDBConnection> db;

public:
	CVariablesTab(CWnd* pParent = nullptr);
	virtual ~CVariablesTab();

	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };

protected:
	BOOL OnInitDialog();
	afx_msg void OnEnChangeEditVarsearch();
	afx_msg void OnBnClickedBtnRefreshVar();
	afx_msg void OnCbnSelchangeCmbVars();
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnLvnItemchangedListVars(NMHDR* pNMHDR, LRESULT* pResult);

	bool PopulateVariablesList();
	void UpdateListFilter();
	void AddVariableToList(CListCtrl* pListCtrl, const CString& variable, const CString& value);
	bool PopulateVariablesCombo();

	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};
