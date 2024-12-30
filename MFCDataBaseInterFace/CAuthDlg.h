#pragma once

#include <memory>

#include "CMainDlg.h"
#include "MFCPendulumDlg.h"

class CDBConnection;

class CAuthDlg : public CDialogEx
{
private:
  HICON m_hIcon;
  std::shared_ptr<CDBConnection> db;
  CMainDlg mainWindow;
  CEdit m_editServerName;
  CString m_serverPassword;
  CString m_cServer;
  CString m_cUser;
  CString m_cPassword;
  CString m_cDatabase;
  CMFCPendulumDlg* m_pendulumDlg;
public:
  CAuthDlg(CWnd* pParent = nullptr);
  CAuthDlg(std::shared_ptr<CDBConnection> db, CWnd* pParent = nullptr);



#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_MFCDATABASEINTERFACE_DIALOG };
#endif

protected:

  virtual BOOL OnInitDialog();
  HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  afx_msg void OnBnClickedBtnConnect();
  afx_msg void OnBnClickedBtnExit();
  afx_msg void OnBnClickedBtnShowPassword();
  afx_msg void OnCbnSelchangeComboLanguage();
  void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  BOOL OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
  BOOL OnEraseBkgnd(CDC* pDC);
  BOOL PreTranslateMessage(MSG* pMsg);

  BOOL PopulateLangCombo();

  virtual void DoDataExchange(CDataExchange* pDX);
  DECLARE_MESSAGE_MAP()

private:
  void StartDemoMode();
};
