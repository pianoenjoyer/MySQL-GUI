// CCharsetsTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CCharsetsTab.h"
#include "resource.h"
#include "Convertions.h"
// CCharsetsTab dialog

IMPLEMENT_DYNAMIC(CCharsetsTab, CDialogEx)

CCharsetsTab::CCharsetsTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARSETS, pParent)
{

}

CCharsetsTab::~CCharsetsTab()
{
}

void CCharsetsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharsetsTab, CDialogEx)
END_MESSAGE_MAP()

BOOL CCharsetsTab::OnInitDialog()
{

    CDialogEx::OnInitDialog();
    PopulateCharacterSetsList();

    CStatic* pStaticControl = (CStatic*)GetDlgItem(IDC_TAB_TITLE);
    if (pStaticControl)
    {
        // Set font size and make it bold
        CFont font;
        font.CreateFontW(
            500,                        // nHeight
            50,                         // nWidth
            0,                         // nEscapement
            0,                         // nOrientation
            FW_BOLD,                 // nWeight
            FALSE,                     // bItalic
            FALSE,                     // bUnderline
            0,                         // cStrikeOut
            ANSI_CHARSET,              // nCharSet
            OUT_DEFAULT_PRECIS,        // nOutPrecision
            CLIP_DEFAULT_PRECIS,       // nClipPrecision
            DEFAULT_QUALITY,           // nQuality
            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
            _T("Times New Roman"));                 // lpszFacename
        pStaticControl->SetFont(&font);
    }

    return TRUE;
}


void CCharsetsTab::PopulateCharacterSetsList()
{
    // Assuming m_listCtrl is the ID of your list control
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_CHARSETS);

    // Add columns if not already added
    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Character Set"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Default Collation"), LVCFMT_LEFT, 200);
    }

    // Clear existing items
    pListCtrl->DeleteAllItems();

    // Assuming db is an instance of your database class
    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW CHARACTER SET");

    // Process the result set
    while (resultSet->next())
    {
        CString characterSet = SQLStringToCString(resultSet->getString("Charset"));
        CString defaultCollation = SQLStringToCString(resultSet->getString("Default collation"));
        AddCharacterSetToList(pListCtrl, characterSet, defaultCollation);
    }

    // Don't forget to close the result set
    delete resultSet;
}

void CCharsetsTab::AddCharacterSetToList(CListCtrl* pListCtrl, const CString& characterSet, const CString& defaultCollation)
{
    int nIndex = pListCtrl->GetItemCount();

    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, characterSet);

    // Set text for the second column
    pListCtrl->SetItemText(nIndex, 1, defaultCollation);
}
