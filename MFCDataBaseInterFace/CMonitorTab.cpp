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

    m_cpuUsageData = getCPULast60Seconds();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMonitorTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MONSTART, &CMonitorTab::OnBnClickedBtnMonstart)
END_MESSAGE_MAP()


// CMonitorTab message handlers


void CMonitorTab::OnBnClickedBtnMonstart()
{
	UpdateData(true);
    m_cpuUsageData = getCPULast60Seconds();

    //TODO adjust min and max based on vector values
    auto min_iter = std::max_element(m_cpuUsageData.begin(), m_cpuUsageData.end());
    auto max_iter = std::min_element(m_cpuUsageData.begin(), m_cpuUsageData.end());
    int cpuGraphMax = *max_iter;
    int cpuGraphMin = *min_iter;

    int connGraphMax = 10;
    int connGraphMin = 0;

    int traffGraphMax = 1000;
    int traffGraphMin = 0;
        
    int procGraphMax = 1000;
    int procGraphMin = 0;

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

std::vector<double> CMonitorTab::getTrafficLast60Seconds()
{
    std::vector<double> traffic;
    sql::ResultSet* res = db->ExecuteQuery("SELECT `bytes_sent` + `bytes_received` AS `total_traffic` FROM `performance_schema`.`network_io` WHERE `event_time` > NOW() - INTERVAL 1 MINUTE;");

    while (res->next())
    {
        traffic.push_back(res->getDouble("total_traffic"));
    }
    delete res;
    return traffic;
}

std::vector<double> CMonitorTab::getCPULast60Seconds()
{
    std::vector<double> cpuUsage;
    sql::ResultSet* res = db->ExecuteQuery("SELECT ROUND(SUM(`TIMER_WAIT`) / 1000000000, 2) AS `cpu_usage` FROM `performance_schema`.`events_stages_history_long` WHERE `EVENT_NAME` = 'idle' AND `TIMER_WAIT` > 0 AND `TIMER_START` > NOW() - INTERVAL 1 MINUTE;");
    if (!res)
    {
        return cpuUsage;
    }
    while (res->next())
    {
        cpuUsage.push_back(res->getDouble("cpu_usage"));
    }
    delete res;
    return cpuUsage;
    
}

std::vector<int> getConnectionCountLast60Seconds(sql::Statement* stmt)
{
    std::vector<int> connectionCount;

    try
    {
        sql::ResultSet* res = stmt->executeQuery("SELECT `total_connections` FROM `performance_schema`.`global_status` WHERE `variable_name` = 'Threads_connected' AND `event_time` > NOW() - INTERVAL 1 MINUTE;");

        while (res->next())
        {
            connectionCount.push_back(res->getInt("total_connections"));
        }

        delete res;
    }
    catch (sql::SQLException& e)
    {

    }

    return connectionCount;
}


std::vector<int> getProcessCountLast60Seconds(sql::Statement* stmt)
{
    std::vector<int> processCount;

    try
    {
        sql::ResultSet* res = stmt->executeQuery("SELECT `count` FROM `performance_schema`.`threads` WHERE `name` = 'thread/sql/main' AND `event_time` > NOW() - INTERVAL 1 MINUTE;");

        while (res->next())
        {
            processCount.push_back(res->getInt("count"));
        }

        delete res;
    }
    catch (sql::SQLException& e)
    {

    }

    return processCount;
}