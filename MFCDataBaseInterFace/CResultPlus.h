#pragma once
#include "afxdialogex.h"


// CResultPlus dialog

class CResultPlus : public CDialogEx
{
	DECLARE_DYNAMIC(CResultPlus)

public:
	CResultPlus(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CResultPlus();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT_SHORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
