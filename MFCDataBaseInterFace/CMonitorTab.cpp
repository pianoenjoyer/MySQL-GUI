// CMonitorTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CMonitorTab.h"
#include "resource.h"

// CMonitorTab dialog
#define TIMER_ID 1
IMPLEMENT_DYNAMIC(CMonitorTab, CDialogEx)

static bool TimerStatus = 0;

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
    InitGraph();
    UpdateNetworkTrafficTitle();
    UpdateServerRunningTime();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMonitorTab, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MONSTART, &CMonitorTab::OnBnClickedBtnMonstart)
    ON_BN_CLICKED(IDC_BTN_TRAFFIC_UPDATE, &CMonitorTab::OnBnClickedBtnTrafficUpdate)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_MONCLEAR, &CMonitorTab::OnBnClickedBtnMonclear)
END_MESSAGE_MAP()


double CMonitorTab::GetCurrentCpuUsage() 
{
    sql::ResultSet* res = db->ExecuteQuery("SHOW GLOBAL STATUS LIKE 'CPU_USAGE';");

    if (res && res->next()) {
        double cpuUsage = res->getDouble("Value");
        delete res;
        return cpuUsage;
    }
    return 0;
}

double CMonitorTab::GetCurrentQuestions()
{
    sql::ResultSet* res = db->ExecuteQuery("SHOW GLOBAL STATUS LIKE 'QUESTIONS';");

    if (res && res->next()) {
        double questions = res->getDouble("Value");
        delete res;
        return questions;
    }
    return 0;
}


void CMonitorTab::OnBnClickedBtnMonstart()
{
    auto pBtn = GetDlgItem(IDC_BTN_MONSTART);

    if (pBtn)
    {
        UpdateGraph();
        if (!TimerStatus)
        {
            pBtn->SetWindowTextW(L"PAUSE");
            TimerStatus = true;
            SetTimer(TIMER_ID, 1000, nullptr);
        }
        else
        {
            pBtn->SetWindowTextW(L"START");
            TimerStatus = false;
            KillTimer(TIMER_ID);
        }
    }
    
}

void CMonitorTab::OnTimer(UINT_PTR nIDEvent) {
    if (nIDEvent == TIMER_ID) {

        UpdateGraph();
    }
    CDialog::OnTimer(nIDEvent);
}

void CMonitorTab::InitGraph() 
{
    drwCpuUsage.Create(GetDlgItem(IDC_PIC_CPU)->GetSafeHwnd());
    drwConnections.Create(GetDlgItem(IDC_PIC_CONNECTIONS)->GetSafeHwnd());
    drwTraffic.Create(GetDlgItem(IDC_PIC_TRAFFIC)->GetSafeHwnd());
    drwProcesses.Create(GetDlgItem(IDC_PIC_PROCESSES)->GetSafeHwnd());
}

void CMonitorTab::UpdateGraph() 
{
    UpdateData(true);
    if (m_cpuUsageData.size() == 61) {
        m_cpuUsageData.erase(m_cpuUsageData.begin());
        m_ProcessesData.erase(m_ProcessesData.begin());
        m_TrafficData.erase(m_TrafficData.begin());
        m_ConnectionData.erase(m_ConnectionData.begin());
    }


    double curCpuUsage = GetCurrentQuestions();
    double curConnections = GetConnectionCount();
    double curTrafficUsage = GetCurrentNetworkTraffic();
    int curProcesses = GetProcessCount();

    if (!m_TrafficData.empty()) {
        curTrafficUsage -= m_TrafficData.back();
    }

    m_cpuUsageData.push_back(curCpuUsage);
    m_ProcessesData.push_back(curProcesses);
    m_TrafficData.push_back(curTrafficUsage);
    m_ConnectionData.push_back(curConnections);

    std::vector<double> timeStamps;
    int i = 1;
    for (auto& value : timeStamps)
    {
        timeStamps.push_back(i);
        ++i;
    }


    //TODO adjust min and max based on vector values
    int questionsGraphMax = 100000;
    int questionsGraphMin = 0;

    int connXGraphMax = 10;
    int connXGraphMin = 0;

    double traffGraphMax = 1;
    double traffGraphMin = 0;

    int procGraphMax = 10;
    int procGraphMin = 0;

    int timeAxisMin = 0;
    int timeAxisMax = 60;

    std::thread thCpu([&]()
        {
            drwCpuUsage.Draw(questionsGraphMax, questionsGraphMin, timeAxisMax, timeAxisMin, m_cpuUsageData, timeStamps);
        });

    std::thread thCon([&]()
        {
            drwConnections.Draw(connXGraphMax, connXGraphMin, timeAxisMax, timeAxisMin, m_ConnectionData, timeStamps);
        });
    std::thread thTraf([&]()
        {
            drwTraffic.Draw(traffGraphMax, traffGraphMin, timeAxisMax, timeAxisMin, m_TrafficData, timeStamps);
        });
    std::thread thProc([&]()
        {
            drwProcesses.Draw(procGraphMax, procGraphMin, timeAxisMax, timeAxisMin, m_ProcessesData, timeStamps);
        });

    thCpu.join();
    thCon.join();
    thTraf.join();
    thProc.join();
}

void CMonitorTab::UpdateNetworkTrafficTitle()
{
    // Assuming m_groupControl is the ID of your group control
    CStatic* pStaticControl = (CStatic*)GetDlgItem(IDC_STATIC_TRAFFIC_TITLE);
    if (pStaticControl)
    {
        // Retrieve and set the current network traffic since startup
        double currentTraffic = GetCurrentNetworkTraffic();
        CString title;
        title.Format(_T("Network traffic since startup: %.2f MiB"), currentTraffic);
        pStaticControl->SetWindowTextW(title);
    }
}

void CMonitorTab::UpdateServerRunningTime()
{
    // Assuming m_editControl is the ID of your edit control
    CEdit* pEditControl = (CEdit*)GetDlgItem(IDC_EDIT_SERVER_RUNNING_TIME);
    if (pEditControl)
    {
        // Retrieve and set the running time information
        CString runningTimeInfo = GetServerRunningTime();
        CString startupTimeInfo = GetServerStartupTime();
        pEditControl->SetWindowTextW(L"This MySQL server has been running for " + runningTimeInfo + L"." + startupTimeInfo + L".");
    }
}

double CMonitorTab::GetCurrentNetworkTraffic() {

        sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_received';");
        res->next();
        double bytesReceived = res->getDouble("Value");
        delete res;

        res = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_sent';");
        res->next();
        double bytesSent = res->getDouble("Value");
        delete res;
        double trafficInMiB = (bytesReceived + bytesSent) / (1024.0 * 1024.0);
        return trafficInMiB;
}

double CMonitorTab::GetCurrentNetworkTrafficReceived() {

    sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_received';");
    res->next();
    double bytesReceived = res->getDouble("Value");
    delete res;
    return bytesReceived;
}

double CMonitorTab::GetCurrentNetworkTrafficSent() {

    sql::ResultSet* res =  db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_sent';");
    res->next();
    double bytesSent = res->getDouble("Value");
    delete res;
    return bytesSent;
}

CString CMonitorTab::GetServerRunningTime() {
        sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Uptime';");
        res->next();
        int uptimeSeconds = res->getInt("Value");
        delete res;

        int days = uptimeSeconds / (60 * 60 * 24);
        int hours = (uptimeSeconds % (60 * 60 * 24)) / (60 * 60);
        int minutes = (uptimeSeconds % (60 * 60)) / 60;

        CString result;
        result.Format(_T("%d days, %d hours, %d minutes"), days, hours, minutes);
        return result;
}

CString CMonitorTab::GetServerStartupTime() {
        sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Uptime';");
        if (!res)
        {
            return CString(_T("Error retrieving startup time."));
        }
        res->next();
        int uptimeSeconds = res->getInt("Value");
        delete res;

        time_t currentTime;
        time(&currentTime);

        time_t startupTime = currentTime - uptimeSeconds;
        struct tm* tmInfo = localtime(&startupTime);

        char buffer[100];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmInfo);
        return CString(_T(" It started up on ")) + CString(buffer);
}


void CMonitorTab::OnBnClickedBtnTrafficUpdate()
{
    UpdateNetworkTrafficTitle();
    UpdateServerRunningTime();
}

double CMonitorTab::GetTrafficUsage()
{
    // Assuming Bytes_received and Bytes_sent are in bytes
    double bytesReceived = 0.0;
    double bytesSent = 0.0;

    sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_received';");
    if (res->next())
    {
        bytesReceived = res->getDouble("Value");
    }
    delete res;

    res = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_sent';");
    if (res->next())
    {
        bytesSent = res->getDouble("Value");
    }
    delete res;

    // Convert bytes to MiB
    bytesReceived /= (1024 * 1024);
    bytesSent /= (1024 * 1024);

    return bytesReceived + bytesSent;
}

int CMonitorTab::GetConnectionCount()
{
    int connectionCount = 0;

    sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Threads_connected';");
    if (res->next())
    {
        connectionCount = res->getInt("Value");
    }
    delete res;

    return connectionCount;
}

int CMonitorTab::GetProcessCount()
{
    int processCount = 0;

    sql::ResultSet* res = db->ExecuteQuery("SHOW STATUS LIKE 'Threads_running';");
    if (res->next())
    {
        processCount = res->getInt("Value");
    }
    delete res;

    return processCount;
}


void CMonitorTab::OnBnClickedBtnMonclear()
{
    m_cpuUsageData.clear();
    m_ProcessesData.clear();
    m_TrafficData.clear();
    m_ConnectionData.clear();
    UpdateGraph();
}
