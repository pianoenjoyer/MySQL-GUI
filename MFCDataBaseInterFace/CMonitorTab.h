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
	std::vector<double> m_TrafficData;
	std::vector<double> m_ConnectionData;
	std::vector<double> m_ProcessesData;
	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
public:
	void CMonitorTab::UpdateTrafficList();
	afx_msg void OnBnClickedBtnMonstart();
	void CMonitorTab::InitGraph();
	BOOL CMonitorTab::IsTimerActive(UINT_PTR nIDEvent) const;
	DWORD CMonitorTab::GetTimerElapse(UINT_PTR nIDEvent) const;
	double CMonitorTab::GetCurrentNetworkTrafficSent();
	void CMonitorTab::UpdateConnectionsList();
	void CMonitorTab::UpdateNetworkTrafficTitle();
	double CMonitorTab::GetCurrentQuestions();
	double CMonitorTab::GetCurrentCpuUsage();
	void CMonitorTab::UpdateServerRunningTime();
	double CMonitorTab::GetCurrentNetworkTrafficReceived();
	double CMonitorTab::GetCurrentNetworkTraffic();
	CString CMonitorTab::GetServerRunningTime();
	CString CMonitorTab::GetServerStartupTime();
	BOOL CMonitorTab::IsTimerRunning() const;
	double CMonitorTab::GetTrafficUsage();
	int CMonitorTab::GetConnectionCount();
	int CMonitorTab::GetProcessCount();
	void CMonitorTab::UpdateGraph();
	void CMonitorTab::OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnTrafficUpdate();
	afx_msg void OnBnClickedBtnMonclear();
};
