// CHomeTab.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CHomeTab.h"
#include "resource.h"
#include "Convertions.h"
#include "CChangeUserPasswordDlg.h"
#include "SharedFunctions.h"
#include "Colors.h"

IMPLEMENT_DYNAMIC(CHomeTab, CDialogEx)

CHomeTab::CHomeTab(CWnd* pParent)
  : CDialogEx(IDD_HOME, pParent)
{

}

CHomeTab::~CHomeTab()
{

}

void CHomeTab::PopulateGeneralInfo()
{
  CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SERVER_INFO);
  if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0)
  {
    pListCtrl->InsertColumn(0, _T("Property"), LVCFMT_LEFT, 120);
    pListCtrl->InsertColumn(1, _T("Value"), LVCFMT_LEFT, 2000);
  }
  pListCtrl->DeleteAllItems();


  sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version'");

  if (!resultSet)
  {
    return;
  }

  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Server Version"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version_comment'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Server Comment"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version_compile_os'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Server OS"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'protocol_version'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Protocol Version"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'character_set_server'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Server Charset"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'max_connections'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Max Connections"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW STATUS LIKE 'Threads_connected'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Threads Connected"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'hostname'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Hostname"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SELECT SUBSTRING_INDEX(host, ':', 1) AS ServerIP FROM information_schema.processlist WHERE ID = connection_id()");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Server IP"), SQLStringToCString(resultSet->getString("ServerIP")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'port'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Port"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'datadir'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("Data Directory"), SQLStringToCString(resultSet->getString("Value")));
  }

  resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'innodb_version'");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("InnoDB Version"), SQLStringToCString(resultSet->getString("Value")));
  }



  resultSet = db->ExecuteQuery("SELECT USER() AS UserHost");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("UserHost"), SQLStringToCString(resultSet->getString("UserHost")));
  }

  resultSet = db->ExecuteQuery("SELECT CURRENT_USER() AS CurrentUser");
  while (resultSet->next()) {
    AddGeneralInfoItem(pListCtrl, _T("CurrentUser"), SQLStringToCString(resultSet->getString("CurrentUser")));
  }


  AdjustColumnWidths(pListCtrl);
  delete resultSet;
}


bool CHomeTab::AddGeneralInfoItem(CListCtrl* pListCtrl, LPCTSTR lpszProperty, LPCTSTR lpszValue)
{
  int nItemIndex = pListCtrl->GetItemCount();
  if (pListCtrl)
  {
    if (!pListCtrl->InsertItem(nItemIndex, lpszProperty)) return false;
    if (!pListCtrl->SetItemText(nItemIndex, 1, lpszValue)) return false;
    if (!pListCtrl->SetColumnWidth(0, LVSCW_AUTOSIZE)) return false;
    if (!pListCtrl->SetColumnWidth(1, LVSCW_AUTOSIZE)) return false;
  }
  return true;
}

bool CHomeTab::PopulateConnectionCollationDropdown()
{
  CComboBox* pCollationCombo = (CComboBox*)GetDlgItem(IDC_CONNCOLL);

  if (!pCollationCombo)
  {
    return false;
  }
  pCollationCombo->ResetContent();

  try
  {
    auto ResultSet = db->ExecuteQuery("SHOW COLLATION");
    if (!ResultSet)
    {
      return false;
    }
    while (ResultSet->next())
    {
      CString collationName = SQLStringToCString(ResultSet->getString("Collation"));
      pCollationCombo->AddString(collationName);
    }
  }
  catch (sql::SQLException& e)
  {
    AfxMessageBox(CString("SQL Error: ") + e.what());
  }
  return true;
}

void CHomeTab::PopulateEnginesList()
{
  CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_ENGINES);

  if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
    pListCtrl->InsertColumn(0, _T("Engine"), LVCFMT_LEFT, 200);
    pListCtrl->InsertColumn(1, _T("Comment"), LVCFMT_LEFT, 2000);
  }
  pListCtrl->DeleteAllItems();

  auto resultSet = db->ExecuteQuery("SHOW ENGINES;");
  if (!resultSet)
  {
    return;
  }

  while (resultSet->next()) {
    CString engine = SQLStringToCString(resultSet->getString("Engine"));
    CString comment = SQLStringToCString(resultSet->getString("Comment"));
    AddEngineInfoToList(pListCtrl, engine, comment);
  }
  AdjustColumnWidths(pListCtrl);
  delete resultSet;
}


void CHomeTab::AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment)
{
  int nIndex = pListCtrl->GetItemCount();
  pListCtrl->InsertItem(nIndex, engine);
  pListCtrl->SetItemText(nIndex, 1, comment);
  pListCtrl->SetColumnWidth(0, LVSCW_AUTOSIZE);
  pListCtrl->SetColumnWidth(1, LVSCW_AUTOSIZE);
}


void CHomeTab::PopulatePluginsList()
{
  CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PLUGINS);

  if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
    pListCtrl->InsertColumn(0, _T("Plugin"), LVCFMT_LEFT, 200);
    pListCtrl->InsertColumn(1, _T("Status"), LVCFMT_LEFT, 200);
    pListCtrl->InsertColumn(2, _T("Type"), LVCFMT_LEFT, 200);
    pListCtrl->InsertColumn(4, _T("License"), LVCFMT_LEFT, 200);
  }

  pListCtrl->DeleteAllItems();

  auto resultSet = db->ExecuteQuery("SHOW PLUGINS;");
  if (!resultSet)
  {
    return;
  }

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


void CHomeTab::AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license)
{
  int nIndex = pListCtrl->GetItemCount();
  pListCtrl->InsertItem(nIndex, name);
  pListCtrl->SetItemText(nIndex, 1, status);
  pListCtrl->SetItemText(nIndex, 2, type);
  pListCtrl->SetItemText(nIndex, 3, license);

  pListCtrl->SetColumnWidth(1, LVSCW_AUTOSIZE);
  pListCtrl->SetColumnWidth(2, LVSCW_AUTOSIZE);
  pListCtrl->SetColumnWidth(3, LVSCW_AUTOSIZE);
}

bool SetImage(CStatic* pPicCtrl, CString path)
{
  CImage image;
  if (SUCCEEDED(image.Load(path)))
  {
    HBITMAP hBmp = image.Detach();
    pPicCtrl->SetBitmap(hBmp);
    return true;
  }
  else
  {
    return false;
  }

}

CString CHomeTab::GetCurrentConnectionCollation()
{
  auto resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'collation_connection';");
  if (!resultSet)
  {
    return {};
  }
  if (resultSet->next()) {
    return SQLStringToCString(resultSet->getString("Value"));
  }

  return _T("Unknown");
}

BOOL CHomeTab::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  PopulateGeneralInfo();
  PopulatePluginsList();
  PopulateEnginesList();
  PopulateConnectionCollationDropdown();

  CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_MySQLGUI);
  SetImage(pPicCtrl, L".\\Pictures\\MySQLHOME.png");



  CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CONNCOLL);
  if (pCombo)
  {
    CString curConnColl = GetCurrentConnectionCollation();
    if (curConnColl != L"Unknow")
    {
      FindElemByNameInComboBox(pCombo, curConnColl);
    }
  }
  return TRUE;
}


void CHomeTab::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomeTab, CDialogEx)
  ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CHomeTab::OnNMClickSyslink1)
  ON_CBN_SELCHANGE(IDC_CONNCOLL, &CHomeTab::OnCbnSelchangeConncoll)
END_MESSAGE_MAP()


void CHomeTab::OnNMClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult)
{
  CChangeUserPasswordDlg dialog(nullptr, this->db);
  dialog.DoModal();
  *pResult = 0;
}

bool CHomeTab::SetConnectionCollation(const CString& collation)
{
  CString query = _T("SET collation_connection = '") + collation + _T("';");
  CString error;
  bool result = db->ExecuteNonQuery(CStringToSQLString(query));
  if (result)
  {
    AfxMessageBox(_T("Collation changed to " + collation));
    return true;
  }
  else
  {
    AfxMessageBox(_T("Failed to set connection collation to " + collation));
    return false;
  }
}

void CHomeTab::OnCbnSelchangeConncoll()
{
  CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CONNCOLL);
  if (pCombo) {
    int selectedIndex = pCombo->GetCurSel();
    if (selectedIndex != CB_ERR) {
      CString selectedCollation;
      pCombo->GetLBText(selectedIndex, selectedCollation);
      if (!SetConnectionCollation(selectedCollation))
      {
#ifdef DEBUG
        OutputDebugString(L"SetConnectionCollation error\n");
#endif
#ifdef DEBUG_AFXMESSAGE
        AfxMessageBox(L"SetConnectionCollation error\n");
#endif
      }
    }
  }
  else
  {
#ifdef DEBUG
    OutputDebugString(L"pCombo is null\n");
#endif
  }
}
