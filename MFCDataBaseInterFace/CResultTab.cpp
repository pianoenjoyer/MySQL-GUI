// CResultTab.cpp : implementation file
//
#pragma once
#include "pch.h"
#include "afxdialogex.h"
#include "CResultTab.h"
#include "resource.h"
#include "Convertions.h"
#include "CMainDlg.h"
#include "SharedFunctions.h"
// CResultTab dialog

IMPLEMENT_DYNAMIC(CResultTab, CDialogEx)



CResultTab::CResultTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESULT, pParent)
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

    m_AllItems.clear();  // Clear the existing state

    int columnCount = pList->GetHeaderCtrl()->GetItemCount();

    // Loop through all items in the CListCtrl
    for (int i = 0; i < pList->GetItemCount(); ++i) {
        ListItem item;
        item.mainItem = pList->GetItemText(i, 0);

        // Save subitems (columns) for the current item
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


// CResultTab message handlers

int CResultTab::BuildResultList(sql::ResultSet* resultSet, int offset) {
    // Ensure resultSet is valid
    if (resultSet == nullptr) return 0;
    
    CComboBox* dropdown = (CComboBox*)GetDlgItem(IDC_COMBO_NMB_OF_ROWS);
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

    // Insert columns
    for (int i = 1; i <= columnCount; i++) {
        columnName = metaData->getColumnName(i).c_str();
        pList->InsertColumn(i - 1, columnName, LVCFMT_LEFT, 100);
    }

    // Populate rows
    int populatedRows = 0;
    resultSet->absolute(offset);

    pList->SetRedraw(FALSE);  // Prevent redraw during data insertion

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
    // Adjust column widths
    for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); i++) {
        pList->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }

    pList->SetRedraw(TRUE);  // Re-enable drawing


    // Handle pagination
    CString strMaxPages;
    auto rowsCount = resultSet->rowsCount();
    if (limit == 0) {
        strMaxPages.Format(_T("%d"), 1);
    }
    else {
        auto maxPages = (rowsCount + limit - 1) / limit;  // Efficient way to compute ceiling of rowsCount/limit
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
    // Retrieve the text from the edit control
    CString searchText;
    GetDlgItem(IDC_LIST_SEARCH)->GetWindowText(searchText);
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_QUERY);

    // First, restore the list from m_AllItems
    pList->DeleteAllItems();
    for (const auto& listItem : m_AllItems) {
        int index = pList->InsertItem(0, listItem.mainItem);
        for (size_t i = 0; i < listItem.subItems.size(); ++i) {
            pList->SetItemText(index, i + 1, listItem.subItems[i]);
        }
    }

    // Now, apply the search on the restored list
    int columnCount = pList->GetHeaderCtrl()->GetItemCount();

    // Loop through the items in the list control
    for (int i = pList->GetItemCount() - 1; i >= 0; --i) {

        bool matchFound = false;

        // Iterate through each column for the current row
        for (int j = 0; j < columnCount; ++j) {
            CString itemText = pList->GetItemText(i, j);

            // If the item contains the search text, flag it as a match and break the loop
            if (itemText.Find(searchText) != -1) {
                matchFound = true;
                break;
            }
        }

        // If no match is found in any column for the current item, remove it
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
        //pEdit->SetWindowTextW(L"0");
        return;
    }

    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);
    if (pageNumberStr == L"")
    {
        pEdit->SetWindowTextW(L"1");
        return; // Return here after setting the page to 1 to avoid further calculations in this call.
    }
    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);

    // Ensure pageNumber is at least 1
    if (pageNumber < 1)
    {
        pageNumber = 1;
        pEdit->SetWindowTextW(L"1");
    }

    // Get limit from dropdown
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

    int offset = (pageNumber - 1) * limit; // Fixed offset calculation
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
        return; // If it's already the first page, just return.
    }
    std::wstring wstr(pageNumberStr);
    int pageNumber = std::stoi(wstr);
    if (pageNumber > 1)  // Ensure we don't go to negative or zero page numbers
    {
        pageNumber--;
        pageNumberStr.Format(_T("%d"), pageNumber);
        pEdit->SetWindowTextW(pageNumberStr);  // Set updated page number
    }
}

void CResultTab::OnBnClickedBtnNextpage()
{
    auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CURRENTPAGE); // Cast to CEdit* for clarity

    CStringW pageNumberStr;
    pEdit->GetWindowTextW(pageNumberStr);

    // If the current page number string is empty, default it to "1" and return.
    if (pageNumberStr.IsEmpty())  // Use IsEmpty() method instead of == L""
    {
        pEdit->SetWindowTextW(L"1");
        return;
    }

    // Get the maximum page number from the IDC_STAT_MAXPAGE control
    CStringW maxPageCStr;
    auto pMaxPageCtrl = (CStatic*)GetDlgItem(IDC_STAT_MAXPAGE); // Cast to CStatic* for clarity
    pMaxPageCtrl->GetWindowTextW(maxPageCStr);

    // Convert page numbers from CStringW to integers
    int currentPage = _wtoi(pageNumberStr);  // Use _wtoi for a simpler string to int conversion
    int maxPage = _wtoi(maxPageCStr);

    // Check if we're already on the last page
    if (currentPage >= maxPage)
    {
        // Optionally: notify the user or handle this case differently
        return;
    }

    // Increment the page number and update the control
    currentPage++;
    pageNumberStr.Format(_T("%d"), currentPage);
    pEdit->SetWindowTextW(pageNumberStr);
}


void CResultTab::BeginPageState() 
{
    CStringW maxpage;
    CStringW curpage;
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->GetWindowTextW(curpage);
    GetDlgItem(IDC_STAT_MAXPAGE)->GetWindowTextW(maxpage);
    if (maxpage == L"0")
    {
        GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"0");
    }
    else
    {
        GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
    }
}


void CResultTab::SendMessageToQueryInfo(CString msg, COLORREF color)
{
    CRichEditCtrl* p_richEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICH_QUERYINFO);
    p_richEdit->SetWindowTextW(L"");
    CTime currentTime = CTime::GetCurrentTime();
    // Format 
    CString timeStr = currentTime.Format(_T("%H:%M:%S"));
    // Adding timestamp
    CString fullMsg = timeStr + _T(" - ") + msg + _T("\r\n");
    // Append the text with a specific color
    AppendTextToRichEdit(*p_richEdit, fullMsg, color);
}

void CResultTab::OnBnClickedCheckShowbinary()
{
    RebuildResultList();
}
