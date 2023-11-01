// CResultTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CResultTab.h"
#include "resource.h"


// CResultTab dialog

IMPLEMENT_DYNAMIC(CResultTab, CDialogEx)

CResultTab::CResultTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESULT, pParent)
{

}

CResultTab::~CResultTab()
{
}

void CResultTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUERY, m_resultList);
	DDX_Control(pDX, IDC_COMBO_NMB_OF_ROWS, m_comboLimit);
}


BEGIN_MESSAGE_MAP(CResultTab, CDialogEx)
END_MESSAGE_MAP()


// CResultTab message handlers
