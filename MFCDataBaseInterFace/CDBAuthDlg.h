
#pragma once
#include "CDBConnection.h"
#include "CDBMainDlg.h"
#include <memory>
// CDBAuthDlg dialog
class CDBAuthDlg : public CDialogEx
{
// Construction
public:
	CDBAuthDlg(CWnd* pParent = nullptr);	// standard constructor
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDATABASEINTERFACE_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void CDBAuthDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	BOOL CDBAuthDlg::PreTranslateMessage(MSG* pMsg);
// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CWinThread* m_workerThread;
	afx_msg void OnBnClickedBtnConnect();
	std::shared_ptr<CDBConnection> db;
	CDBMainDlg mainWindow;
	CEdit m_editServerName;

	CString m_serverPassword;
	CString m_cServer;
	CString m_cUser;
	CString m_cPassword;
	CString m_cDatabase;

	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnShowPassword();
};
