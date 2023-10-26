#pragma once
#include "afxdialogex.h"


// CNewDBDlg dialog

class CNewDBDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewDBDlg)

public:
	CNewDBDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewDBDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWDB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
