// CResultPlus.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CResultPlus.h"
#include "resource.h"

// CResultPlus dialog

IMPLEMENT_DYNAMIC(CResultPlus, CDialogEx)

CResultPlus::CResultPlus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESULT_SHORT, pParent)
{

}

CResultPlus::~CResultPlus()
{
}

void CResultPlus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResultPlus, CDialogEx)
END_MESSAGE_MAP()


// CResultPlus message handlers
