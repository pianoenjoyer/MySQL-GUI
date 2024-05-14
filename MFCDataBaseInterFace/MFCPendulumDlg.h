
#pragma once
#include "CPendulum.h"
#include "Drawer.h"

class CMFCPendulumDlg : public CDialogEx
{
public:
	CMFCPendulumDlg(CWnd* pParent = nullptr);	// standard constructor
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPENDULUM_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	void CMFCPendulumDlg::UpdateGraph();
	Drawer m_drawer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	void UpdateEnergyInfo();
	DECLARE_MESSAGE_MAP()
public:
	CDC* pDC;
	CRect rectPendulum;
	CRect rectGraph;

	CPendulum m_pendulum;
	//CProgressCtrl m_progress;
	UINT_PTR m_timerID;
	CStatic m_graphStatic;
	CStatic m_pendulumStatic;
	afx_msg void OnBnClickedAddForce();
	CProgressCtrl m_progressPotentional;
	CProgressCtrl m_progressKinetic;
	CStatic m_staticPotentional;
	CStatic m_staticKinetic;
	afx_msg void OnEnChangeEditLength();
	afx_msg void OnEnChangeEditAngle();
	afx_msg void OnEnChangeEditGravity();
	afx_msg void OnEnChangeEditMass();
	afx_msg void OnEnChangeEditResist();
	CEdit m_editLength;
	CEdit m_editAngle;
	CEdit m_editGravity;
	CEdit m_editMass;
	CEdit m_editResist;
	bool m_buttonState;

	double defaultLength;
	double defaultAngle;
	double defaultGravity;
	double defaultMass;
	double defaultResistance;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedDecreaseForce();
	afx_msg void OnBnClickedBtnMassIncrease();
	afx_msg void OnBnClickedBtnMassDecrease();
	afx_msg void OnBnClickedBtnLengthIncrease();
	afx_msg void OnBnClickedBtnLengthDecrease();
};
