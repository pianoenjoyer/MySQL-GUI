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

void PopulateCharacterSetDropdown(CComboBox* pComboBox, CDBConnection* db)
{
    if (!pComboBox || !db)
        return;

    auto resultSet = db->ExecuteQuery("SHOW CHARACTER SET;");
    if (resultSet)
    {
        while (resultSet->next())
        {
            std::string charsetName = resultSet->getString("Charset"); // Get the character set name
            pComboBox->AddString(CString(charsetName.c_str())); // Add the character set to the combo box
        }
        delete resultSet;  
    }
}


void SetTotalNum(CListCtrl* pList, CWnd* pStatic)
{
    int totalItems = pList->GetItemCount();
    CString totalItemsStr;
    totalItemsStr.Format(_T("%d"), totalItems);
    pStatic->SetWindowText(totalItemsStr);
}