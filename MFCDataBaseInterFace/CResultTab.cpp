// CResultTab.cpp : implementation file
//
#pragma once
#include "pch.h"
#include "afxdialogex.h"
#include "CResultTab.h"
#include "resource.h"
#include "Convertions.h"
#include "CDBMainDlg.h"
// CResultTab dialog

IMPLEMENT_DYNAMIC(CResultTab, CDialogEx)

CResultTab::CResultTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESULT, pParent)
{

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
END_MESSAGE_MAP()


// CResultTab message handlers

int CResultTab::FillListControl(sql::ResultSet* resultSet, int offset) {
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
                metaData->getColumnType(i) == sql::DataType::LONGVARBINARY) {
                colData = BinaryDataToHexString(SQLStringToCString(resultSet->getString(i)));
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

void CResultTab::OnBnClickedBtnClroutput()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnBnClickedCheckShowall()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnEnChangeListSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CResultTab::OnCbnSelchangeComboNmbOfRows()
{
	// TODO: Add your control notification handler code here
}


void CResultTab::OnEnChangeEditCurrentpage()
{
    auto pEdit = GetDlgItem(IDC_EDIT_CURRENTPAGE);
    sql::ResultSet* m_resultSet;
    CDBMainDlg* pParentDialog;
    CWnd* pTabCtrl = GetParent();
    if (pTabCtrl) {
        pParentDialog = (CDBMainDlg*)pTabCtrl->GetParent();
        if (pParentDialog) {
            m_resultSet = pParentDialog->GetResultSet();
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
    FillListControl(m_resultSet, offset);
}



void CResultTab::OnBnClickedBtnFirstpage()
{
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(L"1");
}


void CResultTab::OnBnClickedBtnLastpage()
{
    CStringW title;
    GetDlgItem(IDC_STAT_MAXPAGE)->GetWindowTextW(title);
    GetDlgItem(IDC_EDIT_CURRENTPAGE)->SetWindowTextW(title);
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