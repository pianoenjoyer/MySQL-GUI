// CDatabasesTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CDatabasesTab.h"
#include "resource.h"

// CDatabasesTab dialog

IMPLEMENT_DYNAMIC(CDatabasesTab, CDialogEx)

CDatabasesTab::CDatabasesTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATABASES, pParent)
{

}

CDatabasesTab::~CDatabasesTab()
{
}

void CDatabasesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatabasesTab, CDialogEx)
END_MESSAGE_MAP()


// CDatabasesTab message handlers
