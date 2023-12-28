#pragma once
#include "afxdialogex.h"
#include "Drawer.h"
#include "CDBConnection.h"
// CMonitorTab dialog

class CMonitorTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorTab)

public:
	CMonitorTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMonitorTab();
	BOOL CMonitorTab::OnInitDialog();
	inline void CMonitorTab::SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> CMonitorTab::GetDatabaseObject() { return db; };
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_MONITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	Drawer drwCpuUsage;
	Drawer drwConnections;
	Drawer drwTraffic;
	Drawer drwProcesses;
	std::vector<double> m_cpuUsageData;
	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnMonstart();
	std::vector<double> CMonitorTab::getTrafficLast60Seconds();
	std::vector<double> CMonitorTab::getCPULast60Seconds();
};
