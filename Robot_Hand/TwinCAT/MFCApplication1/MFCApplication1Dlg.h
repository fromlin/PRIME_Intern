#pragma comment(lib, "bCAPClient.lib")
#pragma comment(lib, "bCAPClientd.lib")
#pragma comment(lib, "bCAPClients.lib")
#pragma comment(lib, "bCAPClientsd.lib")
// MFCApplication1Dlg.h : 헤더 파일
//

#include <stdio.h>
#include <atlbase.h>
#include <stdint.h>
#include <iostream>
#include "bCAPClient/bcap_client.h"

#pragma once
#include "afxwin.h"


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList_Log();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton_Connect();
	afx_msg void OnBnClickedButton_Disconnect();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_comboDO;
	CEdit mEDIT;
	CStatic mState;
	CListBox Log;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticDo1();
	afx_msg void OnStnClickedStaticDo2();
	afx_msg void OnStnClickedStaticDo3();
	afx_msg void OnStnClickedStaticDo4();
	afx_msg void OnStnClickedStaticDo5();
	afx_msg void OnStnClickedStaticDo6();
	afx_msg void OnStnClickedStaticDo7();
	afx_msg void OnStnClickedStaticDo8();
	afx_msg void OnBnClickedDoexecute();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedDoexecute2();
	afx_msg void OnStnClickedStaticAo2();

	// Data read thread
	static UINT DataReadThread(LPVOID _mothod);
	CButton m_check_DO1;
	CButton m_check_DO2;
	CButton m_check_DO3;
	CButton m_check_DO4;
	CButton m_check_DO5;
	CButton m_check_DO6;
	CButton m_check_DO7;
	CButton m_check_DO8;

	CString tState;
	CString AO_Value;
	CString Angle_Dev;
	CEdit m_AO1;
	CEdit m_AO2;
	CEdit m_AO3;
	CEdit m_AO4;
	CStatic m_AI1;
	CStatic m_AI2;
	CStatic m_AI3;
	CStatic m_AI4;
	CStatic m_print_AO1;
	CStatic m_print_AO2;
	CStatic m_print_AO3;
	CStatic m_print_AO4;
	afx_msg void OnStnClickedStaticAo1();
	afx_msg void OnBnClickedInitialization();
	afx_msg void OnBnClickedShutdown();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedXp();
	afx_msg void OnBnClickedXm();
	afx_msg void OnBnClickedYm();
	afx_msg void OnBnClickedYplus();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedTool();
};
