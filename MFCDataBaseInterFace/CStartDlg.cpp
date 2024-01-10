// CStartDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CStartDlg.h"
#include "resource.h"
#include "CMainDlg.h"
// CStartDlg dialog

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent)
{

}

CStartDlg::CStartDlg(CWnd* pParent, CMainDlg* mainDlg)
	: CDialogEx(IDD_LOADING, pParent), m_mainDlg(mainDlg)
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
	m_mainDlg->DoModal();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
END_MESSAGE_MAP()


// CStartDlg message handlers
