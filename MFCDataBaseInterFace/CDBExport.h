#pragma once
#include "afxdialogex.h"


// CDBExport dialog

class CDBExport : public CDialogEx
{
	DECLARE_DYNAMIC(CDBExport)

public:
	virtual BOOL OnInitDialog();
	CDBExport(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDBExport();
	HICON m_hIcon;
	CListCtrl* m_pList;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedRadCsv();
	afx_msg void OnBnClickedRadXml();
	afx_msg void OnBnClickedRadTsv();
	afx_msg void OnBnClickedRadText();
	afx_msg void OnBnClickedRadHtml();
};
