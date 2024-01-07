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
    ON_EN_CHANGE(IDC_EDIT_CHARSETSEARCH, &CCharsetsTab::OnEnChangeEditCharsetsearch)
    ON_BN_CLICKED(IDC_BTN_REFRESH_CHARSETS, &CCharsetsTab::OnBnClickedBtnRefreshCharsets)
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
        font.CreateFontW(18, 0, 0, 0,
            FW_DONTCARE,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,
            100,
            VARIABLE_PITCH,
            TEXT("Times New Roman")
        );
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
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW CHARACTER SET"));

    // Process the result set
    while (resultSet->next())
    {
        CString characterSet = SQLStringToCString(resultSet->getString("Charset"));
        CString defaultCollation = SQLStringToCString(resultSet->getString("Default collation"));
        AddCharacterSetToList(pListCtrl, characterSet, defaultCollation);
    }

    // Don't forget to close the result set

}

void CCharsetsTab::AddCharacterSetToList(CListCtrl* pListCtrl, const CString& characterSet, const CString& defaultCollation)
{
    int nIndex = pListCtrl->GetItemCount();

    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, characterSet);

    // Set text for the second column
    pListCtrl->SetItemText(nIndex, 1, defaultCollation);
}


void CCharsetsTab::UpdateListFilter()
{
    CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_EDIT_CHARSETSEARCH);
    CString filterText;
    pEditFilter->GetWindowText(filterText);

    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_CHARSETS);
    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW CHARACTER SET;");

    while (resultSet->next()) {
        CString characterSet = SQLStringToCString(resultSet->getString("Charset"));
        CString defaultCollation = SQLStringToCString(resultSet->getString("Default collation"));

        // Check if the filter text is empty or matches the character set or default collation
        if (filterText.IsEmpty() || characterSet.Find(filterText) != -1 || defaultCollation.Find(filterText) != -1) {
            AddCharacterSetToList(pListCtrl, characterSet, defaultCollation);
        }
    }

    delete resultSet;
}

void CCharsetsTab::OnEnChangeEditCharsetsearch()
{
    UpdateListFilter();
}


void CCharsetsTab::OnBnClickedBtnRefreshCharsets()
{
    PopulateCharacterSetsList();
}
