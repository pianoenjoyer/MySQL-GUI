#pragma once
#include "afxdialogex.h"


// CProceduresDlg dialog

class CProceduresDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProceduresDlg)

public:
	CProceduresDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProceduresDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCEDURES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
