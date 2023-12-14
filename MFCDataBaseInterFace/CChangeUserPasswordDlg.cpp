// CChangeUserPasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CChangeUserPasswordDlg.h"
#include "resource.h"

// CChangeUserPasswordDlg dialog

IMPLEMENT_DYNAMIC(CChangeUserPasswordDlg, CDialogEx)

CChangeUserPasswordDlg::CChangeUserPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEUSRPWD, pParent)
{

}

CChangeUserPasswordDlg::CChangeUserPasswordDlg(CWnd* pParent /*=nullptr*/, std::shared_ptr<CDBConnection> db)
	: CDialogEx(IDD_CHANGEUSRPWD, pParent) , db(db)
{

}


CChangeUserPasswordDlg::~CChangeUserPasswordDlg()
{
}

void CChangeUserPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangeUserPasswordDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeUserPasswordDlg message handlers
