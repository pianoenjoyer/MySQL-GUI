// CHomeTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CHomeTab.h"
#include "resource.h"

// CHomeTab dialog

IMPLEMENT_DYNAMIC(CHomeTab, CDialogEx)

CHomeTab::CHomeTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOME, pParent)
{

}

CHomeTab::~CHomeTab()
{
}

void CHomeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomeTab, CDialogEx)
END_MESSAGE_MAP()


// CHomeTab message handlers
