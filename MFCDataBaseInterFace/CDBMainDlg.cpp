#include <fstream>
#include "pch.h"
#include "CDBInterfaceApp.h"
#include "afxdialogex.h"
#include "CDBMainDlg.h"
#include "framework.h"
#include "CDBAuthDlg.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <iomanip>


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
const CString MSG_DBCHANGE_OK("Databased changed");
const CString MSG_DBCHANGE_ERR("Databased change error");


IMPLEMENT_DYNAMIC(CDBMainDlg, CDialogEx)
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
//    sql::ResultSet* resultSet;
//
//    try {
//        // Fetch all tables
//        resultSet = db->ExecuteQuery("SHOW TABLES");
//        if (!resultSet) return; // Exit if there was an error
//
//        while (resultSet->next()) {
//            std::string tableName = resultSet->getString(1);
//            HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()));
//
//            // For each table, fetch its columns
//            sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + tableName);
//            if (columnResultSet) {
//                while (columnResultSet->next()) {
//                    std::string columnName = columnResultSet->getString(1);
//                    treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
//                }
//                delete columnResultSet;
//            }
//        }
//        delete resultSet;
//
//    }
//    catch (sql::SQLException& e) {
//        // Handle exception: you might want to show a message box or log the error.
//        AfxMessageBox(CA2T(e.what()));
//    }
//}

void CDBMainDlg::FillTreeControlWithDBTables(CTreeCtrl& treeCtrl) { 
    if (treeCtrl.GetRootItem()) 
    {
        treeCtrl.DeleteAllItems();
    }

    HTREEITEM tablesRoot = treeCtrl.InsertItem(_T("[Tables]"));
    HTREEITEM proceduresRoot = treeCtrl.InsertItem(_T("[Procedures]"));
    HTREEITEM functionsRoot = treeCtrl.InsertItem(_T("[Functions]"));
    sql::ResultSet* resultSet;

    CStringW queryBase = L"SHOW %s STATUS WHERE Db = '" + m_titleDatabaseName + L"'";
    CStringA query;

    try {
        // Fetch all tables
        resultSet = db->ExecuteQuery("SHOW TABLES");
        if (resultSet) {
            while (resultSet->next()) {
                std::string tableName = resultSet->getString(1);
                HTREEITEM tableItem = treeCtrl.InsertItem(CA2T(tableName.c_str()), tablesRoot);

                // For each table, fetch its columns
                sql::ResultSet* columnResultSet = db->ExecuteQuery("SHOW COLUMNS FROM " + tableName);
                if (columnResultSet) {
                    while (columnResultSet->next()) {
                        std::string columnName = columnResultSet->getString(1);
                        treeCtrl.InsertItem(CA2T(columnName.c_str()), tableItem);
                    }
                    delete columnResultSet;
                }
            }
            delete resultSet;
        }

        // Fetch all procedures
        query.Format(CStringA(queryBase), "PROCEDURE");
        std::string queryProcedure(query.GetBuffer());
        resultSet = db->ExecuteQuery(queryProcedure);
        if (resultSet) {
            while (resultSet->next()) {
                std::string procedureName = resultSet->getString("Name");
                treeCtrl.InsertItem(CA2T(procedureName.c_str()), proceduresRoot);
            }
            delete resultSet;
        }

        // Fetch all functions
        query.Format(CStringA(queryBase), "FUNCTION");
        std::string queryFunction(query.GetBuffer());
        resultSet = db->ExecuteQuery(queryFunction);
        if (resultSet) {
            while (resultSet->next()) {
                std::string functionName = resultSet->getString("Name");
                treeCtrl.InsertItem(CA2T(functionName.c_str()), functionsRoot);
            }
            delete resultSet;
        }

    }
    catch (sql::SQLException& e) {
        // Handle exception: you might want to show a message box or log the error.
        AfxMessageBox(CA2T(e.what()));
    }
}


void CDBMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
 
}

BOOL CDBMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    //set icon
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    //set title || Now show current db name
    this->SetWindowTextW(m_titleDatabaseName);

    //set visible at task bar
    ModifyStyleEx(0, WS_EX_APPWINDOW);

 
    //set list ctrl to table style
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    pList->SetView(LV_VIEW_DETAILS);

    FillDatabaseDropdown();
    FillLimitDropdown();
    return TRUE;
}

// fill drop down with table names of db

bool CDBMainDlg::FillDatabaseDropdown() 
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_CMB_SEL_DB));
    std::vector<sql::SQLString> databases;
    databases = db->GetDatabases();
    PopulateDropdown(pComboBox, databases);
    //pComboBox->SetCurSel(0);
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
    pComboBox->AddString(L"25");
    pComboBox->AddString(L"50");
    pComboBox->AddString(L"100");
    pComboBox->AddString(L"250");
    pComboBox->AddString(L"500");
    pComboBox->AddString(L"All");
    pComboBox->SetCurSel(3);
    return true;
}




BEGIN_MESSAGE_MAP(CDBMainDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BROWSE, &CDBMainDlg::OnBnClickedBtnBrowse)
    ON_BN_CLICKED(IDC_BTN_GO, &CDBMainDlg::OnBnClickedBtnGo)
    ON_EN_CHANGE(IDC_EDIT_QTEXT, &CDBMainDlg::OnEnChangeEditQtext)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CDBMainDlg::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CDBMainDlg::OnBnClickedBtnDisconnect)
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
END_MESSAGE_MAP()

//open .sql file
void CDBMainDlg::OnBnClickedBtnBrowse()
{
    CFileDialog fileOpenDialog(TRUE,
        L"SQL files|sql",  // Добавляем поддержку .sql файлов
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        L"SQL files (*.sql)|*.sql|All files (*.*)|*.*||",  // Изменяем фильтр файлов
        nullptr);

    if (fileOpenDialog.DoModal() == IDOK)
    {
        m_pathToFile = fileOpenDialog.GetPathName();
        GetDlgItem(IDC_EDT_FILENAME)->SetWindowTextW(m_pathToFile);
        GetDlgItem(IDC_EDT_FILENAME)->EnableWindow(FALSE);
    }

    m_SQLtextQuery = ReadFileContent();
    GetDlgItem(IDC_EDIT_QTEXT)->SetWindowTextW(m_SQLtextQuery);
}


CString CDBMainDlg::ReadFileContent()
{
    CString fileContent;
    try
    {
        CFile file;
        if (file.Open(m_pathToFile, CFile::modeRead))
        {
            ULONGLONG fileSize = file.GetLength();
            char* buffer = new char[fileSize];

            file.Read(buffer, static_cast<UINT>(fileSize));
            file.Close();

            fileContent = CString(buffer, static_cast<int>(fileSize));

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

CString SQLStringToCString(const sql::SQLString& sqlStr)
{
    std::string utf8Str = sqlStr.asStdString();

    // Calculate the required buffer size
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), NULL, 0);

    CString utf16CString;
    wchar_t* buf = utf16CString.GetBufferSetLength(size_needed);

    // Convert the UTF-8 string to UTF-16
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), buf, size_needed);

    utf16CString.ReleaseBuffer();

    return utf16CString;
}

sql::SQLString CStringToSQLString(const CString& cstr)
{
    // Calculate the required buffer size for the UTF-8 conversion
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), -1, NULL, 0, NULL, NULL);

    std::string utf8Str(size_needed, 0);

    // Convert the UTF-16 string (CString) to UTF-8
    WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), -1, &utf8Str[0], size_needed, NULL, NULL);

    return sql::SQLString(utf8Str);
}


void CDBMainDlg::OnBnClickedBtnGo()
{
    CString sqlText;
    CString resultString;
    GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(sqlText);

    // sql::SQLString query(CW2A(sqlText.GetString())); //old method 
    sql::SQLString query = CStringToSQLString(sqlText);
    
    sql::ResultSet* resultSet = db->ExecuteQuery(query, errorString);
    if (resultSet)
    {
        SendMessageToConsole(MSG_QUERY_OK, GREEN);
    }
    else {
        SendMessageToConsole(errorString, RED);
    }
    FillListControl(resultSet);
    //SendMessageToConsole();
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

int CDBMainDlg::FillListControl(sql::ResultSet* resultSet) {
    // Получение лимита из выпадающего списка
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

    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    if (!resultSet || !pList) {
        return 1;
    }

    // Очистка текущих элементов из CListCtrl
    pList->DeleteAllItems();
    while (pList->DeleteColumn(0)); // Удаление всех столбцов

    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
    int columnCount = metaData->getColumnCount();
    CString columnName(L""), firstColData(L""), colData(L"");

    // Заполнение столбцов
    for (int i = 1; i <= columnCount; i++) {
        columnName = metaData->getColumnName(i).c_str();
        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
    }

    int populatedRows = 0;

    // Заполнение строк с учетом лимита
    while (resultSet->next() && (limit == 0 || populatedRows < limit)) {
        firstColData = SQLStringToCString(resultSet->getString(1));
        int nItemCount = pList->GetItemCount();
        int nIndex = pList->InsertItem(LVIF_TEXT, nItemCount, firstColData, 0, 0, 0, 0);

        for (int i = 1; i <= columnCount; i++) {
            // Проверка типа столбца: является ли он бинарным
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

    return 0;
}

//save changes to vector to redo and undo in future
void CDBMainDlg::OnEnChangeEditQtext()
{
    CString currentText;
    GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(currentText);
    history.push_back(currentText);
}


void CDBMainDlg::OnBnClickedBtnClear()
{
    CString emptyString = L"";
    GetDlgItem(IDC_EDT_FILENAME)->SetWindowTextW(emptyString);
    GetDlgItem(IDC_EDT_FILENAME)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_QTEXT)->SetWindowTextW(emptyString);
}


void CDBMainDlg::OnBnClickedBtnDisconnect()
{
    this->EndDialog(IDOK);
    db->Disconnect();
}

void CDBMainDlg::OnBnClickedBtnPrinttable()
{
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
    SendMessageToConsole(MSG_QUERY_OK, GREEN);
    FillListControl(resultSet);
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



void CDBMainDlg::OnMenuOpen()
{
    OnBnClickedBtnBrowse();
}


void CDBMainDlg::OnConnectionDisconnect()
{
    db->Disconnect();
    this->EndDialog(IDOK);
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
        FillTreeControl();
    }
    else
    {
        SendMessageToConsole(MSG_DBCHANGE_ERR, RED);
    }
    SendMessageToConsole(MSG_DBCHANGE_OK, GREEN);
}
