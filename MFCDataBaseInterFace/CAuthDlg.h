#pragma once
#include <memory>
#include "CDBConnection.h"
#include "CMainDlg.h"

// CAuthDlg dialog
class CAuthDlg : public CDialogEx
{
public:
    CAuthDlg(CWnd* pParent = nullptr);    // standard constructor

    afx_msg void OnBnClickedBtnConnect();
    afx_msg void OnBnClickedBtnExit();
    afx_msg void OnBnClickedBtnShowPassword();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCDATABASEINTERFACE_DIALOG };
#endif

protected:
    //CLoginDataSave dataSaver;
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    BOOL PreTranslateMessage(MSG* pMsg);

    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    CWinThread* m_workerThread;
    std::shared_ptr<CDBConnection> db;
    CMainDlg mainWindow;

    CEdit m_editServerName;
    CString m_serverPassword;
    CString m_cServer;
    CString m_cUser;
    CString m_cPassword;
    CString m_cDatabase;
};