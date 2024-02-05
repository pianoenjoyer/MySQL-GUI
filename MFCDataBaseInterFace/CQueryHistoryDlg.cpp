// CQueryHistoryDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CQueryHistoryDlg.h"
#include "resource.h"
#include "CQueryTab.h"
// CQueryHistoryDlg dialog


IMPLEMENT_DYNAMIC(CQueryHistoryDlg, CDialogEx)

CQueryHistoryDlg::CQueryHistoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HYSTORY, pParent)
{
    m_filename = L"QueryHistory.txt";
}

CQueryHistoryDlg::~CQueryHistoryDlg()
{
}

void CQueryHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

#include <fstream>

void CQueryHistoryDlg::LoadHistoryToListCtrl()
{
    CListCtrl* pListHistory = (CListCtrl*)GetDlgItem(IDC_LIST_HYSTORY);
    if (!pListHistory)
    {
        return;
    }
    pListHistory->DeleteAllItems();

    CStringW delimDateStart = L"|;Q|";
    CStringW delimDateEnd = L"|Q;|";
    CStringW delimQueryStart = L"|;D|";
    CStringW delimQueryEnd = L"|D;|";

    std::wifstream inputFile(m_filename);
    if (inputFile.is_open())
    {
        std::wstring line;
        int itemIndex = 0;

        while (std::getline(inputFile, line))
        {
            size_t dateStart = line.find(delimDateStart);
            size_t dateEnd = line.find(delimDateEnd, dateStart + delimDateStart.GetLength());
            size_t queryStart = line.find(delimQueryStart, dateEnd + delimDateEnd.GetLength());
            size_t queryEnd = line.find(delimQueryEnd, queryStart + delimQueryStart.GetLength());

            if (dateStart != std::wstring::npos && dateEnd != std::wstring::npos && queryStart != std::wstring::npos && queryEnd != std::wstring::npos)
            {
                CStringW date = line.substr(dateStart + delimDateStart.GetLength(), dateEnd - (dateStart + delimDateStart.GetLength())).c_str();
                CStringW query = line.substr(queryStart + delimQueryStart.GetLength(), queryEnd - (queryStart + delimQueryStart.GetLength())).c_str();

                pListHistory->InsertItem(itemIndex, date);
                pListHistory->SetItemText(itemIndex, 1, query);

                itemIndex++;
            }
        }

        inputFile.close();
    }
    else
    {
        // Handle the case where the file couldn't be opened
        // For example, you can log an error message or take appropriate action.
    }
}




BOOL CQueryHistoryDlg::OnInitDialog() 
{
    CListCtrl* pListHistory = (CListCtrl*)GetDlgItem(IDC_LIST_HYSTORY);
    pListHistory->InsertColumn(0, _T("Date"), LVCFMT_LEFT, 100);
    pListHistory->InsertColumn(1, _T("Query"), LVCFMT_LEFT, 1000);
    LoadHistoryToListCtrl();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CQueryHistoryDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SEL_HIST_REC, &CQueryHistoryDlg::OnBnClickedBtnSelHistRec)
    ON_BN_CLICKED(IDC_BTN_OPN_HIST_FILE, &CQueryHistoryDlg::OnBnClickedBtnOpnHistFile)
    ON_BN_CLICKED(IDC_BTN_CLR_HIST, &CQueryHistoryDlg::OnBnClickedBtnClrHist)
END_MESSAGE_MAP()


// CQueryHistoryDlg message handlers

void CQueryHistoryDlg::OnBnClickedBtnSelHistRec()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_HYSTORY);
    if (!pList)
    {
        return;
    }

    auto pParent = (CQueryTab*)this->GetParent();
    if (!pParent)
    {
        return;
    }

    int selectedIndex = pList->GetSelectionMark();
    if (selectedIndex == -1)
    {
        return;
    }

    m_selectedQueryFromHistory = pList->GetItemText(selectedIndex, 1);
    this->EndDialog(IDOK);

}


void CQueryHistoryDlg::OnBnClickedBtnOpnHistFile()
{
    HINSTANCE result = ShellExecute(nullptr, L"open", m_filename, nullptr, nullptr, SW_SHOWNORMAL);

    if ((intptr_t)result <= 32)
    {
        AfxMessageBox(L"Failed to open the file.", MB_OK | MB_ICONERROR);
    }
}


void CQueryHistoryDlg::OnBnClickedBtnClrHist()
{
    int result = AfxMessageBox(L"Are you sure you want to clear the history file?", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES)
    {
       std::wofstream historyFile(m_filename, std::ios::trunc);
        if (historyFile.is_open())
        {
            historyFile.close();
            AfxMessageBox(L"The history file has been cleared.");
            LoadHistoryToListCtrl();
        }
    }
    else
    {

    }
}
