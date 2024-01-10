// CStartDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CStartDlg.h"
#include "resource.h"

#include "resource.h"
#include "CMainDlg.h"
// CStartDlg dialog

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent)
{

}


CStartDlg::~CStartDlg()
{

}

void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)

END_MESSAGE_MAP()


// CStartDlg message handlers
