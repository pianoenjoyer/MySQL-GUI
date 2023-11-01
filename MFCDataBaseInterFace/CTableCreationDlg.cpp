// CTableCreationDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CTableCreationDlg.h"
#include "resource.h"

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
    m_ListCreateTable.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(2, _T("Length/Values"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(3, _T("Collation"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(4, _T("Attributes"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(5, _T("Null"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(6, _T("Index"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(7, _T("A_I"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(8, _T("Comments"), LVCFMT_LEFT, 100);
    m_ListCreateTable.InsertColumn(9, _T("Virtuality"), LVCFMT_LEFT, 100);

    // Embedding controls (pseudo-code for high-level concept)

    // 1. For columns that need an edit control (Name, Length/Values, Comments)
    CEdit* pEdit = new CEdit;
    pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_EDIT1);
    // Position and size appropriately using CListCtrl's GetSubItemRect
    pEdit->MoveWindow(&CRect(0, 0, 0, 0));
    // 2. For columns that need a combobox (Type, Collation, Attributes, Index, Virtuality)
    CComboBox* pCombo = new CComboBox;
    pCombo->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, CRect(0, 0, 0, 0), this, IDC_COMBO1);
    // Populate with appropriate items using AddString
    // Position and size appropriately

    // 3. For columns that need a checkbox (Null, A_I)
    CButton* pCheck = new CButton;
    pCheck->Create(_T("Caption"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, CRect(0, 0, 0, 0), this, IDC_CHECK1);
    // Position and size appropriately

    // NOTE: For a clean implementation, you'll probably want to handle LVN_ITEMACTIVATE to decide when
    // to show which control. Also, controls will need to be dynamically shown/hidden and values synced
    // with the list control items/subitems.

    return TRUE;
}

CTableCreationDlg::~CTableCreationDlg()
{
}

void CTableCreationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TABLECREATE, m_ListCreateTable);
}


BEGIN_MESSAGE_MAP(CTableCreationDlg, CDialogEx)
END_MESSAGE_MAP()


// CTableCreationDlg message handlers
