#pragma once
#include "afxdialogex.h"


// CServerInfoDlg dialog

class CServerInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerInfoDlg)
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CServerInfoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CServerInfoDlg();
	int CServerInfoDlg::OnInitDialog();
	afx_msg void OnBnClickedBtnClose();
	CTabCtrl* m_pTabControl;
	afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};
