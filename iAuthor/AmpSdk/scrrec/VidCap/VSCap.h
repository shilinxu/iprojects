// VSCap.h: interface for the VSCap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _VSCap_H_
#define _VSCap_H_

/////////////////////////////////////////////////////////////////////////////////////////
// 
// Note :
// --------------------------------------------------------------------------------------
// Disabling hardware acceleration might drastically improve the overall quality
// and performance of the capture application.
//
// (Desktop properties | settings | Advanced | Troubleshoot)
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <windowsx.h>
#include "vfw.h"
#include "Utils/buffer.h"
#include "Utils/SoundFile.h"
//#include "Utils/HMWaveFile.h"

#define SAFE_DELETE(p)			if(p) { delete p; p = NULL; }
#define SAFE_RELEASE(p)			if(p) { p->Release(); p = NULL; }
#define SAFE_RELEASE_GLOBAL(p)	if(p) { GlobalFreePtr(p); p = NULL; }

#define MAX_AUD_QUEUE_SIZE	10

// audio compressor
//
#define VSC_ACM_PCM		1
#define VSC_ACM_ADPCM	2
#define VSC_ACM_GSM610	3

typedef int (CALLBACK* VSCAPPROC)(WPARAM wParam, LPARAM lParam);

#pragma pack(1)
typedef struct vscWaveformat_tag
{
	WAVEFORMATEX    wfx;
	WORD            wSamplesPerBlock;
} VSCWAVEFORMAT;
typedef VSCWAVEFORMAT* PVSCWAVEFORMAT;
#pragma pack()

class VSCap  
{
public:

	enum {Recording=1, Paused, Stoped};

public:
	~VSCap() { UnInitialize(); }

	static void Initialize();
	static void UnInitialize();

	static void SetVidComp(int nVidComp) { m_nVidComp = nVidComp; }
	static void SetAudComp(int nAudComp) { m_nAudComp = nAudComp; }	// default: VSC_ACM_GSM610

	static void SetSourceRect(int l, int t, int w, int h)
	{
					m_rcSource.left = l;    m_rcSource.top = t;
					m_rcSource.right = l+w; m_rcSource.bottom = t+h;
	}
	static void SetTargetSize(int w, int h)
	{
					m_rcTarget.left = 0;  m_rcTarget.top = 0;
					m_rcTarget.right = w; m_rcTarget.bottom = h;
	}

	static int  RecordStart(LPCTSTR pszFileName, int fps=15, int nKeyFramesEvery=15,
							bool bRecordCursor=false);	// return HANDLE of thread
	static void RecordPause();
	static void RecordResume();
	static void RecordStop();
	static int  GetRecordState()	{ return m_RecordState; }

	static ULONG WINAPI VidRecordThread(LPVOID pParam);

	static HANDLE Bitmap2Dib(HBITMAP hbitmap, UINT bits);
	static LPBITMAPINFOHEADER CaptureScreenFrame(int l=0, int t=0, int r=320, int b=240, bool bRecordCursor=false);


private:
	static bool PrepareAviFile();
	static void AviFileClose();

	static void DataFromSoundIn(char* pBuffer, int length);
	static void WaveInErrorMsg(MMRESULT result);
	static int  AddInputBufferToQueue();
	static void CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	static void BuildAudRecordingFormat();
	static HRESULT InitAudRecording();
	static HRESULT StartAudRecording();
	static void StopAudioRecording();

	static VSCAPPROC m_pCallBackProc;
	static HANDLE m_hVidRecordThread;

	static PAVIFILE   m_pAviFile;
	static PAVISTREAM m_pVidStream;
	static PAVISTREAM m_pAudStream;

	static TCHAR * m_pszFileName;
	static int m_RecordState;	// 1:recording, 2: paused, 3: stoped


	// Video Recording Variables
	//
	static int  m_fps;
	static int  m_Timelapse;
	static int  m_nKeyFramesEvery;
	static int  m_nCompQuality;
	static RECT m_rcSource;
	static RECT m_rcTarget;


	// Audio Recording Variables
	//
	static bool			m_bEnableAudioDevice;
	static UINT			m_AudDeviceID;
	static HWAVEIN		m_hAudInputDevice;
	static VSCWAVEFORMAT m_AudFormat;
	static int			m_AudBufferSize;
	static LPWAVEHDR	m_pAudQueue[MAX_AUD_QUEUE_SIZE];
	static int			m_AudQueuedBuffers;
//	static CSoundFile*  m_pAudFile;

	// Aud/Vid Compressor Variables
	//
	static DWORD  m_nVidComp;
	static int    m_nAudComp;

	static int m_ScreenBits;
	static bool m_bRecordCursor;

	static DWORD m_FrameTime;		

};

#endif // !_VSCap_H_
