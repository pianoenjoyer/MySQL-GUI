#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"

// CNewDBDlg dialog

class CNewDBDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewDBDlg)

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWDB };
#endif

protected:
	BOOL OnInitDialog();
	void PopulateCharacterSetDropdown();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CNewDBDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewDBDlg();
	std::shared_ptr<CDBConnection> db;
	afx_msg void OnBnClickedBtnCreatedb();
};
