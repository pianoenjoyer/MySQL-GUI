// CMonitorTab.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CMonitorTab.h"
#include "resource.h"
#include "Convertions.h"
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
    UpdateTrafficList();
    UpdateConnectionsList();
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
    if (m_TrafficSentDataToDraw.size() > 60) 
    {
        m_TrafficSentDataToDraw.erase(m_TrafficSentDataToDraw.begin());
        m_ProcessesDataToDraw.erase(m_ProcessesDataToDraw.begin());
        m_TrafficReceivedDataToDraw.erase(m_TrafficReceivedDataToDraw.begin());
        m_ConnectionDataToDraw.erase(m_ConnectionDataToDraw.begin());

        m_TrafficSentDataHystory.erase(m_TrafficSentDataHystory.begin());
        m_TrafficReceivedDataHystory.erase(m_TrafficReceivedDataHystory.begin());
        m_ConnectionDataHystory.erase(m_ConnectionDataHystory.begin());
        m_ProcessesDataHystory.erase(m_ProcessesDataHystory.begin());
    }

    double curTrafficSentUsage = GetCurrentNetworkTrafficSent();
    double curTrafficReceivedUsage = GetCurrentNetworkTrafficReceived();
    double curConnections = GetConnectionCount();
    int curProcesses = GetProcessCount();

    m_TrafficSentDataHystory.push_back(curTrafficSentUsage);
    m_ProcessesDataHystory.push_back(curProcesses);
    m_TrafficReceivedDataHystory.push_back(curTrafficReceivedUsage);
    m_ConnectionDataHystory.push_back(curConnections);

    size_t lastIndex = m_TrafficSentDataHystory.size() - 1;
    if (!m_TrafficSentDataHystory.empty())
    {
        curTrafficSentUsage = curTrafficSentUsage - m_TrafficSentDataHystory[lastIndex - 1];
    }

    lastIndex = m_TrafficReceivedDataHystory.size() - 1;
    if (!m_TrafficReceivedDataHystory.empty())
    {
        curTrafficReceivedUsage = curTrafficReceivedUsage - m_TrafficReceivedDataHystory[lastIndex - 1];
    }

    m_TrafficReceivedDataToDraw.push_back(curTrafficReceivedUsage);
    m_TrafficSentDataToDraw.push_back(curTrafficSentUsage);
    m_ProcessesDataToDraw.push_back(curProcesses);
    m_ConnectionDataToDraw.push_back(curConnections);


    std::vector<double> timeStamps;
    for (int i = 1; i <= 60; ++i)
    {
        timeStamps.push_back(i);
    }
    //TODO adjust min and max based on vector values

    double traffReceivedGraphMax = 1;
    double traffReceivedGraphMin = 0;

    double traffSentGraphMax = 2;
    double traffSentGraphMin = 0;

    int connXGraphMax = 10;
    int connXGraphMin = 0;

    int procGraphMax = 10;
    int procGraphMin = 0;

    int timeAxisMin = 0;
    int timeAxisMax = 60;

    if (traffReceivedGraphMax < curTrafficReceivedUsage || traffReceivedGraphMin > curTrafficReceivedUsage)
    {
        traffReceivedGraphMax = curTrafficReceivedUsage * 2;
        traffReceivedGraphMin = curTrafficReceivedUsage / 2;
    }

    if (traffSentGraphMax < curTrafficReceivedUsage || traffSentGraphMin > curTrafficReceivedUsage)
    {
        traffSentGraphMax = curTrafficReceivedUsage * 2;
        traffSentGraphMin = curTrafficReceivedUsage / 2;
    }


    std::thread thTrafReceived([&]()
        {
            drwCpuUsage.Draw(traffReceivedGraphMax, traffReceivedGraphMin, timeAxisMax, timeAxisMin, m_TrafficReceivedDataToDraw, timeStamps);
        });

    std::thread thCon([&]()
        {
            drwConnections.Draw(connXGraphMax, connXGraphMin, timeAxisMax, timeAxisMin, m_ConnectionDataToDraw, timeStamps);
        });
    std::thread thTrafSent([&]()
        {
            drwTraffic.Draw(traffSentGraphMax, traffSentGraphMin, timeAxisMax, timeAxisMin, m_TrafficSentDataToDraw, timeStamps);
        });
    std::thread thProc([&]()
        {
            drwProcesses.Draw(procGraphMax, procGraphMin, timeAxisMax, timeAxisMin, m_ProcessesDataToDraw, timeStamps);
        });

    thTrafReceived.join();
    thTrafSent.join();
    thCon.join();
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
    double trafficInKiB = bytesReceived / 1024.0;
    return trafficInKiB;
}

double CMonitorTab::GetCurrentNetworkTrafficSent() {

    sql::ResultSet* res =  db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_sent';");
    res->next();
    double bytesSent = res->getDouble("Value");
    delete res;
    double trafficInMiB = bytesSent / 1024.0;
    return trafficInMiB;
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
    if (!res) {
        return CString(_T("Error retrieving startup time."));
    }

    if (res->next()) {
        int uptimeSeconds = res->getInt("Value");
        delete res;

        time_t currentTime;
        time(&currentTime);

        time_t startupTime = currentTime - uptimeSeconds;
        struct tm tmInfo;
        localtime_s(&tmInfo, &startupTime);

        char buffer[100];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tmInfo);
        return CString(_T(" It started up on ")) + CString(buffer);
    }
    else {
        delete res;
        return CString(_T("No result found for startup time."));
    }
}


void CMonitorTab::UpdateTrafficList()
{
    auto pList = (CListCtrl*)GetDlgItem(IDC_LIST_TRAFFIC);
    if (!pList)
    {
        return;
    }

    pList->InsertColumn(0, _T("Traffic type"), LVCFMT_LEFT, 200);
    pList->InsertColumn(1, _T("#"), LVCFMT_LEFT, 200);
    pList->InsertColumn(2, _T("ø per hour"), LVCFMT_LEFT, 200);  


    int nIndex = pList->GetItemCount();

    // Execute your SQL query to fetch traffic data from the database
    sql::ResultSet* resultSet = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_sent';");
    nIndex = 0;
    while (resultSet->next())
    {
        CString trafficType = SQLStringToCString(resultSet->getString("Variable_name"));
        CString number = SQLStringToCString(resultSet->getString("Value"));

        pList->InsertItem(nIndex, trafficType);
        pList->SetItemText(nIndex, 1, number);

        nIndex++;
    }
    delete resultSet;

    resultSet = db->ExecuteQuery("SHOW STATUS LIKE 'Bytes_received';");
    nIndex = 1;
    while (resultSet->next())
    {
        CString trafficType = SQLStringToCString(resultSet->getString("Variable_name"));
        CString number = SQLStringToCString(resultSet->getString("Value"));

        pList->InsertItem(nIndex, trafficType);
        pList->SetItemText(nIndex, 1, number);

        nIndex++;
    }

}

void CMonitorTab::UpdateConnectionsList()
{
    auto pList = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTIONS);
    if (!pList)
    {
        return;
    }

    pList->InsertColumn(0, _T("Connections"), LVCFMT_LEFT, 200);
    pList->InsertColumn(1, _T("#"), LVCFMT_LEFT, 100);
    pList->InsertColumn(2, _T("ø per hour"), LVCFMT_LEFT, 100);  
    pList->InsertColumn(3, _T("%"), LVCFMT_LEFT, 100);

    int nIndex = pList->GetItemCount();

    pList->InsertItem(nIndex, _T("Max. concurrent connections	"));
    pList->SetItemText(nIndex, 1, L"200");
    pList->SetItemText(nIndex, 2, L"220");
    pList->SetItemText(nIndex, 3, L"100%");
    nIndex++;
    pList->InsertItem(nIndex, _T("Failed attempts"));
    pList->SetItemText(nIndex, 1, L"300");
    pList->SetItemText(nIndex, 2, L"330");
    pList->SetItemText(nIndex, 3, L"100%");
    nIndex++;
    pList->InsertItem(nIndex, _T("Aborted"));
    pList->SetItemText(nIndex, 1, L"400");
    pList->SetItemText(nIndex, 2, L"440");
    pList->SetItemText(nIndex, 3, L"100%");
    nIndex++;
    pList->InsertItem(nIndex, _T("Total"));
    pList->SetItemText(nIndex, 1, L"400");
    pList->SetItemText(nIndex, 2, L"440");
    pList->SetItemText(nIndex, 3, L"100%");


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
    m_TrafficSentDataToDraw.clear();
    m_ProcessesDataToDraw.clear();
    m_TrafficReceivedDataToDraw.clear();
    m_ConnectionDataToDraw.clear();

    m_TrafficSentDataHystory.clear();
    m_TrafficReceivedDataHystory.clear();
    m_ConnectionDataHystory.clear();
    m_ProcessesDataHystory.clear();
    UpdateGraph();
}
