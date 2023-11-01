// CQueryTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CQueryTab.h"
#include "resource.h"

// CQueryTab dialog

IMPLEMENT_DYNAMIC(CQueryTab, CDialogEx)

CQueryTab::CQueryTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUERY, pParent)
{

}

CQueryTab::~CQueryTab()
{
}

void CQueryTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryTab, CDialogEx)
END_MESSAGE_MAP()


// CQueryTab message handlers
