#ifndef __I_AR_STREAMING_KIT_H__
#define __I_AR_STREAMING_KIT_H__
#include "IArRtcEngine.h"

enum PushMode
{
	PM_AudMix = 0,	// 音频合流模式，不合成视频
	PM_VidMix = 1,	// 视频合流模式，音视频均混合
};
enum PushState
{
	PS_Connecting = 0,
	PS_LostConnection,
	PS_ReConnecting,
	PS_Failed,
};
enum PushErrReason
{
	PE_OK = 0,
	PE_Network,
	PE_Timeout,
	PE_Auth,
};

struct PushStats
{
	bool  hasVideo;

	bool  hasAudio;

	bool  vidMix;

	char* vidCode;

	int vidWidth;

	int vidHeight;

	int vidFps;

	int vidBitrate;

	int delayMs;

	char* audCodec;

	int audBitrate;
};

class IArRtmpPushEventHandler
{
public:
	IArRtmpPushEventHandler(void) {};
	virtual ~IArRtmpPushEventHandler(void) {};

	virtual void onStreamPushSuccess(int elapsedTime) {};
	virtual void onStreamPushReSuccess(int elapsedTime) {};
	virtual void onStreamPushStateChanged(PushState pState, PushErrReason pReason, int elapsedTime) {};
	virtual void onStreamPushStats(PushStats pStats) {};
};

class IArStreamingKit
{
protected:
	IArStreamingKit(void) {};
public:
	virtual ~IArStreamingKit(void) {};

	virtual int SetIRtcEngine(AR::IRtcEngine* rtcEngine) = 0;
	virtual int PushStream(const char*strUrl) = 0;
	virtual int UnPushStream() = 0;
	virtual int SetMode(PushMode eMode) = 0;
	virtual int SetLiveTranscoding(AR::LiveTranscoding liveTranscoding) = 0;
};

AR_API IArStreamingKit* AR_CALL createArStreamingKit();

#endif	// __I_AR_STREAMING_KIT_H__
