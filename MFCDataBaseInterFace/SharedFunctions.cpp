#include "pch.h"

#include "SharedFunctions.h"
#include "CDBConnection.h"
#include "framework.h"


void AppendTextToRichEdit(CRichEditCtrl& ctrl, const CString& text, COLORREF color)
{
  CHARRANGE saveCharRange;
  ctrl.GetSel(saveCharRange);
  long lTextLength = ctrl.GetTextLength();
  ctrl.SetSel(lTextLength, lTextLength);
  CHARFORMAT cf = { 0 };
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = color;
  ctrl.SetSelectionCharFormat(cf);
  ctrl.ReplaceSel(text);
  ctrl.SetSel(saveCharRange);
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
  for (int i = 0; i < pListCtrl->GetHeaderCtrl()->GetItemCount(); i++) {
    int maxColWidth = 0;
    for (int j = 0; j < pListCtrl->GetItemCount(); j++) {
      CString itemText = pListCtrl->GetItemText(j, i);
      int itemWidth = pListCtrl->GetStringWidth(itemText);
      maxColWidth = max(maxColWidth, itemWidth);
    }
    HDITEM hdi;
    hdi.mask = HDI_TEXT;
    hdi.pszText = new TCHAR[256];
    hdi.cchTextMax = 256;
    if (pListCtrl->GetHeaderCtrl()->GetItem(i, &hdi)) {
      CString colHeaderText = hdi.pszText;
      int colHeaderWidth = pListCtrl->GetStringWidth(colHeaderText);
      maxColWidth = max(maxColWidth, colHeaderWidth);
    }
    delete[] hdi.pszText;
    pListCtrl->SetColumnWidth(i, maxColWidth);
  }
}