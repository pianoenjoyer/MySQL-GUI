// CTableCreationDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CTableCreationDlg.h"
#include "resource.h"
#include "SharedFunctions.h"
// CTableCreationDlg dialog

IMPLEMENT_DYNAMIC(CTableCreationDlg, CDialogEx)

CTableCreationDlg::CTableCreationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABLES, pParent)
{

}

BOOL CTableCreationDlg::OnInitDialog()
{

    CDialog::OnInitDialog();

    // Assuming m_ListCtrl is the CListCtrl variable associated with the control.

    // Setting up the columns
    //m_ListCreateTable.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(2, _T("Length/Values"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(3, _T("Collation"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(4, _T("Attributes"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(5, _T("Null"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(6, _T("Index"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(7, _T("A_I"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(8, _T("Comments"), LVCFMT_LEFT, 100);
    //m_ListCreateTable.InsertColumn(9, _T("Virtuality"), LVCFMT_LEFT, 100);

    return TRUE;
}

CTableCreationDlg::~CTableCreationDlg()
{
}

void CTableCreationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TABLENAME, m_editTableName);
    DDX_Control(pDX, IDC_COLLATION, m_cmbCollation);
    DDX_Control(pDX, IDC_COMMENTS, m_editComments);
    DDX_Control(pDX, IDC_COLUMNS, m_editAddColumns);
}


BEGIN_MESSAGE_MAP(CTableCreationDlg, CDialogEx)
    ON_BN_CLICKED(IDC_GO, &CTableCreationDlg::OnBnClickedGo)
END_MESSAGE_MAP()


// CTableCreationDlg message handlers


void CTableCreationDlg::OnBnClickedGo()
{
    CStringW tableNameString;
    CStringW tableCommentsString;
    CStringW tableColumnAmount;

    m_editTableName.GetWindowTextW(tableNameString);
    m_editComments.GetWindowTextW(tableCommentsString);
    m_editAddColumns.GetWindowTextW(tableColumnAmount);


}
