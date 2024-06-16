// CDatabasesTab.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CDatabasesTab.h"
#include "resource.h"
#include "Convertions.h"
#include "SharedFunctions.h"
#include "Theme.h"

IMPLEMENT_DYNAMIC(CDatabasesTab, CDialogEx)

CDatabasesTab::CDatabasesTab(CWnd* pParent)
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
    PopulateDatabaseList();
    PopulateCharacterSetDropdown();
    PopulateDatabaseDropdown();

    SetTotalNum((CListCtrl*)GetDlgItem(IDC_LIST_DATABASES), GetDlgItem(IDC_TOTAL));
    return TRUE;
}

bool CDatabasesTab::PopulateDatabaseDropdown()
{
    auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_DATABASES);
    if (!pCombo)
    {
        return false;
    }
    pCombo->ResetContent();
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW DATABASES"));

    while (resultSet->next())
    {
        CString databaseName = SQLStringToCString(resultSet->getString(1));
        pCombo->AddString(databaseName);

    }
    return true;
}


void CDatabasesTab::PopulateDatabaseList()
{
    auto pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATABASES);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Database"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Collation"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(2, _T("Size (MB)"), LVCFMT_LEFT, 100);
    }

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW DATABASES"));
    pListCtrl->DeleteAllItems();

    while (resultSet->next())
    {
        CString databaseName = SQLStringToCString(resultSet->getString(1));
        CString collation = GetDatabaseCollation(databaseName);
        CString size = GetDatabaseSize(databaseName);
        AddDatabaseInfoToList(pListCtrl, databaseName, collation, size);

    }
}


CString CDatabasesTab::GetDatabaseSize(const CString& databaseName)
{
    CString query;
    query.Format(_T("SELECT SUM(DATA_LENGTH + INDEX_LENGTH) / 1024 / 1024 AS size_MB FROM information_schema.TABLES WHERE TABLE_SCHEMA = '%s'"), databaseName);

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery(CStringToSQLString(query)));

    if (resultSet->next())
    {
        double sizeMB = resultSet->getDouble("size_MB");
        CString sizeStr;
        sizeStr.Format(_T("%.2f MB"), sizeMB);
        return sizeStr;
    }
    return _T("N/A");
}

void CDatabasesTab::AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation,
    const CString& size)
{
    int nIndex = pListCtrl->GetItemCount();
    pListCtrl->InsertItem(nIndex, databaseName);
    pListCtrl->SetItemText(nIndex, 1, collation);
    pListCtrl->SetItemText(nIndex, 2, size);
}

CString CDatabasesTab::GetDatabaseCollation(const CString& databaseName)
{
    CString query = L"SELECT DEFAULT_COLLATION_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '";
    query += databaseName;
    query += "'";

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery(CStringToSQLString(query)));

    CString collation;

    if (resultSet->next())
    {
        collation = SQLStringToCString(resultSet->getString(1));
    }

    return collation;
}




BEGIN_MESSAGE_MAP(CDatabasesTab, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CREATEDB, &CDatabasesTab::OnBnClickedBtnCreatedb)
    ON_BN_CLICKED(IDC_BTN_DELETEDB, &CDatabasesTab::OnBnClickedBtnDeletedb)
    ON_EN_CHANGE(IDC_FILTER_DATABASES, &CDatabasesTab::OnEnChangeFilterDatabases)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATABASES, &CDatabasesTab::OnLvnItemchangedListDatabases)
    ON_BN_CLICKED(IDC_BTN_REFRESHDBS, &CDatabasesTab::OnBnClickedBtnRefreshdbs)
END_MESSAGE_MAP()


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
        std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery(CStringToSQLString(query), query));
        if (query = L"No result available")
        {
            AfxMessageBox(L"Database created succesfuly");
            PopulateDatabaseList();
            return;
        }
        AfxMessageBox(query);
        PopulateDatabaseList();
        PopulateDatabaseDropdown();
    }
    else
    {
        AfxMessageBox(L"Fields are empty");
    }
}



void CDatabasesTab::PopulateCharacterSetDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COLLATION));
    if (!pComboBox || !db)
        return;

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW CHARACTER SET;"));
    if (resultSet)
    {
        while (resultSet->next())
        {
            std::string charsetName = resultSet->getString("Charset"); 
            pComboBox->AddString(CString(charsetName.c_str()));
        }
    }
}


void CDatabasesTab::OnBnClickedBtnDeletedb()
{
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_CMB_DATABASES);
    int selectedIndex = pComboBox->GetCurSel();

    if (selectedIndex != CB_ERR) {
        CString dbName;
        pComboBox->GetLBText(selectedIndex, dbName);

        CString message;
        message.Format(L"Are you sure you want to delete the database '%s'?", dbName);

        if (MessageBox(message, _T("Confirmation"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
            CString query;
            CString errorString;
            query.Format(L"DROP DATABASE IF EXISTS `%s`", dbName);
            int result = db->ExecuteNonQuery(CStringToSQLString(query));

            if (result) {
                AfxMessageBox(_T("Database deleted successfully"));
                PopulateDatabaseList();
                PopulateDatabaseDropdown();
            }
            else {
                AfxMessageBox(_T("Error deleting database: ") + errorString);
            }
        }
    }
    else {
        MessageBox(_T("Please select a database to delete."), _T("Information"), MB_OK | MB_ICONINFORMATION);
    }
}


void CDatabasesTab::UpdateListFilter()
{
    CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_FILTER_DATABASES);
    CString filterText;
    pEditFilter->GetWindowText(filterText);

    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATABASES);
    pListCtrl->DeleteAllItems();

    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW DATABASES"));
    while (resultSet->next())
    {
        CString databaseName = SQLStringToCString(resultSet->getString(1));

        if (databaseName.Find(filterText) != -1)
        {
            CString collation = GetDatabaseCollation(databaseName);
            CString size = GetDatabaseSize(databaseName);

            int nIndex = pListCtrl->GetItemCount();
            pListCtrl->InsertItem(nIndex, databaseName);
            pListCtrl->SetItemText(nIndex, 1, collation);
            pListCtrl->SetItemText(nIndex, 2, size);
        }
    }
}

void CDatabasesTab::OnEnChangeFilterDatabases()
{
    UpdateListFilter();
}


void CDatabasesTab::OnLvnItemchangedListDatabases(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    auto pList = (CListCtrl*)GetDlgItem(IDC_LIST_DATABASES);
    auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_DATABASES);
    if (!pList || !pCombo)
    {
    #ifdef DEBUG
            OutputDebugString(L"PopulateVariablesList()  control ptr is null");
    #endif
        return;
    }
    CString curVarName;
    int currentIndex = pCombo->GetCurSel();
    if (currentIndex != CB_ERR)
    {
        pCombo->GetLBText(currentIndex, curVarName);
    }

    if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
    {
        CString selectedItemText;
        int selectedIndex = pNMLV->iItem;

        if (selectedIndex >= 0)
        {
            selectedItemText = pList->GetItemText(selectedIndex, 0);
        }

        int comboIndex = pCombo->FindStringExact(0, selectedItemText);
        if (comboIndex != CB_ERR)
        {
            pCombo->SetCurSel(comboIndex);
        }
    }
    *pResult = 0;
}


void CDatabasesTab::OnBnClickedBtnRefreshdbs()
{
    PopulateDatabaseList();
    PopulateDatabaseDropdown();
}
