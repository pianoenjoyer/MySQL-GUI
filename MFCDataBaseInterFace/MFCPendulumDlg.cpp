#pragma once
#include "CPendulum.h" 
#include "pch.h"
#include "framework.h"
#include "MFCPendulumDlg.h"
#include "afxdialogex.h"
#include "FlickerFreeDC.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EARTH 9.8
#define MOON 1.6

#define FPS60 (1.0/60.0)
#define FPS30 (1.0/30.0)



CMFCPendulumDlg::CMFCPendulumDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PENDULUM, pParent), m_buttonState(0),
	m_pendulum(0, 0, 0, 0, 0), m_timerID(0), pDC(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	 defaultLength = 1;
	 defaultAngle = -45;
	 defaultGravity = EARTH;
	 defaultMass = 1;
	 defaultResistance = 0.5;
}

void CMFCPendulumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_graphStatic);
	DDX_Control(pDX, IDC_PENDULUM, m_pendulumStatic);
	DDX_Control(pDX, IDC_POTENTIONAL, m_progressPotentional);
	DDX_Control(pDX, IDC_KINETIC, m_progressKinetic);
	DDX_Control(pDX, IDC_STATIC_POTENTIONAL, m_staticPotentional);
	DDX_Control(pDX, IDC_STATIC_KINETIC, m_staticKinetic);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_editLength);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	DDX_Control(pDX, IDC_EDIT_GRAVITY, m_editGravity);
	DDX_Control(pDX, IDC_EDIT_MASS, m_editMass);
	DDX_Control(pDX, IDC_EDIT_RESIST, m_editResist);
}

BEGIN_MESSAGE_MAP(CMFCPendulumDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ADD_FORCE, &CMFCPendulumDlg::OnBnClickedAddForce)
	ON_EN_CHANGE(IDC_EDIT_LENGTH, &CMFCPendulumDlg::OnEnChangeEditLength)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, &CMFCPendulumDlg::OnEnChangeEditAngle)
	ON_EN_CHANGE(IDC_EDIT_GRAVITY, &CMFCPendulumDlg::OnEnChangeEditGravity)
	ON_EN_CHANGE(IDC_EDIT_MASS, &CMFCPendulumDlg::OnEnChangeEditMass)
	ON_EN_CHANGE(IDC_EDIT_RESIST, &CMFCPendulumDlg::OnEnChangeEditResist)
	ON_BN_CLICKED(IDC_START, &CMFCPendulumDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_RESTART, &CMFCPendulumDlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_DECREASE_FORCE, &CMFCPendulumDlg::OnBnClickedDecreaseForce)
	ON_BN_CLICKED(IDC_BTN_MASS_INCREASE, &CMFCPendulumDlg::OnBnClickedBtnMassIncrease)
	ON_BN_CLICKED(IDC_BTN_MASS_DECREASE, &CMFCPendulumDlg::OnBnClickedBtnMassDecrease)
	ON_BN_CLICKED(IDC_BTN_LENGTH_INCREASE, &CMFCPendulumDlg::OnBnClickedBtnLengthIncrease)
	ON_BN_CLICKED(IDC_BTN_LENGTH_DECREASE, &CMFCPendulumDlg::OnBnClickedBtnLengthDecrease)
END_MESSAGE_MAP()


BOOL CMFCPendulumDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetWindowTextW(L" PENDULUM MFC ");
	m_progressPotentional.SetRange(0, 80);
	m_progressKinetic.SetRange(0, 80);

	CString defaultText;
	defaultText.Format(_T("%.2f"), defaultLength);
	m_editLength.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultAngle);
	m_editAngle.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultGravity);
	m_editGravity.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultMass);
	m_editMass.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultResistance);
	m_editResist.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultGravity);

	m_pendulum.Init(defaultLength, defaultAngle,
	defaultGravity, defaultMass, defaultResistance);
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_drawer.Create(GetDlgItem(IDC_GRAPH)->GetSafeHwnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}


HCURSOR CMFCPendulumDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCPendulumDlg::OnTimer(UINT_PTR nIDEvent)
{
    GetDlgItem(IDC_PENDULUM)->GetClientRect(rectPendulum);
    pDC = GetDlgItem(IDC_PENDULUM)->GetDC();
    m_pendulum.Update();
    m_pendulum.DrawPendulum(*pDC, rectPendulum);

    ReleaseDC(pDC);
    UpdateEnergyInfo();
    CDialogEx::OnTimer(nIDEvent);
}


void CMFCPendulumDlg::UpdateGraph() 
{




}

void CMFCPendulumDlg::UpdateEnergyInfo()
{
	CString kenetic, potentional;
	double kenetic_value = m_pendulum.GetKinetic();
	double potentional_value = m_pendulum.GetPotentional();

	kenetic.Format(_T("%f"), kenetic_value);
	potentional.Format(_T("%f"), potentional_value);

	m_staticPotentional.SetWindowText(potentional);
	m_staticKinetic.SetWindowText(kenetic);

	double totalEnergy = std::abs(kenetic_value) + std::abs(potentional_value);

	// Calculate the percentages
	int kineticPercentage = static_cast<int>((std::abs(kenetic_value) / totalEnergy) * 100);
	int potentialPercentage = static_cast<int>((std::abs(potentional_value) / totalEnergy) * 100);

	m_progressPotentional.SetPos(potentialPercentage);
	m_progressKinetic.SetPos(kineticPercentage);
}



void CMFCPendulumDlg::OnEnChangeEditLength()
{
	CString text;
	m_editLength.GetWindowTextW(text);
	double value = _tstoi(text);
	m_pendulum.SetLength(value);
}


void CMFCPendulumDlg::OnEnChangeEditAngle()
{
	CString text;
	m_editAngle.GetWindowTextW(text);
	double value = _tstoi(text);
	m_pendulum.SetAngle(value);
}


void CMFCPendulumDlg::OnEnChangeEditGravity()
{
	CString text;
	m_editGravity.GetWindowTextW(text);
	double value = _tstoi(text);
	m_pendulum.SetGravity(value);
}


void CMFCPendulumDlg::OnEnChangeEditMass()
{
	CString text;
	m_editMass.GetWindowTextW(text);
	double value = _tstoi(text);
	m_pendulum.SetMass(value);
}


void CMFCPendulumDlg::OnEnChangeEditResist()
{
	CString text;
	m_editResist.GetWindowTextW(text);
	double value = _tstoi(text);
}


void CMFCPendulumDlg::OnBnClickedStart()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_START);
	m_buttonState = !m_buttonState;
	if (m_buttonState)
	{
		m_timerID = SetTimer(1, FPS60, nullptr);
		pButton->SetWindowText(_T("STOP")); 
	}
	else
	{
		pButton->SetWindowText(_T("START")); 
		KillTimer(m_timerID);
	}
}


void CMFCPendulumDlg::OnBnClickedRestart()
{
	m_pendulum.setAcceleration(0.0);
	m_pendulum.Init(defaultLength, defaultAngle, 
		defaultGravity, defaultMass, defaultResistance);

	CString defaultText;
	defaultText.Format(_T("%.2f"), defaultLength);
	m_editLength.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultAngle);
	m_editAngle.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultGravity);
	m_editGravity.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultMass);
	m_editMass.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultResistance);
	m_editResist.SetWindowTextW(defaultText);
	defaultText.Format(_T("%.2f"), defaultGravity);


}

void CMFCPendulumDlg::OnBnClickedAddForce()
{
	m_pendulum.addAcceleration(0.01);
}

void CMFCPendulumDlg::OnBnClickedDecreaseForce()
{
	m_pendulum.addAcceleration(-0.01);
}


void CMFCPendulumDlg::OnBnClickedBtnMassIncrease()
{
	double mass = m_pendulum.GetMass();
	m_pendulum.SetMass(mass + 0.5);

	double newMass = m_pendulum.GetMass();
	CString temp;
	temp.Format(_T("%.2f"), newMass);
	GetDlgItem(IDC_EDIT_MASS)->SetWindowTextW(temp);
	m_pendulum.SetMass(newMass);
}


void CMFCPendulumDlg::OnBnClickedBtnMassDecrease()
{
	double mass = m_pendulum.GetMass();
	if (!(mass - 0.5) <= 0)
	{
		m_pendulum.SetMass(mass - 0.5);
	}

	double newMass = m_pendulum.GetMass();
	CString temp;
	temp.Format(_T("%.2f"), newMass);
	GetDlgItem(IDC_EDIT_MASS)->SetWindowTextW(temp);
	m_pendulum.SetMass(newMass);
}


void CMFCPendulumDlg::OnBnClickedBtnLengthIncrease()
{
	double scale = 0.25;
	double len = m_pendulum.GetLength();
	if (!(len + scale) <= 0 && (len + scale) < 1.5)
	{
		m_pendulum.SetLength(len + scale);
	}

	double newLength = m_pendulum.GetLength();
	CString temp;
	temp.Format(_T("%.2f"), newLength);
	GetDlgItem(IDC_EDIT_LENGTH)->SetWindowTextW(temp);
	m_pendulum.SetLength(newLength);
}


void CMFCPendulumDlg::OnBnClickedBtnLengthDecrease()
{
	double scale = 0.25;
	double len = m_pendulum.GetLength();
	if (!(len - scale) <= 0)
	{
		m_pendulum.SetLength(len - scale);
	}

	double newLength = m_pendulum.GetLength();
	CString temp;
	temp.Format(_T("%.2f"), newLength);
	GetDlgItem(IDC_EDIT_LENGTH)->SetWindowTextW(temp);
	m_pendulum.SetLength(newLength);
}

