// CVariablesTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CVariablesTab.h"
#include "resource.h"

// CVariablesTab dialog

IMPLEMENT_DYNAMIC(CVariablesTab, CDialogEx)

CVariablesTab::CVariablesTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VARIABLES, pParent)
{

}

CVariablesTab::~CVariablesTab()
{
}

void CVariablesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVariablesTab, CDialogEx)
END_MESSAGE_MAP()


// CVariablesTab message handlers
