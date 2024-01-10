#pragma once
#include "afxdialogex.h"

// CStartDlg dialog

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = nullptr);   // standard constructor
	bool CStartDlg::SetLoadingState();
	BOOL CStartDlg::OnInitDialog();
	virtual ~CStartDlg();
	BOOL CStartDlg::SetLoadingState(int position, CString message);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	HBRUSH CStartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL CStartDlg::OnEraseBkgnd(CDC* pDC);
	void CStartDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
