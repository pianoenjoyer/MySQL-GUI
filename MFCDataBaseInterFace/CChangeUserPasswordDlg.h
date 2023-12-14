#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"
#include <memory>

// CChangeUserPasswordDlg dialog

class CChangeUserPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeUserPasswordDlg)

public:
	CChangeUserPasswordDlg(CWnd* pParent = nullptr);   // standard constructor
	CChangeUserPasswordDlg(CWnd* pParent /*=nullptr*/, std::shared_ptr<CDBConnection> db);
	virtual ~CChangeUserPasswordDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEUSRPWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
};
