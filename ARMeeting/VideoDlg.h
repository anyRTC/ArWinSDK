#pragma once
#include "AGButton.h"
#include "AGVideoWnd.h"
#include "AGScreenCaptureDlg.h"
#include "AGDesktopCaptureDlg.h"

#include <map>

// CVideoDlg 对话框


#define MAX_DLG	7

class CVideoDlg 
	: public CDialogEx
	, public IRtcEngineEventHandler
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

// 对话框数据
	enum { IDD = IDD_VIDEO_DIALOG };



	HWND GetRemoteVideoWnd(int nIndex);
	HWND GetLocalVideoWnd() { return m_wndLocal.GetSafeHwnd(); };

	UINT GetSmallVideoCount();
	void ResizeControls();
public:
	HICON m_hIcon;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

    afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnShowModeChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWindowShareStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDesktopShareStart(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnBnClickedBtnrest();

    afx_msg void OnBnClickedBtnmessage();
    afx_msg void OnBnClickedBtnmode();
	afx_msg void OnBnClickedBtnaudio();
	afx_msg void OnBnClickedBtnScreenCapture();
    afx_msg void OnBnClickedBtnMore();
	
    afx_msg void OnBnClickedBtntip();
    afx_msg void OnBnClickedBtnsetup();

	afx_msg void OnBnClickedBtnfullscr();

	afx_msg void OnBnClickedScreenshare();
	afx_msg void OnBnClickedWindowshare();

	DECLARE_MESSAGE_MAP()

public:
	void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
	void onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
	void onFirstLocalVideoFrame(int width, int height, int elapsed);

	void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed);

	void onUserJoined(uid_t uid, int elapsed);
	void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason);
	void onLocalVideoStats(const LocalVideoStats& stats);
	void onRemoteVideoStats(const RemoteVideoStats& stats);

protected:
	BOOL NeedShowSizeBox(CPoint point);
	void EnableSize(BOOL bEnable);
	void CreateScreenShareMenu();

	void DrawHead(CDC *pDC);

	void InitCtrls();

	void ShowMulti();

	void ShowButtonsNormal();

	void AdjustButtonsNormal(int cx, int cy);
	void AdjustSizeVideoMulti(int cx, int cy);

private:
	CBrush			m_brHead;

	CAGButton		m_btnMin;
	CAGButton		m_btnRst;
	CAGButton		m_btnClose;

    CAGButton       m_btnMessage;
	CAGButton		m_btnMode;
	CAGButton		m_btnAudio;
	CAGButton		m_btnEndCall;
	CAGButton		m_btnScrCap;
    CAGButton       m_btnMore;

	CAGButton		m_btnShow;

	BOOL			m_bLastStat;
	UINT			m_nBigShowedUID;
	
	BOOL			m_bHasLocal;
	BOOL			m_bHasBig;
	CAGVideoWnd		m_wndBig;
	CAGVideoWnd		m_wndLocal;
	CAGVideoWnd		m_wndVideo[MAX_DLG];

	
	CAGScreenCaptureDlg		m_dlgScreenCapture;
	CAGDesktopCaptureDlg	m_dlgDesktopCapture;

	CRect			m_rcVideoArea;

	int				m_nSmallVideoHeight;
	int				m_nSmallVideoWidth;

	bool			m_bVideoMuted;
	bool			m_bHostMode;
private:	// data	
	BOOL			m_bRecording;
	BOOL			m_bFullScreen;
    BOOL            m_bShowInfo;
	BOOL			m_bMicOn;
	BOOL			m_bCameraOn;

	CAGEngineEventHandler m_eventHandler;

	typedef std::map<std::string, std::string>	MapUID;
	MapUID			m_MapUID;
public:
	CAGEngineEventHandler*GetEventHandler() { return &m_eventHandler; };
public:
	virtual BOOL DestroyWindow();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
};
