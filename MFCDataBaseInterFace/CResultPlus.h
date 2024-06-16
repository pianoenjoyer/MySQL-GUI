#pragma once
#include "pch.h"
#include "CDBConnection.h"
#include "Convertions.h"
class CMainDlg;

class CResultPlus : public CDialogEx
{
	DECLARE_DYNAMIC(CResultPlus)
	struct ListItem 
	{
		CString mainItem;
		std::vector<CString> subItems;
	};

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
								CResultPlus(CWnd* pParent = nullptr);
								CResultPlus(int DIALOG_ID, CWnd* pParent);
	virtual						~CResultPlus();
	void						SetCurrentResultSet(sql::ResultSet* NewResultSet);
	sql::ResultSet*				m_curResSet;
	std::vector<ListItem>		m_AllItems;
	std::shared_ptr<CDBConnection> db;
	CListCtrl					m_resultList;
	CComboBox					m_comboLimit;
	inline void					SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr
	<CDBConnection>				GetDatabaseObject() { return db; };
	void						SetPagesState();
	BOOL						OnInitDialog();
	afx_msg void				OnBnClickedBtnClroutput();
	void						SendMessageToQueryInfo(CString msg, COLORREF color);
	afx_msg void				OnBnClickedCheckShowall();
	afx_msg void				OnEnChangeListSearch();
	afx_msg void				OnCbnSelchangeComboNmbOfRows();
	afx_msg void				OnBnClickedBtnFirstpage();
	afx_msg void				OnBnClickedBtnPrevpage();
	CMainDlg* m_pMainDlg;
	bool						FillLimitDropdown();
	void						BeginPageState();
	int							BuildResultList(const int offset);
	int							BuildResultList(sql::ResultSet* resultSet);
	void						SaveOriginalListState();
	int							RebuildResultList();
	afx_msg void				OnEnChangeEditCurrentpage();
	afx_msg void				OnBnClickedBtnNextpage();
	afx_msg void				OnBnClickedBtnLastpage();
	afx_msg void				OnBnClickedCheckShowbinary();
};
