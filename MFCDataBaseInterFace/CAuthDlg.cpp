
#include "pch.h"
#include "CDBInterfaceApp.h"
#include "MFCPendulumDlg.h"
#include "CAuthDlg.h"
#include "CLoginDataSave.h"
#include "CDBConnection.h"
#include "Colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString GetAppCurrentDirectory()
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	return CString(buffer);
	
}
BOOL CAuthDlg::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocusWnd = GetFocus();
		OnBnClickedBtnConnect();
		return TRUE; 
	}

	if (pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_SHIFT) < 0)
	{
		switch (pMsg->wParam)
		{
		case 'P':
		{
			m_pendulumDlg->Create(IDD_PENDULUM,0);
			return TRUE;
		}
		case 'D':
		{
			MessageBox(L"Demo mode is not available for now ");
			return TRUE;
		}
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}



CAuthDlg::CAuthDlg(CWnd* pParent)
	: CDialogEx(IDD_AUTH, pParent), db(std::make_shared<CDBConnection>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAuthDlg::CAuthDlg(std::shared_ptr<CDBConnection> db, CWnd* pParent)
	: CDialogEx(IDD_AUTH, pParent), db(db),
	m_pendulumDlg(new CMFCPendulumDlg()),
	m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
{

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
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CAuthDlg::OnCbnSelchangeComboLanguage)
END_MESSAGE_MAP()


void CAuthDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_BUTTON)
	{
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		UINT state = lpDrawItemStruct->itemState;
		if (nIDCtl == IDC_BTN_CONNECT)
		{
			if (state & ODS_SELECTED)
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(100, 100, 100));
			}
			else if (state & ODS_HOTLIGHT)
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 0, 0));
			}
			else
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(216, 120, 29));
			}

			dc.SetTextColor(RGB(255, 255, 255));
		}
		else if (nIDCtl == IDC_BTN_EXIT)
		{
			if (state & ODS_SELECTED)
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(50, 50, 50));
			}
			else if (state & ODS_HOTLIGHT)
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(0, 255, 0));
			}
			else
			{
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(66, 101, 173));
			}
			dc.SetTextColor(RGB(255, 255, 255));
		}
		CFont font;
		font.CreateFontW(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_SWISS, _T("Leelawadee UI"));
		dc.SelectObject(&font);
		auto pCheckbox = (CCheckListBox*)GetDlgItem(IDC_SAVE_LOGIN);
		pCheckbox->SetFont(&font);
		CString buttonText;
		GetDlgItemText(nIDCtl, buttonText);
		dc.DrawText(buttonText, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}
}



BOOL CAuthDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	pDC->FillSolidRect(&rectClient, RGB(60, 67, 98));
	return TRUE;
}


HBRUSH CAuthDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	TCHAR classname[MAX_PATH];
	if (::GetClassName(pWnd->m_hWnd, classname, MAX_PATH) == 0)
		return hbr;
	if (_tcsicmp(classname, _T("EDIT")) == 0)
		return hbr;

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	hbr = CreateSolidBrush(RGB(60, 67, 98));
	return hbr;
}


BOOL CAuthDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	this->SetWindowTextW(_T("Authentication"));
	CImage image;
	if (SUCCEEDED(image.Load(L".\\Pictures\\mysql_logo_white.png")))
	{
		CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_PIC_LOGO);
		HBITMAP hBmp = image.Detach();
		if (pPicCtrl)
		{
			pPicCtrl->SetBitmap(hBmp);
		}

	}

	CLoginDataSave dataSaver;

	if (dataSaver.ShouldRemember())
	{
		CString serverIP, username, password;
		bool rememberMe;

		if (dataSaver.LoadData(serverIP, username, password, rememberMe))
		{
			GetDlgItem(IDC_SERVER_NAME)->SetWindowTextW(serverIP);
			GetDlgItem(IDC_USER_NAME2)->SetWindowTextW(username);
			GetDlgItem(IDC_PASSWORD)->SetWindowTextW(password);
			((CButton*)GetDlgItem(IDC_SAVE_LOGIN))->SetCheck(rememberMe ? BST_CHECKED : BST_UNCHECKED);
		}
	}

	PopulateLangCombo();

	return TRUE;
}

BOOL CAuthDlg::PopulateLangCombo()
{
	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LANGUAGE);
	if (!pCombo)
	{
		return false;
	}
	pCombo->AddString(L"en");
	pCombo->AddString(L"ru");
	pCombo->SetCurSel(0);
	OnCbnSelchangeComboLanguage();
	return true;
}

void CAuthDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CAuthDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		
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

	if (rememberMe)
	{
		dataSaver.SaveData(server, user, password, true);
	}
	else
	{
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
			this->EndDialog(IDOK);
		}
		
}


void CAuthDlg::OnBnClickedBtnExit()
{
	this->EndDialog(IDCANCEL);
}


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
}


void CAuthDlg::OnCbnSelchangeComboLanguage()
{
	auto pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LANGUAGE);
	if (!pCombo)
	{
		return;
	}

	int curSel = pCombo->GetCurSel();
	if (curSel != CB_ERR)
	{
		CString ComboLangString;
		pCombo->GetLBText(curSel, ComboLangString);
		((CDBInterfaceApp*)AfxGetApp())->m_language = ComboLangString;
	}
}
