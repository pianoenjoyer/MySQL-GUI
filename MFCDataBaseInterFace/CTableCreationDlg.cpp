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
    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_TABLES);

    m_structureTab.Create(IDD_TABLE_STRUCTURE, pTabCtrl);
    m_resultTab.Create(IDD_RESULT, pTabCtrl);
   // m_recordsTab.Create(IDD_RESULT, pTabCtrl);

    TCITEM structure, records;
    structure.mask = TCIF_TEXT | TCIF_PARAM;
    structure.lParam = (LPARAM)&m_structureTab;
    structure.pszText = _T("Structure");
    pTabCtrl->InsertItem(0, &structure);

    records.mask = TCIF_TEXT | TCIF_PARAM;
    records.lParam = (LPARAM)&m_resultTab;
    records.pszText = _T("Records");
    pTabCtrl->InsertItem(1, &records);

    CRect rcStructure;
    pTabCtrl->GetItemRect(0, &rcStructure);
    m_structureTab.SetWindowPos(NULL, rcStructure.left, rcStructure.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcRecords;
    pTabCtrl->GetItemRect(1, &rcRecords);
    m_resultTab.SetWindowPos(NULL, rcStructure.left, rcRecords.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    m_structureTab.ShowWindow(TRUE);
    m_resultTab.ShowWindow(FALSE);

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
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TABLES, &CTableCreationDlg::OnTcnSelchangeTabTables)
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


void CTableCreationDlg::OnTcnSelchangeTabTables(NMHDR* pNMHDR, LRESULT* pResult)
{
    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_TABLES);
    int nSelectedTab = pTabCtrl->GetCurSel();
    m_recordsTab.ShowWindow(FALSE);
    m_structureTab.ShowWindow(FALSE);
    m_resultTab.ShowWindow(FALSE);
    switch (nSelectedTab)
    {
    case 0: 
        m_structureTab.ShowWindow(TRUE);
        break;

    case 1: 
        m_resultTab.ShowWindow(TRUE);
        break;

    default:
        
        break;
    }

    *pResult = 0;
}
