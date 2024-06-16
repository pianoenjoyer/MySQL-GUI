// CDBExportDlg.cpp : implementation file

#include "pch.h"
#include "CDBInterfaceApp.h"
#include "afxdialogex.h"
#include "CExportDlg.h"
#include <fstream>
#include <atlstr.h>
#include "CMainDlg.h"

IMPLEMENT_DYNAMIC(CDBExportDlg, CDialogEx)

CString strCSVdescription("A simple file format for storing tabular data such as"
    " a spreadsheet or database. CSV files use a comma to"
    " separate values. Each line in the file corresponds"
    " to a row in the table.");

CString strXMLdescription("A markup language defining rules for encoding documents"
    " in a format that's human-readable and machine-readable."
    " Primarily used for data sharing across systems.");

CString strHTMLdescription("The standard markup language for web browser display."
    " HTML describes web page structure and pairs with"
    " technologies like CSS and JavaScript for interactive content.");

CString strTSVdescription("Similar to CSV, TSV stores tabular data. Instead of commas,"
    " TSV files use tabs to separate values. Each line represents"
    " a table row.");

CString strTextdescription("A plain text file contains only readable content without"
    " formatting. It can be opened by any text editor. Often used"
    " for config files or simple note-taking.");



CDBExportDlg::CDBExportDlg(CWnd* pParent)
	: CDialogEx(IDD_EXPORT, pParent)
{

}

CDBExportDlg::~CDBExportDlg()
{

}

void CDBExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBExportDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BROWSE, &CDBExportDlg::OnBnClickedBtnBrowse)
    ON_BN_CLICKED(IDC_RAD_CSV, &CDBExportDlg::OnBnClickedRadCsv)
    ON_BN_CLICKED(IDC_RAD_XML, &CDBExportDlg::OnBnClickedRadXml)
    ON_BN_CLICKED(IDC_RAD_TSV, &CDBExportDlg::OnBnClickedRadTsv)
    ON_BN_CLICKED(IDC_RAD_TEXT, &CDBExportDlg::OnBnClickedRadText)
    ON_BN_CLICKED(IDC_RAD_HTML, &CDBExportDlg::OnBnClickedRadHtml)
END_MESSAGE_MAP()
BOOL CDBExportDlg::OnInitDialog()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    CButton* pRadio = (CButton*)GetDlgItem(IDC_RAD_CSV);
    pRadio->SetCheck(BST_CHECKED);
    OnBnClickedRadCsv();
    this->SetWindowTextW(_T("Export as..."));
    return TRUE;
}

bool ExportListCtrlToText(CListCtrl& listCtrl, const CString& filePath)
{
    CStdioFile file;
    if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
        return false;
    }
    CHeaderCtrl* header = listCtrl.GetHeaderCtrl();
    int columnCount = header->GetItemCount();

    CString headerLine = L"";
    for (int col = 0; col < columnCount; ++col) {
        CString headerText;
        HDITEM hdi = { 0 };
        hdi.mask = HDI_TEXT;
        hdi.pszText = headerText.GetBuffer(100);
        hdi.cchTextMax = 100;
        header->GetItem(col, &hdi);
        headerText.ReleaseBuffer();

        headerLine += headerText;
        if (col < columnCount - 1) headerLine += L"\t";
    }
    file.WriteString(headerLine + L"\n");
    int itemCount = listCtrl.GetItemCount();
    for (int item = 0; item < itemCount; ++item) {
        CString line = L"";
        for (int subItem = 0; subItem < columnCount; ++subItem) {
            CString cellText = listCtrl.GetItemText(item, subItem);
            line += cellText;
            if (subItem < columnCount - 1) line += L"\t";
        }
        file.WriteString(line + L"\n");
    }

    file.Close();
    return true;
}


bool ExportListCtrlToXML(CListCtrl& listCtrl, const CString& filePath)
{
    CW2A utf8FilePath(filePath, CP_UTF8);
    std::ofstream outFile(utf8FilePath.m_psz);
    if (!outFile.is_open())
    {
        return false;
    }

    outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    outFile << "<ListData>\n";
    CHeaderCtrl* header = listCtrl.GetHeaderCtrl();
    int columnCount = header ? header->GetItemCount() : 0;
    int itemCount = listCtrl.GetItemCount();
    for (int item = 0; item < itemCount; ++item)
    {
        outFile << "  <Row>\n";
        for (int subItem = 0; subItem < columnCount; ++subItem)
        {
            CString cellText = listCtrl.GetItemText(item, subItem);
            CString headerText;
            HDITEM hdi = { 0 };
            hdi.mask = HDI_TEXT;
            hdi.pszText = headerText.GetBuffer(100);
            hdi.cchTextMax = 100;
            header->GetItem(subItem, &hdi);
            headerText.ReleaseBuffer();
            CW2A utf8HeaderText(headerText, CP_UTF8);
            CW2A utf8CellText(cellText, CP_UTF8);

            outFile << "    <" << utf8HeaderText.m_psz << ">"
                << utf8CellText.m_psz
                << "</" << utf8HeaderText.m_psz << ">\n";
        }
        outFile << "  </Row>\n";
    }

    outFile << "</ListData>\n";
    outFile.close();

    return true;
}


bool ExportListCtrlToCSV(CListCtrl& listCtrl, const CString& filePath)
{
    CFile outFile;
    if (!outFile.Open(filePath, CFile::modeCreate | CFile::modeWrite))
    {
        return false;
    }
    WORD bom = 0xFEFF;
    outFile.Write(&bom, sizeof(bom));
    CHeaderCtrl* header = listCtrl.GetHeaderCtrl();
    int columnCount = header->GetItemCount();
    CString delimeter = L",";
    for (int col = 0; col < columnCount; ++col)
    {
        CString headerText;
        HDITEM hdi = { 0 };
        hdi.mask = HDI_TEXT;
        hdi.pszText = headerText.GetBuffer(100);
        hdi.cchTextMax = 100;
        header->GetItem(col, &hdi);
        headerText.ReleaseBuffer();

        outFile.Write(L"\"", sizeof(WCHAR));
        outFile.Write(headerText, headerText.GetLength() * sizeof(WCHAR));
        outFile.Write(L"\"", sizeof(WCHAR));

        if (col < columnCount - 1)
        {
            outFile.Write(delimeter, sizeof(WCHAR));
        }
    }
    outFile.Write(L"\r\n", 2 * sizeof(WCHAR));
    int itemCount = listCtrl.GetItemCount();
    for (int item = 0; item < itemCount; ++item)
    {
        for (int subItem = 0; subItem < columnCount; ++subItem)
        {
            CString cellText = listCtrl.GetItemText(item, subItem);

            outFile.Write(L"\"", sizeof(WCHAR));
            outFile.Write(cellText, cellText.GetLength() * sizeof(WCHAR));
            outFile.Write(L"\"", sizeof(WCHAR));

            if (subItem < columnCount - 1)
            {
                outFile.Write(delimeter, sizeof(WCHAR));
            }
        }
        outFile.Write(L"\r\n", 2 * sizeof(WCHAR));
    }

    outFile.Close();
    return true;
}


bool ExportListCtrlToHTML(CListCtrl& listCtrl, const CString& filePath) {
    std::ofstream outFile(CT2A(filePath).m_psz);
    if (!outFile.is_open()) {
        return false;
    }
    outFile << "<!DOCTYPE html>\n";
    outFile << "<html>\n";
    outFile << "<head>\n";
    outFile << "<title>Data Export</title>\n";
    outFile << "</head>\n";
    outFile << "<body>\n";

    outFile << "<table border=\"1\">\n";
    CHeaderCtrl* header = listCtrl.GetHeaderCtrl();
    int columnCount = header ? header->GetItemCount() : 0;
    outFile << "  <tr>\n";
    for (int col = 0; col < columnCount; ++col) {
        CString headerText;
        HDITEM hdi = { 0 };
        hdi.mask = HDI_TEXT;
        hdi.pszText = headerText.GetBuffer(100);
        hdi.cchTextMax = 100;
        header->GetItem(col, &hdi);
        headerText.ReleaseBuffer();

        outFile << "    <th>" << CT2A(headerText.GetString()).m_psz << "</th>\n";
    }
    outFile << "  </tr>\n";
    int itemCount = listCtrl.GetItemCount();
    for (int item = 0; item < itemCount; ++item) {
        outFile << "  <tr>\n";
        for (int subItem = 0; subItem < columnCount; ++subItem) {
            CString cellText = listCtrl.GetItemText(item, subItem);
            outFile << "    <td>" << CT2A(cellText.GetString()).m_psz << "</td>\n";
        }
        outFile << "  </tr>\n";
    }

    outFile << "</table>\n";
    outFile << "</body>\n";
    outFile << "</html>\n";

    outFile.close();
    return true;
}


bool ExportListCtrlToTSV(CListCtrl& listCtrl, const CString& filePath) {
    std::ofstream outFile(CT2A(filePath).m_psz);
    if (!outFile.is_open()) {
        return false;
    }
    CHeaderCtrl* header = listCtrl.GetHeaderCtrl();
    int columnCount = header ? header->GetItemCount() : 0;
    for (int col = 0; col < columnCount; ++col) {
        CString headerText;
        HDITEM hdi = { 0 };
        hdi.mask = HDI_TEXT;
        hdi.pszText = headerText.GetBuffer(100);
        hdi.cchTextMax = 100;
        header->GetItem(col, &hdi);
        headerText.ReleaseBuffer();

        outFile << CT2A(headerText.GetString()).m_psz;
        if (col < columnCount - 1) outFile << "\t";
    }
    outFile << "\n";
    int itemCount = listCtrl.GetItemCount();
    for (int item = 0; item < itemCount; ++item) {
        for (int subItem = 0; subItem < columnCount; ++subItem) {
            CString cellText = listCtrl.GetItemText(item, subItem);
            outFile << CT2A(cellText.GetString()).m_psz;
            if (subItem < columnCount - 1) outFile << "\t";
        }
        outFile << "\n";
    }

    outFile.close();
    return true;
}


void CDBExportDlg::OnBnClickedBtnBrowse()
{
    typedef bool (*ExportFunc)(CListCtrl&, const CString&);

    CString filter;
    CString defaultExt;
    ExportFunc exportFunc = nullptr;
    auto pTab = this->GetParent();
    CMainDlg* mainDlg = (CMainDlg*)pTab->GetParent();
    m_pList = (CListCtrl*)mainDlg->m_resultTab.GetDlgItem(IDC_LIST_QUERY);
    if (((CButton*)GetDlgItem(IDC_RAD_CSV))->GetCheck() == BST_CHECKED)
    {
        filter = L"CSV files (*.csv)|*.csv|All files (*.*)|*.*||";
        defaultExt = L"csv";
        exportFunc = &ExportListCtrlToCSV;
    }
    else if (((CButton*)GetDlgItem(IDC_RAD_XML))->GetCheck() == BST_CHECKED)
    {
        filter = L"XML files (*.xml)|*.xml|All files (*.*)|*.*||";
        defaultExt = L"xml";
        exportFunc = &ExportListCtrlToXML;
    }
    else if (((CButton*)GetDlgItem(IDC_RAD_HTML))->GetCheck() == BST_CHECKED)
    {
        filter = L"HTML files (*.html)|*.html|All files (*.*)|*.*||";
        defaultExt = L"html";
        exportFunc = &ExportListCtrlToHTML;
    }
    else if (((CButton*)GetDlgItem(IDC_RAD_TSV))->GetCheck() == BST_CHECKED)
    {
        filter = L"TSV files (*.tsv)|*.tsv|All files (*.*)|*.*||";
        defaultExt = L"tsv";
        exportFunc = &ExportListCtrlToTSV;
    }
    else if (((CButton*)GetDlgItem(IDC_RAD_TEXT))->GetCheck() == BST_CHECKED)
    {
        filter = L"Text files (*.txt)|*.txt|All files (*.*)|*.*||";
        defaultExt = L"txt";
        exportFunc = &ExportListCtrlToText;
    }

    if (!exportFunc) return;
    CFileDialog fileSaveDialog(FALSE, defaultExt, L"exported", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this);

    if (fileSaveDialog.DoModal() == IDOK)
    {
        CString exportFilePath = fileSaveDialog.GetPathName();
        if (!exportFilePath.Right(defaultExt.GetLength()).CompareNoCase(defaultExt))
        {
        }

        if (m_pList && (*exportFunc)(*m_pList, exportFilePath))
        {
            AfxMessageBox(L"Exported successfully");
        }
        else
        {
            AfxMessageBox(L"Something went wrong");
        }
    }
}



void CDBExportDlg::OnBnClickedRadCsv()
{
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowTextW(strCSVdescription);
}


void CDBExportDlg::OnBnClickedRadXml()
{
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowTextW(strXMLdescription);
}


void CDBExportDlg::OnBnClickedRadTsv()
{
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowTextW(strTSVdescription);
}


void CDBExportDlg::OnBnClickedRadText()
{
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowTextW(strTextdescription);
}


void CDBExportDlg::OnBnClickedRadHtml()
{
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowTextW(strHTMLdescription);
}
