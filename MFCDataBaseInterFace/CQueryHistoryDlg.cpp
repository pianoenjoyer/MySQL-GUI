// CQueryHistoryDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CQueryHistoryDlg.h"
#include "resource.h"

// CQueryHistoryDlg dialog

IMPLEMENT_DYNAMIC(CQueryHistoryDlg, CDialogEx)

CQueryHistoryDlg::CQueryHistoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HYSTORY, pParent)
{

}

CQueryHistoryDlg::~CQueryHistoryDlg()
{
}

void CQueryHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

#include <fstream>

void CQueryHistoryDlg::LoadHistoryToListCtrl(CListCtrl* pListHistory)
{
    if (!pListHistory)
    {
        return;
    }

    CStringW fileName = L"QueryHistory.txt";
    CStringW delimDateStart = L"|;Q|";
    CStringW delimDateEnd = L"|Q;|";
    CStringW delimQueryStart = L"|;D|";
    CStringW delimQueryEnd = L"|D;|";

    std::wifstream inputFile(fileName);
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

                // Insert into the list control
                pListHistory->InsertItem(itemIndex, date);
                pListHistory->SetItemText(itemIndex, 1, query);

                // Move to the next row
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
    pListHistory->InsertColumn(0, _T("Date"), LVCFMT_LEFT, 200);
    pListHistory->InsertColumn(1, _T("Query"), LVCFMT_LEFT, 1000);
    LoadHistoryToListCtrl(pListHistory);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CQueryHistoryDlg, CDialogEx)
END_MESSAGE_MAP()


// CQueryHistoryDlg message handlers
