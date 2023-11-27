
#include "pch.h"
#include "afxdialogex.h"
#include "CDBInterfaceApp.h"

#include "CResultTab.h"
#include "CMainDlg.h"
#include "CAuthDlg.h"
#include "CServerInfoDlg.h"
#include "CNewDBDlg.h"
#include "CAboutDlg.h"

#include "Convertions.h"
#include "SendMessagesUtils.h"
#include <fstream>


IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

void ExpandAllItems(CTreeCtrl* pTree, HTREEITEM hItem, UINT nCode);

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MAIN, pParent)
{
    m_resultSet = nullptr;
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg() 
{

}


void CMainDlg::BuildDatabaseTree(CTreeCtrl& treeCtrl)
{
    if (treeCtrl.GetRootItem())
    {
        treeCtrl.DeleteAllItems();
    }

    //add new node 
    HTREEITEM tablesRoot = treeCtrl.InsertItem(_T("New"), treeCtrl.GetRootItem());
    // Fetch databases
    sql::ResultSet* databaseResultSet = db->ExecuteQuery("SHOW DATABASES");
    if (databaseResultSet)
    {
        while (databaseResultSet->next())
        {
            std::string databaseName = databaseResultSet->getString(1);
            HTREEITEM databaseItem = treeCtrl.InsertItem(CA2T(databaseName.c_str()));
            //insert create new db element


            // Fetch tables for the current database
            sql::ResultSet* tableResultSet = db->ExecuteQuery("SHOW TABLES IN " + databaseName);
            if (tableResultSet && tableResultSet->rowsCount() > 0)
            {
                HTREEITEM tablesRoot = treeCtrl.InsertItem(_T("[TABLES]"), databaseItem);
                while (tableResultSet->next())
                {
                    std::string tableName = tableResultSet->getString(1);
                    HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()), tablesRoot);

                    // Fetch columns for the current table
                    sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + databaseName + "." + tableName);
                    if (columnResultSet)
                    {
                        while (columnResultSet->next())
                        {
                            std::string columnName = columnResultSet->getString(1);
                            treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
                        }
                        delete columnResultSet;
                    }
                }
                delete tableResultSet;
            }

            // Fetch procedures
            CString query;
            CString database(CA2T(databaseName.c_str()));
            query.Format(CStringW(L"SHOW PROCEDURE STATUS WHERE Db = '%s'"), database);
            sql::ResultSet* procResultSet = db->ExecuteQuery(CStringToSQLString(query));
            if (procResultSet && procResultSet->rowsCount() > 0)
            {
                HTREEITEM proceduresRoot = treeCtrl.InsertItem(_T("[PROCEDURES]"), databaseItem);
                while (procResultSet->next())
                {
                    std::string procedureName = procResultSet->getString("Name");
                    treeCtrl.InsertItem(CA2T(procedureName.c_str()), proceduresRoot);
                }
                delete procResultSet;
            }

            // Fetch functions
            query.Format(CStringW(L"SHOW FUNCTION STATUS WHERE Db = '%s'"), database);
            sql::ResultSet* funcResultSet = db->ExecuteQuery(CStringToSQLString(query));
            if (funcResultSet && funcResultSet->rowsCount() > 0)
            {
                HTREEITEM functionsRoot = treeCtrl.InsertItem(_T("[FUNCTIONS]"), databaseItem);
                while (funcResultSet->next())
                {
                    std::string functionName = funcResultSet->getString("Name");
                    treeCtrl.InsertItem(CA2T(functionName.c_str()), functionsRoot);
                }
                delete funcResultSet;
            }

            // Fetch views
            sql::ResultSet* viewsResultSet = db->ExecuteQuery("SHOW FULL TABLES IN " + databaseName + " WHERE TABLE_TYPE LIKE 'VIEW'");
            if (viewsResultSet && viewsResultSet->rowsCount() > 0)
            {
                HTREEITEM viewsRoot = treeCtrl.InsertItem(_T("[VIEWS]"), databaseItem);
                while (viewsResultSet->next())
                {
                    std::string viewName = viewsResultSet->getString(1);
                    treeCtrl.InsertItem(CA2T(viewName.c_str()), viewsRoot);
                }
                delete viewsResultSet;
            }
        }
        delete databaseResultSet;
    }
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAINTAB, m_mainTabCtrl);
}


void SetRichControlTextSize(CRichEditCtrl* pRichEdit, int size)
{
    CHARFORMAT2 cf;
    ZeroMemory(&cf, sizeof(CHARFORMAT2));
    cf.cbSize = sizeof(CHARFORMAT2);
    cf.dwMask = CFM_SIZE; //change only size
    pRichEdit->GetSelectionCharFormat(cf);
    cf.yHeight = size;
    pRichEdit->SetSelectionCharFormat(cf);
}


BOOL CMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    //((CProgressCtrl*)GetDlgItem(IDC_EXECPROGRESS))->SetRange(0, 100);

    //set icon
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    m_resultSet = nullptr;
    //set visible at task bar
    ModifyStyleEx(0, WS_EX_APPWINDOW);

    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
    //give pointer to db object
    m_homeTab.db = this->db;
    m_queryTab.db = this->db;
    m_resultTab.db = this->db;
    m_tableTab.db = this->db;
    m_databasesTab.db = this->db;
    m_varsTab.db = this->db;
    m_charsetsTab.db = this->db;
    //fill db and tables
    
    // Create the two tabbed dialogs
    FillDatabaseDropdown();

    //init dlgs
    m_homeTab.Create(IDD_HOME, pTabCtrl);
    m_queryTab.Create(IDD_QUERY, pTabCtrl);
    m_resultTab.Create(IDD_RESULT, pTabCtrl);
    m_exportTab.Create(IDD_EXPORT, pTabCtrl);
    m_tableTab.Create(IDD_TABLES, pTabCtrl);
    m_charsetsTab.Create(IDD_CHARSETS, pTabCtrl);
    m_databasesTab.Create(IDD_DATABASES, pTabCtrl);
    m_varsTab.Create(IDD_VARIABLES, pTabCtrl);

    //insert into tab control

    TCITEM item0, item1, item2, item3, item4, item5, item6, item7;

    item0.mask = TCIF_TEXT | TCIF_PARAM;
    item0.lParam = (LPARAM)&m_homeTab;
    item0.pszText = _T("Home");
    pTabCtrl->InsertItem(0, &item0);

    item1.mask = TCIF_TEXT | TCIF_PARAM;
    item1.lParam = (LPARAM)&m_queryTab;
    item1.pszText = _T("Query");
    pTabCtrl->InsertItem(1, &item1);

    item2.mask = TCIF_TEXT | TCIF_PARAM;
    item2.lParam = (LPARAM)&m_resultTab;
    item2.pszText = _T("Result");
    pTabCtrl->InsertItem(2, &item2);

    item3.mask = TCIF_TEXT | TCIF_PARAM;
    item3.lParam = (LPARAM)&m_exportTab;
    item3.pszText = _T("Export");
    pTabCtrl->InsertItem(3, &item3);

    item4.mask = TCIF_TEXT | TCIF_PARAM;
    item4.lParam = (LPARAM)&m_tableTab;
    item4.pszText = _T("Tables");
    pTabCtrl->InsertItem(4, &item4);

    item5.mask = TCIF_TEXT | TCIF_PARAM;
    item5.lParam = (LPARAM)&m_databasesTab;//
    item5.pszText = _T("Databases");
    pTabCtrl->InsertItem(5, &item5);

    item6.mask = TCIF_TEXT | TCIF_PARAM;
    item6.lParam = (LPARAM)&m_varsTab;//
    item6.pszText = _T("Variables");
    pTabCtrl->InsertItem(6, &item6);

    item7.mask = TCIF_TEXT | TCIF_PARAM;
    item7.lParam = (LPARAM)&m_charsetsTab;//
    item7.pszText = _T("Charsets");
    pTabCtrl->InsertItem(7, &item7);


    //set pos
    CRect rcItem0;
    pTabCtrl->GetItemRect(0, &rcItem0);
    m_homeTab.SetWindowPos(NULL, rcItem0.left, rcItem0.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem1;
    pTabCtrl->GetItemRect(1, &rcItem1);
    m_queryTab.SetWindowPos(NULL, rcItem0.left, rcItem1.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem2;
    pTabCtrl->GetItemRect(2, &rcItem2);
    m_resultTab.SetWindowPos(NULL, rcItem0.left, rcItem2.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem3;
    pTabCtrl->GetItemRect(3, &rcItem3);
    m_exportTab.SetWindowPos(NULL, rcItem0.left, rcItem3.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem4;
    pTabCtrl->GetItemRect(4, &rcItem4);
    m_tableTab.SetWindowPos(NULL, rcItem0.left, rcItem4.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem5;
    pTabCtrl->GetItemRect(5, &rcItem5);
    m_databasesTab.SetWindowPos(NULL, rcItem0.left, rcItem5.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem6;
    pTabCtrl->GetItemRect(6, &rcItem6);
    m_varsTab.SetWindowPos(NULL, rcItem0.left, rcItem6.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    CRect rcItem7;
    pTabCtrl->GetItemRect(7, &rcItem7);
    m_charsetsTab.SetWindowPos(NULL, rcItem0.left, rcItem7.bottom + 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //initial show and hide
    m_homeTab.ShowWindow(SW_SHOW);
    m_queryTab.ShowWindow(SW_HIDE);
    m_resultTab.ShowWindow(SW_HIDE);
    m_exportTab.ShowWindow(SW_HIDE);
    m_proceduresTab.ShowWindow(SW_HIDE);
    m_tableTab.ShowWindow(SW_HIDE);
    m_databasesTab.ShowWindow(SW_HIDE);
    m_varsTab.ShowWindow(SW_HIDE);
    m_charsetsTab.ShowWindow(SW_HIDE);
    OnBnClickedBtnUpdate();

    return TRUE;
}

CString GetComboBoxSelectedValue(CComboBox* pComboBox)
{
    int index = pComboBox->GetCurSel(); // Get the index of the selected item.

    if (index != CB_ERR) {
        CString selectedValue;
        pComboBox->GetLBText(index, selectedValue); // Get the text of the selected item.
        return selectedValue;
    }

    // Return an empty string or handle the case where no item is selected.
    return L"";
}


void CMainDlg::SetCurDataBase()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_CMB_SEL_DB));
    CString database = GetComboBoxSelectedValue(pComboBox);
    db->ChangeCurrentDatabase(CStringToSQLString(database));
}

// fill drop down with table names of db
bool CMainDlg::FillDatabaseDropdown() 
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_CMB_SEL_DB));
    std::vector<sql::SQLString> databases;
    databases = db->GetDatabases();
    PopulateDropdown(pComboBox, databases);
    //pComboBox->SetCurSel(0);
    return true;
}

bool CMainDlg::FillTreeControl() 
{
    //Fill tree with db tables structure
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    BuildDatabaseTree(*pTree);
    //ExpandAllItems(pTree, TVI_ROOT, TVE_EXPAND);
    return true;
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BROWSE, &CMainDlg::OnBnClickedBtnBrowse)
    ON_BN_CLICKED(IDC_EXPORT, &CMainDlg::OnBnClickedExport)
    ON_BN_CLICKED(IDC_BTN_COLLAPSE, &CMainDlg::OnBnClickedBtnCollapse)
    ON_BN_CLICKED(IDC_BTN_EXPAND, &CMainDlg::OnBnClickedBtnExpand)
    ON_BN_CLICKED(IDC_BTN_UPDATE, &CMainDlg::OnBnClickedBtnUpdate)
    ON_COMMAND(ID_MENU_OPEN, &CMainDlg::OnMenuOpen)
    ON_COMMAND(ID_CONNECTION_DISCONNECT, &CMainDlg::OnConnectionDisconnect)
    ON_COMMAND(ID_CONNECTION_CHECKCONNECTION, &CMainDlg::OnConnectionCheckconnection)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK_SERVERINFO, &CMainDlg::OnNMClickSyslinkServerinfo)
    ON_COMMAND(ID_FILE_SAVEAS, &CMainDlg::OnFileSaveas)
    ON_COMMAND(ID_FILE_EXIT, &CMainDlg::OnFileExit)
    ON_COMMAND(ID_FILE_EXPORT, &CMainDlg::OnFileExport)
    ON_COMMAND(ID_EDIT_SELECTALL, &CMainDlg::OnEditSelectall)
    ON_COMMAND(ID_HELP_MYSQLDOCUMENTATION, &CMainDlg::OnHelpMysqldocumentation)
    ON_COMMAND(ID_ABOUT_VERSIONANDCREDITS, &CMainDlg::OnAboutVersionandcredits)
    ON_COMMAND(ID_HELP_SERVERINFO, &CMainDlg::OnHelpServerinfo)
    ON_BN_CLICKED(IDC_BTN_FORWARD, &CMainDlg::OnBnClickedBtnForward)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_STRUCTURE, &CMainDlg::OnTvnSelchangedTreeStructure)
    ON_NOTIFY(NM_CLICK, IDC_TREE_STRUCTURE, &CMainDlg::OnNMClickTreeStructure)
    ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, &CMainDlg::OnTcnSelchangeMaintab)
    ON_COMMAND(ID_EDIT_UNDO32772, &CMainDlg::OnEditUndo)
    ON_COMMAND(ID_EDIT_REDO32773, &CMainDlg::OnEditRedo)
    ON_COMMAND(ID_EDIT_CUT32787, &CMainDlg::OnEditCut)
    ON_COMMAND(ID_EDIT_COPY32788, &CMainDlg::OnEditCopy)
    ON_WM_SIZE()
    ON_COMMAND(ID_EDIT_PASTE32794, &CMainDlg::OnEditPaste)
    ON_CBN_SELCHANGE(IDC_CMB_SEL_DB, &CMainDlg::OnCbnSelchangeCmbSelDb)
END_MESSAGE_MAP()

//open .sql file
void CMainDlg::OnBnClickedBtnBrowse()
{
    CFileDialog fileOpenDialog(TRUE,
        L"SQL files|sql",  // .sql
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        L"SQL files (*.sql)|*.sql|All files (*.*)|*.*||",  // filter
        nullptr);

    if (fileOpenDialog.DoModal() == IDOK)
    {
        m_pathToFile = fileOpenDialog.GetPathName();
        m_SQLtextQuery = ReadFileContent();
        m_queryTab.GetDlgItem(IDC_EDIT_QTEXT)->SetWindowTextW(m_SQLtextQuery);
    }
}


CStringW CMainDlg::ReadFileContent() 
{
    CStringW fileContent;
    try
    {
        CFile file;
        if (file.Open(m_pathToFile, CFile::modeRead))
        {
            ULONGLONG fileSize = file.GetLength();
            wchar_t* buffer = new wchar_t[fileSize / 2];

            file.Read(buffer, static_cast<UINT>(fileSize));
            file.Close();

            fileContent = CStringW(buffer, static_cast<int>(fileSize / 2));

            delete[] buffer;
        }
    }
    catch (CFileException* e)
    {
        e->ReportError();
        e->Delete();
    }
    return fileContent;
}


void CMainDlg::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values)
{
    pComboBox->ResetContent();
    if (values.empty())
    {
        //pComboBox->AddString(L"No elements found");
        //m_queryTab.SendMessageToConsole(L"No elements found", RED);
    }
    for (const std::string& value : values)
    {
        CString item(value.c_str());
        pComboBox->AddString(item);
    }
}


void CMainDlg::OnBnClickedExport()
{
    m_queryTab.SendMessageToConsole(MSG_EXPORT_START, BLACK);
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    exportWindow.m_pList = pList;
    
    switch (exportWindow.DoModal())
    {
    case(IDOK):
        m_queryTab.SendMessageToConsole(MSG_EXPORT_OK, GREEN);
        break;
    case IDABORT:
        m_queryTab.SendMessageToConsole(MSG_EXPORT_ERR, RED);
        break;
    case IDCANCEL:
        m_queryTab.SendMessageToConsole(MSG_EXPORT_CANCEL,RED);
        break;
    default:
        break;
    }
}


void ExpandAllItems(CTreeCtrl* pTree, HTREEITEM hItem, UINT nCode) {
    if (!pTree || !hItem) return;

    pTree->Expand(hItem, nCode);
    HTREEITEM hChildItem = pTree->GetChildItem(hItem);
    while (hChildItem != NULL) {
        ExpandAllItems(pTree, hChildItem, nCode);
        hChildItem = pTree->GetNextSiblingItem(hChildItem);
    }
}


void CMainDlg::OnBnClickedBtnCollapse()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    ExpandAllItems(pTree, TVI_ROOT, TVE_COLLAPSE);
}


void CMainDlg::OnBnClickedBtnExpand()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    HTREEITEM hItem = pTree->GetChildItem(TVI_ROOT);

    while (hItem != NULL)
    {
        pTree->Expand(hItem, TVE_EXPAND);
        hItem = pTree->GetNextSiblingItem(hItem);
    }
}


void CMainDlg::OnBnClickedBtnUpdate()
{
    FillDatabaseDropdown();
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    pTree->DeleteAllItems();
    BuildDatabaseTree(*pTree);
    
    //OnCbnSelchangeCmbSelDb();
}





void CMainDlg::OnBnClickedButtonSave()
{
    // Open a file save dialog to select where to save the SQL file
    CFileDialog fileSaveDialog(FALSE,  // FALSE means this is a "Save" dialog
        L".sql",                 // Default extension
        nullptr,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        L"SQL files (*.sql)|*.sql|All files (*.*)|*.*||",
        this);  // parent window

    if (fileSaveDialog.DoModal() == IDOK)
    {
        m_pathToFile = fileSaveDialog.GetPathName();

        // Get the text from the IDC_EDIT_QTEXT control
        CString content;
        m_queryTab.GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(content);

        // Save the content to the file
        CFile file;
        if (file.Open(m_pathToFile, CFile::modeCreate | CFile::modeWrite))
        {
            file.Write((const void*)content.GetString(), content.GetLength() * sizeof(TCHAR));
            file.Close();

            AfxMessageBox(L"File saved successfully!", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            AfxMessageBox(L"Error saving the file.", MB_OK | MB_ICONERROR);
        }
    }
}


void CMainDlg::OnConnectionCheckconnection()
{
    if (db->CheckConnection())
    {
        AfxMessageBox(L"Connected");
    }
    else
    {
        AfxMessageBox(L"Disconnect");
    }
}


void CMainDlg::OnNMClickSyslinkServerinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    CServerInfoDlg serverinfoWindow;
    auto status = serverinfoWindow.DoModal();
    //*pResult = 0;
}


// <--------------------------- MENU HANDLERS -------------------------------->
void CMainDlg::OnFileSaveas()
{
    OnBnClickedButtonSave();
}


void CMainDlg::OnFileExit()
{
    db->Disconnect();
    this->EndDialog(IDCANCEL);
}


void CMainDlg::OnHelpMysqldocumentation()
{
    ShellExecute(0, 0, L"https://dev.mysql.com/doc/refman/8.1/en/", 0, 0, SW_SHOW);
}


void CMainDlg::OnFileExport()
{
    OnBnClickedExport();
}





void CMainDlg::OnMenuOpen()
{
    OnBnClickedBtnBrowse();
}


void CMainDlg::OnConnectionDisconnect()
{
    CListCtrl* pList = (CListCtrl*)m_resultTab.GetDlgItem(IDC_LIST_QUERY);
    if (m_queryTab.m_resultSet)
    {
        delete m_queryTab.m_resultSet;
        m_queryTab.m_resultSet = nullptr;
    }
    pList->DeleteAllItems();
    db->Disconnect();
    this->EndDialog(IDOK);
}





void CMainDlg::OnEditSelectall()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->SetSel(0, -1);
}


void CMainDlg::OnAboutVersionandcredits()
{
    CAboutDlg about;
    about.DoModal();
}


void CMainDlg::OnHelpServerinfo()
{
    OnNMClickSyslinkServerinfo(NULL, NULL);
}


void CMainDlg::OnBnClickedBtnForward()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);  // replace with your tree control's ID
    CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    HTREEITEM hSelectedItem = pTree->GetSelectedItem();

    if (hSelectedItem)
    {
        CString itemText = pTree->GetItemText(hSelectedItem);
        pRichEdit->ReplaceSel(itemText, TRUE);
    }
    else
    {
        AfxMessageBox(_T("No item selected!"));
    }
}


void CMainDlg::OnTvnSelchangedTreeStructure(NMHDR* pNMHDR, LRESULT* pResult)
{
    //single click exa
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    CComboBox* pComboDatabases = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* pComboTables = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    if (!hItem) return;

    // Handle expanding or collapsing the node on single click
   /* if (pTree->ItemHasChildren(hItem))
    {
        UINT state = pTree->GetItemState(hItem, TVIS_EXPANDED);
        if (state & TVIS_EXPANDED)
            pTree->Expand(hItem, TVE_COLLAPSE);
        else
            pTree->Expand(hItem, TVE_EXPAND);
    }*/

    // Check if the selected item is a database or a table
    HTREEITEM parentItem = pTree->GetParentItem(hItem);

    if (!parentItem) // Top level, possibly a database
    {
        CString databaseName = pTree->GetItemText(hItem);
        int index = pComboDatabases->FindStringExact(0, databaseName);
        if (index != CB_ERR)
        {
            pComboDatabases->SetCurSel(index);
            OnCbnSelchangeCmbSelDb();
        }
    }
    else if (pTree->GetItemText(parentItem) == _T("[TABLES]"))
    {
        HTREEITEM grandParentItem = pTree->GetParentItem(parentItem); // This item should be a database
        if (grandParentItem)
        {
            CString databaseName = pTree->GetItemText(grandParentItem);
            int dbIndex = pComboDatabases->FindStringExact(0, databaseName);
            if (dbIndex != CB_ERR)
            {
                pComboDatabases->SetCurSel(dbIndex);
                OnCbnSelchangeCmbSelDb();

            }
        }

        CString tableName = pTree->GetItemText(hItem);
        int index =  m_queryTab.m_comboTables.FindStringExact(0, tableName);
        
        if (m_mainTabCtrl.GetCurSel() == 2) //if result tab is active 
        {   //see table content on tree element click
            if (m_queryTab.m_resultSet)
            {
                delete m_queryTab.m_resultSet;
                m_queryTab.m_resultSet = nullptr;
            }
            CString query(L"SELECT * FROM ");
            query += tableName;
            auto resultSet = db->ExecuteQuery(CStringToSQLString(query));
            m_resultTab.BuildResultList(resultSet, 0);
            m_queryTab.m_resultSet = resultSet;
            m_resultTab.OnEnChangeEditCurrentpage();
        }
        if (index != CB_ERR)
        {
            m_queryTab.m_comboTables.SetCurSel(index);
            m_queryTab.PopulateColumnsList();
        }
    }

    *pResult = 0;
}

// single click expand selected element
void CMainDlg::OnNMClickTreeStructure(NMHDR* pNMHDR, LRESULT* pResult)
{
    UINT flags = 0;
    CPoint point;
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);

    // Get the current mouse position and convert it to client coordinates
    GetCursorPos(&point);
    pTree->ScreenToClient(&point);

    // Get the tree item at the point
    HTREEITEM hItem = pTree->HitTest(point, &flags);

    if (hItem)
    {
            // Check if we clicked on an item (and not on the state icon)
        if (pTree->GetItemText(hItem) == _T("New") && hItem == pTree->GetNextItem(NULL, TVGN_ROOT))
        {
            CNewDBDlg dlg;
            dlg.db = this->db;
            auto status = dlg.DoModal();
            if (status == IDOK)
            {
                OnBnClickedBtnUpdate();
            }
            *pResult = 0;
            return;
        }
            
            HTREEITEM parentItem = pTree->GetParentItem(hItem);
            if (parentItem && pTree->GetItemText(parentItem) == _T("[TABLES]"))
            {
                *pResult = 0;
                return;
            }

            if (pTree->ItemHasChildren(hItem))
            {
                // Expand or collapse the item
                UINT state = pTree->GetItemState(hItem, TVIS_EXPANDED);
                if (state & TVIS_EXPANDED)
                    pTree->Expand(hItem, TVE_COLLAPSE);
                else
                    pTree->Expand(hItem, TVE_EXPAND);
            }
        
    }

    *pResult = 0;
}

//main tab control switch logic
void CMainDlg::OnTcnSelchangeMaintab(NMHDR* pNMHDR, LRESULT* pResult)
{
    //*pResult = 0;
    auto p = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);

    int iSel = p->GetCurSel();

    // Hide all tabs
    m_homeTab.ShowWindow(SW_HIDE);
    m_queryTab.ShowWindow(SW_HIDE);
    m_resultTab.ShowWindow(SW_HIDE);
    m_exportTab.ShowWindow(SW_HIDE);
    m_proceduresTab.ShowWindow(SW_HIDE);
    m_tableTab.ShowWindow(SW_HIDE);
    m_databasesTab.ShowWindow(SW_HIDE);
    m_varsTab.ShowWindow(SW_HIDE);
    m_charsetsTab.ShowWindow(SW_HIDE);
    // Show the appropriate tabbed dialog based on the selected tab
    switch (iSel)
    {

    case 0:
        m_homeTab.ShowWindow(SW_SHOW);
        break;
    case 1:
        m_queryTab.ShowWindow(SW_SHOW);
        break;
    case 2:
        m_resultTab.ShowWindow(SW_SHOW);
        break;
    case 3:
        m_exportTab.ShowWindow(SW_SHOW);
        break;
    case 4:
        m_tableTab.ShowWindow(SW_SHOW);
        break;
    case 5:
        m_databasesTab.ShowWindow(SW_SHOW);
        break;
    case 6:
        m_varsTab.ShowWindow(SW_SHOW);
        break;
    case 7:
        m_charsetsTab.ShowWindow(SW_SHOW);
        break;
    }
    
  

}


void CMainDlg::OnEditUndo()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->Undo();
}


void CMainDlg::OnEditRedo()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->Redo();
}


void CMainDlg::OnEditCut()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->Cut();
}


void CMainDlg::OnEditCopy()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->Copy();
}


void CMainDlg::OnEditPaste()
{
    ((CRichEditCtrl*)m_queryTab.GetDlgItem(IDC_EDIT_QTEXT))->Paste();
}


void CMainDlg::OnCbnSelchangeCmbSelDb()
{
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CString databaseName;
    int selectedIndex = dropdown->GetCurSel();

    if (selectedIndex != CB_ERR)
    {
        dropdown->GetLBText(selectedIndex, databaseName);
    }
    m_titleDatabaseName = databaseName;
    this->SetWindowTextW(m_titleDatabaseName);

    sql::SQLString sqlDatabaseName(CW2A(databaseName.GetString()));
    if (db->ChangeCurrentDatabase(sqlDatabaseName))
    {
        m_queryTab.FillTableDropdown();
        m_queryTab.PopulateColumnsList();
    }
    else
    {
        m_queryTab.SendMessageToConsole(MSG_DBCHANGE_ERR, RED);
    }
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // Only proceed if the child dialogs have been created
    if (m_queryTab.GetSafeHwnd() && m_resultTab.GetSafeHwnd()) {
        CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
        CRect mainRect;
        GetClientRect(&mainRect);

        // Adjust the size and position of the child dialogs based on the new size of the main dialog
        CRect rcTab;
        pTabCtrl->GetClientRect(&rcTab);

        CRect rcItem1, rcItem2;
        pTabCtrl->GetItemRect(0, &rcItem1);
        pTabCtrl->GetItemRect(1, &rcItem2);

        // Adjust the position and size of the child dialogs
        m_queryTab.MoveWindow(rcTab.left, rcTab.top + rcItem1.Height(), rcTab.Width(), rcTab.Height() - rcItem1.Height());
        m_resultTab.MoveWindow(rcTab.left, rcTab.top + rcItem2.Height(), rcTab.Width(), rcTab.Height() - rcItem2.Height());
    }
}


void CMainDlg::SetProgressBarPosition(int position)
{
    ((CProgressCtrl*)GetDlgItem(IDC_EXECPROGRESS))->SetPos(position);
}

void CMainDlg::SwitchTabByName(const CString& tabName) {

    CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
    int tabCount = pTabCtrl->GetItemCount();

    for (int i = 0; i < tabCount; i++) {
        CString currentTabName;
        TCITEM tabItem;
        tabItem.mask = TCIF_TEXT;
        tabItem.pszText = currentTabName.GetBuffer(MAX_PATH);
        tabItem.cchTextMax = MAX_PATH;

        if (pTabCtrl->GetItem(i, &tabItem)) {
            currentTabName.ReleaseBuffer();
            if (currentTabName == tabName) {
                pTabCtrl->SetCurSel(i); // Activate the tab with the matching name
                break;
            }
        }
    }
}


