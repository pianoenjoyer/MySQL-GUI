// CTableCreationDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CTableCreationDlg.h"
#include "resource.h"
#include "SharedFunctions.h"
#include "Convertions.h"
// CTableCreationDlg dialog

IMPLEMENT_DYNAMIC(CTableCreationDlg, CDialogEx)

CTableCreationDlg::CTableCreationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABLES, pParent)
{

}

BOOL CTableCreationDlg::OnInitDialog()
{

    CDialog::OnInitDialog();
    PopulateCharacterSetDropdown();
    PopulateStorageEngineDropdown();
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
}


BEGIN_MESSAGE_MAP(CTableCreationDlg, CDialogEx)
    ON_BN_CLICKED(IDC_GO, &CTableCreationDlg::OnBnClickedGo)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CTableCreationDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()

void CTableCreationDlg::PopulateStorageEngineDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_STORAGE_ENGINE));
    if (!pComboBox || !db)
        return;

    // Execute the query to retrieve available storage engines
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW ENGINES;"));

    if (resultSet)
    {
        // Clear existing items in the combo box
        pComboBox->ResetContent();

        while (resultSet->next())
        {
            // Get the storage engine name
            std::string engineName = resultSet->getString("Engine");

            // Add the storage engine to the combo box
            pComboBox->AddString(CString(engineName.c_str()));
        }
    }
    else
    {
        // Handle the case where the query fails
        AfxMessageBox(L"Error retrieving storage engines from the database");
    }
}


void CTableCreationDlg::PopulateCharacterSetDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CHARSET));

    // Ensure the combo box and database connection are valid
    if (!pComboBox || !db)
        return;

    // Execute the query to retrieve available character sets
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW CHARACTER SET;"));

    if (resultSet)
    {
        // Clear existing items in the combo box
        pComboBox->ResetContent();

        while (resultSet->next())
        {
            // Get the character set name
            std::string charsetName = resultSet->getString("Charset");

            // Add the character set to the combo box
            pComboBox->AddString(CString(charsetName.c_str()));
        }
    }
    else
    {
        // Handle the case where the query fails
        AfxMessageBox(L"Error retrieving character sets from the database");
    }
}


void CTableCreationDlg::OnBnClickedGo()
{
    CStringW tableNameString;
    CStringW tableCommentsString;
    CStringW tableColumnAmount;

    m_editTableName.GetWindowTextW(tableNameString);
    m_editComments.GetWindowTextW(tableCommentsString);

    // Get selected character set from combo box
    CComboBox* pCharsetComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COLLATION));
    CStringW selectedCharset;
    pCharsetComboBox->GetWindowTextW(selectedCharset);

    // Get selected storage engine from combo box
    CComboBox* pEngineComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_STORAGE_ENGINE));
    CStringW selectedEngine;
    pEngineComboBox->GetWindowTextW(selectedEngine);

    if (!tableNameString.IsEmpty() && !selectedCharset.IsEmpty() && !selectedEngine.IsEmpty())
    {
        CStringW sqlQuery;
        sqlQuery.Format(L"CREATE TABLE %s (", tableNameString);

        // Add a placeholder column for demonstration purposes
        CStringW columnName = L"example_column";
        CStringW columnType = L"INT";
        sqlQuery.AppendFormat(L"%s %s", columnName, columnType);

        sqlQuery.Append(L")");

        // Include selected character set and storage engine in the query
        sqlQuery.AppendFormat(L" CHARACTER SET %s ENGINE = %s", selectedCharset, selectedEngine);

        if (db->ExecuteNonQuery(CStringToSQLString(sqlQuery)))
        {
            AfxMessageBox(L"Table created successfully");
        }
        else
        {
            AfxMessageBox(L"Error creating table");
        }
    }
    else
    {
        AfxMessageBox(L"Please enter a valid table name and select character set and storage engine");
    }
}




void CTableCreationDlg::OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult)
{
    ShellExecute(0, 0, L"https://dev.mysql.com/doc/refman/8.0/en/storage-engines.html", 0, 0, SW_SHOW);
    *pResult = 0;
}
