#include "pch.h"
#include "framework.h"
#include "CDBInterfaceApp.h"
#include "CDBAuthDlg.h"
#include "CLoginDataSave.h"
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBMainDlg.h"
#include <atlimage.h>
#define N  M,  24;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString GetAppCurrentDirectory()
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	return CString(buffer);
	
}

//set key enter to connect button
BOOL CDBAuthDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pWnd = GetDlgItem(IDOK); // set your btn id
		if (pWnd && pWnd->IsWindowEnabled())
		{
			pWnd->SetFocus();
			pWnd->SendMessage(WM_LBUTTONDOWN);
			pWnd->SendMessage(WM_LBUTTONUP);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


//main dlg construct
CDBAuthDlg::CDBAuthDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDATABASEINTERFACE_DIALOG, pParent), db(std::make_shared<CDBConnection>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverPassword = _T(""); // Initialize the server password
}

void CDBAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_NAME, m_editServerName);
	DDX_Text(pDX, IDC_PASSWORD, m_serverPassword);
}

BEGIN_MESSAGE_MAP(CDBAuthDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CDBAuthDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDBAuthDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SHOW_PASSWORD, &CDBAuthDlg::OnBnClickedBtnShowPassword)
END_MESSAGE_MAP()
//ON INIT----------------------------------------------!!!
BOOL CDBAuthDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	AfxInitRichEdit2();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//set title
	this->SetWindowTextW(_T("Authentication")); 
	//set image
	CImage image;
	if (SUCCEEDED(image.Load(L".\\Pictures\\mysql_logo.png"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
	{
		CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_LOGO);
		HBITMAP hBmp = image.Detach();
		pPicCtrl->SetBitmap(hBmp);
	}

	//default test data
	m_cServer = "127.0.0.1:3306";
	m_cUser = "root";
	m_cPassword = "";
	//m_cDatabase = "safelighting";
		
	CLoginDataSave DataSaver(L"config.txt");
	DataSaver.SaveLoginInfo(m_cUser, m_cPassword);
	GetDlgItem(IDC_SERVER_NAME)->SetWindowTextW(m_cServer);
	//GetDlgItem(IDC_DATABASE_NAME)->SetWindowTextW(m_cDatabase);
	GetDlgItem(IDC_USER_NAME2)->SetWindowTextW(m_cUser);
	GetDlgItem(IDC_PASSWORD)->SetWindowTextW(m_cPassword);

	

	//OnBnClickedBtnConnect(); // REMOVE AFTER COMPLETE DEBUGGING
	return TRUE;
}

void CDBAuthDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CDBAuthDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CDBAuthDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDBAuthDlg::OnBnClickedBtnConnect()
{

	CString server, user, password, database;
	INT_PTR status = 0;
	GetDlgItemText(IDC_SERVER_NAME, server);
	GetDlgItemText(IDC_USER_NAME2, user);
	GetDlgItemText(IDC_PASSWORD, password);
	GetDlgItemText(IDC_DATABASE_NAME, database);

	sql::SQLString sqlServer(CW2A(server.GetString()));
	sql::SQLString sqlUser(CW2A(user.GetString()));
	sql::SQLString sqlPassword(CW2A(password.GetString()));
	sql::SQLString sqlDatabase(CW2A(database.GetString()));

		if (db->Connect(sqlServer, sqlUser, sqlPassword))
		{
			mainWindow.db = this->db;
			this->ShowWindow(SW_HIDE);
			mainWindow.m_titleDatabaseName = database;
			status = mainWindow.DoModal();
		}
		//check dialog exit status
		switch (status)
		{
		case IDOK: //if disconected button pressed
			this->ShowWindow(SW_SHOW);
			break;
		case IDCANCEL: //if system menu close button
			this->EndDialog(IDOK);
			break;
		case IDABORT:
			this->EndDialog(IDOK);
			break;
		default:
			break;
		}
		
		
}



void CDBAuthDlg::OnBnClickedBtnExit()
{
	/*CString cwd = GetAppCurrentDirectory();
	AfxMessageBox(cwd);*/
	this->EndDialog(IDOK);
}


//hide or show password button
void CDBAuthDlg::OnBnClickedBtnShowPassword()
{	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PASSWORD);
	DWORD dwStyle = pEdit->GetStyle();

	if (dwStyle & ES_PASSWORD)
	{
		pEdit->ModifyStyle(ES_PASSWORD, 0);
		pEdit->SetPasswordChar(0);
	}
	else 
	{
		pEdit->ModifyStyle(0, ES_PASSWORD);
		pEdit->SetPasswordChar(0x2022);
	}
	pEdit->Invalidate();
	//pEdit->RedrawWindow();	// not required
}


