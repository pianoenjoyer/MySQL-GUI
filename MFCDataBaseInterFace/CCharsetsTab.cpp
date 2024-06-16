// CCharsetsTab.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CCharsetsTab.h"
#include "resource.h"
#include "Convertions.h"

IMPLEMENT_DYNAMIC(CCharsetsTab, CDialogEx)

CCharsetsTab::CCharsetsTab(CWnd* pParent)
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
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_CHARSETS);
    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Character Set"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Default Collation"), LVCFMT_LEFT, 200);
    }
    pListCtrl->DeleteAllItems();
    std::unique_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW CHARACTER SET"));
    while (resultSet->next())
    {
        CString characterSet = SQLStringToCString(resultSet->getString("Charset"));
        CString defaultCollation = SQLStringToCString(resultSet->getString("Default collation"));
        AddCharacterSetToList(pListCtrl, characterSet, defaultCollation);
    }

}

void CCharsetsTab::AddCharacterSetToList(CListCtrl* pListCtrl, const CString& characterSet, const CString& defaultCollation)
{
    int nIndex = pListCtrl->GetItemCount();
    pListCtrl->InsertItem(nIndex, characterSet);
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
