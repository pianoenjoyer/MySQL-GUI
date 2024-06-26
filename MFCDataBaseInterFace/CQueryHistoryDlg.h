#pragma once

class CQueryHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryHistoryDlg)

public:
	CQueryHistoryDlg(CWnd* pParent = nullptr);
	virtual ~CQueryHistoryDlg();
	BOOL OnInitDialog();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HYSTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CString m_selectedQueryFromHistory;
	void LoadHistoryToListCtrl();
	CString m_filename;
	DECLARE_MESSAGE_MAP()
public:
	CString GetSelectedQueryFromHistory() { return m_selectedQueryFromHistory; }
	afx_msg void OnBnClickedBtnSelHistRec();
	afx_msg void OnBnClickedBtnOpnHistFile();
	afx_msg void OnBnClickedBtnClrHist();
};
