// VideoDlg.cpp 
//

#include "stdafx.h"
#include <string>
#include "ARMeeting.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "AGEventDef.h"

#define SHARE_UID	"share666"

// CVideoDlg

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
	, m_eventHandler(*this)
{
	m_brHead.CreateSolidBrush(RGB(0, 160, 239));
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
	m_bLastStat = FALSE;
	m_bFullScreen = FALSE;
    m_bShowInfo = FALSE;

	m_bHasBig = FALSE;
	m_bHasLocal = FALSE;
	m_bMicOn=TRUE;
	m_bCameraOn=TRUE;

	m_nSmallVideoHeight=240;
	m_nSmallVideoWidth=180;

	m_bHostMode = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CVideoDlg::~CVideoDlg()
{
	m_brHead.DeleteObject();
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);
	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()

	ON_MESSAGE(WM_SHOWMODECHANGED, &CVideoDlg::OnShowModeChanged)

	ON_MESSAGE(WM_WINDOWSHARE, &CVideoDlg::OnWindowShareStart)
	ON_MESSAGE(WM_DESKTOPSHARE, &CVideoDlg::OnDesktopShareStart)


	ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CVideoDlg::OnBnClickedBtnclose)

    ON_BN_CLICKED(IDC_BTNMESSAGE_VIDEO, &CVideoDlg::OnBnClickedBtnmessage)

	ON_BN_CLICKED(IDC_BTNSCRCAP_VIDEO, &CVideoDlg::OnBnClickedBtnScreenCapture)
    ON_BN_CLICKED(IDC_BTNMORE_VIDEO, &CVideoDlg::OnBnClickedBtnMore)
	ON_BN_CLICKED(IDC_BTNMODE_VIDEO, &CVideoDlg::OnBnClickedBtnmode)
	ON_BN_CLICKED(IDC_BTNAUDIO_VIDEO, &CVideoDlg::OnBnClickedBtnaudio)

    ON_BN_CLICKED(ID_IDR_VIDEOINFO, &CVideoDlg::OnBnClickedBtntip)
    ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)

	ON_BN_CLICKED(IDC_BTNSCR_VIDEO, &CVideoDlg::OnBnClickedBtnfullscr)
	
	ON_BN_CLICKED(ID_SCRSHARE_DESKTOPSHARE, &CVideoDlg::OnBnClickedScreenshare)
	ON_BN_CLICKED(ID_SCRSHARE_WINDOWSHARE, &CVideoDlg::OnBnClickedWindowshare)

	ON_WM_SHOWWINDOW()
    ON_WM_MOVE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVideoDlg


void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	ResizeControls();

	Invalidate(TRUE);
}

void CVideoDlg::ResizeControls()
{
	if (m_wndLocal.GetSafeHwnd() == NULL || m_wndVideo[0].GetSafeHwnd() == NULL || m_wndBig.GetSafeHwnd() == NULL)
		return;

	GetClientRect(&m_rcVideoArea);
	int cx = m_rcVideoArea.Width();
	int cy = m_rcVideoArea.Height();

	if (m_btnMin.GetSafeHwnd() != NULL)
		m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
	if (m_btnRst.GetSafeHwnd() != NULL)
		m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
	if (m_btnClose.GetSafeHwnd() != NULL)
		m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;



	UINT nSmallVideoCount = GetSmallVideoCount();
	if (nSmallVideoCount == 0) {
		m_nSmallVideoWidth = 0;
		m_nSmallVideoHeight = 0;

	}
	else if (nSmallVideoCount < 5) {
		nSmallVideoCount = 5;
		m_nSmallVideoWidth = m_rcVideoArea.Width()/ nSmallVideoCount;
		m_nSmallVideoHeight = m_nSmallVideoWidth*9/16;
	}
	else {
		m_nSmallVideoWidth = m_rcVideoArea.Width()/ nSmallVideoCount;
		m_nSmallVideoHeight = m_nSmallVideoWidth*9/16;
	}

	AdjustSizeVideoMulti(cx, cy);
	AdjustButtonsNormal(cx, cy);
}

void CVideoDlg::ShowButtonsNormal()
{
	CRect	rcClient;
	int nShowMode = SW_HIDE;

	if (!m_bFullScreen)
		nShowMode = SW_SHOW;

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(SW_HIDE);
	
	m_btnAudio.SetBackImage(IDB_BTN_MIC_ON, RGB(0x26, 0x26, 0x26));
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
	m_btnShow.ShowWindow(nShowMode);

	m_btnMore.ShowWindow(SW_HIDE);
	if (m_bHostMode) {
		if (m_bMicOn) {
			m_btnAudio.SetBackImage(IDB_BTN_MIC_ON);
		}
		else {
			m_btnAudio.SetBackImage(IDB_BTN_MIC_OFF);
		}

		if (m_bCameraOn) {
			m_btnMode.SetBackImage(IDB_BTN_CAMERA_ON);
		}
		else {
			m_btnMode.SetBackImage(IDB_BTN_CAMERA_OFF);
		}
	}
	else {
		m_btnAudio.ShowWindow(SW_HIDE);
		m_btnMode.ShowWindow(SW_HIDE);
		m_btnScrCap.ShowWindow(SW_HIDE);
		m_btnMore.ShowWindow(SW_HIDE);
	}

	GetClientRect(&rcClient);
	AdjustButtonsNormal(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsNormal(int cx, int cy)
{
//	if (m_btnSetup.GetSafeHwnd() != NULL)
//		m_btnSetup.MoveWindow(30, cy - 48, 24, 24, TRUE);

    if(m_btnMessage.GetSafeHwnd() != NULL)
        m_btnMessage.MoveWindow(cx / 2 - 312, cy - 60, 48, 48, TRUE);
	if (m_btnMode.GetSafeHwnd() != NULL)
		m_btnMode.MoveWindow(cx / 2 - 216, cy - 60, 48, 48, TRUE);
	if (m_btnAudio.GetSafeHwnd() != NULL)
		m_btnAudio.MoveWindow(cx / 2 - 120, cy - 60, 48, 48, TRUE);
	if (m_btnEndCall.GetSafeHwnd() != NULL)
		m_btnEndCall.MoveWindow(cx / 2 - 24, cy - 60, 48, 48, TRUE);

	if (m_btnScrCap.GetSafeHwnd() != NULL)
		m_btnScrCap.MoveWindow(cx / 2 + 72, cy - 60, 48, 48, TRUE);
    if (m_btnMore.GetSafeHwnd() != NULL)
        m_btnMore.MoveWindow(cx / 2 + 264, cy - 60, 48, 48, TRUE);

	if (m_btnShow.GetSafeHwnd() != NULL)
		m_btnShow.MoveWindow(cx - 72, cy - 48, 48, 48, TRUE);
}


void CVideoDlg::AdjustSizeVideoMulti(int cx, int cy)
{
	int nLocalIndex = 0;

	if (m_bHasBig)
	{
		CRect rcBig = m_rcVideoArea;
		rcBig.top = m_rcVideoArea.top + m_nSmallVideoHeight;

		UINT nWidth = 1280;
		UINT nHeight = 720;
		m_wndBig.GetVideoResolution(&nWidth, &nHeight);
		UINT nWndWidth = rcBig.Width();
		UINT nWndHeight = rcBig.Height();
		if (nWndWidth*nHeight >= nWidth * nWndHeight) {
			UINT nOffsetWidth = nWidth * nWndHeight / nHeight;
			int nOffset = (nWndWidth - nOffsetWidth) / 2;
			rcBig.left += nOffset;
			rcBig.right -= nOffset;
		}
		else {
			UINT nOffsetHeiht = nHeight *  nWndWidth / nWidth;
			int nOffset = (nWndHeight - nOffsetHeiht) / 2;
			rcBig.top += nOffset;
			rcBig.bottom -= nOffset;
		}
		m_wndBig.MoveWindow(&rcBig, TRUE);
		m_wndBig.ShowWindow(SW_SHOW);
		m_wndBig.SetParent(this);
	}

	int nSmallVideoCount = GetSmallVideoCount();
	int nOffsetX = 0;

	if (m_bHasLocal) {
		int nWidth = m_nSmallVideoWidth;
		int nHeight = m_nSmallVideoHeight;
		int nYPos = m_rcVideoArea.top;
		m_wndLocal.MoveWindow(0, nYPos, nWidth, nHeight, TRUE);
		m_wndLocal.ShowWindow(SW_SHOW);
		m_wndLocal.SetParent(this);

		nOffsetX += m_nSmallVideoWidth;
	}

	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		int nXPos = nOffsetX + (m_nSmallVideoWidth * nLocalIndex);
		int nYPos = m_rcVideoArea.top ;

		if (m_wndVideo[nIndex].GetUID().length() != 0) {
			int nWidth = m_nSmallVideoWidth;
			int nHeight = m_nSmallVideoHeight;
			m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, nWidth, nHeight, TRUE);
			m_wndVideo[nIndex].ShowWindow(SW_SHOW);
			m_wndVideo[nIndex].SetParent(this);
			nLocalIndex++;
		}
	}
}

void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CVideoDlg::NeedShowSizeBox(CPoint point)
{
	CRect rcClient;

	GetClientRect(&rcClient);

	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;
	
	if (rcClient.PtInRect(point) || IsZoomed())
		return FALSE;

	return TRUE;
}

void CVideoDlg::EnableSize(BOOL bEnable)
{
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (bEnable)
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle | WS_SIZEBOX);
	else
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle&(~WS_SIZEBOX));

	CRect rcClient;

	if (bEnable)
		GetWindowRect(&rcClient);
	else
		GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	if (m_bLastStat != bEnable) {
		m_bLastStat = bEnable;
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

//		if (m_btnTip.GetSafeHwnd() != NULL)
//			m_btnTip.MoveWindow(cx / 2 - 372, cy - 84, 72, 72, TRUE);

		if (m_btnScrCap.GetSafeHwnd() != NULL)
			m_btnScrCap.MoveWindow(48, cy - 84, 48, 48, TRUE);

//		if (m_btnSetup.GetSafeHwnd() != NULL)
//			m_btnSetup.MoveWindow(cx / 2 - 216, cy - 84, 72, 72, TRUE);
		if (m_btnMode.GetSafeHwnd() != NULL)
			m_btnMode.MoveWindow(cx / 2 - 96, cy - 84, 48, 48, TRUE);
		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 + 24, cy - 84, 48, 48, TRUE);
		if (m_btnShow.GetSafeHwnd() != NULL)
            m_btnShow.MoveWindow(cx - 72, cy - 84, 48, 48, TRUE);
		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx - 120, cy - 84, 48, 48, TRUE);

		Invalidate(FALSE);
	}
}

void CVideoDlg::OnPaint()
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
		DrawHead(&dc);
	}
}

HCURSOR CVideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CVideoDlg::OnNcHitTest(CPoint point)
{
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CVideoDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CVideoDlg::OnBnClickedBtnclose()
{
	CWnd*pMainWnd = GetParent();
	if (pMainWnd) {
		pMainWnd->ShowWindow(TRUE);
	}

	IRtcEngine*pEngine=m_eventHandler.GetEngine();
	pEngine->stopPreview();
	VideoCanvas vc;
	vc.uid = 0;
	vc.view = NULL;
	pEngine->setupLocalVideo(vc);
	int nRet = pEngine->leaveChannel();

    // unmute local video
    m_btnMode.SetBackImage(IDB_BTN_CAMERA_ON);

    // unmute local audio
    m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
    m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
    m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	PostMessage(WM_CLOSE);
}

void CVideoDlg::OnBnClickedBtnrest()
{
	if (IsZoomed()) {
		m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_RESTORE);
	}
	else{
		m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_MAXIMIZE);
	}

	CRect	rcClient;

	GetClientRect(&rcClient);
	m_rcVideoArea.CopyRect(&rcClient);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;
	  

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	AdjustSizeVideoMulti(cx, cy);
	AdjustButtonsNormal(cx, cy);

	Invalidate(TRUE);
}


void CVideoDlg::OnBnClickedBtnfullscr()
{
	int nShowMode = 0;

	if (m_bFullScreen) {
		m_bFullScreen = FALSE;
		nShowMode = SW_SHOW;		
		ShowWindow(SW_RESTORE);

		GetClientRect(&m_rcVideoArea);
		m_rcVideoArea.top += 24;
		m_rcVideoArea.bottom -= 72;	
	}
	else{
		m_bFullScreen = TRUE;
		nShowMode = SW_HIDE;
		ShowWindow(SW_MAXIMIZE);

		GetClientRect(&m_rcVideoArea);
	}

	m_btnMin.ShowWindow(nShowMode);
	m_btnRst.ShowWindow(nShowMode);
	m_btnClose.ShowWindow(nShowMode);

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	
	m_btnShow.ShowWindow(nShowMode);

	m_btnMore.ShowWindow(SW_HIDE);
	if (m_bHostMode) {

	}
	else {
		m_btnAudio.ShowWindow(SW_HIDE);
		m_btnMode.ShowWindow(SW_HIDE);
		m_btnScrCap.ShowWindow(SW_HIDE);
		m_btnMore.ShowWindow(SW_HIDE);
	}

	ShowMulti();

	Invalidate(TRUE);
}

void CVideoDlg::OnBnClickedScreenshare()
{
	m_dlgDesktopCapture.SaveScreen(NULL);
	m_dlgDesktopCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::OnBnClickedWindowshare()
{
	m_dlgScreenCapture.RefreashWndInfo();
	m_dlgScreenCapture.SaveScreen(NULL);

	m_dlgScreenCapture.ShowWindow(SW_MAXIMIZE);
}


void CVideoDlg::OnBnClickedBtntip()
{
	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++)
        m_wndVideo[nIndex].ShowVideoInfo(!m_bShowInfo);

    m_bShowInfo = !m_bShowInfo;
}

void CVideoDlg::OnBnClickedBtnScreenCapture()
{
	if (m_eventHandler.IsScreenCaptureEnabled()) {
		m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		m_eventHandler.EnableScreenCapture(NULL, 0, NULL, FALSE);
	}
	else
		CreateScreenShareMenu();

	Invalidate();
}

void CVideoDlg::OnBnClickedBtnMore()
{
}

void CVideoDlg::CreateScreenShareMenu()
{
	CRect				rcButton;
	CMenu				Menu;
	MENUITEMINFO		mii;

	Menu.LoadMenu(IDR_MENU_SCRSHARE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_SCRSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_SCRSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
	
	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_WNDSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_WNDSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
	m_btnScrCap.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}

void CVideoDlg::OnBnClickedBtnsetup()
{
}

void CVideoDlg::OnBnClickedBtnmessage()
{
	IRtcEngine *lpRtcEngine = m_eventHandler.GetEngine();
	if (m_bHostMode) {
		m_bHostMode = false;
		lpRtcEngine->setClientRole(AR::CLIENT_ROLE_AUDIENCE);

		m_eventHandler.LocalVideoPreview(m_wndLocal.GetSafeHwnd(), false);
		m_bHasLocal = false;

		m_btnMessage.SetBackImage(IDB_BTN_SMAI);
	}
	else {
		m_bHostMode = true;
		lpRtcEngine->setClientRole(AR::CLIENT_ROLE_BROADCASTER);


		m_btnMessage.SetBackImage(IDB_BTN_XMAI);


		if (m_bMicOn) {
			m_eventHandler.MuteLocalAudio(false);
			m_btnAudio.SetBackImage(IDB_BTN_MIC_ON);
		}
		else {
			m_eventHandler.MuteLocalAudio(true);
			m_btnAudio.SetBackImage(IDB_BTN_MIC_OFF);
		}

		if (m_bCameraOn) {
			m_btnMode.SetBackImage(IDB_BTN_CAMERA_ON);
			m_eventHandler.LocalVideoPreview(m_wndLocal.GetSafeHwnd(), true);
		}
		else {
			m_btnMode.SetBackImage(IDB_BTN_CAMERA_OFF);
		}
	}
	ShowMulti();
}

void CVideoDlg::OnBnClickedBtnmode()
{
	if (!m_eventHandler.IsLocalVideoMuted()) {
		m_eventHandler.LocalVideoPreview(m_wndLocal.GetSafeHwnd(), false);

		m_btnMode.SetBackImage(IDB_BTN_CAMERA_OFF);
		m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96);
		m_wndLocal.ShowBackground(TRUE);

		m_bCameraOn = FALSE;
		m_bHasLocal = false;
		ShowMulti();
	}
	else {
		m_eventHandler.LocalVideoPreview(m_wndLocal.GetSafeHwnd(), true);

		m_btnMode.SetBackImage(IDB_BTN_CAMERA_ON);
		m_wndLocal.ShowBackground(FALSE);

		m_bCameraOn = TRUE;
	}
}

void CVideoDlg::OnBnClickedBtnaudio()
{
	if (m_eventHandler.IsLocalAudioMuted()) {
		m_eventHandler.MuteLocalAudio(false);

		m_btnAudio.SetBackImage(IDB_BTN_MIC_ON);
		m_bMicOn = TRUE;
	}
	else {
		m_eventHandler.MuteLocalAudio(true);

		m_btnAudio.SetBackImage(IDB_BTN_MIC_OFF);
		m_bMicOn = FALSE;
	}
}

void CVideoDlg::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	m_btnMessage.EnableWindow(TRUE);
}

void CVideoDlg::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
}

void CVideoDlg::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	m_bHasLocal = true;
	ShowMulti();
}

void CVideoDlg::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{
	if (strcmp(uid, SHARE_UID) == 0) {
		VideoCanvas canvas;
		canvas.renderMode = RENDER_MODE_FIT;
		canvas.uid = uid;
		m_bHasBig = true;
		m_wndBig.SetUID(canvas.uid);
		m_wndBig.SetVideoResolution(width, height);
		ShowMulti();

		canvas.view = m_wndBig.GetSafeHwnd();
		m_eventHandler.GetEngine()->setupRemoteVideo(canvas);
		m_wndBig.ShowWindow(SW_SHOW);

		return;
	}


	BOOL bFound = FALSE;
	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		std::string strUID = m_wndVideo[nIndex].GetUID();
		if (strUID == uid) {
			bFound = TRUE;
			break;
		}
	}
	if (!bFound) {
		int nFreeIndex = 0;
		for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
			std::string strUID = m_wndVideo[nIndex].GetUID();
			if (strUID.length()==0) {
				nFreeIndex = nIndex;
				break;
			}
		}
		m_wndVideo[nFreeIndex].SetUID(uid);
		m_wndVideo[nFreeIndex].SetVideoResolution(width, height);
		ShowMulti();

		VideoCanvas canvas;
		canvas.renderMode = RENDER_MODE_FIT;
		canvas.uid = uid;
		canvas.view = m_wndVideo[nFreeIndex].GetSafeHwnd();
		m_eventHandler.GetEngine()->setupRemoteVideo(canvas);

	}
}

void CVideoDlg::onUserJoined(uid_t uid, int elapsed)
{
	std::string strUID = uid;
	m_MapUID[strUID] = strUID;
	if (m_MapUID.size() < MAX_DLG) {
		m_btnMessage.EnableWindow(TRUE);
	}
	else {
		m_btnMessage.EnableWindow(FALSE);
	}

	if (strcmp(uid, SHARE_UID) == 0) {
		m_btnScrCap.EnableWindow(FALSE);
	}
}

void CVideoDlg::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	std::string strUID = uid;
	m_MapUID.erase(strUID);

	if (m_MapUID.size() < MAX_DLG) {
		m_btnMessage.EnableWindow(TRUE);
	}
	else {
		m_btnMessage.EnableWindow(FALSE);
	}

	if (strcmp(uid, SHARE_UID) == 0) {
		VideoCanvas canvas;
		canvas.uid = uid;
		canvas.view = NULL;
		m_eventHandler.GetEngine()->setupRemoteVideo(canvas);

		m_wndBig.SetUID("");
		m_bHasBig = false;
		m_wndBig.ShowWindow(SW_HIDE);

		m_btnScrCap.EnableWindow(TRUE);

		return;
	}

	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		std::string strUID = m_wndVideo[nIndex].GetUID();
		if (strUID == uid) {
			m_wndVideo[nIndex].SetUID("");
			m_wndVideo[nIndex].ShowWindow(SW_HIDE);

			VideoCanvas canvas;
			canvas.uid = strUID.c_str();
			canvas.view = NULL;

			m_eventHandler.GetEngine()->setupRemoteVideo(canvas);

			ResizeControls();
			break;
		}
	}
}

void CVideoDlg::onLocalVideoStats(const LocalVideoStats& stats)
{
	m_wndLocal.SetFrameRateInfo(stats.rendererOutputFrameRate);
	m_wndLocal.SetBitrateInfo(stats.encodedBitrate);
	m_wndLocal.SetVideoResolution(stats.encodedFrameWidth, stats.encodedFrameHeight);
}

void CVideoDlg::onRemoteVideoStats(const RemoteVideoStats& stats)
{  
	if (strcmp(stats.uid,SHARE_UID) == 0) {
		m_wndBig.SetFrameRateInfo(stats.rendererOutputFrameRate);
		m_wndBig.SetBitrateInfo(stats.receivedBitrate);
		m_wndBig.SetVideoResolution(stats.width, stats.height);
		return;
	}

	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		std::string strUID = m_wndVideo[nIndex].GetUID();
		if (strUID == stats.uid) {
			m_wndVideo[nIndex].SetFrameRateInfo(stats.rendererOutputFrameRate);
			m_wndVideo[nIndex].SetBitrateInfo(stats.receivedBitrate);
			m_wndVideo[nIndex].SetVideoResolution(stats.width, stats.height);
			break;
		}
	}
}

void CVideoDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;

	if (m_bFullScreen)
		return;

	GetWindowText(strTitle);
	GetClientRect(&rcTitle);
	CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);

	rcTitle.bottom = 24;
	pDC->FillRect(&rcTitle, &m_brHead);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SelectObject(lpDefaultBrush);
}

void CVideoDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	
	GetClientRect(&rcClient);

    m_btnMessage.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMESSAGE_VIDEO);
    m_btnMode.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMODE_VIDEO);
	m_btnAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNAUDIO_VIDEO);
	m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNENDCALL_VIDEO);
	m_btnScrCap.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCRCAP_VIDEO);
    m_btnMore.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMORE_VIDEO);

	m_btnShow.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCR_VIDEO);
	
	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++){
		m_wndVideo[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
		m_wndVideo[nIndex].SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
		m_wndVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
		m_wndVideo[nIndex].SetUID(NULL);
		m_wndVideo[nIndex].SetParent(this);
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].MoveWindow(0,0, 96, 96,TRUE);
	}

	m_wndLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + MAX_DLG);
	m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndLocal.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndLocal.SetUID(NULL);
	m_wndLocal.SetParent(this);
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.MoveWindow(0, 0, 96, 96, TRUE);

	m_wndBig.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + MAX_DLG + 1);
	m_wndBig.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndBig.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndBig.SetUID(NULL);
	m_wndBig.SetParent(this);
	m_wndBig.ShowWindow(SW_HIDE);
	m_wndBig.MoveWindow(0, 0, 96, 96, TRUE);

	m_btnMin.MoveWindow(rcClient.Width() - 72, 0, 22, 22, TRUE);
	m_btnRst.MoveWindow(rcClient.Width() - 48, 0, 22, 22, TRUE);
	m_btnClose.MoveWindow(rcClient.Width() - 24, 0, 22, 22, TRUE);

    m_btnMessage.MoveWindow(rcClient.Width() / 2 - 144, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMode.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnShow.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMore.MoveWindow(rcClient.Width() / 2 + 264, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnEndCall.MoveWindow(rcClient.Width() - 120, rcClient.Height() - 84, 48, 48, TRUE);
    
	m_wndVideo[0].MoveWindow(0, 24, rcClient.Width(), rcClient.Height() - 96, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));

	m_btnScrCap.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnScrCap.EnableFrameEffect(FALSE);
	m_btnScrCap.SetBackImage(IDB_BTNSCRCAP_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnMessage.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMessage.EnableFrameEffect(FALSE);
    m_btnMessage.SetBackImage(IDB_BTN_SMAI, RGB(0x26, 0x26, 0x26));

	m_btnMode.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnMode.EnableFrameEffect(FALSE);
	m_btnMode.SetBackImage(IDB_BTN_CAMERA_ON, RGB(0x26, 0x26, 0x26));

	m_btnAudio.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAudio.EnableFrameEffect(FALSE);
	m_btnAudio.SetBackImage(IDB_BTN_MIC_ON, RGB(0x26, 0x26, 0x26));
	
	m_btnShow.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnShow.EnableFrameEffect(FALSE);
	m_btnShow.SetBackImage(IDB_BTNFULLSCR_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnEndCall.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEndCall.EnableFrameEffect(FALSE);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnMore.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMore.EnableFrameEffect(FALSE);
    m_btnMore.SetBackImage(IDB_BTNMORE_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnAudio.ShowWindow(SW_HIDE);
	m_btnMode.ShowWindow(SW_HIDE);
	m_btnScrCap.ShowWindow(SW_HIDE);
	m_btnMore.ShowWindow(SW_HIDE);
}

void CVideoDlg::ShowMulti()
{
	ResizeControls();

	int nLocalIndex = 0;

	if (m_bHasBig)
	{
		m_wndBig.ShowWindow(SW_SHOW);

		CRect rcBig= m_rcVideoArea;
		rcBig.top = m_rcVideoArea.top + m_nSmallVideoHeight;

		UINT nWidth = 1280;
		UINT nHeight = 720;
		m_wndBig.GetVideoResolution(&nWidth, &nHeight);
		UINT nWndWidth = rcBig.Width();
		UINT nWndHeight = rcBig.Height();
		if (nWndWidth*nHeight >= nWidth * nWndHeight) {
			UINT nOffsetWidth = nWidth * nWndHeight / nHeight;
			int nOffset = (nWndWidth - nOffsetWidth) / 2;
			rcBig.left += nOffset;
			rcBig.right -= nOffset;
		}
		else {
			UINT nOffsetHeiht = nHeight * nWndWidth / nWidth;
			int nOffset = (nWndHeight - nOffsetHeiht) / 2;
			rcBig.top += nOffset;
			rcBig.bottom -= nOffset;
		}

		m_wndBig.MoveWindow(&rcBig, TRUE);
		m_wndBig.ShowWindow(SW_SHOW);
		m_wndBig.SetParent(this);
	}

	
	int nSmallVideoCount = GetSmallVideoCount();
	int nOffsetX = 0;

	if (m_bHasLocal) {
		int nWidth = m_nSmallVideoWidth;
		int nHeight = m_nSmallVideoHeight;
		int nYPos = m_rcVideoArea.top;
		m_wndLocal.MoveWindow(0, nYPos, nWidth, nHeight, TRUE);
		m_wndLocal.ShowWindow(SW_SHOW);
		m_wndLocal.SetParent(this);

		nOffsetX += m_nSmallVideoWidth;
	}
	else {
		m_wndLocal.ShowWindow(SW_HIDE);
	}


	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		int nXPos = nOffsetX + (m_nSmallVideoWidth * nLocalIndex);
		int nYPos = m_rcVideoArea.top;

		if (m_wndVideo[nIndex].GetUID().length() != 0) {
			int nWidth = m_nSmallVideoWidth;
			int nHeight = m_nSmallVideoHeight;
			m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, nWidth, nHeight, TRUE);
			m_wndVideo[nIndex].ShowWindow(SW_SHOW);
			m_wndVideo[nIndex].SetParent(this);
			nLocalIndex++;
		}
	}


	ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_MapUID.clear();

	m_bLastStat = FALSE;
	m_bFullScreen = FALSE;
	m_bShowInfo = FALSE;

	m_bHasBig = FALSE;
	m_bHasLocal = FALSE;

	m_nSmallVideoHeight = 240;
	m_nSmallVideoWidth = 180;

	m_bHostMode = false;


	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_dlgScreenCapture.Create(CAGScreenCaptureDlg::IDD, this);
	m_dlgDesktopCapture.Create(CAGDesktopCaptureDlg::IDD, this);
	InitCtrls();
    
	m_btnMessage.EnableWindow(FALSE);

	m_eventHandler.Start();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}


HWND CVideoDlg::GetRemoteVideoWnd(int nIndex)
{
	if (nIndex < 0 || nIndex > 3)
		return NULL;

	return m_wndVideo[nIndex].GetSafeHwnd();
}

UINT CVideoDlg::GetSmallVideoCount()
{
	UINT nCount = 0;

	for (int nIndex = 0; nIndex < MAX_DLG; nIndex++) {
		if (m_wndVideo[nIndex].GetUID().length() != 0) {
			nCount++;
		}
	}

	if (m_bHasLocal) {
		nCount += 1;
	}

	return nCount;
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			if (m_bFullScreen)
				OnBnClickedBtnfullscr();
			
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CVideoDlg::OnShowModeChanged(WPARAM wParam, LPARAM lParam)
{
	ShowMulti();

	return 0;
}

LRESULT CVideoDlg::OnWindowShareStart(WPARAM wParam, LPARAM lParam)
{
	if (!m_btnScrCap.IsWindowEnabled()) {
		return 0;
	}

    HWND hShareWnd = (HWND)wParam;

	m_eventHandler.EnableScreenCapture((HWND)wParam, 15, NULL, TRUE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}

LRESULT CVideoDlg::OnDesktopShareStart(WPARAM wParam, LPARAM lParam)
{
	if (!m_btnScrCap.IsWindowEnabled()) {
		return 0;
	}

	LPDESKTOP_SHARE_PARAM	lpDesktopShareParam = (LPDESKTOP_SHARE_PARAM)(wParam);
	CRect rcRegion;


	rcRegion.left = lpDesktopShareParam->nX;
	rcRegion.top = lpDesktopShareParam->nY;
	rcRegion.right = rcRegion.left + lpDesktopShareParam->nWidth;
	rcRegion.bottom = rcRegion.top + lpDesktopShareParam->nHeight;

	m_eventHandler.EnableScreenCapture(NULL, lpDesktopShareParam->nFPS, &rcRegion, TRUE);

	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}


BOOL CVideoDlg::DestroyWindow()
{
	m_eventHandler.Stop();

	return CDialogEx::DestroyWindow();
}


void CVideoDlg::OnCancel()
{
}


void CVideoDlg::OnOK()
{
}


void CVideoDlg::OnClose()
{
	DestroyWindow();
}
