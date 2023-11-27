
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
	ON_WM_CTLCOLOR()
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CAuthDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_BUTTON)
	{
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		// Customize the appearance based on the button ID
		if (nIDCtl == IDC_BTN_CONNECT)
		{
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(216, 120, 29));
			dc.SetTextColor(RGB(255, 255, 255));
		}
		else if (nIDCtl == IDC_BTN_EXIT)
		{
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(66, 101, 173));
			dc.SetTextColor(RGB(255, 255, 255));
		}
		else if (nIDCtl == IDC_SAVE_LOGIN) // This is a checkbox
		{
			// Draw checkbox background
			DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONCHECK);

			// Set a custom font for the checkbox text
			CFont font;
			font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
				OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				DEFAULT_PITCH | FF_SWISS, _T("Arial"));
			dc.SelectObject(&font);

			// Draw checkbox text
			CString checkboxText;
			GetDlgItemText(nIDCtl, checkboxText);
			dc.SetTextColor(RGB(0, 0, 255)); // Blue text color
			dc.DrawText(checkboxText, &lpDrawItemStruct->rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			dc.Detach();
		}

		// Set a custom font
		CFont font;
		font.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_SWISS, _T("Leelawadee UI"));
		dc.SelectObject(&font);

		// Draw the text based on the button type
		CString buttonText;
		GetDlgItemText(nIDCtl, buttonText);
		dc.DrawText(buttonText, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}
}

HBRUSH CAuthDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Check if the control is a static control
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// Set the desired text color
		pDC->SetTextColor(RGB(255, 255, 255)); // Red color, change as needed
		pDC->SetBkMode(TRANSPARENT); // Make the background transparent
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH); // Return a null brush to prevent background painting
	}

	return hbr;
}

BOOL CAuthDlg::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCustomDraw = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);

	// Check if the custom draw is for a button
	if (pNMCustomDraw->dwDrawStage == CDDS_PREPAINT &&
		pNMCustomDraw->dwItemSpec == (UINT)-1)  // Entire control
	{
		*pResult = CDRF_NOTIFYITEMDRAW; // Notify for item drawing
		return TRUE;
	}
	else if (pNMCustomDraw->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// Check if it's the button you want to customize
		if (pNMCustomDraw->dwItemSpec == IDC_BTN_CONNECT ||
			pNMCustomDraw->dwItemSpec == IDC_BTN_EXIT)
		{
			// Customize the appearance of the button here
			// You can change the background color, text color, etc.

			// For example, set the background color to light gray
			CDC* pDC = CDC::FromHandle(pNMCustomDraw->hdc);
			pDC->FillSolidRect(&pNMCustomDraw->rc, RGB(0, 0, 0)); // Light gray color

			// Set text color
			pDC->SetTextColor(RGB(0, 0, 255)); // Blue text color

			*pResult = CDRF_SKIPDEFAULT; // Skip default drawing
			return TRUE;
		}
	}

	*pResult = 0;
	return FALSE;
}


BOOL CAuthDlg::OnInitDialog()
{
	SetBackgroundColor(RGB(29, 48, 82));
	CDialogEx::OnInitDialog();
	AfxInitRichEdit2();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//set title
	this->SetWindowTextW(_T("Authentication")); 
	//set image
	CImage image;
	if (SUCCEEDED(image.Load(L".\\Pictures\\mysql_logo_white.png"))) //if (SUCCEEDED(image.Load(L"D:\\RTX.png")))
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


