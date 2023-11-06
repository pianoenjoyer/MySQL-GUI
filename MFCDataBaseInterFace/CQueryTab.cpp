// CQueryTab.cpp : implementation file
//
#include "pch.h"
#include "afxdialogex.h"
#include "CDBInterfaceApp.h"

#include "CQueryTab.h"
#include "CDBMainDlg.h"
#include "Convertions.h"
#include "SharedFunctions.h"

#include <chrono>
// CQueryTab dialog
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

IMPLEMENT_DYNAMIC(CQueryTab, CDialogEx)

CQueryTab::CQueryTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUERY, pParent)
{
    m_resultSet = nullptr;
}

CQueryTab::~CQueryTab()
{
}

void CQueryTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEL_TABLE, m_comboTables);
}

BOOL CQueryTab::OnInitDialog() 
{
    CDialogEx::OnInitDialog();
    FillTableDropdown();
    PopulateColumnsList();
    return TRUE;
}

CString GetCurValDropdown(CComboBox* pCombo) 
{
    if (pCombo == nullptr)
    {
        return L"";
    }
    int curSel = pCombo->GetCurSel();
    if (curSel != CB_ERR)
    {
        CString curText;
        pCombo->GetLBText(curSel, curText);
        return curText;
    }
    return L"";
}

void CQueryTab::PopulateColumnsList() 
{
    CString tableName = GetCurValDropdown(((CComboBox*)GetDlgItem(IDC_SEL_TABLE)));
    auto columns = db->GetTableColumns(CStringToSQLString(tableName));
    auto pListBox = (CListBox*)GetDlgItem(IDC_LIST_COLUMNS);

    if (!pListBox) {
        AfxMessageBox(L"List box doesn't exist");
        return;
    }

    pListBox->ResetContent(); // Clear the existing items

    for (const auto& value : columns) {
        pListBox->AddString(SQLStringToCString(value)); // Add items to the list box
    }
}



void CQueryTab::PopulateDropdown(CComboBox* pComboBox, const std::vector<sql::SQLString>& values)
{
    pComboBox->ResetContent();
    if (values.empty())
    {
        //pComboBox->AddString(L"No elements found");
        //SendMessageToConsole(L"No elements found", RED);
    }
    for (const std::string& value : values)
    {
        CString item(value.c_str());
        pComboBox->AddString(item);
    }
}

bool CQueryTab::FillTableDropdown()
{
    std::vector<sql::SQLString> tableNames;
    CTabCtrl* pMainTab = ((CTabCtrl*)this->GetParent());
    CDBMainDlg* pMainDlg = (CDBMainDlg*)pMainTab->GetParent();

    tableNames = pMainDlg->db->GetTables();
    PopulateDropdown(&m_comboTables, tableNames);
    m_comboTables.SetCurSel(0);
    return true;
}

BEGIN_MESSAGE_MAP(CQueryTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GO, &CQueryTab::OnBnClickedBtnGo)
    ON_BN_CLICKED(IDC_BTN_SELECTALL, &CQueryTab::OnBnClickedBtnSelectall)
    ON_BN_CLICKED(IDC_BTNN_SELECT, &CQueryTab::OnBnClickedBtnnSelect)
    ON_BN_CLICKED(IDC_BTN_INSERT, &CQueryTab::OnBnClickedBtnInsert)
    ON_BN_CLICKED(IDC_BTN_UPDATERECORD, &CQueryTab::OnBnClickedBtnUpdaterecord)
    ON_BN_CLICKED(IDC_BTN_DELETERECORD, &CQueryTab::OnBnClickedBtnDeleterecord)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CQueryTab::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_REFACTOR, &CQueryTab::OnBnClickedBtnRefactor)
    ON_BN_CLICKED(IDC_BTN_CLEARMSG, &CQueryTab::OnBnClickedBtnClearmsg)
    ON_CBN_SELCHANGE(IDC_SEL_TABLE, &CQueryTab::OnCbnSelchangeSelTable)
    ON_BN_CLICKED(IDC_BTN_FORWARD, &CQueryTab::OnBnClickedBtnForward)
    ON_BN_CLICKED(IDC_BTN_SCHEMA, &CQueryTab::OnBnClickedBtnSchema)
END_MESSAGE_MAP()


void CQueryTab::SendMessageToConsole(CString msg, COLORREF color)
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

inline sql::SQLString CStringToSQLString(const CString& cstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), cstr.GetLength(), NULL, 0, NULL, NULL);

    std::string utf8Str(size_needed, 0);

    // Convert the UTF-16 string (CString) to UTF-8
    WideCharToMultiByte(CP_UTF8, 0, cstr.GetString(), cstr.GetLength(), &utf8Str[0], size_needed, NULL, NULL);

    return sql::SQLString(utf8Str);
}

// CQueryTab message handlers
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

//if query text from rich edit
void CQueryTab::ExecuteQueryMainDlg()
{
    CDBMainDlg* pParentDialog = nullptr;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CDBMainDlg*)pTabCtrl->GetParent();
        if (!pParentDialog) {
            return;
        }
    }

    if (m_resultSet)
    {
        delete m_resultSet;
        m_resultSet = nullptr;
    }

    CStringW sqlText;
    GetDlgItem(IDC_EDIT_QTEXT)->GetWindowTextW(sqlText);

    //sql::SQLString query(CW2A(sqlText.GetString())); //old method 

    sql::SQLString query = CStringToSQLString(sqlText);

    auto start = std::chrono::high_resolution_clock::now();
    m_resultSet = db->ExecuteQuery(query, errorString);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    double timeTaken = duration.count();
    CString timeTakenStr;


    if (m_resultSet)
    {
        int rowsCount = m_resultSet->rowsCount();
        timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

        SendMessageToConsole(timeTakenStr, GREEN);
        //send exac same msg to resultTab edit
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->m_resultTab.SendMessageToQueryInfo(timeTakenStr, GREEN);
        start = std::chrono::high_resolution_clock::now();
        pParentDialog->m_resultTab.FillListControl(m_resultSet, 0);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(end - start);
        timeTaken = duration.count();
        //timeTakenStr.Format(_T("Built list took: %.2f seconds"), timeTaken);
        //SendMessageToConsole(timeTakenStr, BLACK);
        if (rowsCount == 0)
        {
            pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
        }
        else
        {
            pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
        }
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->SwitchTabByName(L"Result");
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->OnTcnSelchangeMaintab(nullptr, nullptr);
    }
    else
    {
        SendMessageToConsole(errorString, RED);
    }
    //delete resultSet;
}

//if query from string
void CQueryTab::ExecuteQueryMainDlg(sql::SQLString queryText)
{
    CDBMainDlg* pParentDialog = nullptr;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CDBMainDlg*)pTabCtrl->GetParent();
        if (!pParentDialog) {
            return;
        }
    }

    if (m_resultSet)
    {
        delete m_resultSet;
        m_resultSet = nullptr;
    }


    auto start = std::chrono::high_resolution_clock::now();
    m_resultSet = db->ExecuteQuery(queryText, errorString);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);
    double timeTaken = duration.count();
    CString timeTakenStr;


    if (m_resultSet)
    {
        int rowsCount = m_resultSet->rowsCount();
        timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

        SendMessageToConsole(timeTakenStr, GREEN);
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->m_resultTab.SendMessageToQueryInfo(timeTakenStr, GREEN);
        start = std::chrono::high_resolution_clock::now();
        pParentDialog->m_resultTab.FillListControl(m_resultSet, 0);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(end - start);
        timeTaken = duration.count();
        //timeTakenStr.Format(_T("Built list took: %.2f seconds"), timeTaken);
        //SendMessageToConsole(timeTakenStr, BLACK);
        if (rowsCount == 0)
        {
            pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
        }
        else
        {
            pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
        }
        //pParentDialog->SetResultSet(resultSet);
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->SwitchTabByName(L"Result");
        ((CDBMainDlg*)(this->GetParent()->GetParent()))->OnTcnSelchangeMaintab(nullptr, nullptr);
    }
    else
    {
        SendMessageToConsole(errorString, RED);
    }
    //delete resultSet;
}


void CQueryTab::OnBnClickedBtnGo()
{
    //((CDBMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(75);
    ExecuteQueryMainDlg();
    //((CDBMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(100);
    //((CDBMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(0);
}


void CQueryTab::OnBnClickedBtnClear()
{
    CRichEditCtrl* pEdit = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    pEdit->SetWindowTextW(L"");
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
                token.CompareNoCase(L"ON") == 0 ||
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


void CQueryTab::OnBnClickedBtnRefactor()
{
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);

    CString currentQuery;
    queryText->GetWindowTextW(currentQuery);

    CString formattedQuery = FormatSQLQuery(currentQuery);
    queryText->SetWindowTextW(formattedQuery);
}


void CQueryTab::OnBnClickedBtnSelectall()
{
    OnBnClickedBtnClear();
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent(); 

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }

    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();

    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }

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
    queryText->SetWindowTextW(L"SELECT * FROM " + database + "." + table + " " + "WHERE 1");

} 

void CQueryTab::OnBnClickedBtnnSelect()
{
    OnBnClickedBtnClear();
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
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


void CQueryTab::OnBnClickedBtnInsert()
{
    OnBnClickedBtnClear();
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
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
    queryText->SetWindowTextW(L"INSERT INTO " + database + "." + table + " (" + columnList +
        ")" + " VALUES " + "(" + valuesList + ")");
}


void CQueryTab::OnBnClickedBtnUpdaterecord()
{
    OnBnClickedBtnClear();
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
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


void CQueryTab::OnBnClickedBtnDeleterecord()
{
    OnBnClickedBtnClear();
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }

    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    CString table;
    CString database;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    queryText->SetWindowTextW(L"DELETE FROM " + database + "." + table + " WHERE 0");
}


void CQueryTab::OnBnClickedBtnClearmsg()
{
    CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_MSGS);
    if (pRichEdit)
    {
        pRichEdit->SetWindowTextW(L"");
    }
    UpdateData(FALSE);
}

void CQueryTab::OnCbnSelchangeSelTable()
{
    PopulateColumnsList();
}


void CQueryTab::OnBnClickedBtnForward()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_COLUMNS);  // Replace with your ListBox's ID
    CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_EDIT_QTEXT);

    int nSelectedItem = pListBox->GetCurSel();

    if (nSelectedItem != LB_ERR)
    {
        CString itemText;
        pListBox->GetText(nSelectedItem, itemText);
        pRichEdit->ReplaceSel(itemText, TRUE);
    }
    else
    {
        SendMessageToConsole(L"No item selected!", RED);
    }
}




void CQueryTab::OnBnClickedBtnSchema()
{
    CString tableName;
    CString resultString;
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    int selectedIndex = tablesDropdown->GetCurSel();

    if (selectedIndex != CB_ERR)
    {
        tablesDropdown->GetLBText(selectedIndex, tableName);
    }

    CString Query = L"DESCRIBE " + tableName + ";";
    sql::SQLString query(CW2A(Query.GetString()));

    ExecuteQueryMainDlg(query);
}
