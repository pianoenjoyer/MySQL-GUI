#include "pch.h"
#include "framework.h"
#include "MFCDataBaseInterFace.h"
#include "MFCDataBaseInterFaceDlg.h"
#include "CLoginDataSave.h"
#include "afxdialogex.h"
#include "CDBConnection.h"
#include "CDBMain.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//main dlg construct
CMFCDataBaseInterFaceDlg::CMFCDataBaseInterFaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDATABASEINTERFACE_DIALOG, pParent), db(std::make_shared<CDBConnection>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverPassword = _T(""); // Initialize the server password
}

void CMFCDataBaseInterFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_NAME, m_editServerName);
	DDX_Text(pDX, IDC_PASSWORD, m_serverPassword);
}

BEGIN_MESSAGE_MAP(CMFCDataBaseInterFaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMFCDataBaseInterFaceDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CMFCDataBaseInterFaceDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SHOW_PASSWORD, &CMFCDataBaseInterFaceDlg::OnBnClickedBtnShowPassword)
END_MESSAGE_MAP()
//ON INIT----------------------------------------------!!!
BOOL CMFCDataBaseInterFaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AfxInitRichEdit2();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//set title
	this->SetWindowTextW(_T("Authentication")); 
	//set image
	CImage image;
	if (SUCCEEDED(image.Load(L".\\mysql_logo.png"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
	{
		CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_LOGO);
		HBITMAP hBmp = image.Detach();
		pPicCtrl->SetBitmap(hBmp);
	}
	if (SUCCEEDED(image.Load(L".\\mysql_logo.png"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
	{
		CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_LOGO2);
		HBITMAP hBmp = image.Detach();
		pPicCtrl->SetBitmap(hBmp);
	}
	//default test data
	m_cServer = "127.0.0.1:3306";
	m_cUser = "root";
	m_cPassword = "";
	m_cDatabase = "safelighting";
		
	CLoginDataSave DataSaver(L"config.txt");
	DataSaver.SaveLoginInfo(m_cUser, m_cPassword);
	GetDlgItem(IDC_SERVER_NAME)->SetWindowTextW(m_cServer);
	GetDlgItem(IDC_DATABASE_NAME)->SetWindowTextW(m_cDatabase);
	GetDlgItem(IDC_USER_NAME2)->SetWindowTextW(m_cUser);
	GetDlgItem(IDC_PASSWORD)->SetWindowTextW(m_cPassword);

	

	//OnBnClickedBtnConnect(); // REMOVE AFTER COMPLETE DEBUGGING
	return TRUE;
}

void CMFCDataBaseInterFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CMFCDataBaseInterFaceDlg::OnPaint()
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

HCURSOR CMFCDataBaseInterFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCDataBaseInterFaceDlg::OnBnClickedBtnConnect()
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

		if (db->Connect(sqlServer, sqlUser, sqlPassword, sqlDatabase))
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

CString GetAppCurrentDirectory()
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	return CString(buffer);
}

void CMFCDataBaseInterFaceDlg::OnBnClickedBtnExit()
{
	CString cwd = GetAppCurrentDirectory();
	AfxMessageBox(cwd);
	this->EndDialog(IDOK);
}


//hide or show password button
void CMFCDataBaseInterFaceDlg::OnBnClickedBtnShowPassword()
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

