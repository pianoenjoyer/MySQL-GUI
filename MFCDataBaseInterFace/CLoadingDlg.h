#pragma once
#include "afxdialogex.h"

// CLoadingDlg dialog

class CLoadingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadingDlg)

public:
	CLoadingDlg(CWnd* pParent = nullptr);   // standard constructor
	bool CLoadingDlg::SetLoadingState();
	BOOL CLoadingDlg::OnInitDialog();
	virtual ~CLoadingDlg();
	BOOL CLoadingDlg::SetLoadingState(int position, CString message);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	HBRUSH CLoadingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL CLoadingDlg::OnEraseBkgnd(CDC* pDC);
	void CLoadingDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
