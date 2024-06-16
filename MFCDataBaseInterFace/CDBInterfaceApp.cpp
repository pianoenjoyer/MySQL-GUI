

#include "pch.h"
#include "framework.h"
#include "CDBInterfaceApp.h"
#include "CAuthDlg.h"
#include "CLoadingDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CDBInterfaceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CDBInterfaceApp::CDBInterfaceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CDBInterfaceApp theApp;

BOOL CDBInterfaceApp::InitInstance()
{
	if (!AfxInitRichEdit2()) 
	{
		#ifdef DEBUG
				OutputDebugString(L"AfxInitRichEdit2() init error \n");
		#endif
		#ifdef DEBUG_AFXMESSAGE
				AfxMessageBox(L"AfxInitRichEdit2() init error\n");
		#endif
		return FALSE;
	}
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	std::shared_ptr<CDBConnection> db(std::make_shared<CDBConnection>());
	INT_PTR status_auth = IDOK;
	INT_PTR status_main = IDOK;

	while (status_auth == IDOK && status_main == IDOK)
	{
		CAuthDlg authDlg(db);
		status_auth = authDlg.DoModal();
		if (status_auth == IDOK)
		{
			CMainDlg mainWindow(db);
			status_main = mainWindow.DoModal();
			if (db)
			{
				db->Disconnect();
			}
		}	
	}
	
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}


#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif
	return FALSE;
}

