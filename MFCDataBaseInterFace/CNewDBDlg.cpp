// CNewDBDlg.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "CNewDBDlg.h"
#include "resource.h"
#include "CMainDlg.h"

IMPLEMENT_DYNAMIC(CNewDBDlg, CDialogEx)

CNewDBDlg::CNewDBDlg(CWnd* pParent)
	: CDialogEx(IDD_NEWDB, pParent)
{

}

CNewDBDlg::~CNewDBDlg()
{
}

void CNewDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CNewDBDlg::OnInitDialog()
{
	this->SetWindowTextW(L"Create new database...");
    PopulateCharacterSetDropdown();

	return TRUE;
};

BEGIN_MESSAGE_MAP(CNewDBDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATEDB, &CNewDBDlg::OnBnClickedBtnCreatedb)
END_MESSAGE_MAP()

void CNewDBDlg::PopulateCharacterSetDropdown()
{
    CComboBox* pComboBox = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CHARSET));
    if (!pComboBox || !db)
        return;

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
}
void CNewDBDlg::OnBnClickedBtnCreatedb()
{
    CString databaseName, charsetName;

    CComboBox* pComboBox = (CComboBox*)(GetDlgItem(IDC_COMBO_CHARSET));
    int selIndex = pComboBox->GetCurSel();
    if (selIndex != CB_ERR)
    {
        pComboBox->GetLBText(selIndex, charsetName);
    }

    GetDlgItem(IDC_EDIT_DBNAME)->GetWindowTextW(databaseName);

    if (!databaseName.IsEmpty() && !charsetName.IsEmpty())
    {
        CString query;
        query.Format(_T("CREATE DATABASE `%s` CHARACTER SET %s;"), databaseName, charsetName);
        int result = db->ExecuteNonQuery(CStringToSQLString(query));
        if (result)
        {
            AfxMessageBox(L"Database created.");
        }
        else
        {
            AfxMessageBox(L"Something went wrong.");
        }
        this->EndDialog(IDOK);

    }
    else
    {
        AfxMessageBox(L"Fields are empty");
    }
}
