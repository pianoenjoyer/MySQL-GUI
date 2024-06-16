#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include <memory>

class CChangeUserPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeUserPasswordDlg)

public:
	CChangeUserPasswordDlg(CWnd* pParent = nullptr);
	CChangeUserPasswordDlg(CWnd* pParent, std::shared_ptr<CDBConnection> db);
	virtual ~CChangeUserPasswordDlg();
	BOOL OnInitDialog();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEUSRPWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
	CString GetCurrentUsername();
	bool GetEnteredPasswords(CString& new_password, CString& retyped);
	bool IsNoPasswordSelected();
	bool UpdatePassword(const CString& username, const CString& new_password);
public:
	afx_msg void OnBnClickedBtnGenerate();
	afx_msg void OnEnChangeEditEnterpassword();
	void CheckPasswordStrength(CString password);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedChangepwd();
	afx_msg void OnBnClickedRadioNopassword();
	afx_msg void OnBnClickedRadioPassword();
};
