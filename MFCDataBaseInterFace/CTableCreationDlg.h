#pragma once
#include "afxdialogex.h"
#include <memory>
#include "CDBConnection.h"
// CTableCreationDlg dialog

class CTableCreationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTableCreationDlg)

public:
	CTableCreationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTableCreationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLES };
#endif

protected:
	BOOL CTableCreationDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCreateTable;
	CRichEditCtrl m_editTableName;
	CComboBox m_cmbCollation;
	std::shared_ptr<CDBConnection> db;
	CRichEditCtrl m_editComments;
	CRichEditCtrl m_editAddColumns;
	afx_msg void OnBnClickedGo();
};
