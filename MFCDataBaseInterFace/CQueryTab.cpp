// CQueryTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CQueryTab.h"
#include "resource.h"
#include <chrono>
#include "CDBMainDlg.h"
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

}

CQueryTab::~CQueryTab()
{
}

void CQueryTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEL_TABLE, m_comboTables);
}


BEGIN_MESSAGE_MAP(CQueryTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GO, &CQueryTab::OnBnClickedBtnGo)
END_MESSAGE_MAP()


//sent msg to output
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


void CQueryTab::ExecuteQueryMainDlg()
{
    sql::ResultSet* m_resultSet;

    CDBMainDlg* pParent = dynamic_cast<CDBMainDlg*>(GetParent());
    if (pParent) {
        m_resultSet = pParent->GetResultSet();
        if (m_resultSet != nullptr)
        {
            //delete pParent->m_resultSet;
        }
    }
    else {
        // Handle the case where the parent is not valid.
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
        //FillListControl(resultSet);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(end - start);
        timeTaken = duration.count();
        //timeTakenStr.Format(_T("Built list took: %.2f seconds"), timeTaken);
        SendMessageToConsole(timeTakenStr, BLACK);
        pParent->SetResultSet(resultSet);
    }
    else
    {
        SendMessageToConsole(errorString, RED);
    }
  
    //GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
    //delete resultSet;
}


void CQueryTab::OnBnClickedBtnGo()
{
	ExecuteQueryMainDlg();
}
