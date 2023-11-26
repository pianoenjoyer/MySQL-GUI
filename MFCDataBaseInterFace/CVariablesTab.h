#pragma once
#include "afxdialogex.h"


// CVariablesTab dialog

class CVariablesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CVariablesTab)

public:
	CVariablesTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CVariablesTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VARIABLES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
