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
    PopulateDatabaseDropdown();
    PopulateTableDropdown();

    UpdateStructureList();

    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_TABLES);

    m_structureTab.Create(IDD_TABLE_STRUCTURE, pTabCtrl);
    m_resultTab.Create(IDD_RESULT_SHORT, pTabCtrl);
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


bool CTableCreationDlg::UpdateStructureList()
{
    CListCtrl* pListCtrl = (CListCtrl*)m_structureTab.GetDlgItem(IDC_LIST_STRUCTURE);
    CComboBox* pDatabaseCombo = (CComboBox*)GetDlgItem(IDC_CMB_DBS);
    CComboBox* pTableCombo = (CComboBox*)GetDlgItem(IDC_CMB_TBS);

    if (!pListCtrl || !pTableCombo || !pDatabaseCombo || !db)
    {
        return false;
    }

    // Delete all items and columns
    pListCtrl->DeleteAllItems();
    int numCols = pListCtrl->GetHeaderCtrl()->GetItemCount();
    for (int colIndex = numCols - 1; colIndex >= 0; --colIndex)
    {
        pListCtrl->DeleteColumn(colIndex);
    }

    CString selectedTable;
    pTableCombo->GetLBText(pTableCombo->GetCurSel(), selectedTable);

    CString selectedDatabase;
    pDatabaseCombo->GetLBText(pDatabaseCombo->GetCurSel(), selectedDatabase);

    CString query = L"DESCRIBE " + selectedDatabase + L"." + selectedTable;
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery(CStringToSQLString(query)));

    if (!resultSet)
    {
        return false;
    }

    // Insert columns dynamically based on the query result
    numCols = resultSet->getMetaData()->getColumnCount();
    for (int colIndex = 0; colIndex < numCols; ++colIndex)
    {
        CString colName(resultSet->getMetaData()->getColumnName(colIndex + 1).c_str());
        if (pListCtrl->InsertColumn(colIndex, colName, LVCFMT_LEFT, 150) == -1)
        {
            // Handle column insertion failure
            // You might want to log an error message or take appropriate action
            return false;
        }
    }

    int index = 0;
    while (resultSet->next())
    {
        // Insert a new item for each row
        int itemIndex = pListCtrl->InsertItem(index, L"");  // Insert an empty item for each row
        if (itemIndex == -1)
        {
            // Handle item insertion failure
            // You might want to log an error message or take appropriate action
            return false;
        }

        for (int colIndex = 0; colIndex < numCols; ++colIndex)
        {
            CString colValue(resultSet->getString(colIndex + 1).c_str());
            if (pListCtrl->SetItemText(itemIndex, colIndex, colValue) == FALSE)
            {
                // Handle text insertion failure
                // You might want to log an error message or take appropriate action
                return false;
            }
        }

        ++index;
    }

    return true;
}



bool CTableCreationDlg::UpdateRecorldsList()
{
    CComboBox* pDatabaseCombo = (CComboBox*)GetDlgItem(IDC_CMB_DBS);
    CComboBox* pTableCombo = (CComboBox*)GetDlgItem(IDC_CMB_TBS);
    if (m_pCurResultSet)
    {
        delete m_pCurResultSet;
        m_pCurResultSet = nullptr;
    }
    if (!pTableCombo || !pDatabaseCombo || !db)
    {
        return false;
    }

    CString selectedTable;
    pTableCombo->GetLBText(pTableCombo->GetCurSel(), selectedTable);

    CString selectedDatabase;
    pDatabaseCombo->GetLBText(pDatabaseCombo->GetCurSel(), selectedDatabase);

    CString query = L"SELECT * FROM " + selectedDatabase + L"." + selectedTable;
    m_pCurResultSet = db->ExecuteQuery(CStringToSQLString(query));

    if (!m_pCurResultSet)
    {
        return false;
    }

    m_resultTab.BuildResultList(m_pCurResultSet, 0);

    return false;
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
    ON_CBN_SELCHANGE(IDC_CMB_DBS, &CTableCreationDlg::OnCbnSelchangeCmbDbs)
    ON_CBN_SELCHANGE(IDC_CMB_TBS, &CTableCreationDlg::OnCbnSelchangeCmbTbs)
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
    pComboBox->SetCurSel(0);
}


bool CTableCreationDlg::PopulateDatabaseDropdown()
{
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CMB_DBS);
    if (!pCombo || !db)
    {
        return false;
    }
    pCombo->ResetContent();

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW DATABASES"));
    if (!resultSet)
    {
        return false;
    }

    while (resultSet->next())
    {
        std::string dbName = resultSet->getString(1);
        CString strDbName(dbName.c_str());
        pCombo->AddString(strDbName);
    }
    pCombo->SetCurSel(0);

    return true;
}

bool CTableCreationDlg::PopulateTableDropdown()
{
    CComboBox* pComboTable = (CComboBox*)GetDlgItem(IDC_CMB_TBS);
    CComboBox* pComboDatabase = (CComboBox*)GetDlgItem(IDC_CMB_DBS);
    if (!pComboTable || !pComboDatabase || !db)
    {
        return false;
    }
    pComboTable->ResetContent();

    CString selectedDatabase;
    pComboDatabase->GetLBText(pComboDatabase->GetCurSel(), selectedDatabase);

    CString query = L"SHOW TABLES FROM " + selectedDatabase;

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery(CStringToSQLString(query)));
    if (!resultSet)
    {
        return false;
    }

    while (resultSet->next())
    {
        std::string tableName = resultSet->getString(1);
        CString strTableName(tableName.c_str());
        pComboTable->AddString(strTableName);
    }
    pComboTable->SetCurSel(0);

    return true;
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
    pComboBox->SetCurSel(0);
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


void CTableCreationDlg::OnCbnSelchangeCmbDbs()
{
    PopulateTableDropdown();
}


void CTableCreationDlg::OnCbnSelchangeCmbTbs()
{
    UpdateStructureList();
    UpdateRecorldsList();

}
