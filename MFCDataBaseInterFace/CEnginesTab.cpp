// CEnginesTab.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CEnginesTab.h"
#include "resource.h"
#include "SharedFunctions.h"
#include "Convertions.h"

IMPLEMENT_DYNAMIC(CEnginesTab, CDialogEx)

CEnginesTab::CEnginesTab(CWnd* pParent)
	: CDialogEx(IDD_ENGINES, pParent)
{

}

CEnginesTab::~CEnginesTab()
{
}

void CEnginesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEnginesTab, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_ENGINESSEARCH, &CEnginesTab::OnEnChangeEditEnginessearch)
    ON_BN_CLICKED(IDC_BTN_REFRESH_ENGINES, &CEnginesTab::OnBnClickedBtnRefreshEngines)
END_MESSAGE_MAP()


BOOL CEnginesTab::OnInitDialog()
{
    PopulateEnginesList();
    CDialogEx::OnInitDialog();
    return TRUE;
}

void CEnginesTab::PopulateEnginesList()
{
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_ENGINESTAB);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Engine"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Comment"), LVCFMT_LEFT, 2000);
    }
    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW ENGINES;");
    while (resultSet->next()) {
        CString engine = SQLStringToCString(resultSet->getString("Engine"));
        CString comment = SQLStringToCString(resultSet->getString("Comment"));
        AddEngineInfoToList(pListCtrl, engine, comment);
    }
    AdjustColumnWidths(pListCtrl);
    delete resultSet;
}


void CEnginesTab::AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment)
{
    int nIndex = pListCtrl->GetItemCount();
    pListCtrl->InsertItem(nIndex, engine);
    pListCtrl->SetItemText(nIndex, 1, comment);
    pListCtrl->SetColumnWidth(0, LVSCW_AUTOSIZE);
    pListCtrl->SetColumnWidth(1, LVSCW_AUTOSIZE);
}

void CEnginesTab::UpdateListFilter()
{
    CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_EDIT_ENGINESSEARCH);
    CString filterText;
    pEditFilter->GetWindowText(filterText);

    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_ENGINESTAB);
    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW ENGINES;");

    while (resultSet->next()) {
        CString engine = SQLStringToCString(resultSet->getString("Engine"));
        CString comment = SQLStringToCString(resultSet->getString("Comment"));
        if (filterText.IsEmpty() || engine.Find(filterText) != -1 || comment.Find(filterText) != -1) {
            AddEngineInfoToList(pListCtrl, engine, comment);
        }
    }

    delete resultSet;
}

void CEnginesTab::OnEnChangeEditEnginessearch()
{
    UpdateListFilter();
}


void CEnginesTab::OnBnClickedBtnRefreshEngines()
{
    UpdateListFilter();
}
