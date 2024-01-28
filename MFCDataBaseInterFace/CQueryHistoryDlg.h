#pragma once
#include "afxdialogex.h"


// CQueryHistoryDlg dialog

class CQueryHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryHistoryDlg)

public:
	CQueryHistoryDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CQueryHistoryDlg();
	BOOL OnInitDialog();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HYSTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadHistoryToListCtrl(CListCtrl* listCtrl);
	DECLARE_MESSAGE_MAP()
};
