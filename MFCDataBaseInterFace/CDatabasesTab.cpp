// CDatabasesTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CDatabasesTab.h"
#include "resource.h"
#include "Convertions.h"
// CDatabasesTab dialog

IMPLEMENT_DYNAMIC(CDatabasesTab, CDialogEx)

void SetTotalNum(CListCtrl* pList, CWnd* pStatic)
{
    int totalItems = pList->GetItemCount();
    CString totalItemsStr;
    totalItemsStr.Format(_T("%d"), totalItems);
    pStatic->SetWindowText(totalItemsStr);
}

CDatabasesTab::CDatabasesTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATABASES, pParent)
{

}

CDatabasesTab::~CDatabasesTab()
{
}

void CDatabasesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CDatabasesTab::OnInitDialog()
{

    CDialogEx::OnInitDialog();
    AfxInitRichEdit2();
    PopulateDatabaseList();
    PopulateCharacterSetDropdown();


    SetTotalNum((CListCtrl*)GetDlgItem(IDC_LIST_DATABASES), GetDlgItem(IDC_TOTAL));
    return TRUE;
}

void CDatabasesTab::PopulateDatabaseList()
{
    auto pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATABASES);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Database"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Collation"), LVCFMT_LEFT, 200);
    }

    auto resultSet = db->ExecuteQuery("SHOW DATABASES");

    // Clear existing items
    pListCtrl->DeleteAllItems();

    while (resultSet->next())
    {
        // Get the database name
        CString databaseName = SQLStringToCString(resultSet->getString(1));

        // Get the collation for the current database
        CString collation = GetDatabaseCollation(databaseName);

        // Add the database and collation to the list
        AddDatabaseInfoToList(pListCtrl, databaseName, collation);
    }

    delete resultSet;
}

CString CDatabasesTab::GetDatabaseCollation(const CString& databaseName)
{
    // Query to get the collation for a specific database
    CString query = L"SELECT DEFAULT_COLLATION_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '";
    query += databaseName;
    query += "'";

    auto collationResultSet = db->ExecuteQuery(CStringToSQLString(query));

    CString collation;

    if (collationResultSet->next())
    {
        collation = SQLStringToCString(collationResultSet->getString(1));
    }

    delete collationResultSet;

    return collation;
}


void CDatabasesTab::AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation)
{
    int nIndex = pListCtrl->GetItemCount();
    pListCtrl->InsertItem(nIndex, databaseName);
    pListCtrl->SetItemText(nIndex, 1, collation);
}

BEGIN_MESSAGE_MAP(CDatabasesTab, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CREATEDB, &CDatabasesTab::OnBnClickedBtnCreatedb)
    ON_BN_CLICKED(IDC_BTN_DELETEDB, &CDatabasesTab::OnBnClickedBtnDeletedb)
END_MESSAGE_MAP()


// CDatabasesTab message handlers


void CDatabasesTab::OnBnClickedBtnCreatedb()
{
    CString databaseName, charsetName;

    CComboBox* pComboBox = (CComboBox*)(GetDlgItem(IDC_COMBO_CHARSET));
    int selIndex = pComboBox->GetCurSel();
    if (selIndex != CB_ERR)
    {
        pComboBox->GetLBText(selIndex, charsetName);
    }

    GetDlgItem(IDC_EDIT_DBNAME)->GetWindowTextW(databaseName);

    if (!databaseName.IsEmpty() && !charsetName.IsEmpty())
    {
        CString query;
        query.Format(_T("CREATE DATABASE `%s` CHARACTER SET %s;"), databaseName, charsetName);
        db->ExecuteQuery(CStringToSQLString(query), query);
        if (query = L"No result available")
        {
            AfxMessageBox(L"Database created succesfuly");
            PopulateDatabaseList();
            return;
        }
        AfxMessageBox(query);
        PopulateDatabaseList();
    }
    else
    {
        AfxMessageBox(L"Fields are empty");
    }
}


void CDatabasesTab::PopulateCharacterSetDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CHARSET));
    if (!pComboBox || !db)
        return;

    auto resultSet = db->ExecuteQuery("SHOW CHARACTER SET;");
    if (resultSet)
    {
        while (resultSet->next())
        {
            std::string charsetName = resultSet->getString("Charset"); // Get the character set name
            pComboBox->AddString(CString(charsetName.c_str())); // Add the character set to the combo box
        }
        delete resultSet;  // Clean up
    }
}



void CDatabasesTab::OnBnClickedBtnDeletedb()
{
    // Assuming m_listCtrl is the ID of your list control
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATABASES);

    // Get the selected item index
    int selectedIndex = pListCtrl->GetNextItem(-1, LVNI_SELECTED);

    // Check if any item is selected
    if (selectedIndex != -1) {
        // Get the name of the selected database
        CString dbName = pListCtrl->GetItemText(selectedIndex, 0);

        // Prompt for confirmation
        CString message;
        message.Format(_T("Are you sure you want to delete the database '%s'?"), dbName);

        if (MessageBox(message, _T("Confirmation"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
            // Execute the query to delete the database
            CString query;
            CString errorString;
            query.Format(_T("DROP DATABASE IF EXISTS `%s`"), dbName);
            db->ExecuteQuery(CStringToSQLString(query), errorString);
            if (errorString = L"No result available")
            {
                AfxMessageBox(L"Database deleted successfuly");
                PopulateDatabaseList();
            }

        }
    }
    else {
        // No item selected, show a message or take appropriate action
        MessageBox(_T("Please select a database to delete."), _T("Information"), MB_OK | MB_ICONINFORMATION);
    }
}

