#pragma once

#include "framework.h"

class CDBConnection;

void AppendTextToRichEdit(CRichEditCtrl& ctrl, const CString& text, COLORREF color);

void AdjustColumnWidths(CListCtrl* pListCtrl);

bool PopulateCharacterSetDropdown(CComboBox* pComboBox, CDBConnection* db);

bool FindElemByNameInComboBox(CComboBox* pComboBox, const CString& collation);

bool SetTotalNum(CListCtrl* pList, CWnd* pStatic);
