// CPluginsTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CPluginsTab.h"
#include "resource.h"
#include "SharedFunctions.h"
#include "Convertions.h"


IMPLEMENT_DYNAMIC(CPluginsTab, CDialogEx)

CPluginsTab::CPluginsTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLUGINS, pParent)
{

}

CPluginsTab::~CPluginsTab()
{
}

void CPluginsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPluginsTab, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_PLUGINSEARCH, &CPluginsTab::OnEnChangeEditPluginsearch)
    ON_BN_CLICKED(IDC_BTN_REFRESH_PLUGINS, &CPluginsTab::OnBnClickedBtnRefreshPlugins)
END_MESSAGE_MAP()


BOOL CPluginsTab::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    PopulatePluginsList();
    return TRUE;
}

void CPluginsTab::PopulatePluginsList()
{
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PLUGINSTAB);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Plugin"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Status"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(2, _T("Type"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(4, _T("License"), LVCFMT_LEFT, 200);
    }

    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW PLUGINS;");
    while (resultSet->next()) {
        CString name = SQLStringToCString(resultSet->getString("Name"));
        CString status = SQLStringToCString(resultSet->getString("Status"));
        CString type = SQLStringToCString(resultSet->getString("Type"));
        CString library = SQLStringToCString(resultSet->getString("Library"));
        CString license = SQLStringToCString(resultSet->getString("License"));
        AddPluginInfoToList(pListCtrl, name, status, type, license);
    }
    AdjustColumnWidths(pListCtrl);
    delete resultSet;
}


void CPluginsTab::AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license)
{
    int nIndex = pListCtrl->GetItemCount();


    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, name);

    // Set text for the remaining columns
    pListCtrl->SetItemText(nIndex, 1, status);
    pListCtrl->SetItemText(nIndex, 2, type);
    pListCtrl->SetItemText(nIndex, 3, license);

    pListCtrl->SetColumnWidth(1, LVSCW_AUTOSIZE);
    pListCtrl->SetColumnWidth(2, LVSCW_AUTOSIZE);
    pListCtrl->SetColumnWidth(3, LVSCW_AUTOSIZE);
}


void CPluginsTab::UpdateListFilter()
{
    CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_EDIT_PLUGINSEARCH);
    CString filterText;
    pEditFilter->GetWindowText(filterText);


    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PLUGINSTAB);
    pListCtrl->DeleteAllItems();

    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES");

    while (resultSet->next())
    {
        CString variable = SQLStringToCString(resultSet->getString("Variable_name"));
        CString value = SQLStringToCString(resultSet->getString("Value"));
        if (variable.Find(filterText) != -1 || value.Find(filterText) != -1)
        {
            int nIndex = pListCtrl->GetItemCount();
            pListCtrl->InsertItem(nIndex, variable);
            pListCtrl->SetItemText(nIndex, 1, value);
        }
    }
    delete resultSet;
}


void CPluginsTab::OnEnChangeEditPluginsearch()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    UpdateListFilter();
}


void CPluginsTab::OnBnClickedBtnRefreshPlugins()
{
    UpdateListFilter();
}
