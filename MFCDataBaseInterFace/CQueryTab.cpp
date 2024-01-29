
#include "pch.h"
#include "afxdialogex.h"
#include "CDBInterfaceApp.h"
#include "SendMessagesUtils.h"
#include "CQueryTab.h"
#include "CMainDlg.h"
#include "SharedFunctions.h"
#include "Colors.h"
#include <chrono>
#include "Convertions.h"
#include "CQueryHistoryDlg.h"

#include <fstream>

IMPLEMENT_DYNAMIC(CQueryTab, CDialogEx)

CQueryTab::CQueryTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUERY, pParent)
{
    m_resultSet = nullptr;
}

CQueryTab::~CQueryTab()
{
    delete m_resultSet;
}

void CQueryTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEL_TABLE, m_comboTables);
}

BOOL CQueryTab::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0)
    {
        switch (pMsg->wParam)
        case 'G':
        {
            OnBnClickedBtnGo();
            return TRUE;
        }
    }
    if (pMsg->message == WM_KEYDOWN && GetKeyState(VK_SHIFT) < 0)
    {
        CTabCtrl* pParentTab = (CTabCtrl*)this->GetParent();
        if (!pParentTab)
        {
            return FALSE;
        }

        CMainDlg* pParentMain = (CMainDlg*)pParentTab->GetParent();
        if (!pParentMain)
        {
            return FALSE;
        }

        switch (pMsg->wParam)
        {
        case 'H':
        {
            pParentMain->SetCurActiveTab(0);
            return TRUE;
        }
        case 'Q':
        {
            pParentMain->SetCurActiveTab(1);
            return TRUE;
        }
        case 'R':
        {
            pParentMain->SetCurActiveTab(2);
            return TRUE;
        }

        case 'X':
        {
            pParentMain->SetCurActiveTab(3);
            return TRUE;
        }
        case 'T':
        {
            pParentMain->SetCurActiveTab(4);
            return TRUE;
        }
        case 'D':
        {
            pParentMain->SetCurActiveTab(5);
            return TRUE;
        }
        case 'V':
        {
            pParentMain->SetCurActiveTab(6);
            return TRUE;
        }
        case 'C':
        {
            pParentMain->SetCurActiveTab(7);
            return TRUE;
        }
        case 'S':
        {
            pParentMain->SetCurActiveTab(8);
            return TRUE;
        }
        case 'P':
        {
            pParentMain->SetCurActiveTab(9);
            return TRUE;
        }
        case 'E':
        {
            pParentMain->SetCurActiveTab(10);
            return TRUE;
        }
        default:
            return FALSE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CQueryTab::SetDefaultFontSize() 
{
    auto pComboFontSize = (CComboBox*)GetDlgItem(IDC_FONT_SIZE);
    if (pComboFontSize)
    {
        pComboFontSize->SetCurSel(1);
        return true;
    }
    return false;
}

BOOL CQueryTab::SetDefaultFont()
{
    auto pComboFontCombo = (CMFCFontComboBox*)GetDlgItem(IDC_FONTCOMBO);
    if (pComboFontCombo)
    {
        pComboFontCombo->SetCurSel(0);
        return true;
    }
    return false;
}

void CQueryTab::SetSQLEditText(const CString& text)  
{
    auto pRichSql = GetDlgItem(IDC_RICH_SQL);
    if (!pRichSql)
    {
        return;
    }
    pRichSql->SetWindowText(text);

}

BOOL CQueryTab::OnInitDialog() 
{
    CDialogEx::OnInitDialog();
    SetBackgroundColor(TABWHITE);
    auto pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    DWORD dwMask = pRichEdit->GetEventMask();
    dwMask |= (ENM_CHANGE | ENM_SCROLL);
    pRichEdit->SetEventMask(dwMask);
    PopulateFontSizesDropdown();
    SetDefaultFontSize();
    SetDefaultFont();
    //SetBackgroundColor(RGB(200, 200, 200));
    UpdateStringCounter();
    //FillTableDropdown();
    //PopulateColumnsList();
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
    CMainDlg* pMainDlg = (CMainDlg*)pMainTab->GetParent();

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
    ON_LBN_DBLCLK(IDC_LIST_COLUMNS, &CQueryTab::OnLbnDblclkListColumns)
    ON_EN_CHANGE(IDC_RICH_SQL, &CQueryTab::OnEnChangeEditQuery)
    ON_EN_VSCROLL(IDC_RICH_SQL, &CQueryTab::OnEnVscrollEditQuery)
    ON_WM_VSCROLL()
    ON_EN_VSCROLL(IDC_RICH_LINENUMBERER, &CQueryTab::OnEnVscrollStringcounter)
    ON_EN_CHANGE(IDC_RICH_SQL, &CQueryTab::OnEnChangeRichSql)
    ON_EN_VSCROLL(IDC_RICH_SQL, &CQueryTab::OnEnVscrollRichSql)
    ON_CBN_SELCHANGE(IDC_FONT_SIZE, &CQueryTab::OnCbnSelchangeFontSize)
    ON_CBN_SELCHANGE(IDC_FONTCOMBO, &CQueryTab::OnCbnSelchangeFontcombo)
    ON_BN_CLICKED(IDC_COLOR_FONT, &CQueryTab::OnBnClickedColorFont)
    ON_BN_CLICKED(IDC_BTN_HISTORY, &CQueryTab::OnBnClickedBtnHistory)
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

CStringW RemoveSQLComments(const CStringW& sqlText)
{
    // Simple implementation to remove SQL comments
    CStringW result;
    bool inComment = false;

    for (int i = 0; i < sqlText.GetLength(); ++i)
    {
        if (sqlText[i] == L'-' && i + 1 < sqlText.GetLength() && sqlText[i + 1] == L'-')
        {
            // Start of single-line comment
            inComment = true;
        }
        else if (sqlText[i] == L'\n' && inComment)
        {
            // End of single-line comment
            inComment = false;
        }
        else if (sqlText[i] == L'/' && i + 1 < sqlText.GetLength() && sqlText[i + 1] == L'*')
        {
            // Start of multi-line comment
            inComment = true;
            ++i;  // Skip the next character as well
        }
        else if (sqlText[i] == L'*' && i + 1 < sqlText.GetLength() && sqlText[i + 1] == L'/' && inComment)
        {
            // End of multi-line comment
            inComment = false;
            ++i;  // Skip the next character as well
        }
        else if (!inComment)
        {
            result += sqlText[i];
        }
    }

    return result;
}


#include <fstream>

void CQueryTab::AddQueryToHistoryFile(const CStringW& query)
{
    CStringW fileName = L"QueryHistory.txt";
    CStringW delimDateStart = L"|;Q|";
    CStringW delimDateEnd = L"|Q;|";
    CStringW delimQueryStart = L"|;D|";
    CStringW delimQueryEnd = L"|D;|";

    // Get the current date
    CTime currentTime = CTime::GetCurrentTime();
    CStringW currentDate = currentTime.Format(L"%d.%m.%Y");

    // Open the file in append mode
    std::ofstream historyFile(fileName, std::ios::app);

    if (historyFile.is_open())
    {
        // Convert wide strings to UTF-8-encoded narrow strings
        std::string utf8DelimDateStart = CW2A(delimDateStart.GetString(), CP_UTF8);
        std::string utf8CurrentDate = CW2A(currentDate.GetString(), CP_UTF8);
        std::string utf8DelimDateEnd = CW2A(delimDateEnd.GetString(), CP_UTF8);
        std::string utf8DelimQueryStart = CW2A(delimQueryStart.GetString(), CP_UTF8);
        std::string utf8Query = CW2A(query.GetString(), CP_UTF8);
        std::string utf8DelimQueryEnd = CW2A(delimQueryEnd.GetString(), CP_UTF8);

        // Write the entry to the file
        historyFile << utf8DelimDateStart << utf8CurrentDate << utf8DelimDateEnd
            << " " << utf8DelimQueryStart << utf8Query << utf8DelimQueryEnd << std::endl;

        // Close the file
        historyFile.close();
    }
    else
    {
        // Handle the case where the file couldn't be opened
        // For example, you can log an error message or take appropriate action.
    }
}


//if query text from rich edit
void CQueryTab::ExecuteQueryMainDlg()
{
    CMainDlg* pParentDialog = nullptr;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CMainDlg*)pTabCtrl->GetParent();
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
    GetDlgItem(IDC_RICH_SQL)->GetWindowTextW(sqlText);
    sqlText = RemoveSQLComments(sqlText);

    CStringW delimiter = L";";
    int pos = 0;
    CStringW statement;

    // Check if there is a delimiter in the SQL text
    if ((pos = sqlText.Find(delimiter)) != -1)
    {
        do
        {
            statement = sqlText.Left(pos);
            sqlText = sqlText.Mid(pos + delimiter.GetLength());

            statement = RemoveSQLComments(statement);

            sql::SQLString query = CStringToSQLString(statement);

            auto start = std::chrono::high_resolution_clock::now();
            m_resultSet = db->ExecuteQuery(query, errorString);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double>(end - start);
            double timeTaken = duration.count();
            CString timeTakenStr;

            if (m_resultSet)
            {
                AddQueryToHistoryFile(statement);
                size_t rowsCount = m_resultSet->rowsCount();
                timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

                SendMessageToConsole(timeTakenStr, GREEN);
                // Send the exact same message to resultTab edit
                ((CMainDlg*)(this->GetParent()->GetParent()))->m_resultTab.SendMessageToQueryInfo(timeTakenStr, GREEN);
                start = std::chrono::high_resolution_clock::now();
                pParentDialog->m_resultTab.BuildResultList(m_resultSet, 0);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration<double>(end - start);
                timeTaken = duration.count();

                if (rowsCount == 0)
                {
                    pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
                }
                else
                {
                    pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
                }
                ((CMainDlg*)(this->GetParent()->GetParent()))->SwitchTabByName(L"Result");
                ((CMainDlg*)(this->GetParent()->GetParent()))->OnTcnSelchangeMaintab(nullptr, nullptr);
            }
            else
            {
                SendMessageToConsole(errorString, RED);
            }

            // Check if there is another delimiter in the remaining SQL text
            pos = sqlText.Find(delimiter);
        } while (pos != -1);
    }
    else
    {
        // If no delimiter is found, treat the entire input as a single query
        statement = RemoveSQLComments(sqlText);
        sql::SQLString query = CStringToSQLString(statement);

        auto start = std::chrono::high_resolution_clock::now();
        m_resultSet = db->ExecuteQuery(query, errorString);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start);
        double timeTaken = duration.count();
        CString timeTakenStr;

        if (m_resultSet)
        {
            AddQueryToHistoryFile(statement);
            size_t rowsCount = m_resultSet->rowsCount();
            timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

            SendMessageToConsole(timeTakenStr, GREEN);
            // Send the exact same message to resultTab edit
            ((CMainDlg*)(this->GetParent()->GetParent()))->m_resultTab.SendMessageToQueryInfo(timeTakenStr, GREEN);
            start = std::chrono::high_resolution_clock::now();
            pParentDialog->m_resultTab.BuildResultList(m_resultSet, 0);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration<double>(end - start);
            timeTaken = duration.count();

            if (rowsCount == 0)
            {
                pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
            }
            else
            {
                pParentDialog->m_resultTab.GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
            }
            ((CMainDlg*)(this->GetParent()->GetParent()))->SwitchTabByName(L"Result");
            ((CMainDlg*)(this->GetParent()->GetParent()))->OnTcnSelchangeMaintab(nullptr, nullptr);
        }
        else
        {
            SendMessageToConsole(errorString, RED);
        }
    }
}



//if query from string
void CQueryTab::ExecuteQueryMainDlg(sql::SQLString queryText)
{
    CMainDlg* pParentDialog = nullptr;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CMainDlg*)pTabCtrl->GetParent();
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
        size_t rowsCount = m_resultSet->rowsCount();
        timeTakenStr.Format(_T("%d total, Query took: %.4f seconds"), rowsCount, timeTaken);

        SendMessageToConsole(timeTakenStr, GREEN);
        ((CMainDlg*)(this->GetParent()->GetParent()))->m_resultTab.SendMessageToQueryInfo(timeTakenStr, GREEN);
        start = std::chrono::high_resolution_clock::now();
        pParentDialog->m_resultTab.BuildResultList(m_resultSet, 0);
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
        ((CMainDlg*)(this->GetParent()->GetParent()))->SwitchTabByName(L"Result");
        ((CMainDlg*)(this->GetParent()->GetParent()))->OnTcnSelchangeMaintab(nullptr, nullptr);
    }
    else
    {
        SendMessageToConsole(errorString, RED);
    }
    //delete resultSet;
}


void CQueryTab::OnBnClickedBtnGo()
{
    //((CMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(75);
    ExecuteQueryMainDlg();
    //((CMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(100);
    //((CMainDlg*)(this->GetParent()->GetParent()))->SetProgressBarPosition(0);
}


void CQueryTab::OnBnClickedBtnClear()
{
    CRichEditCtrl* pEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    pEdit->SetWindowTextW(L"");
    UpdateStringCounter();
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
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);

    CString currentQuery;
    queryText->GetWindowTextW(currentQuery);

    CString formattedQuery = FormatSQLQuery(currentQuery);
    queryText->SetWindowTextW(formattedQuery);
}


void CQueryTab::OnBnClickedBtnSelectall()
{
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown = nullptr;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent(); 

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }

    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();

    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    OnBnClickedBtnClear();
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
    UpdateStringCounter();
} 

void CQueryTab::OnBnClickedBtnnSelect()
{
    
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown = nullptr;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    OnBnClickedBtnClear();
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
    UpdateStringCounter();
}


void CQueryTab::OnBnClickedBtnInsert()
{
    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown = nullptr;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    OnBnClickedBtnClear();
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
    UpdateStringCounter();
}


void CQueryTab::OnBnClickedBtnUpdaterecord()
{

    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown = nullptr;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }
    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    OnBnClickedBtnClear();
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
    UpdateStringCounter();
}


void CQueryTab::OnBnClickedBtnDeleterecord()
{

    CWnd* pTabCtrl = GetParent();
    CComboBox* dbDropdown = nullptr;
    if (pTabCtrl) {
        CWnd* pParentDialog = pTabCtrl->GetParent();

        if (pParentDialog) {
            dbDropdown = (CComboBox*)pParentDialog->GetDlgItem(IDC_CMB_SEL_DB);
        }
    }

    CRichEditCtrl* queryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CComboBox* tablesDropdown = (CComboBox*)GetDlgItem(IDC_SEL_TABLE);

    int selectedDBNumber = dbDropdown->GetCurSel();
    int selectedTableNumber = tablesDropdown->GetCurSel();
    if (selectedDBNumber == CB_ERR || selectedTableNumber == CB_ERR)
    {
        SendMessageToConsole(L"Select table first!", RED);
        return;
    }
    OnBnClickedBtnClear();
    CString table;
    CString database;

    dbDropdown->GetLBText(selectedDBNumber, database);
    tablesDropdown->GetLBText(selectedTableNumber, table);
    queryText->SetWindowTextW(L"DELETE FROM " + database + "." + table + " WHERE 0");
    UpdateStringCounter();
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
    CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);

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


void CQueryTab::OnLbnDblclkListColumns()
{
    OnBnClickedBtnForward();
}


void CQueryTab::UpdateStringCounter()
{
    // Get pointers to the string counter and query text controls
    CRichEditCtrl* pStringCounter = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    CRichEditCtrl* pQueryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);

    // Get the number of lines in the query text
    int lineCount = pQueryText->GetLineCount();

    // Calculate the maximum line number length
    int maxLineNumberLength = static_cast<int>(log10(lineCount) + 1);

    // Prepare the formatted line numbers
    CString strLineCount;
    CString margin = L" ";
    CString newContent;

    for (int i = 1; i <= lineCount; ++i)
    {
        // Format the line number with padding
        strLineCount.Format(L"%*d%s\r\n", maxLineNumberLength, i, margin);

        // Append the formatted line number to the new content
        newContent += strLineCount;
    }

    // Set the updated content to the string counter
    pStringCounter->SetWindowTextW(newContent);
}



void CQueryTab::OnEnChangeEditQuery()
{
    UpdateStringCounter();
    OnEnVscrollEditQuery();
    //SynchronizeFontProperties();
}


void CQueryTab::OnEnVscrollEditQuery()
{
    CRichEditCtrl* pStringCounter = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    CRichEditCtrl* pQueryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    int nPos = pQueryText->GetFirstVisibleLine();
    pStringCounter->LineScroll(nPos - pStringCounter->GetFirstVisibleLine());

    // Get the character format of the SQL edit control
    CHARFORMAT2 sqlCharFormat;
    sqlCharFormat.cbSize = sizeof(CHARFORMAT2);
    pQueryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    pQueryText->GetDefaultCharFormat(sqlCharFormat);

    // Apply the character format to the line numberer control
    pStringCounter = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    pStringCounter->SetDefaultCharFormat(sqlCharFormat);
}

void CQueryTab::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CEdit* pStringCounter = (CEdit*)GetDlgItem(IDC_RICH_LINENUMBERER);
    CEdit* pQueryText = (CEdit*)GetDlgItem(IDC_RICH_SQL);

    // Check if the event is from the scrollbar of the QueryText control
    if (pScrollBar->GetDlgCtrlID() == IDC_RICH_SQL)
    {
        // Get the current scroll position of the QueryText control
        int nPosQueryText = pQueryText->GetFirstVisibleLine();

        // Scroll the StringCounter to match the QueryText
        pStringCounter->LineScroll(nPosQueryText - pStringCounter->GetFirstVisibleLine());
    }

    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CQueryTab::OnEnVscrollStringcounter()
{
    CEdit* pStringCounter = (CEdit*)GetDlgItem(IDC_RICH_LINENUMBERER);
    CEdit* pQueryText = (CEdit*)GetDlgItem(IDC_RICH_SQL);
    int nPos = pStringCounter->GetFirstVisibleLine();
    pQueryText->LineScroll(nPos - pQueryText->GetFirstVisibleLine());
}


void CQueryTab::OnEnChangeRichSql()
{
    UpdateStringCounter();
    OnEnVscrollRichSql();
    //SynchronizeFontProperties();
}


void CQueryTab::OnEnVscrollRichSql()
{
    CRichEditCtrl* pStringNumberer = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    CRichEditCtrl* pQueryText = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    int nPos = pQueryText->GetFirstVisibleLine();
    pStringNumberer->LineScroll(nPos - pStringNumberer->GetFirstVisibleLine());
}

void CQueryTab::PopulateFontSizesDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_FONT_SIZE));
    if (!pComboBox)
        return;

    for (int fontSize = 8; fontSize <= 20; fontSize += 2)
    {
        CString strSize;
        strSize.Format(L"%d", fontSize);
        pComboBox->AddString(strSize);
    }
}

void CQueryTab::SynchronizeFontProperties()
{
    // Get the font size, font type, and color from one of the controls
    auto pRichEditSQL = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    if (!pRichEditSQL)
    {
        return;
    }

    // Store information about the current selection
    long start, end;
    pRichEditSQL->GetSel(start, end);

    CHARFORMAT2 cf;
    cf.cbSize = sizeof(cf);
    pRichEditSQL->GetSelectionCharFormat(cf);

    // Apply the font properties to the other control (line numberer)
    auto pRichEditString = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    if (!pRichEditString)
    {
        return;
    }

    // Set font size, font type, and color for both controls
    pRichEditSQL->SetSel(0, -1);
    pRichEditSQL->SetSelectionCharFormat(cf);
    pRichEditSQL->SetSel(start, end);  // Restore the previous selection

    pRichEditString->SetSel(0, -1);
    pRichEditString->SetSelectionCharFormat(cf);
    pRichEditString->SetSel(start, end);  // Restore the previous selection
}



void CQueryTab::OnCbnSelchangeFontSize()
{
    auto pComboFontSize = GetDlgItem(IDC_FONT_SIZE);
    auto pComboFontCombo = GetDlgItem(IDC_FONTCOMBO);
    CString strFontType;
    CString strSize;

    pComboFontSize->GetWindowTextW(strSize);
    pComboFontCombo->GetWindowTextW(strFontType);
    int nSize = _ttoi(strSize);

    ApplyFontSize(nSize);
    ApplyFontType(strFontType);
    SynchronizeFontProperties();
}


void CQueryTab::OnCbnSelchangeFontcombo()
{
    auto pComboFontSize = GetDlgItem(IDC_FONT_SIZE);
    auto pComboFontCombo = GetDlgItem(IDC_FONTCOMBO);
    CString strFontType;
    CString strSize;

    pComboFontSize->GetWindowTextW(strSize);
    pComboFontCombo->GetWindowTextW(strFontType);
    int nSize = _ttoi(strSize);

    ApplyFontSize(nSize);
    ApplyFontType(strFontType);
    SynchronizeFontProperties();
}

void CQueryTab::OnBnClickedColorFont()
{
    auto pColorPicker = (CMFCColorButton*)GetDlgItem(IDC_COLOR_FONT);
    if (pColorPicker)
    {
        auto color = pColorPicker->GetColor();
        ApplyFontColor(color);
    }
}

// Functions to apply font properties to the rich edit control
void CQueryTab::ApplyFontSize(int nSize)
{
    auto pRichEditSQL = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    auto pRichEditString = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    if (!pRichEditSQL || !pRichEditString)
    {
        return;
    }
    CHARFORMAT2 cf;
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_SIZE;
    cf.yHeight = nSize * 20;  // Convert to twips (1/1440 inch)

    pRichEditSQL->SetSel(0, -1);  // Set the selection to the entire text
    pRichEditSQL->SetSelectionCharFormat(cf);

    pRichEditString->SetSel(0, -1);  // Set the selection to the entire text
    pRichEditString->SetSelectionCharFormat(cf);
    
}

void CQueryTab::ApplyFontType(const CString& strFontType)
{
    auto pRichEditSQL = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    auto pRichEditString = (CRichEditCtrl*)GetDlgItem(IDC_RICH_LINENUMBERER);
    if (!pRichEditSQL || !pRichEditString)
    {
        return;
    }
    CHARFORMAT2 cf;
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_FACE;
    _tcscpy_s(cf.szFaceName, LF_FACESIZE, strFontType);

    pRichEditSQL->SetSel(0, -1);  // Set the selection to the entire text
    pRichEditSQL->SetSelectionCharFormat(cf);

    pRichEditString->SetSel(0, -1);  // Set the selection to the entire text
    pRichEditString->SetSelectionCharFormat(cf);

}

void CQueryTab::ApplyFontColor(COLORREF color)
{
    auto pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_SQL);
    CHARFORMAT2 cf;
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = RGB(GetBValue(color), GetGValue(color), GetRValue(color));

    pRichEdit->SetSel(0, -1);  // Set the selection to the entire text
    pRichEdit->SetSelectionCharFormat(cf);
}



void CQueryTab::OnBnClickedBtnHistory()
{
    CQueryHistoryDlg historyDlg;
    historyDlg.DoModal();
    CString value = historyDlg.GetSelectedQueryFromHistory();
    SetSQLEditText(value);

}
