// CHomeTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CHomeTab.h"
#include "resource.h"
#include "Convertions.h"
// CHomeTab dialog

IMPLEMENT_DYNAMIC(CHomeTab, CDialogEx)

CHomeTab::CHomeTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOME, pParent)
{

}

CHomeTab::~CHomeTab()
{
}

void CHomeTab::PopulateGeneralInfo()
{
    // Assuming m_listCtrl is the ID of your list control
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SERVER_INFO);

    // Add columns if not already added
    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Property"), LVCFMT_LEFT, 120);
        pListCtrl->InsertColumn(1, _T("Value"), LVCFMT_LEFT, 2000);
    }

    pListCtrl->DeleteAllItems();


    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Server Version"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version_comment'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Server Comment"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'version_compile_os'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Server OS"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'protocol_version'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Protocol Version"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'character_set_server'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Server Charset"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'max_connections'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Max Connections"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW STATUS LIKE 'Threads_connected'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Threads Connected"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'hostname'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Hostname"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SELECT SUBSTRING_INDEX(host, ':', 1) AS ServerIP FROM information_schema.processlist WHERE ID = connection_id()");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Server IP"), SQLStringToCString(resultSet->getString("ServerIP")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'port'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Port"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'datadir'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("Data Directory"), SQLStringToCString(resultSet->getString("Value")));
    }

    resultSet = db->ExecuteQuery("SHOW VARIABLES LIKE 'innodb_version'");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("InnoDB Version"), SQLStringToCString(resultSet->getString("Value")));
    }

    //USER INFO



    resultSet = db->ExecuteQuery("SELECT USER() AS UserHost");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("UserHost"), SQLStringToCString(resultSet->getString("UserHost")));
    }

    resultSet = db->ExecuteQuery("SELECT CURRENT_USER() AS CurrentUser");
    while (resultSet->next()) {
        AddServerInfoItem(pListCtrl, _T("CurrentUser"), SQLStringToCString(resultSet->getString("CurrentUser")));
    }



    delete resultSet;
}


void CHomeTab::AddServerInfoItem(CListCtrl* pListCtrl, LPCTSTR lpszProperty, LPCTSTR lpszValue)
{
    int nItemIndex = pListCtrl->GetItemCount();
    pListCtrl->InsertItem(nItemIndex, lpszProperty);
    pListCtrl->SetItemText(nItemIndex, 1, lpszValue);
}


void CHomeTab::PopulateEnginesList()
{
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_ENGINES);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Engine"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Comment"), LVCFMT_LEFT, 2000);  // Corrected column index
    }
    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW ENGINES;");
    while (resultSet->next()) {
        CString engine = SQLStringToCString(resultSet->getString("Engine"));
        CString comment = SQLStringToCString(resultSet->getString("Comment"));
        AddEngineInfoToList(pListCtrl, engine, comment);
    }
    delete resultSet;
}

void CHomeTab::AddEngineInfoToList(CListCtrl* pListCtrl, const CString& engine, const CString& comment)
{
    int nIndex = pListCtrl->GetItemCount();
    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, engine);
    pListCtrl->SetItemText(nIndex, 1, comment);  // Corrected column index
}




void CHomeTab::PopulatePluginsList()
{
    CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PLUGINS);

    if (pListCtrl->GetItemCount() == 0 && pListCtrl->GetHeaderCtrl()->GetItemCount() == 0) {
        pListCtrl->InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(1, _T("Status"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(2, _T("Type"), LVCFMT_LEFT, 200);
        pListCtrl->InsertColumn(4, _T("License"), LVCFMT_LEFT, 200);
    }

    pListCtrl->DeleteAllItems();

    auto resultSet = db->ExecuteQuery("SHOW PLUGINS;");
    while (resultSet->next()) {
        CString name = SQLStringToCString(resultSet->getString("Name"));
        CString status = SQLStringToCString(resultSet->getString("Status"));
        CString type = SQLStringToCString(resultSet->getString("Type"));
        CString library = SQLStringToCString(resultSet->getString("Library"));
        CString license = SQLStringToCString(resultSet->getString("License"));
        AddPluginInfoToList(pListCtrl, name, status, type, license);
    }
    delete resultSet;
}

void CHomeTab::AddPluginInfoToList(CListCtrl* pListCtrl, const CString& name, const CString& status, const CString& type, const CString& license)
{
    int nIndex = pListCtrl->GetItemCount();

    // InsertItem for the first column and set its text
    pListCtrl->InsertItem(nIndex, name);

    // Set text for the remaining columns
    pListCtrl->SetItemText(nIndex, 1, status);
    pListCtrl->SetItemText(nIndex, 2, type);
    pListCtrl->SetItemText(nIndex, 3, license);
}



BOOL CHomeTab::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	AfxInitRichEdit2();
    PopulateGeneralInfo();
    PopulatePluginsList();
    PopulateEnginesList();

    CImage image;
    if (SUCCEEDED(image.Load(L".\\Pictures\\MySQLHOME.jpg"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
    {
        CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_MySQLGUI);
        HBITMAP hBmp = image.Detach();
        pPicCtrl->SetBitmap(hBmp);
    }

	return TRUE;
}


void CHomeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHomeTab, CDialogEx)
END_MESSAGE_MAP()


// CHomeTab message handlers
