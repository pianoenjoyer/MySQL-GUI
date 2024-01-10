
// MFCDataBaseInterFace.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "CDBInterfaceApp.h"
#include "CAuthDlg.h"
#include "CStartDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CDBInterfaceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CDBInterfaceApp::CDBInterfaceApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDBInterfaceApp object

CDBInterfaceApp theApp;


// CDBInterfaceApp initialization

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
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	std::shared_ptr<CDBConnection> db(std::make_shared<CDBConnection>());

	//m_pMainWnd = &dlg; // causing dlg to shutdown
	CAuthDlg authDlg(db);
	CMainDlg mainWindow(db);
	INT_PTR status_auth = IDOK;
	INT_PTR status_main = IDOK;

	while (status_auth == IDOK && status_main == IDOK)
	{
		status_auth = authDlg.DoModal();
		if (status_auth == IDOK)
		{
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

