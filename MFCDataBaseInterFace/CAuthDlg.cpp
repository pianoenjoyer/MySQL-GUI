
#include "pch.h"
#include "afxdialogex.h"
#include "CDBInterfaceApp.h"

#include "CAuthDlg.h"
#include "CLoginDataSave.h"
#include "CDBConnection.h"
#include "CMainDlg.h"
#include "CAboutDlg.h"

#include <memory>
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
BOOL CAuthDlg::PreTranslateMessage(MSG* pMsg)
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
CAuthDlg::CAuthDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDATABASEINTERFACE_DIALOG, pParent), db(std::make_shared<CDBConnection>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverPassword = _T(""); // Initialize the server password
}

void CAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_NAME, m_editServerName);
	DDX_Text(pDX, IDC_PASSWORD, m_serverPassword);
}

BEGIN_MESSAGE_MAP(CAuthDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CAuthDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CAuthDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SHOW_PASSWORD, &CAuthDlg::OnBnClickedBtnShowPassword)
END_MESSAGE_MAP()
//ON INIT----------------------------------------------!!!
BOOL CAuthDlg::OnInitDialog()
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

	CLoginDataSave dataSaver;

	if (dataSaver.ShouldRemember())
	{
		CString serverIP, username, password;
		bool rememberMe;

		// Load the saved data
		if (dataSaver.LoadData(serverIP, username, password, rememberMe))
		{
			// Assuming you have member variables m_editServerIP, m_editUsername, m_editPassword, and m_chkRememberMe
			// linked to the respective controls using DDX_Control in DoDataExchange
			GetDlgItem(IDC_SERVER_NAME)->SetWindowTextW(serverIP);
			GetDlgItem(IDC_USER_NAME2)->SetWindowTextW(username);
			GetDlgItem(IDC_PASSWORD)->SetWindowTextW(password);
			((CButton*)GetDlgItem(IDC_SAVE_LOGIN))->SetCheck(rememberMe ? BST_CHECKED : BST_UNCHECKED);
		}
	}

	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LANGUAGE);
	pCombo->AddString(L"en");
	pCombo->SetCurSel(0);

	//OnBnClickedBtnConnect(); // REMOVE AFTER COMPLETE DEBUGGING
	return TRUE;
}

void CAuthDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CAuthDlg::OnPaint()
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

HCURSOR CAuthDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAuthDlg::OnBnClickedBtnConnect()
{
	CLoginDataSave dataSaver;

	CString server, user, password;
	GetDlgItem(IDC_SERVER_NAME)->GetWindowTextW(server);
	GetDlgItem(IDC_USER_NAME2)->GetWindowTextW(user);
	GetDlgItem(IDC_PASSWORD)->GetWindowTextW(password);
	bool rememberMe = (((CButton*)GetDlgItem(IDC_SAVE_LOGIN))->GetCheck() == BST_CHECKED);

	// If "Remember Me" is checked, save the data
	if (rememberMe)
	{
		dataSaver.SaveData(server, user, password, true);
	}
	else
	{	//if not clear data from regisry
		dataSaver.ClearData();
	}

	INT_PTR status = 0;
	GetDlgItemText(IDC_SERVER_NAME, server);
	GetDlgItemText(IDC_USER_NAME2, user);
	GetDlgItemText(IDC_PASSWORD, password);

	sql::SQLString sqlServer(CW2A(server.GetString()));
	sql::SQLString sqlUser(CW2A(user.GetString()));
	sql::SQLString sqlPassword(CW2A(password.GetString()));

		if (db->Connect(sqlServer, sqlUser, sqlPassword))
		{
			mainWindow.db = this->db;
			this->ShowWindow(SW_HIDE);
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



void CAuthDlg::OnBnClickedBtnExit()
{
	/*CString cwd = GetAppCurrentDirectory();
	AfxMessageBox(cwd);*/
	this->EndDialog(IDOK);
}


//hide or show password button
void CAuthDlg::OnBnClickedBtnShowPassword()
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


