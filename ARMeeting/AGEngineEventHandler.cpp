#include "StdAfx.h"
#include "AGEngineEventHandler.h"
#include "AGEventDef.h"

#define WM_USER_MSG				(WM_USER+1)


IMPLEMENT_DYNAMIC(CAGEngineEventHandler, CWnd)
CAGEngineEventHandler::CAGEngineEventHandler(IRtcEngineEventHandler&rEventHandler)
	: m_rEventHandler(rEventHandler)
	, m_lpArEngine(NULL)
	, m_bVideoEnable(false)
	, m_bLocalAudioMuted(false)
	, m_bScreenCapture(false)
{
	m_bLocalVideoMuted = false;

}

CAGEngineEventHandler::~CAGEngineEventHandler(void)
{
}

CString CAGEngineEventHandler::LoadAppID()
{
	TCHAR szFilePath[MAX_PATH];
	CString strAppID(APP_ID);

	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	LPTSTR lpLastSlash = _tcsrchr(szFilePath, _T('\\'));

	if (lpLastSlash == NULL)
		return strAppID;

	SIZE_T nNameLen = MAX_PATH - (lpLastSlash - szFilePath + 1);
	_tcscpy_s(lpLastSlash + 1, nNameLen, _T("AppID.ini"));

	if (!PathFileExists(szFilePath)) {
		HANDLE handle = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
		CloseHandle(handle);
	}

	TCHAR szAppid[MAX_PATH] = { 0 };
	::GetPrivateProfileString(_T("AppID"), _T("AppID"), NULL, szAppid, MAX_PATH, szFilePath);
	if (_tcslen(szAppid) == 0) {
		::WritePrivateProfileString(_T("AppID"), _T("AppID"), _T(""), szFilePath);
		::ShellExecute(NULL, _T("open"), szFilePath, NULL, NULL, SW_MAXIMIZE);
	}

	strAppID = szAppid;

	return strAppID;
}


BEGIN_MESSAGE_MAP(CAGEngineEventHandler, CWnd)
	ON_MESSAGE(WM_USER_MSG, OnMsg)
END_MESSAGE_MAP()

LRESULT CAGEngineEventHandler::OnMsg(WPARAM wParam, LPARAM lParam)
{
	DWORD dwEID = (DWORD)wParam;
	switch (dwEID)
	{
	case EID_JOINCHANNEL_SUCCESS:
		{
			LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)lParam;
			m_rEventHandler.onJoinChannelSuccess(lpData->channel, lpData->uid, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_REJOINCHANNEL_SUCCESS:
		{
			LPAGE_REJOINCHANNEL_SUCCESS lpData = (LPAGE_REJOINCHANNEL_SUCCESS)lParam;
			m_rEventHandler.onRejoinChannelSuccess(lpData->channel, lpData->uid, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_ERROR:
		{
			LPAGE_ERROR lpData = (LPAGE_ERROR)lParam;
			m_rEventHandler.onError(lpData->err, lpData->msg);
			if (lpData->msg) delete lpData->msg;
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_AUDIO_QUALITY:
		{
			LPAGE_AUDIO_QUALITY lpData = (LPAGE_AUDIO_QUALITY)lParam;
			m_rEventHandler.onAudioQuality(lpData->uid, lpData->quality, lpData->delay, lpData->lost);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_AUDIO_VOLUME_INDICATION:
		{
			LPAGE_AUDIO_VOLUME_INDICATION lpData = (LPAGE_AUDIO_VOLUME_INDICATION)lParam;
			m_rEventHandler.onAudioVolumeIndication(lpData->speakers, lpData->speakerNumber, lpData->totalVolume);
			delete lpData->speakers;
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_LEAVE_CHANNEL:
		{
			LPAGE_LEAVE_CHANNEL lpData = (LPAGE_LEAVE_CHANNEL)lParam;
			m_rEventHandler.onLeaveChannel(lpData->rtcStat);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_AUDIO_DEVICE_STATE_CHANGED:
		{
			LPAGE_AUDIO_DEVICE_STATE_CHANGED lpData = (LPAGE_AUDIO_DEVICE_STATE_CHANGED)lParam;
			m_rEventHandler.onAudioDeviceStateChanged(lpData->deviceId, lpData->deviceType, lpData->deviceState);
			delete lpData->deviceId;
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_VIDEO_DEVICE_STATE_CHANGED:
		{
			LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = (LPAGE_VIDEO_DEVICE_STATE_CHANGED)lParam;
			m_rEventHandler.onVideoDeviceStateChanged(lpData->deviceId, lpData->deviceType, lpData->deviceState);
			delete lpData->deviceId;
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_LASTMILE_QUALITY:
		{
			LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)lParam;
			m_rEventHandler.onLastmileQuality(lpData->quality);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_FIRST_LOCAL_VIDEO_FRAME:
		{
			LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = (LPAGE_FIRST_LOCAL_VIDEO_FRAME)lParam;
			m_rEventHandler.onFirstLocalVideoFrame(lpData->width, lpData->height, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_FIRST_REMOTE_VIDEO_DECODED:
		{
			LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)lParam;
			m_rEventHandler.onFirstRemoteVideoDecoded(lpData->uid, lpData->width, lpData->height, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_FIRST_REMOTE_VIDEO_FRAME:
		{
			LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = (LPAGE_FIRST_REMOTE_VIDEO_FRAME)lParam;
			m_rEventHandler.onFirstRemoteVideoFrame(lpData->uid, lpData->width, lpData->height, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_USER_JOINED:
		{
			LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)lParam;
			m_rEventHandler.onUserJoined(lpData->uid, lpData->elapsed);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_USER_OFFLINE:
		{
			LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)lParam;
			m_rEventHandler.onUserOffline(lpData->uid, lpData->reason);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_USER_MUTE_AUDIO:
		{
			LPAGE_USER_MUTE_AUDIO lpData = (LPAGE_USER_MUTE_AUDIO)lParam;
			m_rEventHandler.onUserMuteAudio(lpData->uid, lpData->muted);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_USER_MUTE_VIDEO:
		{
			LPAGE_USER_MUTE_VIDEO lpData = (LPAGE_USER_MUTE_VIDEO)lParam;
			m_rEventHandler.onUserMuteVideo(lpData->uid, lpData->muted);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_STREAM_MESSAGE:
		{
			LPAGE_STREAM_MESSAGE lpData = (LPAGE_STREAM_MESSAGE)lParam;
			m_rEventHandler.onStreamMessage(lpData->uid, lpData->streamId, lpData->data, lpData->length);
			delete lpData->data;
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_LOCAL_VIDEO_STAT:
		{
			LPAGE_LOCAL_VIDEO_STAT lpData = (LPAGE_LOCAL_VIDEO_STAT)lParam;
			m_rEventHandler.onLocalVideoStats(lpData->stats);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_REMOTE_VIDEO_STAT:
		{
			LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)lParam;
			m_rEventHandler.onRemoteVideoStats(lpData->stats);
			delete lpData;
			lpData = NULL;
		}
		break;
	case EID_CAMERA_READY:
		{
			m_rEventHandler.onCameraReady();
		}
		break;
	case EID_VIDEO_STOPPED:
		{
			m_rEventHandler.onVideoStopped();
		}
		break;
	case EID_CONNECTION_LOST:
		{
			m_rEventHandler.onConnectionLost();
		}
		break;

	default:
		break;
	}
	return 0;
}

int CAGEngineEventHandler::Start()
{
	if (!CreateEx(WS_EX_CLIENTEDGE, AfxRegisterWndClass(CS_CLASSDC), _T("CAGEngineEventHandler"), WS_POPUP, 0, 0, 1, 1, NULL, NULL))
	{
		return -1;
	}

	RtcEngineContext ctx;
	ctx.eventHandler = this;
	CString strAppID = CAGEngineEventHandler::LoadAppID();

	if (_tcslen(strAppID) == 0) {
		MessageBox(_T("Please define your own APP_ID in source code"), _T("information"), MB_OK | MB_ICONINFORMATION);
		::PostQuitMessage(0);
	}
#ifdef UNICODE
	char szVendorKey[128];

	::WideCharToMultiByte(CP_ACP, 0, strAppID, -1, szVendorKey, 128, NULL, NULL);
	ctx.appId = szVendorKey;
#else
	ctx.appId = strAppID;
#endif

	m_lpArEngine = (IRtcEngine *)createARRtcEngine();
	m_lpArEngine->initialize(ctx);
	m_lpArEngine->setClientRole(AR::CLIENT_ROLE_AUDIENCE);
	EnableVideo();

	return 0;
}

void CAGEngineEventHandler::Stop()
{
	if (m_lpArEngine != NULL)
	{
		m_lpArEngine->stopAllEffects();
		m_lpArEngine->disableVideo();
		m_lpArEngine->leaveChannel();
		m_lpArEngine->release(true);
		m_lpArEngine = NULL;
	}

	if (CWnd::GetSafeHwnd())
	{
		DestroyWindow();
	}
}


void CAGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = new AGE_JOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);

	PostMessage(WM_USER_MSG, EID_JOINCHANNEL_SUCCESS, (LPARAM)lpData);
}

void CAGEngineEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = new AGE_REJOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);

	PostMessage(WM_USER_MSG, EID_REJOINCHANNEL_SUCCESS, (LPARAM)lpData);
}

void CAGEngineEventHandler::onWarning(int warn, const char* msg)
{
}

void CAGEngineEventHandler::onError(int err, const char* msg)
{
	LPAGE_ERROR lpData = new AGE_ERROR;

	int nMsgLen = 0;

	// attention: the pointer of msg maybe NULL!!!
	if(msg != NULL) {
		nMsgLen = strlen(msg) + 1;
		lpData->msg = new char[nMsgLen];
		strcpy_s(lpData->msg, nMsgLen, msg);
	}
	else
		lpData->msg = NULL;
	
	lpData->err = err;

	PostMessage(WM_USER_MSG, EID_ERROR, (LPARAM)lpData);
}

void CAGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
	LPAGE_AUDIO_QUALITY lpData = new AGE_AUDIO_QUALITY;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->quality = quality;
	lpData->delay = delay;
	lpData->lost = lost;

	PostMessage(WM_USER_MSG, EID_AUDIO_QUALITY, (LPARAM)lpData);
}


void CAGEngineEventHandler::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = new AGE_AUDIO_VOLUME_INDICATION;

	lpData->speakers = new AudioVolumeInfo[speakerNumber];
	memcpy(lpData->speakers, speakers, speakerNumber*sizeof(AudioVolumeInfo));
	lpData->speakerNumber = speakerNumber;
	lpData->totalVolume = totalVolume;

	PostMessage(WM_USER_MSG, EID_AUDIO_VOLUME_INDICATION, (LPARAM)lpData);
}

void CAGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
	LPAGE_LEAVE_CHANNEL lpData = new AGE_LEAVE_CHANNEL;

	memcpy(&lpData->rtcStat, &stat, sizeof(RtcStats));

	PostMessage(WM_USER_MSG, EID_LEAVE_CHANNEL, (LPARAM)lpData);
}

void CAGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
}

void CAGEngineEventHandler::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	LPAGE_AUDIO_DEVICE_STATE_CHANGED lpData = new AGE_AUDIO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	PostMessage(WM_USER_MSG, EID_AUDIO_DEVICE_STATE_CHANGED, (LPARAM)lpData);
}

void CAGEngineEventHandler::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = new AGE_VIDEO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	PostMessage(WM_USER_MSG, EID_VIDEO_DEVICE_STATE_CHANGED, (LPARAM)lpData);
}

void CAGEngineEventHandler::onLastmileQuality(int quality)
{
    LPAGE_LASTMILE_QUALITY lpData = new AGE_LASTMILE_QUALITY;

	lpData->quality = quality;

	PostMessage(WM_USER_MSG, EID_LASTMILE_QUALITY, (LPARAM)lpData);
}

void CAGEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = new AGE_FIRST_LOCAL_VIDEO_FRAME;

	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	PostMessage(WM_USER_MSG, EID_FIRST_LOCAL_VIDEO_FRAME, (LPARAM)lpData);
}

void CAGEngineEventHandler::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = new AGE_FIRST_REMOTE_VIDEO_DECODED;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	PostMessage(WM_USER_MSG, EID_FIRST_REMOTE_VIDEO_DECODED, (LPARAM)lpData);
}

void CAGEngineEventHandler::onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed)
{
	LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = new AGE_FIRST_REMOTE_VIDEO_FRAME;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	PostMessage(WM_USER_MSG, EID_FIRST_REMOTE_VIDEO_FRAME, (LPARAM)lpData);
}

void CAGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
	LPAGE_USER_JOINED lpData = new AGE_USER_JOINED;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->elapsed = elapsed;

	PostMessage(WM_USER_MSG, EID_USER_JOINED, (LPARAM)lpData);
}

void CAGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	LPAGE_USER_OFFLINE lpData = new AGE_USER_OFFLINE;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->reason = reason;

	PostMessage(WM_USER_MSG, EID_USER_OFFLINE, (LPARAM)lpData);
}

void CAGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
	LPAGE_USER_MUTE_AUDIO lpData = new AGE_USER_MUTE_AUDIO;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->muted = muted;

	PostMessage(WM_USER_MSG, EID_USER_MUTE_AUDIO, (LPARAM)lpData);
}

void CAGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
	LPAGE_USER_MUTE_VIDEO lpData = new AGE_USER_MUTE_VIDEO;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->muted = muted;

	PostMessage(WM_USER_MSG, EID_USER_MUTE_VIDEO, (LPARAM)lpData);
}

void CAGEngineEventHandler::onStreamMessage(uid_t uid, int streamId, const char* data, size_t length)
{
    LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;

	if (uid != NULL) {
		strcpy(lpData->uid, uid);
	}
	else {
		lpData->uid[0] = '\0';
	}
	lpData->streamId = streamId;
    lpData->data = new char[length];
    lpData->length = length;

    memcpy_s(lpData->data, length, data, length);

	PostMessage(WM_USER_MSG, EID_STREAM_MESSAGE, (LPARAM)lpData);
}


void CAGEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{
	LPAGE_LOCAL_VIDEO_STAT lpData = new AGE_LOCAL_VIDEO_STAT;
	memcpy(&(lpData->stats), &stats, sizeof(LocalVideoStats));

	PostMessage(WM_USER_MSG, EID_LOCAL_VIDEO_STAT, (LPARAM)lpData);
}

void CAGEngineEventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
{
	LPAGE_REMOTE_VIDEO_STAT lpData = new AGE_REMOTE_VIDEO_STAT;
	memcpy(&(lpData->stats), &stats, sizeof(RemoteVideoStats));

	PostMessage(WM_USER_MSG, EID_REMOTE_VIDEO_STAT, (LPARAM)lpData);
}

void CAGEngineEventHandler::onCameraReady()
{
	PostMessage(WM_USER_MSG, EID_CAMERA_READY, (LPARAM)0);
}

void CAGEngineEventHandler::onVideoStopped()
{
	PostMessage(WM_USER_MSG, EID_VIDEO_STOPPED, (LPARAM)0);
}

void CAGEngineEventHandler::onConnectionLost()
{
	PostMessage(WM_USER_MSG, EID_CONNECTION_LOST, (LPARAM)0);
}

void CAGEngineEventHandler::onConnectionInterrupted()
{
}

void CAGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{

}

void CAGEngineEventHandler::onStartRecordingService(int error)
{
	//if (m_hMainWnd != NULL)
	//	::PostMessage(m_hMainWnd, WM_MSGID(EID_START_RCDSRV), 0, 0);
}

void CAGEngineEventHandler::onStopRecordingService(int error)
{
	//if (m_hMainWnd != NULL)
	//	::PostMessage(m_hMainWnd, WM_MSGID(EID_STOP_RCDSRV), 0, 0);
}

void CAGEngineEventHandler::onRefreshRecordingServiceStatus(int status)
{
	//LPAGE_RCDSRV_STATUS lpData = new AGE_RCDSRV_STATUS;

	//lpData->status = status;

	//if (m_hMainWnd != NULL)
	//	::PostMessage(m_hMainWnd, WM_MSGID(EID_REFREASH_RCDSRV), (WPARAM)lpData, 0);
}


bool CAGEngineEventHandler::EnableVideo(bool bEnable)
{
	int nRet = 0;

	if (bEnable)
	{
		nRet = m_lpArEngine->enableVideo();
		m_lpArEngine->enableLocalVideo(true);
	}
	else
	{
		m_lpArEngine->enableLocalVideo(false);
		nRet = m_lpArEngine->disableVideo();
	}

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? true : false;
}

bool CAGEngineEventHandler::IsVideoEnabled()
{
	return m_bVideoEnable;
}


bool CAGEngineEventHandler::MuteLocalAudio(bool bMuted)
{
	ASSERT(m_lpArEngine != NULL);

	int ret = m_lpArEngine->muteLocalAudioStream(bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? true : false;
}

bool CAGEngineEventHandler::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}


bool CAGEngineEventHandler::MuteLocalVideo(bool bMuted)
{
	ASSERT(m_lpArEngine != NULL);

	int ret = m_lpArEngine->muteLocalVideoStream(bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? true : false;
}

bool CAGEngineEventHandler::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}


bool CAGEngineEventHandler::LocalVideoPreview(HWND hVideoWnd, bool bPreviewOn)
{
	int nRet = 0;

	if (!bPreviewOn) {
		VideoCanvas vc;
		vc.uid = 0;
		vc.view = NULL;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
		nRet = m_lpArEngine->setupLocalVideo(vc);
		MuteLocalVideo(true);
		nRet = m_lpArEngine->stopPreview();
	}
	else {
		AR::VideoEncoderConfiguration vidEncoderConf;
		vidEncoderConf.dimensions.width = 240;
		vidEncoderConf.dimensions.height = 180;
		vidEncoderConf.bitrate = 280;
		vidEncoderConf.minBitrate = 128;
		vidEncoderConf.frameRate = AR::FRAME_RATE_FPS_24;
		vidEncoderConf.minFrameRate = 10;
		nRet = m_lpArEngine->setVideoEncoderConfiguration(vidEncoderConf);

		VideoCanvas vc;
		vc.uid = 0; 
		vc.view = hVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
		nRet = m_lpArEngine->setupLocalVideo(vc);
		MuteLocalVideo(false);
		nRet = m_lpArEngine->startPreview();
	}

	return nRet == 0 ? true : false;
}


BOOL CAGEngineEventHandler::EnableScreenCapture(HWND hWnd, int nCapFPS, LPCRECT lpCapRect, BOOL bEnable, int nBitrate)
{
	ASSERT(m_lpArEngine != NULL);

	int ret = 0;

	AR::Rectangle rcCap;
	ScreenCaptureParameters capParam;
	capParam.bitrate = nBitrate;
	capParam.frameRate = nCapFPS;
	capParam.captureAudio = true;

	if (bEnable) {
		if (lpCapRect == NULL) {
			RECT rc;
			if (hWnd) {
				::GetWindowRect(hWnd, &rc);
				capParam.dimensions.width = rc.right - rc.left;
				capParam.dimensions.height = rc.bottom - rc.top;
				ret = m_lpArEngine->startScreenCaptureByWindowId(hWnd, rcCap, capParam);
			}
			else {
				::GetWindowRect(::GetDesktopWindow(), &rc);
				AR::Rectangle screenRegion = { rc.left, rc.right, rc.right - rc.left, rc.bottom - rc.top };
				capParam.dimensions.width = rc.right - rc.left;
				capParam.dimensions.height = rc.bottom - rc.top;
				ret = m_lpArEngine->startScreenCaptureByScreenRect(screenRegion, rcCap, capParam);
			}
			//startScreenCapture(hWnd, nCapFPS, NULL, nBitrate);
		}
		else {
			capParam.dimensions.width = lpCapRect->right - lpCapRect->left;
			capParam.dimensions.height = lpCapRect->bottom - lpCapRect->top;

			rcCap.x = lpCapRect->left;
			rcCap.y = lpCapRect->top;
			rcCap.width = lpCapRect->right - lpCapRect->left;
			rcCap.height = lpCapRect->bottom - lpCapRect->top;

			if (hWnd)
				ret = m_lpArEngine->startScreenCaptureByWindowId(hWnd, rcCap, capParam);
			else {

				AR::Rectangle screenRegion = rcCap;
				ret = m_lpArEngine->startScreenCaptureByScreenRect(screenRegion, rcCap, capParam);
			}
		}
	}
	else
		ret = m_lpArEngine->stopScreenCapture();

	if (ret == 0)
		m_bScreenCapture = bEnable;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAGEngineEventHandler::IsScreenCaptureEnabled()
{
	return m_bScreenCapture;
}
