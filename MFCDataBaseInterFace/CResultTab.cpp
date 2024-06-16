#pragma once
#include "pch.h"
#include "afxdialogex.h"
#include "CResultTab.h"
#include "resource.h"
#include "Convertions.h"
#include "CMainDlg.h"
#include "SharedFunctions.h"

IMPLEMENT_DYNAMIC(CResultTab, CDialogEx)

CResultTab::CResultTab(CWnd* pParent)
	: CDialogEx(IDD_RESULT, pParent)
{
    m_pMainDlg = nullptr;
}

CResultTab::CResultTab(int DIALOG_ID, CWnd* pParent)
    : CDialogEx(DIALOG_ID, pParent)
{
    m_pMainDlg = nullptr;
}

 CResultTab::~CResultTab()
{

}

void CResultTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUERY, m_resultList);
	DDX_Control(pDX, IDC_COMBO_NMB_OF_ROWS, m_comboLimit);
}



void CResultTab::SaveOriginalListState()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);

    m_AllItems.clear();

    int columnCount = pList->GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < pList->GetItemCount(); ++i) {
        ListItem item;
        item.mainItem = pList->GetItemText(i, 0);
        for (int j = 1; j < columnCount; ++j) {
            item.subItems.push_back(pList->GetItemText(i, j));
        }

        m_AllItems.push_back(item);
    }
}

BEGIN_MESSAGE_MAP(CResultTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLROUTPUT, &CResultTab::OnBnClickedBtnClroutput)
	ON_BN_CLICKED(IDC_CHECK_SHOWALL, &CResultTab::OnBnClickedCheckShowall)
	ON_EN_CHANGE(IDC_LIST_SEARCH, &CResultTab::OnEnChangeListSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_NMB_OF_ROWS, &CResultTab::OnCbnSelchangeComboNmbOfRows)
	ON_BN_CLICKED(IDC_BTN_FIRSTPAGE, &CResultTab::OnBnClickedBtnFirstpage)
	ON_BN_CLICKED(IDC_BTN_PREVPAGE, &CResultTab::OnBnClickedBtnPrevpage)
	ON_EN_CHANGE(IDC_EDIT_CURRENTPAGE, &CResultTab::OnEnChangeEditCurrentpage)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, &CResultTab::OnBnClickedBtnNextpage)
	ON_BN_CLICKED(IDC_BTN_LASTPAGE, &CResultTab::OnBnClickedBtnLastpage)
    ON_BN_CLICKED(IDC_CHECK_SHOWBINARY, &CResultTab::OnBnClickedCheckShowbinary)
END_MESSAGE_MAP()

int CResultTab::BuildResultList(sql::ResultSet* resultSet, int offset) {
    if (!resultSet) 
    {
        return false;
    }
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
    if (!dropdown)
    {
        return false;
    }
    int selectedIndex = dropdown->GetCurSel();
    CString dropdownText;
    int limit;

    if (selectedIndex != CB_ERR) {
        dropdown->GetLBText(selectedIndex, dropdownText);
    }

    limit = (dropdownText == L"All") ? 0 : _wtoi(dropdownText);

    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    if (!pList) return 1;

    pList->DeleteAllItems();
    while (pList->DeleteColumn(0));

    sql::ResultSetMetaData* metaData = resultSet->getMetaData();
    int columnCount = metaData->getColumnCount();
    CStringW columnName;
    for (int i = 1; i <= columnCount; i++) {
        columnName = metaData->getColumnName(i).c_str();
        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
    }
    int populatedRows = 0;
    resultSet->absolute(offset);

    pList->SetRedraw(FALSE);

    while (resultSet->next() && (limit == 0 || populatedRows < limit)) {

        CString rowData = SQLStringToCString(resultSet->getString(1));
        int nIndex = pList->InsertItem(populatedRows, rowData);

        for (int i = 2; i <= columnCount; i++) {
            CString colData;
            if (metaData->getColumnType(i) == sql::DataType::BINARY ||
                metaData->getColumnType(i) == sql::DataType::VARBINARY ||
                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) 
            {
                if (((CButton*)GetDlgItem(IDC_CHECK_SHOWBINARY))->GetCheck() == BST_CHECKED)
                {
                    colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
                }
                else 
                {
                    colData = L"<binary data>";
                }
            }
            else {
                colData = SQLStringToCString(resultSet->getString(i));
            }
            pList->SetItemText(nIndex, i - 1, colData);
        }

        populatedRows++;
    }
    SaveOriginalListState();
    for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); i++) {
        pList->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }

    pList->SetRedraw(TRUE);
    CString strMaxPages;
    auto rowsCount = resultSet->rowsCount();
    if (limit == 0) {
        strMaxPages.Format(_T("%d"), 1);
    }
    else {
        auto maxPages = (rowsCount + limit - 1) / limit;
        strMaxPages.Format(_T("%d"), maxPages);
    }
    GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(strMaxPages);

    return 0;
}

int CResultTab::RebuildResultList() 
{
    auto resultSet = m_pMainDlg->m_queryTab.m_resultSet;
    if (resultSet)
    {
        BuildResultList(resultSet, 0);
        return 0;
    }
    return 1;
}
bool CResultTab::FillLimitDropdown()
{
	m_comboLimit.AddString(L"30");
	m_comboLimit.AddString(L"50");
	m_comboLimit.AddString(L"100");
	m_comboLimit.AddString(L"250");
	m_comboLimit.AddString(L"500");
	m_comboLimit.AddString(L"1000");
	m_comboLimit.AddString(L"All");
	m_comboLimit.SetCurSel(1);
	return true;
}

BOOL CResultTab::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    auto pMainTab = (CTabCtrl*)this->GetParent();
    m_pMainDlg = (CMainDlg*)pMainTab->GetParent();

    FillLimitDropdown();
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
    GetDlgItem(IDC_STAT_MAXPAGE)->SetWindowTextW(L"0");
    return TRUE;
}

void CResultTab::OnBnClickedBtnClroutput()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    pList->DeleteAllItems();
    int numCols = pList->GetHeaderCtrl()->GetItemCount();
    for (int col = numCols - 1; col >= 0; col--) {
        pList->DeleteColumn(col);
    }

}


void CResultTab::OnBnClickedCheckShowall()
{
    if (AfxMessageBox(_T("Do you really want to see all of the rows? For a "
        "big table this could a long time "),
        MB_YESNO | MB_ICONQUESTION) == IDYES)
    {


    }
    else
    {

    }
}


void CResultTab::OnEnChangeListSearch()
{
    CString searchText;
    GetDlgItem(IDC_LIST_SEARCH)->GetWindowText(searchText);
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);
    pList->DeleteAllItems();
    for (auto it = m_AllItems.rbegin(); it != m_AllItems.rend(); ++it) {
        const auto& listItem = *it;
        int index = pList->InsertItem(0, listItem.mainItem);
        for (size_t i = 0; i < listItem.subItems.size(); ++i) {
            pList->SetItemText(index, i + 1, listItem.subItems[i]);
        }
    }
    int columnCount = pList->GetHeaderCtrl()->GetItemCount();
    for (int i = pList->GetItemCount() - 1; i >= 0; --i) {

        bool matchFound = false;
        for (int j = 0; j < columnCount; ++j) {
            CString itemText = pList->GetItemText(i, j);
            if (itemText.Find(searchText) != -1) {
                matchFound = true;
                break;
            }
        }
        if (!matchFound) {
            pList->DeleteItem(i);
        }
    }
}


void CResultTab::OnCbnSelchangeComboNmbOfRows()
{
    OnEnChangeEditCurrentpage();
}


void CResultTab::OnEnChangeEditCurrentpage()
{
    auto pEdit = GetDlgItem(IDC_EDIT_CURRENTPAGE);
    auto pStaticMaxPage = GetDlgItem(IDC_STAT_MAXPAGE);
    CString MaxPage;
    pStaticMaxPage->GetWindowTextW(MaxPage);
    sql::ResultSet* m_resultSet = nullptr;
    CMainDlg* pParentDialog;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CMainDlg*)pTabCtrl->GetParent();
        if (pParentDialog) {
            m_resultSet = pParentDialog->m_queryTab.m_resultSet;
        }
    }
    if (m_resultSet == nullptr)
    {
        return;
    }

    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);

    if (pageNumberStr == L"0" && MaxPage == L"0")
    {
        return;
    }
    else if (pageNumberStr == L"")
    {
        pEdit->SetWindowTextW(L"1");
        return;
    }
    else if (pageNumberStr != L"0" && MaxPage == L"0")
    {
        pEdit->SetWindowTextW(L"0");
        return;
    }


    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);
    if (pageNumber < 1)
    {
        pageNumber = 1;
        pEdit->SetWindowTextW(L"1");
    }
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
    int selectedIndex = dropdown->GetCurSel();
    CString dropdownText;
    int limit;

    if (selectedIndex != CB_ERR) {
        dropdown->GetLBText(selectedIndex, dropdownText);
    }
    if (dropdownText == L"All") {
        limit = 0;
    }
    else {
        std::wstring wstr(dropdownText);
        limit = std::stoi(wstr);
    }

    int offset = (pageNumber - 1) * limit;
    BuildResultList(m_resultSet, offset);
}



void CResultTab::OnBnClickedBtnFirstpage()
{
    CStringW maxpage;
    CStringW curpage;
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->GetWindowTextW(curpage);
    GetDlgItem(IDC_STAT_MAXPAGE)->GetWindowTextW(maxpage);
    if (maxpage == L"0" || curpage == L"0" || curpage == L"1")
    {
        return;
    }
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
}


void CResultTab::OnBnClickedBtnLastpage()
{
    CStringW maxpage;
    CStringW curpage;
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->GetWindowTextW(curpage);
    GetDlgItem(IDC_STAT_MAXPAGE)->GetWindowTextW(maxpage);
    if (maxpage == curpage)
    {
        return;
    }
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(maxpage);
}


void CResultTab::OnBnClickedBtnPrevpage()
{
    auto pEdit = GetDlgItem(IDC_EDIT_CURRENTPAGE);
    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);
    if (pageNumberStr == L"")
    {
        pEdit->SetWindowTextW(L"1");
        return;
    }
    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);
    if (pageNumber > 1)
    {
        pageNumber--;
        pageNumberStr.Format(_T("%d"), pageNumber);
        pEdit->SetWindowTextW(pageNumberStr);
    }
}

void CResultTab::OnBnClickedBtnNextpage()
{
    auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CURRENTPAGE);

    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);
    if (pageNumberStr.IsEmpty())
    {
        pEdit->SetWindowTextW(L"1");
        return;
    }
    CStringW maxPageCStr;
    auto pMaxPageCtrl = (CStatic*)GetDlgItem(IDC_STAT_MAXPAGE);
    pMaxPageCtrl->GetWindowTextW(maxPageCStr);
    int currentPage = _wtoi(pageNumberStr);
    int maxPage = _wtoi(maxPageCStr);
    if (currentPage >= maxPage)
    {
        return;
    }
    currentPage++;
    pageNumberStr.Format(_T("%d"), currentPage);
    pEdit->SetWindowTextW(pageNumberStr);
}


void CResultTab::SendMessageToQueryInfo(CString msg, COLORREF color)
{
    CRichEditCtrl* p_richEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_QUERYINFO);
    p_richEdit->SetWindowTextW(L"");
    CTime currentTime = CTime::GetCurrentTime();
    CString timeStr = currentTime.Format(_T("%H:%M:%S"));
    CString fullMsg = timeStr + _T(" - ") + msg + _T("\r\n");
    AppendTextToRichEdit(*p_richEdit, fullMsg, color);
}

void CResultTab::OnBnClickedCheckShowbinary()
{
    RebuildResultList();
}
