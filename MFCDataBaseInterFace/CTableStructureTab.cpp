// CTableStructureTab.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CTableStructureTab.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CTableStructureTab, CDialogEx)

CTableStructureTab::CTableStructureTab(CWnd* pParent)
	: CDialogEx(IDD_TABLE_STRUCTURE, pParent)
{

}

CTableStructureTab::~CTableStructureTab()
{
}

void CTableStructureTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTableStructureTab, CDialogEx)
END_MESSAGE_MAP()
