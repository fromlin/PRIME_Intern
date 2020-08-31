
// MFCApplication1Dlg.cpp : 구현 파일

#ifdef _USE_WIN_API
/*ifndef _USE_LINUX_API*/
#include <Windows.h>
#endif

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "adsClient.h"

#include <assert.h>
#include <WinNT.h>
#include <bitset>
#include <math.h>
#include <sstream>
#include <atlconv.h>

#define _USE_MATH_DEFINES
#define TARGET_RC8_IP "169.254.158.74"
#define SERVER_PORT_NUM 5007

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SYSTEM_INIT						0x70000000	
#define Timer1 1
#define IOPORT 350
typedef INT DINT;
int flag = 0;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

// ADS
CadsClient adsClient;
int pchSymbols;

// I/O
bool isPortOpen = FALSE;
bool Parameter;
int cID[8] = { 0,0,0,0,0,0,0,0 };
int sID[8] = { IDC_STATIC_DO1 ,IDC_STATIC_DO2 ,IDC_STATIC_DO3 ,IDC_STATIC_DO4 ,IDC_STATIC_DO5 ,IDC_STATIC_DO6, IDC_STATIC_DO7 ,IDC_STATIC_DO8 };
int siID[8] = { IDC_STATIC_DO9 ,IDC_STATIC_DO10 ,IDC_STATIC_DO11 ,IDC_STATIC_DO12 ,IDC_STATIC_DO13 ,IDC_STATIC_DO14, IDC_STATIC_DO15 ,IDC_STATIC_DO16 };
UINT DO_state;
UINT DI_state;
UINT OPstatus;
bool DO_state_bin[8] = { 0,0,0,0,0,0,0,0 };
bool DI_state_bin[8] = { 0,0,0,0,0,0,0,0 };
int32_t AxisPosition[6];
double AO[4] = { 0.0, 0.0, 0.0, 0.0 };
double AO_print[4] = { 0.0, 0.0, 0.0, 0.0 };
double AI[4] = { 0.0, 0.0, 0.0, 0.0 };
int16_t AO_give[4] = { 0, 0, 0, 0 };
int16_t AI_recv[4] = { 0, 0, 0, 0 };
int16_t AO_recv[4] = { 0, 0, 0, 0 };

// DENSO
int fd;
long *plData;
uint32_t hCtrl, hRob;
double *pdData, dPos[7],PosCmd[7];
BSTR bstr1, bstr2, bstr3, bstr4;
VARIANT vntParam, vntRet;
HRESULT hr;
// Denso X-Y Lim
// X_uppper lim, X_lower lim, Y_upper lim, Y_lower lim
double lim[4] = {560.0, 260.0, 200.0, -200.0};


bool m_bThreadStart;
CWinThread *p1 = NULL;

enum ThreadWorkingType {
	THREAD_STOP = 0,
	THREAD_RUNNING = 1,
	THREAD_PAUSE = 2
};

ThreadWorkingType m_eThreadWork;


using namespace std;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, Log);
	DDX_Control(pDX, IDC_DO1, m_check_DO1);
	DDX_Control(pDX, IDC_DO2, m_check_DO2);
	DDX_Control(pDX, IDC_DO3, m_check_DO3);
	DDX_Control(pDX, IDC_DO4, m_check_DO4);
	DDX_Control(pDX, IDC_DO5, m_check_DO5);
	DDX_Control(pDX, IDC_DO6, m_check_DO6);
	DDX_Control(pDX, IDC_DO7, m_check_DO7);
	DDX_Control(pDX, IDC_DO8, m_check_DO8);
	DDX_Control(pDX, IDC_EDIT1, m_AO1);
	DDX_Control(pDX, IDC_EDIT2, m_AO2);
	DDX_Control(pDX, IDC_EDIT3, m_AO3);
	DDX_Control(pDX, IDC_EDIT4, m_AO4);
	DDX_Control(pDX, IDC_STATIC_AI1, m_AI1);
	DDX_Control(pDX, IDC_STATIC_AI2, m_AI2);
	DDX_Control(pDX, IDC_STATIC_AI3, m_AI3);
	DDX_Control(pDX, IDC_STATIC_AI4, m_AI4);
	DDX_Control(pDX, IDC_STATIC_AO1, m_print_AO1);
	DDX_Control(pDX, IDC_STATIC_AO2, m_print_AO2);
	DDX_Control(pDX, IDC_STATIC_AO3, m_print_AO3);
	DDX_Control(pDX, IDC_STATIC_AO4, m_print_AO4);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMFCApplication1Dlg::OnLbnSelchangeList_Log)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton_Connect)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton_Disconnect)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_DO1, &CMFCApplication1Dlg::OnStnClickedStaticDo1)
	ON_STN_CLICKED(IDC_STATIC_DO2, &CMFCApplication1Dlg::OnStnClickedStaticDo2)
	ON_STN_CLICKED(IDC_STATIC_DO8, &CMFCApplication1Dlg::OnStnClickedStaticDo8)
	ON_STN_CLICKED(IDC_STATIC_DO3, &CMFCApplication1Dlg::OnStnClickedStaticDo3)
	ON_STN_CLICKED(IDC_STATIC_DO4, &CMFCApplication1Dlg::OnStnClickedStaticDo4)
	ON_STN_CLICKED(IDC_STATIC_DO5, &CMFCApplication1Dlg::OnStnClickedStaticDo5)
	ON_STN_CLICKED(IDC_STATIC_DO6, &CMFCApplication1Dlg::OnStnClickedStaticDo6)
	ON_STN_CLICKED(IDC_STATIC_DO7, &CMFCApplication1Dlg::OnStnClickedStaticDo7)
	ON_BN_CLICKED(IDC_DO_execute, &CMFCApplication1Dlg::OnBnClickedDoexecute)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_DO_execute2, &CMFCApplication1Dlg::OnBnClickedDoexecute2)
	ON_STN_CLICKED(IDC_STATIC_AO2, &CMFCApplication1Dlg::OnStnClickedStaticAo2)
	ON_STN_CLICKED(IDC_STATIC_AO1, &CMFCApplication1Dlg::OnStnClickedStaticAo1)
	ON_BN_CLICKED(IDC_Initialization, &CMFCApplication1Dlg::OnBnClickedInitialization)
	ON_BN_CLICKED(IDC_Shutdown, &CMFCApplication1Dlg::OnBnClickedShutdown)
	ON_BN_CLICKED(IDC_Reset, &CMFCApplication1Dlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_XP, &CMFCApplication1Dlg::OnBnClickedXp)
	ON_BN_CLICKED(IDC_Xm, &CMFCApplication1Dlg::OnBnClickedXm)
	ON_BN_CLICKED(IDC_Ym, &CMFCApplication1Dlg::OnBnClickedYm)
	ON_BN_CLICKED(IDC_Yplus, &CMFCApplication1Dlg::OnBnClickedYplus)
	ON_BN_CLICKED(IDC_TEST, &CMFCApplication1Dlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_TOOL, &CMFCApplication1Dlg::OnBnClickedTool)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	for (int i = 0; i < 8; i++) {
		tState.Format(_T("Channel %d"), i+1);
		m_comboDO.AddString(tState);
	}
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	system("CLS");
	cout << "========================= Console Panel =========================" << endl;
	tState.Format(_T("0.000"));
	m_AO1.SetWindowText(tState);
	m_AO2.SetWindowText(tState);
	m_AO3.SetWindowText(tState);
	m_AO4.SetWindowText(tState);
	tState.Format(_T("0000"));
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}



void CMFCApplication1Dlg::OnLbnSelchangeList_Log()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 여기에 State 표시
	switch (nIDEvent) {
	case Timer1: {
		//cout << "========================= Console Panel ============================" << endl;

		//cout << "Digital Output State = ";
		for (int i = 0; i < 8; i++) {
			//cout << DO_state_bin[i] << "\t";
		}
		//cout << endl;

		for (int i = 0; i < 8; i++) {
			GetDlgItem(sID[i])->Invalidate();
		}

		//cout << "Digital Input State = ";
		for (int i = 0; i < 8; i++) {
			//cout << DI_state_bin[i] << "\t";
		}
		//cout << endl;
		
		for (int i = 0; i < 8; i++) {
			GetDlgItem(siID[i])->Invalidate();
		}

		//cout << "Analog Output = ";
		for (int i = 0; i < 4; i++) {
			AO_print[i] = round(1000.0*10.0*(double)AO_recv[i] / 32767.0) / 1000.0;
			//cout << AO_print[i] << "\t";
		}
		//cout << endl;

		//cout << "Analog Input = ";
		for (int i = 0; i < 4; i++) {
			AI[i] = round(1000.0*10.0*(double)AI_recv[i] / 32767.0)/1000.0;
			//cout << AI[i] << "\t";
		}
		//cout << endl;
		
		tState.Format(_T("%2.3f"), AO_print[0]);
		m_print_AO1.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AO_print[1]);
		m_print_AO2.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AO_print[2]);
		m_print_AO3.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AO_print[3]);
		m_print_AO4.SetWindowTextW(tState);

		tState.Format(_T("%2.3f"), AI[0]);
		m_AI1.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AI[1]);
		m_AI2.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AI[2]);
		m_AI3.SetWindowTextW(tState);
		tState.Format(_T("%2.3f"), AI[3]);
		m_AI4.SetWindowTextW(tState);


		// Denso Connection Identification

	}
	}
	CDialogEx::OnTimer(nIDEvent);
}


//TwinCAT Connect
void CMFCApplication1Dlg::OnBnClickedButton_Connect()
{

	int nErr = 0;
	if (isPortOpen == FALSE) {
		nErr = adsClient.Connect(IOPORT);
		if (nErr) {
			tState.Format(_T("TwinCAT Connect Failed >> return = %d"), nErr);
			Log.InsertString(0, tState);
		}
		else if(nErr == 0) {
			tState.Format(_T("TwinCAT Connect Success >> return = %d"), nErr);
			Log.InsertString(0, tState);
			SetTimer(Timer1, 10, 0);
			if (p1 == NULL)
			{
				p1 = AfxBeginThread(DataReadThread, this);

				if(p1 == NULL) AfxMessageBox(L"Error");

				p1->m_bAutoDelete = FALSE;
				m_eThreadWork = THREAD_RUNNING;
			}
			else {
				if (m_eThreadWork == THREAD_PAUSE) {
					p1->ResumeThread();
					m_eThreadWork = THREAD_RUNNING;
				}
			}
			isPortOpen = TRUE;
		}
	}

}

//TwinCAT Disconnect
void CMFCApplication1Dlg::OnBnClickedButton_Disconnect()
{

	for (int i = 0; i < 6; i++) {
		AxisPosition[i] = 0;
	}
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x8100000C, 4, AxisPosition + 0);
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000010, 4, AxisPosition + 1);
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000014, 4, AxisPosition + 2);
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000018, 4, AxisPosition + 3);
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x8100001C, 4, AxisPosition + 4);
	//AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000020, 4, AxisPosition + 5);

	long nErr = 0;

	p1->SuspendThread();
	Sleep(500);

	if (isPortOpen == TRUE)
	{
		nErr = adsClient.Close();
		if (nErr) {
			tState.Format(_T("TwinCAT Disconnect Failed >> return = %d"), nErr);
			Log.InsertString(0, tState);
		}
		else if (nErr == 0) {
			tState.Format(_T("TwinCAT Disconnect Success >> return = %d"), nErr);
			Log.InsertString(0, tState);
			KillTimer(Timer1);
		}
	}
	isPortOpen = FALSE;

	if (p1 == NULL) {
		AfxMessageBox(L"Thread didn't start");
	}
	else {
		DWORD dwResult;
		::GetExitCodeThread(p1->m_hThread, &dwResult);
		
		delete p1;
		p1 = NULL;

		m_eThreadWork = THREAD_STOP;
		CloseHandle(p1);
	}
}

void CMFCApplication1Dlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

HBRUSH CMFCApplication1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  여기서 DC의 특성을 변경합니다.
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	for (int i = 0; i < 8; i++) {
		if (pWnd->GetDlgCtrlID() == sID[i]) {
			switch (DO_state_bin[i]) {
			case 0:
				pDC->SetBkColor(RGB(0, 255, 0));
				break;
			case 1:
				pDC->SetBkColor(RGB(255, 0, 0));
				break;
			}
		}
	}

	for (int i = 0; i < 8; i++) {
		if (pWnd->GetDlgCtrlID() == siID[i]) {
			switch (DI_state_bin[i]) {
			case 0:
				pDC->SetBkColor(RGB(0, 255, 0));
				break;
			case 1:
				pDC->SetBkColor(RGB(255, 0, 0));
				break;
			}
		}
	}
	return hbr;
}

UINT CMFCApplication1Dlg::DataReadThread(LPVOID _mothod) {
	CMFCApplication1Dlg* cDlg = (CMFCApplication1Dlg*)_mothod;
	while (1) {
		int id = 1;
		//DO read/
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x81000024, 1, &DO_state);
		for (int i = 0; i < 8; i++) {
			DO_state_bin[i] = (DO_state & (id << i)) == id << i;
		}

		//DI read/
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000030, 1, &DI_state);
		for (int i = 0; i < 8; i++) {
			DI_state_bin[i] = (DI_state & (id << i)) == id << i;
		}


		// AO read/
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x81000025, 2, AO_recv + 0);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x81000027, 2, AO_recv + 1);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x81000029, 2, AO_recv + 2);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x8100002B, 2, AO_recv + 3);


		// AI read/
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000031, 2, AI_recv + 0);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000033, 2, AI_recv + 1);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000035, 2, AI_recv + 2);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000037, 2, AI_recv + 3);


		// Joint read/
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x8000000C, 4, AxisPosition + 0);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000010, 4, AxisPosition + 1);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000014, 4, AxisPosition + 2);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000018, 4, AxisPosition + 3);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x8000001C, 4, AxisPosition + 4);
		AdsSyncReadReq(adsClient.pSrvAddr, 0x1010010, 0x80000020, 4, AxisPosition + 5);
		
	}
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedDoexecute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool DO[8];
	DO[0] = m_check_DO1.GetCheck(); DO[1] = m_check_DO2.GetCheck(); DO[2] = m_check_DO3.GetCheck(); DO[3] = m_check_DO4.GetCheck();
	DO[4] = m_check_DO5.GetCheck(); DO[5] = m_check_DO6.GetCheck(); DO[6] = m_check_DO7.GetCheck(); DO[7] = m_check_DO8.GetCheck();
	
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000120, 1, DO + 0);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000121, 1, DO + 1);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000122, 1, DO + 2);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000123, 1, DO + 3);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000124, 1, DO + 4);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000125, 1, DO + 5);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000126, 1, DO + 6);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0xC1000127, 1, DO + 7);
}

void CMFCApplication1Dlg::OnStnClickedStaticDo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticDo8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication1Dlg::OnBnClickedDoexecute2()
{
	
	m_AO1.GetWindowText(AO_Value);
	AO[0] = _wtof(AO_Value);
	m_AO2.GetWindowText(AO_Value);
	AO[1] = _wtof(AO_Value);
	m_AO3.GetWindowText(AO_Value);
	AO[2] = _wtof(AO_Value);
	m_AO4.GetWindowText(AO_Value);
	AO[3] = _wtof(AO_Value);

	for (int i = 0; i < 4; i++) {
		AO_give[i] = (int)( (65535 * (AO[i]+10) / 20) - 32767.5);
	}
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000025, 2, AO_give + 0);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000027, 2, AO_give + 1);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x81000029, 2, AO_give + 2);
	AdsSyncWriteReq(adsClient.pSrvAddr, 0x1010010, 0x8100002B, 2, AO_give + 3);

}


void CMFCApplication1Dlg::OnStnClickedStaticAo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnStnClickedStaticAo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMFCApplication1Dlg::OnBnClickedInitialization()
{
	hr = bCap_Open_Client("tcp:" TARGET_RC8_IP, 1000, 3, &fd);

	if (SUCCEEDED(hr)) {

		// Send SERVICE_START packet
		// 통신 시작
		// bCap_ServiceStart(file descriptor, No arg)
		bCap_ServiceStart(fd, NULL);


		bstr1 = SysAllocString(L"");                    /* Name */
		bstr2 = SysAllocString(L"CaoProv.DENSO.VRC");   /* Provider */
		bstr3 = SysAllocString(L"localhost");		/* Machine */
		bstr4 = SysAllocString(L"");  			/* Option */

												/* Connect RC8 */
												// RC8 handle of controller object
												// bCap_#####(file descriptor, arg1, arg2, arg3, arg3, out)
		hr = bCap_ControllerConnect(fd, bstr1, bstr2, bstr3, bstr4, &hCtrl);

		// Alloc 후 Free 필요,
		SysFreeString(bstr1);
		SysFreeString(bstr2);
		SysFreeString(bstr3);
		SysFreeString(bstr4);


		if (SUCCEEDED(hr)) {
			bstr1 = SysAllocString(L"Robot");	// Name
			bstr2 = SysAllocString(L"");		// Option
												/* Get robot handle */
												// Robot에 대한 handle 획득
			hr = bCap_ControllerGetRobot(fd, hCtrl, bstr1, bstr2, &hRob);

			SysFreeString(bstr1);
			SysFreeString(bstr2);

			if (SUCCEEDED(hr)) {
				bstr1 = SysAllocString(L"TakeArm");

				// Variant 변수 초기화
				VariantInit(&vntParam);
				vntParam.vt = (VT_I4 | VT_ARRAY);
				vntParam.parray = SafeArrayCreateVector(VT_I4, 0, 2);
				SafeArrayAccessData(vntParam.parray, (void **)&plData);
				plData[0] = 0; plData[1] = 1;
				SafeArrayUnaccessData(vntParam.parray);

				printf("TakeArm\n");
				hr = bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

				SysFreeString(bstr1);

				VariantClear(&vntParam);
				VariantClear(&vntRet);

				if (SUCCEEDED(hr)) {
					/* EXTSPEED 80% */
					bstr1 = SysAllocString(L"ExtSpeed");
					VariantInit(&vntParam);
					vntParam.vt = (VT_R8 | VT_ARRAY);
					vntParam.parray = SafeArrayCreateVector(VT_R8, 0, 1);
					SafeArrayAccessData(vntParam.parray, (void **)&pdData);
					pdData[0] = 50.0; // 80.0%
					SafeArrayUnaccessData(vntParam.parray);

					printf("External Speed = 80%\n");

					bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
					SysFreeString(bstr1);
					VariantClear(&vntParam);
					VariantClear(&vntRet);

					/* MOTOR ON */
					bstr1 = SysAllocString(L"Motor");

					VariantInit(&vntParam);
					vntParam.vt = VT_I4;
					vntParam.lVal = 1;

					/* Motor on */
					printf("Motor ON\n");
					bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

					SysFreeString(bstr1);

					VariantClear(&vntParam);
					VariantClear(&vntRet);

					//// Move to P(x,y,z,rx,ry,rz,fig) / First Position
					VariantInit(&vntParam);
					vntParam.bstrVal = SysAllocString(L"@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1)");
					vntParam.vt = VT_BSTR;
					printf("@E P(400.0, 0.0, 400.0, -180.0, 90.0, 180.0, 1) \n");
					bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
					VariantClear(&vntParam);

				}
			}
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedShutdown()
{
	bstr1 = SysAllocString(L"Motor");

	VariantInit(&vntParam);
	vntParam.vt = VT_I4;
	vntParam.lVal = 0;

	/* Motor off */
	printf("Motor OFF\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	bstr1 = SysAllocString(L"GiveArm");

	VariantInit(&vntParam);
	vntParam.vt = VT_EMPTY;

	/* Release arm control authority */
	printf("GiveArm\n");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	/* Release robot handle */
	bCap_RobotRelease(fd, &hRob);

	/* Disconnect RC8 */
	bCap_ControllerDisconnect(fd, &hCtrl);

	/* Send SERVICE_STOP packet */
	bCap_ServiceStop(fd);

	/* Close connection */
	bCap_Close_Client(&fd);
}


void CMFCApplication1Dlg::OnBnClickedReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication1Dlg::OnBnClickedXp()
{
	/* Get current position (x,y,z,rx,ry,rz,fig)*/
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurPos");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurPos=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);

	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			PosCmd[i] = dPos[i] + 10.0;
			if (PosCmd[i] >= lim[0]) {
				PosCmd[i] = dPos[i];
			}
		}
		else {
			PosCmd[i] = dPos[i];
		}
	}
	printf("PosCmd=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", PosCmd[0], PosCmd[1], PosCmd[2], PosCmd[3], PosCmd[4], PosCmd[5], PosCmd[6]);

	stringstream command;
	command << "@E P(" << PosCmd[0] << "," << PosCmd[1] << "," << PosCmd[2] << ","
		<< PosCmd[3] << "," << PosCmd[4] << "," << PosCmd[5] << ", 1)";

	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);

}


void CMFCApplication1Dlg::OnBnClickedXm()
{
	/* Get current position (x,y,z,rx,ry,rz,fig)*/
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurPos");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurPos=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);

	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			PosCmd[i] = dPos[i] - 10.0;
			if (PosCmd[i] <= lim[1]) {
				PosCmd[i] = dPos[i];
			}
		}
		else {
			PosCmd[i] = dPos[i];
		}
	}
	printf("PosCmd=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", PosCmd[0], PosCmd[1], PosCmd[2], PosCmd[3], PosCmd[4], PosCmd[5], PosCmd[6]);

	stringstream command;
	command << "@E P(" << PosCmd[0] << "," << PosCmd[1] << "," << PosCmd[2] << ","
		<< PosCmd[3] << "," << PosCmd[4] << "," << PosCmd[5] << ", 1)";

	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void CMFCApplication1Dlg::OnBnClickedYm()
{
	/* Get current position (x,y,z,rx,ry,rz,fig)*/
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurPos");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurPos=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);

	for (int i = 0; i < 6; i++) {
		if (i == 1) {
			PosCmd[i] = dPos[i] - 10.0;
			if (PosCmd[i] <= lim[3]) {
				PosCmd[i] = dPos[i];
			}
		}
		else {
			PosCmd[i] = dPos[i];
		}
	}
	printf("PosCmd=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", PosCmd[0], PosCmd[1], PosCmd[2], PosCmd[3], PosCmd[4], PosCmd[5], PosCmd[6]);

	stringstream command;
	command << "@E P(" << PosCmd[0] << "," << PosCmd[1] << "," << PosCmd[2] << ","
		<< PosCmd[3] << "," << PosCmd[4] << "," << PosCmd[5] << ", 1)";

	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void CMFCApplication1Dlg::OnBnClickedYplus()
{
	/* Get current position (x,y,z,rx,ry,rz,fig)*/
	VariantInit(&vntParam);
	bstr1 = SysAllocString(L"CurPos");
	bCap_RobotExecute(fd, hRob, bstr1, vntParam, &vntRet);
	SafeArrayAccessData(vntRet.parray, (void **)&pdData);
	memcpy(dPos, pdData, 7 * sizeof(double));
	SafeArrayUnaccessData(vntRet.parray);

	SysFreeString(bstr1);

	VariantClear(&vntParam);
	VariantClear(&vntRet);

	printf("CurPos=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", dPos[0], dPos[1], dPos[2], dPos[3], dPos[4], dPos[5], dPos[6]);

	for (int i = 0; i < 6; i++) {
		if (i == 1) {
			PosCmd[i] = dPos[i] + 10.0;
			if (PosCmd[i] >= lim[2]) {
				PosCmd[i] = dPos[i];
			}
		}
		else {
			PosCmd[i] = dPos[i];
		}
	}
	printf("PosCmd=P(%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%1.f)\n", PosCmd[0], PosCmd[1], PosCmd[2], PosCmd[3], PosCmd[4], PosCmd[5], PosCmd[6]);

	stringstream command;
	command << "@E P(" << PosCmd[0] << "," << PosCmd[1] << "," << PosCmd[2] << ","
		<< PosCmd[3] << "," << PosCmd[4] << "," << PosCmd[5] << ", 1)";

	VariantInit(&vntParam);
	vntParam.bstrVal = SysAllocString(CA2W(command.str().c_str()));
	vntParam.vt = VT_BSTR;
	bCap_RobotMove(fd, hRob, 1, vntParam, NULL);
	VariantClear(&vntParam);
}


void CMFCApplication1Dlg::OnBnClickedTest()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CMFCApplication1Dlg::OnBnClickedTool()
{
	bstr1 = SysAllocString(L"TOOL1");

	printf("SetTool as TOOL1");

	bCap_RobotChange(fd, hRob, bstr1);
	SysFreeString(bstr1);
	VariantClear(&vntRet);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
