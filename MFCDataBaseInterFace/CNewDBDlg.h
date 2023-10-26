#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

// CNewDBDlg dialog

class CNewDBDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewDBDlg)

public:
	CNewDBDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewDBDlg();
	std::shared_ptr<CDBConnection> db;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWDB };
#endif

protected:
	BOOL CNewDBDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
