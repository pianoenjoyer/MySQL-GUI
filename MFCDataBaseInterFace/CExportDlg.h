#pragma once
#include "afxdialogex.h"
#include "CDBConnection.h"


// CDBExportDlg dialog

class CDBExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBExportDlg)

public:
	virtual BOOL OnInitDialog();
	CDBExportDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBExportDlg();
	HICON m_hIcon;
	CListCtrl* m_pList;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
public:
	inline void SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedRadCsv();
	afx_msg void OnBnClickedRadXml();
	afx_msg void OnBnClickedRadTsv();
	afx_msg void OnBnClickedRadText();
	afx_msg void OnBnClickedRadHtml();
};
