#pragma once

#include <memory>

class CDBConnection;

class CDatabasesTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabasesTab)

public:
								CDatabasesTab(CWnd* pParent = nullptr);

	virtual						~CDatabasesTab();
	inline void					SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	afx_msg void				OnBnClickedBtnCreatedb();
	afx_msg void				OnBnClickedBtnDeletedb();
	afx_msg void				OnEnChangeFilterDatabases();
	afx_msg void				OnLvnItemchangedListDatabases(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void				OnBnClickedBtnRefreshdbs();

	std::shared_ptr<CDBConnection> db;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASES };
#endif

protected:
	bool						PopulateDatabaseDropdown();
	virtual void				DoDataExchange(CDataExchange* pDX);
	BOOL						OnInitDialog();
	void						PopulateDatabaseList();
	void						UpdateListFilter();
	CString						GetDatabaseCreatedDate(const CString& databaseName);
	CString						GetDatabaseOwner(const CString& databaseName);
	void						AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation);
	CString						GetDatabaseCollation(const CString& databaseName);
	void						AddDatabaseInfoToList(CListCtrl* pListCtrl, const CString& databaseName, const CString& collation,const CString& size);
	void						PopulateCharacterSetDropdown();
	CString						GetDatabaseSize(const CString& databaseName);
	DECLARE_MESSAGE_MAP()
};
