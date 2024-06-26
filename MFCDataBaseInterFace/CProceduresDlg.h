#pragma once

class CProceduresDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProceduresDlg)

public:
	CProceduresDlg(CWnd* pParent = nullptr);
	virtual ~CProceduresDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCEDURES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
