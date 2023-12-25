// CMonitorTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CMonitorTab.h"
#include "resource.h"
#include "thread"
// CMonitorTab dialog

IMPLEMENT_DYNAMIC(CMonitorTab, CDialogEx)

CMonitorTab::CMonitorTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATUS_MONITOR, pParent)
{

}


CMonitorTab::~CMonitorTab()
{

}

void CMonitorTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CMonitorTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	drwCpuUsage.Create(GetDlgItem(IDC_PIC_CPU)->GetSafeHwnd());
	drwConnections.Create(GetDlgItem(IDC_PIC_CONNECTIONS)->GetSafeHwnd());
	drwTraffic.Create(GetDlgItem(IDC_PIC_TRAFFIC)->GetSafeHwnd());
	drwProcesses.Create(GetDlgItem(IDC_PIC_PROCESSES)->GetSafeHwnd());

	m_cpuUsageData = { 1,2,3,4,5,6,7 };///////////////

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMonitorTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MONSTART, &CMonitorTab::OnBnClickedBtnMonstart)
END_MESSAGE_MAP()


// CMonitorTab message handlers


void CMonitorTab::OnBnClickedBtnMonstart()
{
	UpdateData(true);

	for (auto& value : m_cpuUsageData)
	{
		value += 1;
	}

	bool bEarth_Moon = true;
	std::thread thCpu([&]()
		{
			drwCpuUsage.Draw( 50, 0, 60, 1, m_cpuUsageData, m_cpuUsageData);
		});

	std::thread thCon([&]()
		{
			drwConnections.Draw(50, 0, 60, 1, m_cpuUsageData, m_cpuUsageData);
		});
	std::thread thTraf([&]()
		{
			drwTraffic.Draw(50, 0, 60, 1, m_cpuUsageData, m_cpuUsageData);
		});
	std::thread thProc([&]()
		{
			drwProcesses.Draw(50, 0, 60, 1, m_cpuUsageData, m_cpuUsageData);
		});

	thCpu.join();
	thCon.join();
	thTraf.join();
	thProc.join();
}

