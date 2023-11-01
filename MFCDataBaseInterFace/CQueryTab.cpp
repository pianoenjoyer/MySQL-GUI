// CQueryTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CQueryTab.h"
#include "resource.h"
#include <chrono>
#include "CDBMainDlg.h"
// CQueryTab dialog

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

void CQueryTab::ExecuteQueryMainDlg(CStringW sqlText)
{
    sql::ResultSet* m_resultSet = ((CDBMainDlg*)GetParent())->GetResultSet();

    if (m_resultSet != nullptr)
    {
        delete m_resultSet;
    }

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


void CQueryTab::OnBnClickedBtnGo()
{
	ExecuteQueryMainDlg();
}
