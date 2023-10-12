
#include "pch.h"
#include "afxdialogex.h"
#include "CServerInfoDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CServerInfoDlg, CDialogEx)

CServerInfoDlg::CServerInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERINFO, pParent), m_pTabControl(nullptr)
{

}

CServerInfoDlg::~CServerInfoDlg()
{
}

void CServerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CServerInfoDlg::OnBnClickedBtnClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CServerInfoDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

int CServerInfoDlg::OnInitDialog()
{
	m_pTabControl = (CTabCtrl*)GetDlgItem(IDC_TAB);
	m_pTabControl->InsertItem(1, _T("User"));
	m_pTabControl->InsertItem(1, _T("Server"));

	return true;
}




void CServerInfoDlg::OnBnClickedBtnClose()
{
	this->EndDialog(IDCANCEL);
}


void CServerInfoDlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
