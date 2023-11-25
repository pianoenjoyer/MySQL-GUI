#pragma once
#include "afxdialogex.h"


// CHomeTab dialog

class CHomeTab : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeTab)

public:
	CHomeTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHomeTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
