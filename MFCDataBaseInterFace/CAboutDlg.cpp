// CAboutDlg.cpp : implementation file
//
#include "pch.h"
#include "afxdialogex.h"
#include "CDBInterfaceApp.h"
#include "CAboutDlg.h"

#include "CAuthDlg.h"
#include "CLoginDataSave.h"
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CMainDlg.h"
#include "resource.h"

#include <atlimage.h>

// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUT, pParent)
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAboutDlg message handlers
