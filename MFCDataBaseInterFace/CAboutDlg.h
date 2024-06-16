#pragma once

class CAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = nullptr);
	virtual ~CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
