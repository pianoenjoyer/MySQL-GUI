
#include "pch.h"
#include "afxdialogex.h"
#include "CLoadingDlg.h"
#include "resource.h"
#include "Colors.h"
#include "resource.h"
#include "CMainDlg.h"

IMPLEMENT_DYNAMIC(CLoadingDlg, CDialogEx)

CLoadingDlg::CLoadingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent) 
{

}


CLoadingDlg::~CLoadingDlg()
{

}

void CLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool CLoadingDlg::SetImage(CStatic* pPicture, const CString& path) const
{
	CImage image;
	if (SUCCEEDED(image.Load(path)))
	{
		CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_LOADING_PIC);
		HBITMAP hBmp = image.Detach();
		if (pPicCtrl)
		{
			pPicCtrl->SetBitmap(hBmp);
			return true;
		}
	}
	return false;
}


bool CLoadingDlg::SetImage(CStatic* pPicture, const int nImageID) const
{
	HBITMAP hBmp = static_cast<HBITMAP>(LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nImageID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
	if (hBmp)
	{
		if (pPicture)
		{
			pPicture->SetBitmap(hBmp);
			return true;
		}
		else
		{
			DeleteObject(hBmp);
		}
	}
	return false;
}



BOOL CLoadingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_INIT);
	pProgress->SetRange(0, 50);

	CStatic* pPicCtrl = (CStatic*)GetDlgItem(IDC_LOADING_PIC);
	SetImage(pPicCtrl, L".\\Pictures\\Top-MYSQL-GUI-Tools-Edit2.png");
	//SetImage(pPicCtrl, IDB_PNG_LOADING);
	return TRUE;
}


bool CLoadingDlg::SetLoadingState(const int position, const CString& message) const
{
	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_INIT);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_LOADING);

    if (pProgress && pStatic)
    {
        pStatic->SetWindowText(message);
        pProgress->SetPos(position);
        return true;
    }

    return false;
}


void CLoadingDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_BUTTON)
	{
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		// Check the button state
		UINT state = lpDrawItemStruct->itemState;

		// Customize the appearance based on the button ID
		if (nIDCtl == IDC_BTN_CONNECT)
		{
			if (state & ODS_SELECTED)
			{
				// Button is pressed
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(100, 100, 100));
			}
			else if (state & ODS_HOTLIGHT)
			{
				// Mouse is over the button
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 0, 0)); // Change color when hovered
			}
			else
			{
				// Normal state
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(216, 120, 29));
			}

			dc.SetTextColor(RGB(255, 255, 255));
		}
		else if (nIDCtl == IDC_BTN_EXIT)
		{
			if (state & ODS_SELECTED)
			{
				// Button is pressed
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(50, 50, 50));
			}
			else if (state & ODS_HOTLIGHT)
			{
				// Mouse is over the button
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(0, 255, 0)); // Change color when hovered
			}
			else
			{
				// Normal state
				dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(66, 101, 173));
			}
			dc.SetTextColor(RGB(255, 255, 255));
		}


		// Set a custom font for buttons
		CFont font;
		font.CreateFontW(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_SWISS, _T("Leelawadee UI"));
		dc.SelectObject(&font);
		auto pCheckbox = (CCheckListBox*)GetDlgItem(IDC_SAVE_LOGIN);
		pCheckbox->SetFont(&font);
		// Draw the text based on the button type
		CString buttonText;
		GetDlgItemText(nIDCtl, buttonText);
		dc.DrawText(buttonText, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		dc.Detach();
	}
}


BOOL CLoadingDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	pDC->FillSolidRect(&rectClient, RGB(29, 48, 82));
	return TRUE;
}


HBRUSH CLoadingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	TCHAR classname[MAX_PATH];
	if (::GetClassName(pWnd->m_hWnd, classname, MAX_PATH) == 0)
		return hbr;
	if (_tcsicmp(classname, _T("EDIT")) == 0)
		return hbr;

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	hbr = CreateSolidBrush(SQLBLUE);
	return hbr;
}


BEGIN_MESSAGE_MAP(CLoadingDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

