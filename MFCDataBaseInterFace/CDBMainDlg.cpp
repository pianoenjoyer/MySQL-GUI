#include <fstream>
#include "pch.h"
#include "CDBInterfaceApp.h"
#include "afxdialogex.h"
#include "CDBMainDlg.h"
#include "framework.h"
#include "CDBAuthDlg.h"
#include "CServerInfoDlg.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "CNewDBDlg.h"

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 128, 0)
#define BLACK RGB(0, 0, 0)

//CONSOLE MESSEGES
//Query msgs
const CString MSG_QUERY_OK("Query completed");
//Export msgs
const CString MSG_EXPORT_START("Export started");
const CString MSG_EXPORT_OK("Export completed");
const CString MSG_EXPORT_ERR("Export error");
const CString MSG_EXPORT_CANCEL("Export canceled");
//DB change msgs
// 
// CDBMainDlg dialog
const CString MSG_DBCHANGE_OK("Databased selected");
const CString MSG_DBCHANGE_ERR("Databased select error");


IMPLEMENT_DYNAMIC(CDBMainDlg, CDialogEx)

inline sql::SQLString CStringToSQLString(const CString& cstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), cstr.GetLength(), NULL, 0, NULL, NULL);

    std::string utf8Str(size_needed, 0);

    // Convert the UTF-16 string (CString) to UTF-8
    WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), cstr.GetLength(), &utf8Str[0], size_needed, NULL, NULL);

    return sql::SQLString(utf8Str);
}

void ExpandAllItems(CTreeCtrl* pTree, HTREEITEM hItem, UINT nCode);
CDBMainDlg::CDBMainDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MAIN, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDBMainDlg::~CDBMainDlg() 
{

}


//void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl) { 
//    if (treeCtrl.GetRootItem()) 
//    {
//        treeCtrl.DeleteAllItems();
//    }
//
//    HTREEITEM tablesRoot = treeCtrl.InsertItem(_T("[Tables]"));
//    HTREEITEM proceduresRoot = treeCtrl.InsertItem(_T("[Procedures]"));
//    HTREEITEM functionsRoot = treeCtrl.InsertItem(_T("[Functions]"));
//    sql::ResultSet* resultSet;
//
//    CStringW queryBase = L"SHOW %s STATUS WHERE Db = '" + m_titleDatabaseName + L"'";
//    CStringA query;
//
//    try {
//        // Fetch all tables
//        resultSet = db->ExecuteQuery("SHOW TABLES");
//        if (resultSet) {
//            while (resultSet->next()) {
//                std::string tableName = resultSet->getString(1);
//                HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()), tablesRoot);
//
//                // For each table, fetch its columns
//                sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + tableName);
//                if (columnResultSet) {
//                    while (columnResultSet->next()) {
//                        std::string columnName = columnResultSet->getString(1);
//                        treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
//                    }
//                    delete columnResultSet;
//                }
//            }
//            delete resultSet;
//        }
//
//        // Fetch all procedures
//        query.Format(CStringA(queryBase), "PROCEDURE");
//        std::string queryProcedure(query.GetBuffer());
//        resultSet = db->ExecuteQuery(queryProcedure);
//        if (resultSet) {
//            while (resultSet->next()) {
//                std::string procedureName = resultSet->getString("Name");
//                treeCtrl.InsertItem(CA2T(procedureName.c_str()), proceduresRoot);
//            }
//            delete resultSet;
//        }
//
//        // Fetch all functions
//        query.Format(CStringA(queryBase), "FUNCTION");
//        std::string queryFunction(query.GetBuffer());
//        resultSet = db->ExecuteQuery(queryFunction);
//        if (resultSet) {
//            while (resultSet->next()) {
//                std::string functionName = resultSet->getString("Name");
//                treeCtrl.InsertItem(CA2T(functionName.c_str()), functionsRoot);
//            }
//            delete resultSet;
//        }
//
//    }
//    catch (sql::SQLException& e) {
//        AfxMessageBox(CA2T(e.what()));
//    }
//}

//void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl)
//{
//    if (treeCtrl.GetRootItem())
//    {
//        treeCtrl.DeleteAllItems();
//    }
//
//    // Fetch databases
//    sql::ResultSet* databaseResultSet = db->ExecuteQuery("SHOW DATABASES");
//    if (databaseResultSet)
//    {
//        while (databaseResultSet->next())
//        {
//            std::string databaseName = databaseResultSet->getString(1);
//            HTREEITEM databaseItem = treeCtrl.InsertItem(CA2T(databaseName.c_str()));
//
//            // For each database, fetch its tables
//            sql::ResultSet* tableResultSet = db->ExecuteQuery("SHOW TABLES IN " + databaseName);
//            if (tableResultSet)
//            {
//                while (tableResultSet->next())
//                {
//                    std::string tableName = tableResultSet->getString(1);
//                    HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()), databaseItem);
//
//                    // For each table, fetch its columns
//                    sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + databaseName + "." + tableName);
//                    if (columnResultSet)
//                    {
//                        while (columnResultSet->next())
//                        {
//                            std::string columnName = columnResultSet->getString(1);
//                            treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
//                        }
//                        delete columnResultSet;
//                    }
//                }
//                delete tableResultSet;
//            }
//
//            // Fetch procedures for the database (similar to the previous example)
//            CString query;
//            query.Format(CString("SHOW PROCEDURE STATUS WHERE Db = '%s'"), databaseName.c_str());
//            sql::ResultSet* procResultSet = db->ExecuteQuery(CStringToSQLString(query));
//            if (procResultSet)
//            {
//                while (procResultSet->next())
//                {
//                    std::string procedureName = procResultSet->getString("Name");
//                    treeCtrl.InsertItem(CA2T(procedureName.c_str()), databaseItem);  // attach procedures directly to the database
//                }
//                delete procResultSet;
//            }
//
//            // Fetch functions for the database (similar to the procedures)
//            query.Format(CString("SHOW FUNCTION STATUS WHERE Db = '%s'"), databaseName.c_str());
//            sql::ResultSet* funcResultSet = db->ExecuteQuery(CStringToSQLString(query));
//            if (funcResultSet)
//            {
//                while (funcResultSet->next())
//                {
//                    std::string functionName = funcResultSet->getString("Name");
//                    treeCtrl.InsertItem(CA2T(functionName.c_str()), databaseItem);  // attach functions directly to the database
//                }
//                delete funcResultSet;
//            }
//        }
//        delete databaseResultSet;
//    }
//}

//void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl)
//{
//    if (treeCtrl.GetRootItem())
//    {
//        treeCtrl.DeleteAllItems();
//    }
//
//    // Fetch databases
//    sql::ResultSet* databaseResultSet = db->ExecuteQuery("SHOW DATABASES");
//    if (databaseResultSet)
//    {
//        while (databaseResultSet->next())
//        {
//            std::string databaseName = databaseResultSet->getString(1);
//            HTREEITEM databaseItem = treeCtrl.InsertItem(CA2T(databaseName.c_str()));
//
//            // Внутри каждой базы данных создаем узлы для таблиц, процедур и функций
//            HTREEITEM tablesRoot = treeCtrl.InsertItem(_T("[TABLES]"), databaseItem);
//            HTREEITEM proceduresRoot = treeCtrl.InsertItem(_T("[PROCEDURES]"), databaseItem);
//            HTREEITEM functionsRoot = treeCtrl.InsertItem(_T("[FUNCTIONS]"), databaseItem);
//            HTREEITEM viewsRoot = treeCtrl.InsertItem(_T("[VIEWS]"), databaseItem);
//
//
//
//
//
//
//
//            sql::ResultSet* viewsResultSet = db->ExecuteQuery("SHOW FULL TABLES IN " + databaseName + " WHERE TABLE_TYPE LIKE 'VIEW'");
//            if (viewsResultSet)
//            {
//                while (viewsResultSet->next())
//                {
//                    std::string viewName = viewsResultSet->getString(1);
//                    treeCtrl.InsertItem(CA2T(viewName.c_str()), viewsRoot);
//                }
//                delete viewsResultSet;
//            }
//            // Fetch tables for the current database
//            sql::ResultSet* tableResultSet = db->ExecuteQuery("SHOW TABLES IN " + databaseName);
//            if (tableResultSet)
//            {
//                while (tableResultSet->next())
//                {
//                    std::string tableName = tableResultSet->getString(1);
//                    HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()), tablesRoot);
//
//                    // Fetch columns for each table
//                    sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + databaseName + "." + tableName);
//                    if (columnResultSet)
//                    {
//                        while (columnResultSet->next())
//                        {
//                            std::string columnName = columnResultSet->getString(1);
//                            treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
//                        }
//                        delete columnResultSet;
//                    }
//                }
//                delete tableResultSet;
//            }
//       
//            
//            // Fetch procedures for the current database
//            CString query;
//            CString database(CA2T(databaseName.c_str()));
//            query.Format(CStringW(L"SHOW PROCEDURE STATUS WHERE Db = '%s'"), database);
//            sql::ResultSet* procResultSet = db->ExecuteQuery(CStringToSQLString(query));
//            if (procResultSet)
//            {
//                while (procResultSet->next())
//                {
//                    std::string procedureName = procResultSet->getString("Name");
//                    treeCtrl.InsertItem(CA2T(procedureName.c_str()), proceduresRoot);
//                }
//                delete procResultSet;
//            }
//
//            // Fetch functions for the current database
//            query.Format(CStringW("SHOW FUNCTION STATUS WHERE Db = '%s'"), database);
//            sql::ResultSet* funcResultSet = db->ExecuteQuery(CStringToSQLString(query));
//            if (funcResultSet)
//            {
//                while (funcResultSet->next())
//                {
//                    std::string functionName = funcResultSet->getString("Name");
//                    treeCtrl.InsertItem(CA2T(functionName.c_str()), functionsRoot);
//                }
//                delete funcResultSet;
//            }
//        }
//        delete databaseResultSet;
//    }
//}

void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl)
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



void CDBMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
 
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


BOOL CDBMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    //set icon
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    //set title || Now show current db name
    this->SetWindowTextW(L"MySQL GUI");
    //CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    //set visible at task bar
    ModifyStyleEx(0, WS_EX_APPWINDOW);

    //set pic for sever info
    CImage image;
    if (SUCCEEDED(image.Load(L".\\Pictures\\serverIcon(22x22).png"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
    {
        CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_INFO);
        HBITMAP hBmp = image.Detach();
        pPicCtrl->SetBitmap(hBmp);
    }
 
    //set list ctrl to table style
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    pList->SetView(LV_VIEW_DETAILS);

    //current page = 1

    //set default db
    FillDatabaseDropdown();
    FillLimitDropdown();
    //set font size of sql text rich edit
    (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);

    //SetRichControlTextSize((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT), 250 );
    //SetRichControlTextSize((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_MSGS), 250);
    //((CComboBox*)GetDlgItem(IDC_CMB_SEL_DB))->SetCurSel(0);
    OnCbnSelchangeCmbSelDb();
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
    GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(L"0");
    OnBnClickedBtnUpdate();


    return TRUE;
}


// fill drop down with table names of db

bool CDBMainDlg::FillDatabaseDropdown() 
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_CMB_SEL_DB));
    std::vector<sql::SQLString> databases;
    databases = db->GetDatabases();
    PopulateDropdown(pComboBox, databases);
    pComboBox->SetCurSel(0);
    return true;
}

bool CDBMainDlg::FillTableDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_SEL_TABLE));
    std::vector<sql::SQLString> tableNames;
    tableNames = db->GetTables();
    PopulateDropdown(pComboBox, tableNames);
    pComboBox->SetCurSel(0);
    return true;
}

bool CDBMainDlg::FillTreeControl() 
{
    //Fill tree with db tables structure
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    FillTreeControlWithDBTables(*pTree);
    //ExpandAllItems(pTree, TVI_ROOT, TVE_EXPAND);
    return true;
}

bool CDBMainDlg::FillLimitDropdown() 
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_NMB_OF_ROWS));
    pComboBox->AddString(L"30");
    pComboBox->AddString(L"50");
    pComboBox->AddString(L"100");
    pComboBox->AddString(L"250");
    pComboBox->AddString(L"500");
    pComboBox->AddString(L"1000");
    pComboBox->AddString(L"All");
    pComboBox->SetCurSel(1);
    return true;
}




BEGIN_MESSAGE_MAP(CDBMainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BROWSE, &CDBMainDlg::OnBnClickedBtnBrowse)
    ON_BN_CLICKED(IDC_BTN_GO, &CDBMainDlg::OnBnClickedBtnGo)
    ON_EN_CHANGE(IDC_EDIT_QTEXT, &CDBMainDlg::OnEnChangeEditQtext)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CDBMainDlg::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_PRINTTABLE, &CDBMainDlg::OnBnClickedBtnPrinttable)
    ON_BN_CLICKED(IDC_BTN_CLROUTPUT, &CDBMainDlg::OnBnClickedBtnClroutput)
    ON_BN_CLICKED(IDC_BTN_UNDO, &CDBMainDlg::OnBnClickedBtnUndo)
    ON_BN_CLICKED(IDC_BTN_DELETETABLE, &CDBMainDlg::OnBnClickedBtnDeletetable)
    ON_BN_CLICKED(IDC_EXPORT, &CDBMainDlg::OnBnClickedExport)
    ON_BN_CLICKED(IDC_BTN_COLLAPSE, &CDBMainDlg::OnBnClickedBtnCollapse)
    ON_BN_CLICKED(IDC_BTN_EXPAND, &CDBMainDlg::OnBnClickedBtnExpand)
    ON_BN_CLICKED(IDC_BTN_UPDATE, &CDBMainDlg::OnBnClickedBtnUpdate)
    ON_EN_CHANGE(IDC_LIST_SEARCH, &CDBMainDlg::OnEnChangeListSearch)
    ON_BN_CLICKED(IDC_BTN_CLEARMSG, &CDBMainDlg::OnBnClickedBtnClearmsg)
    ON_COMMAND(ID_MENU_OPEN, &CDBMainDlg::OnMenuOpen)
    ON_COMMAND(ID_CONNECTION_DISCONNECT, &CDBMainDlg::OnConnectionDisconnect)
    ON_CBN_SELCHANGE(IDC_CMB_SEL_DB, &CDBMainDlg::OnCbnSelchangeCmbSelDb)
    //ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDBMainDlg::OnBnClickedButtonSave)
    ON_COMMAND(ID_CONNECTION_CHECKCONNECTION, &CDBMainDlg::OnConnectionCheckconnection)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK_SERVERINFO, &CDBMainDlg::OnNMClickSyslinkServerinfo)
    ON_COMMAND(ID_FILE_SAVEAS, &CDBMainDlg::OnFileSaveas)
    ON_COMMAND(ID_FILE_EXIT, &CDBMainDlg::OnFileExit)
    ON_BN_CLICKED(IDC_BTN_SCHEMA, &CDBMainDlg::OnBnClickedBtnSchema)
    ON_COMMAND(ID_FILE_EXPORT, &CDBMainDlg::OnFileExport)

    ON_COMMAND(ID_EDIT_SELECTALL, &CDBMainDlg::OnEditSelectall)
    ON_EN_CHANGE(IDC_EDIT_CURRENTPAGE, &CDBMainDlg::OnEnChangeEditCurrentpage)
    ON_BN_CLICKED(IDC_BTN_FIRSTPAGE, &CDBMainDlg::OnBnClickedBtnFirstpage)
    ON_BN_CLICKED(IDC_BTN_LASTPAGE, &CDBMainDlg::OnBnClickedBtnLastpage)
    ON_BN_CLICKED(IDC_BTN_PREVPAGE, &CDBMainDlg::OnBnClickedBtnPrevpage)
    ON_BN_CLICKED(IDC_BTN_NEXTPAGE, &CDBMainDlg::OnBnClickedBtnNextpage)
    ON_CBN_SELCHANGE(IDC_SEL_TABLE, &CDBMainDlg::OnCbnSelchangeSelTable)
    ON_BN_CLICKED(IDC_CHECK_SHOWALL, &CDBMainDlg::OnBnClickedCheckShowall)
    ON_COMMAND(ID_HELP_MYSQLDOCUMENTATION, &CDBMainDlg::OnHelpMysqldocumentation)
    ON_CBN_SELCHANGE(IDC_COMBO_NMB_OF_ROWS, &CDBMainDlg::OnCbnSelchangeComboNmbOfRows)
    ON_COMMAND(ID_ABOUT_VERSIONANDCREDITS, &CDBMainDlg::OnAboutVersionandcredits)
    ON_BN_CLICKED(IDC_BTN_SELECTALL, &CDBMainDlg::OnBnClickedBtnSelectall)
    ON_BN_CLICKED(IDC_BTN_REFACTOR, &CDBMainDlg::OnBnClickedBtnRefactor)
    ON_COMMAND(ID_HELP_SERVERINFO, &CDBMainDlg::OnHelpServerinfo)
    ON_BN_CLICKED(IDC_BTNN_SELECT, &CDBMainDlg::OnBnClickedBtnnSelect)
    ON_BN_CLICKED(IDC_BTN_INSERT, &CDBMainDlg::OnBnClickedBtnInsert)
    ON_BN_CLICKED(IDC_BTN_UPDATERECORD, &CDBMainDlg::OnBnClickedBtnUpdaterecord)
    ON_BN_CLICKED(IDC_BTN_DELETERECORD, &CDBMainDlg::OnBnClickedBtnDeleterecord)
    ON_BN_CLICKED(IDC_BTN_FORWARD, &CDBMainDlg::OnBnClickedBtnForward)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_STRUCTURE, &CDBMainDlg::OnTvnSelchangedTreeStructure)
    ON_NOTIFY(NM_CLICK, IDC_TREE_STRUCTURE, &CDBMainDlg::OnNMClickTreeStructure)
END_MESSAGE_MAP()

//open .sql file
void CDBMainDlg::OnBnClickedBtnBrowse()
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
        GetDlgItem(IDC_EDIT_QTEXT)->SetWindowTextW(m_SQLtextQuery);
    }
}


CStringW CDBMainDlg::ReadFileContent() // Notice the CStringW here
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


inline CString SQLStringToCString(const sql::SQLString& sqlStr)
{
    // Directly use sqlStr's c_str() method
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, sqlStr.c_str(), (int)sqlStr.length(), NULL, 0);

    CString utf16CString;
    wchar_t* buf = utf16CString.GetBufferSetLength(size_needed);

    // Convert the UTF-8 string to UTF-16
    MultiByteToWideChar(CP_UTF8, 0, sqlStr.c_str(), (int)sqlStr.length(), buf, size_needed);

    utf16CString.ReleaseBuffer();

    return utf16CString;
}


void CDBMainDlg::OnBnClickedBtnGo()
{
    if (m_resultSet != nullptr)
    {
        delete m_resultSet;
    }
    

    CStringW sqlText;
    GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(sqlText);
    
    //sql::SQLString query(CW2A(sqlText.GetString())); //old method 

    sql::SQLString query = CStringToSQLString(sqlText);

    auto start = std::chrono::high_resolution_clock::now();
    sql::ResultSet* resultSet = db->ExecuteQuery(query, errorString);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    double timeTaken = duration.count();
    CString timeTakenStr;
    

    if (resultSet)
    {
        int rowsCount = resultSet->rowsCount();
        timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

        SendMessageToConsole(timeTakenStr, GREEN);
        start = std::chrono::high_resolution_clock::now();
        FillListControl(resultSet);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(end - start);
        timeTaken = duration.count();
        //timeTakenStr.Format(_T("Built list took: %.2f seconds"), timeTaken);
        //SendMessageToConsole(timeTakenStr, BLACK);

    }
    else 
    {
        SendMessageToConsole(errorString, RED);
    }
    m_resultSet = resultSet;
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
    //delete resultSet;
}

CString BinaryDataToHexString(const CString& binaryData) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < binaryData.GetLength(); i++) {
        char c = binaryData[i];
        oss << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
    }
    return CString(oss.str().c_str());
}


//int CDBMainDlg::FillListControl(sql::ResultSet* resultSet)
//{
//    //get limit from drop down
//    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
//    int selectedIndex = dropdown->GetCurSel();
//    CString dropdownText;
//    int limit;
//
//    if (selectedIndex != CB_ERR)
//    {
//        dropdown->GetLBText(selectedIndex, dropdownText);
//    }
//    if (dropdownText == L"All")
//    {
//        limit = 0;
//    }
//    else
//    {
//        std::wstring wstr(dropdownText);
//        limit = std::stoi(wstr);
//    }
//
//    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
//    if (!resultSet || !pList)
//        return 1;
//
//    // Clear existing items from the list control
//    pList->DeleteAllItems();
//    while (pList->DeleteColumn(0)); // Remove all columns
//
//    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
//    int columnCount = metaData->getColumnCount();
//    CString columnName(L""), firstColData(L""), colData(L"");
//
//    // Populate the columns
//    for (int i = 1; i <= columnCount; i++)
//    {
//        columnName = metaData->getColumnName(i).c_str();
//        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100); 
//    }
//
//    int populatedRows = 0;
//
//       
//    // Populate the rows with limit
//    while (resultSet->next() && (limit == 0 || populatedRows < limit))
//    {
//        firstColData = SQLStringToCString(resultSet->getString(1));
//        int nItemCount = pList->GetItemCount();  // Получаем текущее количество элементов
//        int nIndex = pList->InsertItem(LVIF_TEXT, nItemCount, firstColData, 0, 0, 0, 0);  // Вставляем новый элемент после последнего
//
//        for (int i = 1; i <= columnCount; i++)
//        {
//            colData = SQLStringToCString(resultSet->getString(i));
//            pList->SetItemText(nIndex, i - 1, colData);
//        }
//        populatedRows++;
//    }
//    SaveOriginalListState();
//    return 0;
//}

// LAST WORKING METHOD
//int CDBMainDlg::FillListControl(sql::ResultSet* resultSet) {
//    // get limit from dropdown
//    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
//    int selectedIndex = dropdown->GetCurSel();
//    CString dropdownText;
//    int limit;
//
//    if (selectedIndex != CB_ERR) {
//        dropdown->GetLBText(selectedIndex, dropdownText);
//    }
//    if (dropdownText == L"All") {
//        limit = 0;
//    }
//    else {
//        std::wstring wstr(dropdownText);
//        limit = std::stoi(wstr);
//    }
//
//    //get list
//    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
//    if (!resultSet || !pList) {
//        return 1;
//    }
//
//
//
//    // Очистка текущих элементов из CListCtrl
//    pList->DeleteAllItems();
//    while (pList->DeleteColumn(0)); // Удаление всех столбцов
//
//    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
//    int columnCount = metaData->getColumnCount();
//    CStringW columnName(L""), firstColData(L""), colData(L"");
//
//    // Заполнение столбцов
//    for (int i = 1; i <= columnCount; i++) {
//        columnName = metaData->getColumnName(i).c_str();
//        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
//    }
//
//    int populatedRows = 0;
//
//    // Заполнение строк с учетом лимита
//    while (resultSet->next() && (limit == 0 || populatedRows < limit)) {
//        firstColData = SQLStringToCString(resultSet->getString(1));
//
//        int nItemCount = pList->GetItemCount();
//        int nIndex = pList->InsertItem(LVIF_TEXT, nItemCount, firstColData, 0, 0, 0, 0);
//
//        for (int i = 1; i <= columnCount; i++) {
//            // Проверка типа столбца: является ли он бинарным
//            if (metaData->getColumnType(i) == sql::DataType::BINARY ||
//                metaData->getColumnType(i) == sql::DataType::VARBINARY ||
//                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) {
//                colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
//            }
//            else {
//                colData = SQLStringToCString(resultSet->getString(i));
//            }
//            pList->SetItemText(nIndex, i - 1, colData);
//        }
//        populatedRows++;
//    }
//
//    SaveOriginalListState();
//    // Calculate the number of pages
//    CString strMaxPages;
//    CString str;
//    std::map<int, ListItem> pages;
//    auto rowsCount = resultSet->rowsCount();
//
//    if (limit == 0)
//    {
//        strMaxPages.Format(_T("%d"), 1);
//    }
//    else 
//    {
//        auto maxPages = rowsCount / limit;
//        if (rowsCount % limit != 0) {
//            maxPages++; 
//        }
//        strMaxPages.Format(_T("%d"), maxPages);
//        GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(strMaxPages);
//    }
//
//    for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); ++i)
//    {
//        pList->SetColumnWidth(i, LVSCW_AUTOSIZE);
//    }
//
//    return 0;
//}

int CDBMainDlg::FillListControl(sql::ResultSet* resultSet) {
    if (!resultSet) return 1;

    // Get controls
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);

    // Get the limit
    int selectedIndex = dropdown->GetCurSel();
    CString dropdownText;
    int limit;

    dropdown->GetLBText(selectedIndex, dropdownText);
    limit = (dropdownText == L"All") ? 0 : _ttoi(dropdownText);

    // Clear current CListCtrl
    pList->DeleteAllItems();
    while (pList->DeleteColumn(0));

    // Fetch columns and fill them in the CListCtrl
    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
    int columnCount = metaData->getColumnCount();
    for (int i = 1; i <= columnCount; i++) {
        CStringW columnName(metaData->getColumnName(i).c_str());
        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT);
        pList->SetColumnWidth(i - 1, LVSCW_AUTOSIZE);
    }

    int populatedRows = 0;
    while (resultSet->next() && (limit == 0 || populatedRows < limit)) {
        int nItemCount = pList->GetItemCount();
        int nIndex = pList->InsertItem(LVIF_TEXT, nItemCount, SQLStringToCString(resultSet->getString(1)), 0, 0, 0, 0);

        for (int i = 1; i <= columnCount; i++) {
            CString colData;
            if (metaData->getColumnType(i) == sql::DataType::BINARY ||
                metaData->getColumnType(i) == sql::DataType::VARBINARY ||
                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) {
                colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
            }
            else {
                colData = SQLStringToCString(resultSet->getString(i));
            }
            pList->SetItemText(nIndex, i - 1, colData);
        }

        populatedRows++;
    }

    SaveOriginalListState();

    // Calculate the number of pages
    CString strMaxPages;
    auto rowsCount = resultSet->rowsCount();

    if (limit == 0)
    {
        strMaxPages.Format(_T("%d"), 1);
    }
    else
    {
        auto maxPages = rowsCount / limit + (rowsCount % limit != 0);
        strMaxPages.Format(_T("%d"), maxPages);
        GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(strMaxPages);
    }

    return 0;
}
//latest list cntrl
//
//int CDBMainDlg::FillListControl(sql::ResultSet* resultSet, int offset) {
//    // get limit from dropdown
//    if (resultSet == nullptr)
//    {
//        return 0;
//    }
//    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
//    int selectedIndex = dropdown->GetCurSel();
//    CString dropdownText;
//    int limit;
//
//    if (selectedIndex != CB_ERR) 
//    {
//        dropdown->GetLBText(selectedIndex, dropdownText);
//    }
//    if (dropdownText == L"All") 
//    {
//        limit = 0;
//    }
//    else {
//        std::wstring wstr(dropdownText);
//        limit = std::stoi(wstr);
//    }
//
//    //get list
//    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
//    if (!resultSet || !pList) 
//    {
//        return 1;
//    }
//    
//    // Очистка текущих элементов из CListCtrl
//    pList->DeleteAllItems();
//    while (pList->DeleteColumn(0)); // Удаление всех столбцов
//
//    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
//    int columnCount = metaData->getColumnCount();
//    CStringW columnName(L""), firstColData(L""), colData(L"");
//    
//    // Заполнение столбцов
//    for (int i = 1; i <= columnCount; i++) {
//        columnName = metaData->getColumnName(i).c_str();
//        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
//    }
//
//    int populatedRows = 0;
//    resultSet->absolute(offset + 1);
//    // Заполнение строк с учетом лимита
//    while (resultSet->next() && (limit == 0 || populatedRows < limit)) 
//    {
//        firstColData = SQLStringToCString(resultSet->getString(1));
//
//        int nItemCount = pList->GetItemCount();
//        int nIndex = pList->InsertItem(LVIF_TEXT, nItemCount, firstColData, 0, 0, 0, 0);
//
//        for (int i = 1; i <= columnCount; i++) 
//        {
//            // Проверка типа столбца: является ли он бинарным
//            if (metaData->getColumnType(i) == sql::DataType::BINARY ||
//                metaData->getColumnType(i) == sql::DataType::VARBINARY ||
//                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) {
//                colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
//            }
//            else {
//                colData = SQLStringToCString(resultSet->getString(i));
//            }
//            pList->SetItemText(nIndex, i - 1, colData);
//        }
//        populatedRows++;
//    }
//
//    SaveOriginalListState();
//    // Calculate the number of pages
//    CString strMaxPages;
//    CString str;
//    std::map<int, ListItem> pages;
//    auto rowsCount = resultSet->rowsCount();
//
//    if (limit == 0)
//    {
//        strMaxPages.Format(_T("%d"), 1);
//    }
//    else
//    {
//        auto maxPages = rowsCount / limit;
//        if (rowsCount % limit != 0) {
//            maxPages++;
//        }
//        strMaxPages.Format(_T("%d"), maxPages);
//        GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(strMaxPages);
//    }
//
//    for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); ++i)
//    {
//        pList->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
//    }
//
//    return 0;
//}


//save changes to vector to redo and undo in future

int CDBMainDlg::FillListControl(sql::ResultSet* resultSet, int offset) {
    // Ensure resultSet is valid
    if (resultSet == nullptr) return 0;

    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
    int selectedIndex = dropdown->GetCurSel();
    CString dropdownText;
    int limit;

    if (selectedIndex != CB_ERR) {
        dropdown->GetLBText(selectedIndex, dropdownText);
    }

    limit = (dropdownText == L"All") ? 0 : _wtoi(dropdownText);

    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    if (!pList) return 1;

    pList->DeleteAllItems();
    while (pList->DeleteColumn(0));

    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
    int columnCount = metaData->getColumnCount();
    CStringW columnName;

    // Insert columns
    for (int i = 1; i <= columnCount; i++) {
        columnName = metaData->getColumnName(i).c_str();
        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
    }

    // Populate rows
    int populatedRows = 0;
    resultSet->absolute(offset + 1);

    pList->SetRedraw(FALSE);  // Prevent redraw during data insertion

    while (resultSet->next() && (limit == 0 || populatedRows < limit)) {
        CString rowData = SQLStringToCString(resultSet->getString(1));

        int nIndex = pList->InsertItem(populatedRows, rowData);

        for (int i = 2; i <= columnCount; i++) {
            CString colData;
            if (metaData->getColumnType(i) == sql::DataType::BINARY ||
                metaData->getColumnType(i) == sql::DataType::VARBINARY ||
                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) {
                colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
            }
            else {
                colData = SQLStringToCString(resultSet->getString(i));
            }
            pList->SetItemText(nIndex, i - 1, colData);
        }

        populatedRows++;
    }

    // Adjust column widths
    for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); i++) {
        pList->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }

    pList->SetRedraw(TRUE);  // Re-enable drawing


    // Handle pagination
    CString strMaxPages;
    auto rowsCount = resultSet->rowsCount();
    if (limit == 0) {
        strMaxPages.Format(_T("%d"), 1);
    }
    else {
        auto maxPages = (rowsCount + limit - 1) / limit;  // Efficient way to compute ceiling of rowsCount/limit
        strMaxPages.Format(_T("%d"), maxPages);
    }
    GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(strMaxPages);

    return 0;
}


void CDBMainDlg::OnEnChangeEditQtext()
{
    CString currentText;
    GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(currentText);
    history.push_back(currentText);
}


void CDBMainDlg::OnBnClickedBtnClear()
{   
    CRichEditCtrl* pEdit = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    pEdit->SetWindowTextW(L"");
}




void CDBMainDlg::OnBnClickedBtnPrinttable()
{
    if (m_resultSet != nullptr)
    {
        delete m_resultSet;
    }
    CString tableName;
    CString resultString;
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE); 
 
    int selectedIndex = dropdown->GetCurSel();

    if (selectedIndex != CB_ERR) 
    {
        dropdown->GetLBText(selectedIndex, tableName);
    }

    CString Query = L"SELECT * FROM ";
    Query += tableName;
    sql::SQLString query(CW2A(Query.GetString()));
    sql::ResultSet* resultSet = db->ExecuteQuery(query);
    m_resultSet = resultSet;
    //SendMessageToConsole(MSG_QUERY_OK, GREEN);
    FillListControl(resultSet);
    //delete resultSet;
}

//sent msg to output contol
void AppendTextToRichEdit(CRichEditCtrl& ctrl, const CString& text, COLORREF color)
{
    // Save the current selection
    CHARRANGE saveCharRange;
    ctrl.GetSel(saveCharRange);

    // Move the caret to the end of text
    long lTextLength = ctrl.GetTextLength();
    ctrl.SetSel(lTextLength, lTextLength);

    // Set the color
    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = color;
    ctrl.SetSelectionCharFormat(cf);

    // Append the text
    ctrl.ReplaceSel(text);

    // Restore the previous selection
    ctrl.SetSel(saveCharRange);
    // Scroll to the end so the latest text is visible //awesome
    ctrl.SendMessage(EM_SCROLL, SB_PAGEDOWN, 0);
}

void CDBMainDlg::SendMessageToConsole(CString msg, COLORREF color)
{
    CRichEditCtrl* p_richEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_MSGS);
    CTime currentTime = CTime::GetCurrentTime();
    // Format 
    CString timeStr = currentTime.Format(_T("%H:%M:%S"));
    // Adding timestamp
    CString fullMsg = timeStr + _T(" - ") + msg + _T("\r\n");
    // Append the text with a specific color
    AppendTextToRichEdit(*p_richEdit, fullMsg, color); 
}

void CDBMainDlg::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values)
{
    pComboBox->ResetContent();
    if (values.empty())
    {
        //pComboBox->AddString(L"No elements found");
        SendMessageToConsole(L"No elements found", RED);
    }
    for (const std::string& value : values)
    {
        CString item(value.c_str());
        pComboBox->AddString(item);
    }
}

void CDBMainDlg::OnBnClickedBtnClroutput()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    // Clear existing items from the list control
    pList->DeleteAllItems();
    while (pList->DeleteColumn(0)); // Remove all columns
    auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CURRENTPAGE);

    //GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
     //GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(L"0");
    //delete m_resultSet;
}


void CDBMainDlg::OnBnClickedBtnUndo()
{

}


void CDBMainDlg::OnBnClickedBtnDeletetable()
{
    int nResult = ::MessageBox(this->m_hWnd, L"Are you sure you want to delete table?", 
        L"Delete table?", MB_YESNO);

    if (nResult == IDYES)
    {
        // User clicked "Yes"
    }
    else if (nResult == IDNO)
    {
        // User clicked "No"
    }
}

void CDBMainDlg::OnBnClickedExport()
{
    SendMessageToConsole(MSG_EXPORT_START, BLACK);
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    exportWindow.m_pList = pList;
    
    switch (exportWindow.DoModal())
    {
    case(IDOK):
        SendMessageToConsole(MSG_EXPORT_OK, GREEN);
        break;
    case IDABORT:
        SendMessageToConsole(MSG_EXPORT_ERR, RED);
        break;
    case IDCANCEL:
        SendMessageToConsole(MSG_EXPORT_CANCEL,RED);
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


void CDBMainDlg::OnBnClickedBtnCollapse()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    ExpandAllItems(pTree, TVI_ROOT, TVE_COLLAPSE);
}


void CDBMainDlg::OnBnClickedBtnExpand()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    ExpandAllItems(pTree, TVI_ROOT, TVE_EXPAND);
}


void CDBMainDlg::OnBnClickedBtnUpdate()
{
    CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_STRUCTURE);
    pTree->DeleteAllItems();
    FillTreeControlWithDBTables(*pTree);
}


//void CDBMainDlg::OnEnChangeListSearch()
//{
//    // Retrieve the text from the edit control
//    CString searchText;
//    GetDlgItem(IDC_LIST_SEARCH)->GetWindowText(searchText);
//    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
//
//    int columnCount = pList->GetHeaderCtrl()->GetItemCount();
//
//    // Loop through the items in the list control
//    for (int i = pList->GetItemCount() - 1; i >= 0; --i) {
//
//        bool matchFound = false;
//
//        // Iterate through each column for the current row
//        for (int j = 0; j < columnCount; ++j) {
//            CString itemText = pList->GetItemText(i, j);
//
//            // If the item contains the search text, flag it as a match and break the loop
//            if (itemText.Find(searchText) != -1) {
//                matchFound = true;
//                break;
//            }
//        }
//
//        // If no match is found in any column for the current item, remove it
//        if (!matchFound) {
//            pList->DeleteItem(i);
//        }
//    }
//}


void CDBMainDlg::OnEnChangeListSearch()
{
    // Retrieve the text from the edit control
    CString searchText;
    GetDlgItem(IDC_LIST_SEARCH)->GetWindowText(searchText);
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);

    // First, restore the list from m_AllItems
    pList->DeleteAllItems();
    for (const auto& listItem : m_AllItems) {
        int index = pList->InsertItem(0, listItem.mainItem);
        for (size_t i = 0; i < listItem.subItems.size(); ++i) {
            pList->SetItemText(index, i + 1, listItem.subItems[i]);
        }
    }

    // Now, apply the search on the restored list
    int columnCount = pList->GetHeaderCtrl()->GetItemCount();

    // Loop through the items in the list control
    for (int i = pList->GetItemCount() - 1; i >= 0; --i) {

        bool matchFound = false;

        // Iterate through each column for the current row
        for (int j = 0; j < columnCount; ++j) {
            CString itemText = pList->GetItemText(i, j);

            // If the item contains the search text, flag it as a match and break the loop
            if (itemText.Find(searchText) != -1) {
                matchFound = true;
                break;
            }
        }

        // If no match is found in any column for the current item, remove it
        if (!matchFound) {
            pList->DeleteItem(i);
        }
    }
}


void CDBMainDlg::SaveOriginalListState()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);

    m_AllItems.clear();  // Clear the existing state

    int columnCount = pList->GetHeaderCtrl()->GetItemCount();

    // Loop through all items in the CListCtrl
    for (int i = 0; i < pList->GetItemCount(); ++i) {
        ListItem item;
        item.mainItem = pList->GetItemText(i, 0);

        // Save subitems (columns) for the current item
        for (int j = 1; j < columnCount; ++j) {
            item.subItems.push_back(pList->GetItemText(i, j));
        }

        m_AllItems.push_back(item);
    }
}


void CDBMainDlg::OnBnClickedBtnClearmsg()
{
    CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_MSGS);
    if (pRichEdit)
    {
        pRichEdit->SetWindowTextW(L"");
    }
    UpdateData(FALSE);
}


void CDBMainDlg::OnCbnSelchangeCmbSelDb()
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
    if (db->ChangeCurrentDatabase(sqlDatabaseName)) {
        FillTableDropdown();
        //FillTreeControl();
    }
    else
    {
        //SendMessageToConsole(MSG_DBCHANGE_ERR, RED);
    }
   // SendMessageToConsole(MSG_DBCHANGE_OK, GREEN);
}


void CDBMainDlg::OnBnClickedButtonSave()
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
        GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(content);

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


void CDBMainDlg::OnConnectionCheckconnection()
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



void CDBMainDlg::OnNMClickSyslinkServerinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    CServerInfoDlg serverinfoWindow;
    auto status = serverinfoWindow.DoModal();
    //*pResult = 0;
}


void CDBMainDlg::OnBnClickedBtnSchema()
{
    CString tableName;
    CString resultString;
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedIndex = dropdown->GetCurSel();

    if (selectedIndex != CB_ERR)
    {
        dropdown->GetLBText(selectedIndex, tableName);
    }

    CString Query = L"DESCRIBE " + tableName + ";";
    sql::SQLString query(CW2A(Query.GetString()));
    sql::ResultSet* resultSet = db->ExecuteQuery(query);
    //SendMessageToConsole(MSG_QUERY_OK, GREEN);
    FillListControl(resultSet);
    delete resultSet;
}

// <--------------------------- MENU HANDLERS -------------------------------->
void CDBMainDlg::OnFileSaveas()
{
    OnBnClickedButtonSave();
}


void CDBMainDlg::OnFileExit()
{
    db->Disconnect();
    this->EndDialog(IDCANCEL);
}


void CDBMainDlg::OnHelpMysqldocumentation()
{
    ShellExecute(0, 0, L"https://dev.mysql.com/doc/refman/8.1/en/", 0, 0, SW_SHOW);
}


void CDBMainDlg::OnFileExport()
{
    OnBnClickedExport();
}


void CDBMainDlg::OnEditUndo()
{
   ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->Undo();
}


void CDBMainDlg::OnEditRedo()
{
    ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->Redo();
}


void CDBMainDlg::OnMenuOpen()
{
    OnBnClickedBtnBrowse();
}


void CDBMainDlg::OnConnectionDisconnect()
{
    //delete m_resultSet;
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    pList->DeleteAllItems();
    db->Disconnect();
    this->EndDialog(IDOK);
}


void CDBMainDlg::OnEditCut()
{
    ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->Cut();
}


void CDBMainDlg::OnEditCopy()
{
    ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->Copy();
}



void CDBMainDlg::OnEditPaste()
{
    ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->Paste();
}


void CDBMainDlg::OnEditSelectall()
{
    ((CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT))->SetSel(0, -1);
}

///////////////// PAGE SYSTEM FUNCS ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void CDBMainDlg::OnEnChangeEditCurrentpage()
{
    auto pEdit = GetDlgItem(IDC_EDIT_CURRENTPAGE);
    if (m_resultSet == nullptr)
    {
        //pEdit->SetWindowTextW(L"0");
        return;
    }
    
    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);
    if (pageNumberStr == L"")
    {
        pEdit->SetWindowTextW(L"1");
        return; // Return here after setting the page to 1 to avoid further calculations in this call.
    }
    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);

    // Ensure pageNumber is at least 1
    if (pageNumber < 1)
    {
        pageNumber = 1;
        pEdit->SetWindowTextW(L"1");
    }

    // Get limit from dropdown
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
    int selectedIndex = dropdown->GetCurSel();
    CString dropdownText;
    int limit;

    if (selectedIndex != CB_ERR) {
        dropdown->GetLBText(selectedIndex, dropdownText);
    }
    if (dropdownText == L"All") {
        limit = 0;
    }
    else {
        std::wstring wstr(dropdownText);
        limit = std::stoi(wstr);
    }

    int offset = (pageNumber - 1) * limit; // Fixed offset calculation
    FillListControl(m_resultSet, offset);
}



void CDBMainDlg::OnBnClickedBtnFirstpage()
{
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
}


void CDBMainDlg::OnBnClickedBtnLastpage()
{
    CStringW title;
    GetDlgItem(IDC_STAT_MAXPAGE)->GetWindowTextW(title);
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(title);
}


void CDBMainDlg::OnBnClickedBtnPrevpage()
{
    auto pEdit = GetDlgItem(IDC_EDIT_CURRENTPAGE);
    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);
    if (pageNumberStr == L"")
    {
        pEdit->SetWindowTextW(L"1");
        return; // If it's already the first page, just return.
    }
    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);
    if (pageNumber > 1)  // Ensure we don't go to negative or zero page numbers
    {
        pageNumber--;
        pageNumberStr.Format(_T("%d"), pageNumber);
        pEdit->SetWindowTextW(pageNumberStr);  // Set updated page number
    }
}

void CDBMainDlg::OnBnClickedBtnNextpage()
{
    auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CURRENTPAGE); // Cast to CEdit* for clarity

    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);

    // If the current page number string is empty, default it to "1" and return.
    if (pageNumberStr.IsEmpty())  // Use IsEmpty() method instead of == L""
    {
        pEdit->SetWindowTextW(L"1");
        return;
    }

    // Get the maximum page number from the IDC_STAT_MAXPAGE control
    CStringW maxPageCStr;
    auto pMaxPageCtrl = (CStatic*)GetDlgItem(IDC_STAT_MAXPAGE); // Cast to CStatic* for clarity
    pMaxPageCtrl->GetWindowTextW(maxPageCStr);

    // Convert page numbers from CStringW to integers
    int currentPage = _wtoi(pageNumberStr);  // Use _wtoi for a simpler string to int conversion
    int maxPage = _wtoi(maxPageCStr);

    // Check if we're already on the last page
    if (currentPage >= maxPage)
    {
        // Optionally: notify the user or handle this case differently
        return;
    }

    // Increment the page number and update the control
    currentPage++;
    pageNumberStr.Format(_T("%d"), currentPage);
    pEdit->SetWindowTextW(pageNumberStr);
}



void CDBMainDlg::OnCbnSelchangeSelTable()
{
    //OnBnClickedBtnPrinttable();
}


void CDBMainDlg::OnBnClickedCheckShowall()
{
    if (AfxMessageBox(_T("Do you really want to see all of the rows? For a "
        "big table this could a long time "),
        MB_YESNO | MB_ICONQUESTION) == IDYES)
    {


    }
    else
    {

    }
}


void CDBMainDlg::OnCbnSelchangeComboNmbOfRows()
{
    OnEnChangeEditCurrentpage();
}


void CDBMainDlg::OnAboutVersionandcredits()
{
    CAboutDlg about;
    about.DoModal();
}


void CDBMainDlg::OnBnClickedBtnSelectall()
{
    OnBnClickedBtnClear();
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* dbDropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    CString table;
    CString database;

    if (dbDropdown->GetCount() == 0 || tablesDropdown->GetCount() == 0)
    {
        return;
    }

    dbDropdown->GetLBText(selectedDBNumber, database); 
    tablesDropdown->GetLBText(selectedTableNumber, table); 
    

    queryText->SetWindowTextW(L"SELECT * FROM " + database + "." + table + " " + "WHERE 1");
}

CString FormatSQLQuery(const CString& query)
{
    CString formattedQuery;

    // Splitting by space for simplicity.
    int startPos = 0;
    while (startPos >= 0)
    {
        CString token = query.Tokenize(L" ", startPos);
        if (token.IsEmpty()) break;

        // Add a newline and indent for specific keywords.
        if (token.CompareNoCase(L"SELECT") == 0)
        {
            formattedQuery += token + L"\r\n";
        }
        else if 
           (token.CompareNoCase(L"FROM") == 0 ||
            token.CompareNoCase(L"WHERE") == 0 ||
            token.CompareNoCase(L"JOIN") == 0 ||
            token.CompareNoCase(L"INSERT") == 0 ||
            token.CompareNoCase(L"INTO") == 0 ||
            token.CompareNoCase(L"ON") == 0  ||
            token.CompareNoCase(L"UPDATE") == 0 ||
            token.CompareNoCase(L"DELETE FROM") == 0 ||
            token.CompareNoCase(L"VALUES") == 0)
        {
            formattedQuery.TrimRight();  // Trim any trailing spaces or commas
            formattedQuery += L"\r\n" + token + L"\r\n";
        }
        else
        {
            formattedQuery += L"\t" + token + L"\r\n";
        }
    }

    // Trim any extra newlines or whitespace at the end
    formattedQuery.TrimRight();

    return formattedQuery;
}


void CDBMainDlg::OnBnClickedBtnRefactor()
{
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);

    CString currentQuery;
    queryText->GetWindowTextW(currentQuery);

    CString formattedQuery = FormatSQLQuery(currentQuery);
    queryText->SetWindowTextW(formattedQuery);
}

void CDBMainDlg::OnHelpServerinfo()
{
    OnNMClickSyslinkServerinfo(NULL, NULL);
}


void CDBMainDlg::OnBnClickedBtnnSelect()
{
    OnBnClickedBtnClear();
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* dbDropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    CString table;
    CString database;
    CString columnList;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    std::vector<sql::SQLString> tableColumns = db->GetTableColumns(CStringToSQLString(table));

    for (const auto& columns : tableColumns)
    {
        columnList += SQLStringToCString(columns);
        columnList += ",";
    }
    columnList.TrimRight(',');
    queryText->SetWindowTextW(L"SELECT " + columnList + " FROM " + database + "." + table + " " + "WHERE 1");
}


void CDBMainDlg::OnBnClickedBtnInsert()
{
    OnBnClickedBtnClear();
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* dbDropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    CString table;
    CString database;
    CString columnList;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    std::vector<sql::SQLString> tableColumns = db->GetTableColumns(CStringToSQLString(table));
    CString valuesList;
    for (const auto& columns : tableColumns)
    {
        columnList += SQLStringToCString(columns);
        columnList += ",";
        valuesList += "'VALUE',";
    }
    valuesList.TrimRight(',');
    columnList.TrimRight(',');
    queryText->SetWindowTextW(L"INSERT INTO " + database + "." + table +" ("+ columnList +
        ")" + " VALUES " + "(" + valuesList + ")");
}


void CDBMainDlg::OnBnClickedBtnUpdaterecord()
{
    OnBnClickedBtnClear();
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* dbDropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    CString table;
    CString database;
    CString columnList;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    std::vector<sql::SQLString> tableColumns = db->GetTableColumns(CStringToSQLString(table));
    CString valuesList;
    for (const auto& columns : tableColumns)
    {
        columnList += SQLStringToCString(columns);
        columnList += "= 'VALUE',";
    }
    valuesList.TrimRight(',');
    columnList.TrimRight(',');
    queryText->SetWindowTextW(L"UPDATE " + database + "." + table + " SET " + columnList + " WHERE 1");
}


void CDBMainDlg::OnBnClickedBtnDeleterecord()
{
    OnBnClickedBtnClear();
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* dbDropdown = (CComboBox*)GetDlgItem(IDC_CMB_SEL_DB);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    CString table;
    CString database;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    queryText->SetWindowTextW(L"DELETE FROM " + database + "." + table + " WHERE 0");
}


void CDBMainDlg::OnBnClickedBtnForward()
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


void CDBMainDlg::OnTvnSelchangedTreeStructure(NMHDR* pNMHDR, LRESULT* pResult)
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
        int index = pComboTables->FindStringExact(0, tableName);
        if (index != CB_ERR)
        {
            pComboTables->SetCurSel(index);
        }
    }

    *pResult = 0;
}

// single click expand selected element
void CDBMainDlg::OnNMClickTreeStructure(NMHDR* pNMHDR, LRESULT* pResult)
{
    UINT flags;
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
        if (flags & TVHT_ONITEM && !(flags & TVHT_ONITEMSTATEICON))
        {
            if (pTree->GetItemText(hItem) == _T("New") && hItem != pTree->GetRootItem())
            {
                CNewDBDlg dlg;
                dlg.DoModal();
                *pResult = 0;
                return;
            }
            HTREEITEM parentItem = pTree->GetParentItem(hItem);
            if (parentItem && pTree->GetItemText(parentItem) == _T("[TABLES]"))
            {
                // Если элемент является потомком узла [TABLES], ничего не делайте
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
    }

    *pResult = 0;
}

