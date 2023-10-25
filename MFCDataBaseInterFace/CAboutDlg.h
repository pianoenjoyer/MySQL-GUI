#pragma once
#include "pch.h"
#include "framework.h"
#include "CDBInterfaceApp.h"
#include "CDBAuthDlg.h"
#include "CLoginDataSave.h"
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBMainDlg.h"
#include <atlimage.h>
#include "afxdialogex.h"
#include "resource.h"

// CAboutDlg dialog

class CAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
