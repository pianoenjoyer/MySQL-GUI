#pragma once
#include "afxdialogex.h"

// CStartDlg dialog

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = nullptr);   // standard constructor
	CStartDlg(CWnd* pParent, CMainDlg* mainDlg);   // standard constructor
	virtual ~CStartDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL CStartDlg::OnInitDialog();
	CMainDlg* m_mainDlg;
	DECLARE_MESSAGE_MAP()
};
