// DlgMain.cpp: 实现文件
//

#include "stdafx.h"
#include "ARMeeting.h"
#include "DlgMain.h"
#include "afxdialogex.h"


// DlgMain 对话框

IMPLEMENT_DYNAMIC(DlgMain, CDialogEx)

DlgMain::DlgMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

DlgMain::~DlgMain()
{
}

void DlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);
	DDX_Control(pDX, IDC_EDCHNAME_CHANNEL, m_ctrChannel);
	DDX_Control(pDX, IDC_BTNJOIN_CHANNEL, m_btnJoin);
}


BEGIN_MESSAGE_MAP(DlgMain, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNCLOSE, &DlgMain::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNMIN, &DlgMain::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNJOIN_CHANNEL, &DlgMain::OnBnClickedBtnjoinChannel)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// DlgMain 消息处理程序


void DlgMain::OnCancel()
{
}


void DlgMain::OnOK()
{
}


void DlgMain::OnClose()
{
	DestroyWindow();

	OutputDebugString(_T("OnClose\n"));
}


void DlgMain::OnBnClickedBtnclose()
{
	PostMessage(WM_CLOSE);
}


void DlgMain::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}


void DlgMain::OnBnClickedBtnjoinChannel()
{
	CString strChannelName = GetChannelName();
	if (strChannelName.GetLength() == 0)
		return;

	ShowWindow(SW_HIDE);

	m_dlgVideo.Create(CVideoDlg::IDD, this);
	m_dlgVideo.MoveWindow(0, 0, 1280, 720, 1);
	m_dlgVideo.ShowWindow(SW_SHOW);
	m_dlgVideo.CenterWindow();
	m_dlgVideo.SetWindowText(strChannelName);

	IRtcEngine*pEngine = m_dlgVideo.GetEventHandler()->GetEngine();
	CHAR szChannelName[128];
	::WideCharToMultiByte(CP_ACP, 0, strChannelName, -1, szChannelName, 128, NULL, NULL);
	//pEngine->setParameters("{\"Cmd\":\"ConfPriCloudAddr\", \"ServerAdd\": \"192.168.199.245\", \"Port\": 6080}");
	//pEngine->setParameters("{\"Cmd\":\"SetQosParameter\", \"Ms\": 150}");
	pEngine->joinChannel(NULL, szChannelName, NULL, "");
}


BOOL DlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_penFrame.CreatePen(PS_SOLID, 1, RGB(0xD8, 0xD8, 0xD8));
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	CRect ClientRect;

	MoveWindow(0, 0, 720, 400, 1);
	GetClientRect(&ClientRect);


	m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0, 0xFF));

	m_btnJoin.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnJoin.SetFont(&m_ftBtn);
	m_btnJoin.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnJoin.SetWindowText(LANG_STR("IDS_CHN_BTJOIN"));

	m_ctrChannel.MoveWindow(ClientRect.Width() / 2 - 160, 128, 320, 22, TRUE);
	int height = 36;
	m_btnJoin.MoveWindow(ClientRect.Width() / 2 - 188, 172, 360, height, TRUE);

	CMFCButton::EnableWindowsTheming(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL DlgMain::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}


BOOL DlgMain::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_ESCAPE:
			return FALSE;
		case VK_RETURN:
			OnBnClickedBtnjoinChannel();
			return FALSE;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


void DlgMain::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
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
		DrawClient(&dc);
	}
}


HCURSOR DlgMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT DlgMain::OnNcHitTest(CPoint point)
{
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void DlgMain::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	CFont* defFont = lpDC->SelectObject(&m_ftTitle);


	GetClientRect(&rcClient);
	lpDC->FillSolidRect(0, 0, 720, 24, RGB(0, 161, 230));
	lpDC->SetBkColor(RGB(0x00, 0x9E, 0xEB));
	lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = LANG_STR("IDS_TITLE");
	lpDC->TextOut(rcClient.Width() / 2 - 80, 3, lpString, _tcslen(lpString));

	lpDC->SelectObject(&m_penFrame);
	rcText.SetRect(rcClient.Width() / 2 - 188, 120, rcClient.Width() / 2 + 172, 152);
	lpDC->RoundRect(&rcText, CPoint(32, 32));

	lpDC->SelectObject(defFont);
}


CString DlgMain::GetChannelName()
{
	CString strChannelName;

	m_ctrChannel.GetWindowText(strChannelName);

	return strChannelName;
}

