// CVariablesTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CVariablesTab.h"
#include "resource.h"
#include "Convertions.h"
#include "SharedFunctions.h"
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
    ON_EN_CHANGE(IDC_EDIT_VARSEARCH, &CVariablesTab::OnEnChangeEditVarsearch)
END_MESSAGE_MAP()

BOOL CVariablesTab::OnInitDialog()
{

    CDialogEx::OnInitDialog();
    AfxInitRichEdit2();
    PopulateVariablesList();
    SetTotalNum((CListCtrl*)GetDlgItem(IDC_LIST_VARS), GetDlgItem(IDC_TOTAL));

    return TRUE;
}

void CVariablesTab::PopulateVariablesList()
{
    // Assuming m_listCtrl is the ID of your list control
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_VARS);

    // Add columns if not already added
    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Variable"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Value"), LVCFMT_LEFT, 200);
    }

    // Clear existing items
    pListCtrl->DeleteAllItems();

    // Assuming db is an instance of your database class
    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES");

    // Process the result set
    while (resultSet->next()) 
    {
        CString Variable = SQLStringToCString(resultSet->getString("Variable_name"));
        CString Value = SQLStringToCString(resultSet->getString("Value"));
        AddVariableToList(pListCtrl, Variable, Value);
    }

    // Don't forget to close the result set
    delete resultSet;
}


void CVariablesTab::AddVariableToList(CListCtrl* pListCtrl, const CString& variable, const CString& value)
{
    int nIndex = pListCtrl->GetItemCount();

    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, variable);

    // Set text for the second column
    pListCtrl->SetItemText(nIndex, 1, value);
}


void CVariablesTab::UpdateListFilter()
{
    // Get the filter text from the edit control
    CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_EDIT_VARSEARCH);
    CString filterText;
    pEditFilter->GetWindowText(filterText);

    // Assuming m_listCtrl is the ID of your list control
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_VARS);

    // Clear existing items
    pListCtrl->DeleteAllItems();

    // Assuming db is an instance of your database class
    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES");

    // Process the result set and add items that match the filter
    while (resultSet->next())
    {
        CString variable = SQLStringToCString(resultSet->getString("Variable_name"));
        CString value = SQLStringToCString(resultSet->getString("Value"));

        // Check if the variable or value contains the filter text
        if (variable.Find(filterText) != -1 || value.Find(filterText) != -1)
        {
            // Add the item to the list
            int nIndex = pListCtrl->GetItemCount();
            pListCtrl->InsertItem(nIndex, variable);
            pListCtrl->SetItemText(nIndex, 1, value);
        }
    }

    // Don't forget to close the result set
    delete resultSet;
}

void CVariablesTab::OnEnChangeEditVarsearch()
{
    UpdateListFilter();
}
