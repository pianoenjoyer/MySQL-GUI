#pragma once
#include "afxdialogex.h"
#include "Drawer.h"
#include "CDBConnection.h"

class CMonitorTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorTab)

public:
						CMonitorTab(CWnd* pParent = nullptr);   // standard constructor
	virtual				~CMonitorTab();
	BOOL				OnInitDialog();
	inline void			SetDatabaseObject(std::shared_ptr<CDBConnection> obj) { db = obj; };
	inline std::shared_ptr<CDBConnection> GetDatabaseObject() { return db; };
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_MONITOR };
#endif

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	Drawer drwCpuUsage;
	Drawer drwConnections;
	Drawer drwTraffic;
	Drawer drwProcesses;

	std::vector<double> m_TrafficSentDataHystory;
	std::vector<double> m_TrafficReceivedDataHystory;
	std::vector<double> m_ConnectionDataHystory;
	std::vector<double> m_ProcessesDataHystory;

	std::vector<double> m_TrafficSentDataToDraw;
	std::vector<double> m_TrafficReceivedDataToDraw;
	std::vector<double> m_ConnectionDataToDraw;
	std::vector<double> m_ProcessesDataToDraw;

	std::shared_ptr<CDBConnection> db;
	DECLARE_MESSAGE_MAP()
public:
	void				UpdateTrafficList();
	afx_msg void		OnBnClickedBtnMonstart();
	void				InitGraph();
	double				GetCurrentNetworkTrafficSent();
	void				UpdateConnectionsList();
	void				UpdateNetworkTrafficTitle();
	double				GetCurrentQuestions();
	double				GetCurrentCpuUsage();
	void				UpdateServerRunningTime();
	double				GetCurrentNetworkTrafficReceived();
	double				GetCurrentNetworkTraffic();
	CString				GetServerRunningTime();
	CString				GetServerStartupTime();
	double				GetTrafficUsage();
	int					GetConnectionCount();
	int					GetProcessCount();
	void				UpdateGraph();
	void				OnTimer(UINT_PTR nIDEvent);
	afx_msg void		OnBnClickedBtnTrafficUpdate();
	afx_msg void		OnBnClickedBtnMonclear();
	void				OnSize(UINT nType, int cx, int cy);
};
