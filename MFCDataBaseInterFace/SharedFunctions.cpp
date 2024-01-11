#include "pch.h"
#include "SharedFunctions.h"
#include "CDBConnection.h"

void AppendTextToRichEdit(CRichEditCtrl& ctrl, const CString& text, COLORREF color)
{
    // Save the current selection
    CHARRANGE saveCharRange;
    ctrl.GetSel(saveCharRange);

    // Move the caret to the end of text
    long lTextLength = ctrl.GetTextLength();
    ctrl.SetSel(lTextLength, lTextLength);

    // Set the color
    CHARFORMAT cf = { 0 };
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = color;
    ctrl.SetSelectionCharFormat(cf);

    // Append the text
    ctrl.ReplaceSel(text);

    // Restore the previous selection
    ctrl.SetSel(saveCharRange);
    // Scroll to the end so the latest text is visible //awesome
    ctrl.SendMessage(EM_SCROLL, SB_PAGEDOWN, 0);
}

bool PopulateCharacterSetDropdown(CComboBox* pComboBox, CDBConnection* db)
{
    if (!pComboBox || !db)
    {
        return false;
    }

    auto resultSet = db->ExecuteQuery("SHOW CHARACTER SET;");
    if (resultSet)
    {
        while (resultSet->next())
        {
            std::string charsetName = resultSet->getString("Charset"); 
            pComboBox->AddString(CString(charsetName.c_str())); 
        }
        delete resultSet;  
    }
    return true;
}


bool SetTotalNum(CListCtrl* pList, CWnd* pStatic)
{
    if (!pList)
    {
        return false;
    }
    int totalItems = pList->GetItemCount();
    CString totalItemsStr;
    totalItemsStr.Format(_T("%d"), totalItems);
    pStatic->SetWindowTextW(totalItemsStr);
    return true;
}

bool FindElemByNameInComboBox(CComboBox* pComboBox, const CString& collation)
{
    int itemCount = pComboBox->GetCount();
    for (int i = 0; i < itemCount; ++i) {
        CString itemText;
        pComboBox->GetLBText(i, itemText);
        if (itemText.CompareNoCase(collation) == 0) {
            pComboBox->SetCurSel(i);
            return true;
        }
    }
    return false;
}

void AdjustColumnWidths(CListCtrl* pListCtrl)
{
    if (!pListCtrl)
    {
        #ifdef DEBUG
                OutputDebugString(L"PopulateVariablesList()  pListCtrl is null");
        #endif
        return;
    }
    // Iterate through columns
    for (int i = 0; i < pListCtrl->GetHeaderCtrl()->GetItemCount(); i++) {
        int maxColWidth = 0;

        // Iterate through items in the column
        for (int j = 0; j < pListCtrl->GetItemCount(); j++) {
            CString itemText = pListCtrl->GetItemText(j, i);
            int itemWidth = pListCtrl->GetStringWidth(itemText);
            maxColWidth = max(maxColWidth, itemWidth);
        }

        // Ensure the column width is not less than the width of the column header
        HDITEM hdi;
        hdi.mask = HDI_TEXT;
        hdi.pszText = new TCHAR[256]; // Allocate enough space for the column header
        hdi.cchTextMax = 256;
        if (pListCtrl->GetHeaderCtrl()->GetItem(i, &hdi)) {
            CString colHeaderText = hdi.pszText;
            int colHeaderWidth = pListCtrl->GetStringWidth(colHeaderText);
            maxColWidth = max(maxColWidth, colHeaderWidth);
        }
        delete[] hdi.pszText;

        // Set the column width
        pListCtrl->SetColumnWidth(i, maxColWidth);
    }
}