// CResultTab.cpp : implementation file
//
#pragma once
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
	ON_BN_CLICKED(IDC_BTN_CLROUTPUT, &CResultTab::OnBnClickedBtnClroutput)
	ON_BN_CLICKED(IDC_CHECK_SHOWALL, &CResultTab::OnBnClickedCheckShowall)
	ON_EN_CHANGE(IDC_LIST_SEARCH, &CResultTab::OnEnChangeListSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_NMB_OF_ROWS, &CResultTab::OnCbnSelchangeComboNmbOfRows)
	ON_BN_CLICKED(IDC_BTN_FIRSTPAGE, &CResultTab::OnBnClickedBtnFirstpage)
	ON_BN_CLICKED(IDC_BTN_PREVPAGE, &CResultTab::OnBnClickedBtnPrevpage)
	ON_EN_CHANGE(IDC_EDIT_CURRENTPAGE, &CResultTab::OnEnChangeEditCurrentpage)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, &CResultTab::OnBnClickedBtnNextpage)
	ON_BN_CLICKED(IDC_BTN_LASTPAGE, &CResultTab::OnBnClickedBtnLastpage)
END_MESSAGE_MAP()


// CResultTab message handlers


void CResultTab::OnBnClickedBtnClroutput()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnBnClickedCheckShowall()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnEnChangeListSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CResultTab::OnCbnSelchangeComboNmbOfRows()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnBnClickedBtnFirstpage()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnBnClickedBtnPrevpage()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnEnChangeEditCurrentpage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CResultTab::OnBnClickedBtnNextpage()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnBnClickedBtnLastpage()
{
	// TODO: Add your control notification handler code here
}
