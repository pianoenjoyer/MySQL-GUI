// CVariablesTab.cpp : implementation file

#include "pch.h"
#include "CVariablesTab.h"
#include "resource.h"
#include "Convertions.h"
#include "SharedFunctions.h"
#include "CDBConnection.h"

IMPLEMENT_DYNAMIC(CVariablesTab, CDialogEx)

CVariablesTab::CVariablesTab(CWnd* pParent)
  : CDialogEx(IDD_VARIABLES, pParent)
{


}

CVariablesTab::~CVariablesTab()
{
}

void CVariablesTab::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVariablesTab, CDialogEx)
  ON_EN_CHANGE(IDC_EDIT_VARSEARCH, &CVariablesTab::OnEnChangeEditVarsearch)
  ON_BN_CLICKED(IDC_BTN_REFRESH_VAR, &CVariablesTab::OnBnClickedBtnRefreshVar)
  ON_CBN_SELCHANGE(IDC_CMB_VARS, &CVariablesTab::OnCbnSelchangeCmbVars)
  ON_BN_CLICKED(IDC_MFCBUTTON1, &CVariablesTab::OnBnClickedMfcbutton1)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VARS, &CVariablesTab::OnLvnItemchangedListVars)
END_MESSAGE_MAP()

BOOL CVariablesTab::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  PopulateVariablesList();
  PopulateVariablesCombo();
  SetTotalNum((CListCtrl*)GetDlgItem(IDC_LIST_VARS), GetDlgItem(IDC_TOTAL));
  return TRUE;
}

bool CVariablesTab::PopulateVariablesCombo()
{
  auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_VARS);
  if (!pCombo)
  {
#ifdef DEBUG
    OutputDebugString(L"PopulateVariablesCombo() pCombo is null");
#endif
    return false;
  }

  std::shared_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW VARIABLES"));
  if (!resultSet)
  {
    return false;
  }
  while (resultSet->next())
  {
    CString Variable = SQLStringToCString(resultSet->getString("Variable_name"));
    CString Value = SQLStringToCString(resultSet->getString("Value"));
    pCombo->AddString(Variable);
  }
  pCombo->SetCurSel(0);
  OnCbnSelchangeCmbVars();
  return true;

}

bool CVariablesTab::PopulateVariablesList()
{
  CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_VARS);
  if (!pListCtrl)
  {
#ifdef DEBUG
    OutputDebugString(L"PopulateVariablesList()  pListCtrl is null");
#endif
    return false;
  }
  if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
    pListCtrl->InsertColumn(0, _T("Variable"), LVCFMT_LEFT, 200);
    pListCtrl->InsertColumn(1, _T("Value"), LVCFMT_LEFT, 200);
  }

  pListCtrl->DeleteAllItems();

  sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES");
  if (!resultSet)
  {
    return false;
  }
  while (resultSet->next())
  {
    CString Variable = SQLStringToCString(resultSet->getString("Variable_name"));
    CString Value = SQLStringToCString(resultSet->getString("Value"));
    AddVariableToList(pListCtrl, Variable, Value);
  }
  AdjustColumnWidths(pListCtrl);
  delete resultSet;
  return true;
}


void CVariablesTab::AddVariableToList(CListCtrl* pListCtrl, const CString& variable, const CString& value)
{
  int nIndex = pListCtrl->GetItemCount();
  pListCtrl->InsertItem(nIndex, variable);
  pListCtrl->SetItemText(nIndex, 1, value);
}


void CVariablesTab::UpdateListFilter()
{
  CEdit* pEditFilter = (CEdit*)GetDlgItem(IDC_EDIT_VARSEARCH);
  CString filterText;
  pEditFilter->GetWindowText(filterText);


  CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_VARS);
  pListCtrl->DeleteAllItems();

  sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES");
  if (!resultSet)
  {
    return;
  }
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

void CVariablesTab::OnEnChangeEditVarsearch()
{
  UpdateListFilter();
}


void CVariablesTab::OnBnClickedBtnRefreshVar()
{
  PopulateVariablesList();
  PopulateVariablesCombo();
}


void CVariablesTab::OnCbnSelchangeCmbVars()
{
  auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VARVALUE);
  auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_VARS);
  if (!pEdit || !pCombo)
  {
#ifdef DEBUG
    OutputDebugString(L"PopulateVariablesList()  control ptr is null");
#endif
    return;
  }
  CString curVarName;
  int currentIndex = pCombo->GetCurSel();
  if (currentIndex != CB_ERR)
  {
    pCombo->GetLBText(currentIndex, curVarName);
  }

  std::shared_ptr<sql::ResultSet> resultSet(db->ExecuteQuery("SHOW VARIABLES"));
  if (!resultSet)
  {
    return;
  }
  while (resultSet->next())
  {
    CString Variable = SQLStringToCString(resultSet->getString("Variable_name"));
    CString Value = SQLStringToCString(resultSet->getString("Value"));
    if (Variable == curVarName)
    {
      pEdit->SetWindowTextW(Value);
      break;
    }
  }

}

bool IsNumeric(const CString& str)
{
  std::wistringstream iss(str.GetString());
  double d;
  iss >> d;
  return !iss.fail() && iss.eof();
}

void CVariablesTab::OnBnClickedMfcbutton1()
{
  auto pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VARVALUE);
  auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_VARS);
  if (!pEdit || !pCombo)
  {
#ifdef DEBUG
    OutputDebugString(L"PopulateVariablesList()  control ptr is null");
#endif
    return;
  }
  CString curVarName;
  int currentIndex = pCombo->GetCurSel();
  if (currentIndex != CB_ERR)
  {
    pCombo->GetLBText(currentIndex, curVarName);
  }

  CString VariableValue;
  CString QueryText;
  pEdit->GetWindowTextW(VariableValue);
  if (IsNumeric(VariableValue))
  {
    QueryText = L"SET GLOBAL " + curVarName + L" = " + VariableValue;
  }
  else
  {
    QueryText = L"SET GLOBAL " + curVarName + L" = " + L"'" + VariableValue + L"'";
  }

  int result = db->ExecuteNonQuery(CStringToSQLString(QueryText));
  if (result)
  {
    AfxMessageBox(L"Variable " + curVarName + " changed to " + VariableValue);
  }
  else
  {
#ifdef DEBUG
    OutputDebugString(L"Error while var value changing");
#endif
  }
}


void CVariablesTab::OnLvnItemchangedListVars(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  auto pList = (CListCtrl*)GetDlgItem(IDC_LIST_VARS);
  auto pCombo = (CComboBox*)GetDlgItem(IDC_CMB_VARS);
  if (!pList || !pCombo)
  {
#ifdef DEBUG
    OutputDebugString(L"PopulateVariablesList()  control ptr is null");
#endif
    return;
  }
  CString curVarName;
  int currentIndex = pCombo->GetCurSel();
  if (currentIndex != CB_ERR)
  {
    pCombo->GetLBText(currentIndex, curVarName);
  }

  if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
  {
    CString selectedItemText;
    int selectedIndex = pNMLV->iItem;

    if (selectedIndex >= 0)
    {
      selectedItemText = pList->GetItemText(selectedIndex, 0);
    }

    int comboIndex = pCombo->FindStringExact(0, selectedItemText);
    if (comboIndex != CB_ERR)
    {
      pCombo->SetCurSel(comboIndex);
      OnCbnSelchangeCmbVars();
    }
  }

  *pResult = 0;
}
