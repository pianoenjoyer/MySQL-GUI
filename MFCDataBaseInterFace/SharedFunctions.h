#pragma once
#include "pch.h"
#include "CDBConnection.h"
void AppendTextToRichEdit(CRichEditCtrl& ctrl, const CString& text, COLORREF color);

void AdjustColumnWidths(CListCtrl* pListCtrl);

void PopulateCharacterSetDropdown(CComboBox* pComboBox, CDBConnection* db);

bool FindElemByNameInComboBox(CComboBox* pComboBox, const CString& collation);

void SetTotalNum(CListCtrl* pList, CWnd* pStatic);
