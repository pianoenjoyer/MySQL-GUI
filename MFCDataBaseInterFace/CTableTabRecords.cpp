// CTableTabRecords.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CTableTabRecords.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CTableTabRecords, CDialogEx)

CTableTabRecords::CTableTabRecords(CWnd* pParent)
	: CResultTab(IDD_RESULT_SHORT, 0)
{

}

CTableTabRecords::~CTableTabRecords()
{
}

void CTableTabRecords::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTableTabRecords, CDialogEx)
END_MESSAGE_MAP()
