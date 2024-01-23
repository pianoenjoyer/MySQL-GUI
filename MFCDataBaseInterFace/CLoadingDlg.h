#pragma once
#include "afxdialogex.h"

class CLoadingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadingDlg)
public:
	bool				SetImage(CStatic* pPicture, const int nImageID) const;
						CLoadingDlg(CWnd* pParent = nullptr);   // standard constructor
	BOOL				OnInitDialog();
	virtual				~CLoadingDlg();
	bool				SetLoadingState(const int position, const CString& message) const;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	HBRUSH				OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL				OnEraseBkgnd(CDC* pDC);
	bool				SetImage(CStatic* pPicture, const CString& path) const;
	void				OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
