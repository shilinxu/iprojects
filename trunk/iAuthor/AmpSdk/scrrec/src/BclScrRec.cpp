/////////////////////////////////////////////////////////////////////
//	BclScrRec.cpp - Source File for the BCL Screen Recorder DLL API
//
//	Copyright(c) 2003 bclsoft Inc.  All rights reserved.
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BclScrRec.h"
#include "../../prodcode/crc16.h"
#include "../../prodcode/ProductCode.h"

//#include <windows.h>
#include <windowsx.h>
#include <vfw.h>
#include <tchar.h>
#include <strsafe.h>

#include <assert.h>

#include "AVStreamWriter.h"

#define DEFAULT_AVI_FILENAME	_T("C:\\srec_def.avi")

#define MAX_AUD_QUEUE_SIZE	10

/*
#pragma pack(1)
// Microsoft's
// WAVE_FORMAT_GSM 610           0x0031
//
typedef struct gsm610waveformat_tag
{
	WAVEFORMATEX    wfx;
	WORD            wSamplesPerBlock;
} GSM610WAVEFORMAT;

typedef VSCWAVEFORMAT* PVSCWAVEFORMAT;
#pragma pack()
*/

typedef GSM610WAVEFORMAT VSCWAVEFORMAT;
typedef VSCWAVEFORMAT* PVSCWAVEFORMAT;

typedef struct
{
	DWORD				m_AudDeviceID;
	DWORD				m_VidDeviceID;

	HWAVEIN				m_hAudInputHandle;
	DWORD				m_ScreenInput;
	HWND				m_hScreenWnd;

	TCHAR				m_szAviFileName[MAX_PATH];

	HANDLE				m_hVidRecordThread;
	DWORD				m_CaptureState;

	//CSoundFile*		m_pAudFile;
	PAVIFILE			m_pAviFile;
	PAVISTREAM			m_pVidStream[2];	// 0: uncompressed, 1: compressed
	PAVISTREAM			m_pAudStream;

	// Video properties
	//
	BITMAPINFO			m_VidFormat;
	DWORD				m_fps;
	DWORD				m_KeyFramesEvery;
	DWORD				m_CompQuality;
	RECT				m_rcSource;
	BOOL				m_bRecordCursor;
	BOOL				m_bRecordLogo;
	HCURSOR				m_hCustomCursor;
	BOOL				m_bRecordLayeredWnd;

	POINT				m_CursorOffsetPoint;

	// Audio properties
	//
	BOOL				m_bAudEnable;
	BOOL				m_bAudMute;
	VSCWAVEFORMAT		m_AudFormat;
	DWORD				m_AudBufferSize;	// samples per callback
	LPWAVEHDR			m_pAudQueue[MAX_AUD_QUEUE_SIZE];
	DWORD				m_AudQueuedBuffers;

	DWORD				m_VidCodec;
	DWORD				m_AudCodec;
	bool				m_bUseVidCompDlg;

	DWORD				m_FrameNo;	

	// Statistics
	//
	int					m_CaptureFrames;
	int					m_DroppedFrames;
	int					m_Duration;

	// Callback proc.
	//
	SRECVIDEOPROC		m_pVideoCBProc;
	SRECSTATECHANGEPROC	m_pStateCBProc;
	SRECERRORPROC		m_pErrorCBProc;

	// Video Frame buffer
	HDC					m_hMemDC;
	HBITMAP				m_hBitmap;
	HBITMAP				m_hOldBitmap;
	HBITMAP				m_hCustomLogo;

	// Etc
	BOOL		m_bValidLicenseKey;
	srecError	m_LastError;
	AVStreamWriter m_avsWriter;
	BOOL		m_bDrawing;

public:
	inline LONG GetSourceWidth()
	{
		return m_rcSource.right - m_rcSource.left;
	}
	inline LONG GetSourceHeight()
	{
		return m_rcSource.bottom - m_rcSource.top;
	}

	inline LPBITMAPINFO GetVidFormat()
	{
		return &m_VidFormat;
	}

	inline LONG GetVidFormatSize()
	{
		return (m_VidFormat.bmiHeader.biSize);// + m_Format.bmiHeader.biClrUsed * sizeof(RGBQUAD));
	}

	inline DWORD GetVidBuffSize()
	{
		return m_VidFormat.bmiHeader.biSizeImage;
	}

	inline DWORD GetAudBuffSize()
	{
		return m_AudBufferSize;	// GSM610 (fmt->wfx.nBlockAlign), PCM (fmt->wfx.nAvgBytesPerSec / fmt->wfx.nSamplesPerSec)
	}

	//
	// Fill in the GSM610WAVEFORMAT struct's members 
	// using the values shown in assigning data to the struct's members. 
	//
	// The exception is that wfx.nSamplesPerSec can be a rate other than 8000,
	// in which case wfx.nAvgBytesPerSec is computed as (wfx.nSamplesPerSec)/320*65.
	//
	// When using a value for wfx.nSamplesPerSec other than 8000,  be sure
	// it is a multiple of the 320 block size, and falls within the limits supported
	// by the sound card.
	//
	// For example, wfx.nSamplesPerSec = 9920 is valid because it is a multiple
	// of 320 and does not fall below the minimum sampling rate of 8000 found
	// on many sound cards: 
	//
	void MakeAudFormat()
	{
		PVSCWAVEFORMAT fmt = &m_AudFormat;
		::memset(fmt, 0, sizeof(VSCWAVEFORMAT));

		switch (m_AudCodec)
		{
		case acmPCM:
			{
			fmt->wfx.wFormatTag	   = WAVE_FORMAT_PCM;		
			fmt->wfx.nChannels      = 2;						// mono
			fmt->wfx.wBitsPerSample = (WORD) 16;
			fmt->wfx.nSamplesPerSec = 44100;					// 8.0kHz, 11.025kHz, 22.05kHz, 44.1kHz
			fmt->wfx.nBlockAlign    = (WORD) fmt->wfx.nChannels * (fmt->wfx.wBitsPerSample/8);
			fmt->wfx.nAvgBytesPerSec = fmt->wfx.nSamplesPerSec * fmt->wfx.nBlockAlign;
			fmt->wfx.cbSize = 0;

			m_AudBufferSize = fmt->wfx.nAvgBytesPerSec / fmt->wfx.nSamplesPerSec;
			}
			break;
		case acmADPCM:
		case acmGSM610:
		default:
			{
			fmt->wfx.wFormatTag = WAVE_FORMAT_GSM610;
			fmt->wfx.nChannels = 1;
			fmt->wfx.nSamplesPerSec = 44100;	//8000;		// 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz. For non-PCM formats
												// GSM610의 경우 320의 배수여야 함.
			fmt->wfx.nBlockAlign = 65;			// * fixed transfer buffer size ( 8 KHz일 경우 40ms(1/25 sec) 마다 전송(25번) )
			fmt->wfx.wBitsPerSample = 0;		// always 0 for compressed audio format.
			fmt->wfx.nAvgBytesPerSec = 8957;	// 1625 = nAvgBytesPerSec = (nSamplesPerSec)/320 * 65
			fmt->wfx.cbSize = 2;				// a count of extra byte for extension wave formats
												// ( eg. GSM610WAVEFORMAT )
			fmt->wSamplesPerBlock = 320;		// * fixed sampling size ( 샘플링 단위 320 bytes = 1/8000 sec)


			m_AudBufferSize = fmt->wfx.nBlockAlign * 10; //2; // 130 = 80ms
			}
			break;
		}
	}

	bool MakeVidFormat()
	{
		BITMAP	bm;
		HDC		hScreenDC = ::GetDC(0);
		HBITMAP	hBitmap = ::CreateCompatibleBitmap(hScreenDC, GetSourceWidth(), GetSourceHeight());
		::ReleaseDC(0, hScreenDC);

		::GetObject(hBitmap, sizeof(BITMAP), &bm);
		::DeleteObject(hBitmap);

		if (bm.bmBitsPixel < 16)	// sorry! still not support less than 16bpp
			return false;

		LPBITMAPINFO bi = &m_VidFormat;	//new BITMAPINFO;

		LPBITMAPINFOHEADER bih = &bi->bmiHeader;

		DWORD bits = bm.bmBitsPixel;
		DWORD dwLineLen = (bm.bmWidth*bits+31)/32 * 4;
		DWORD dwColSize = sizeof(RGBQUAD)*((bits <= 8) ? 1<<bits : 0);
		DWORD dwSize = (DWORD)(UINT)dwLineLen*(DWORD)(UINT)bm.bmHeight;

		ZeroMemory(bih, sizeof(BITMAPINFO) + dwColSize);
		bih->biSize = sizeof(BITMAPINFOHEADER) + dwColSize;
		bih->biWidth = bm.bmWidth;
		bih->biHeight = bm.bmHeight;
		bih->biPlanes = 1;
		bih->biBitCount = (WORD) bits;
		bih->biCompression = BI_RGB;
		bih->biSizeImage = dwSize;
		bih->biXPelsPerMeter = 0 ;
		bih->biYPelsPerMeter = 0 ;
		bih->biClrUsed = (bits <= 8) ? 1<<bits : 0;
		bih->biClrImportant = 0 ;

		return true;
	}

} SRECINFO;

//
// Declare Internal Functions
//

BOOL				SetCaptureState( SRECHANDLE hCapture, srecState stat );
BOOL				SetLastError( SRECHANDLE hCapture, srecError err );

BOOL				Bitmap2Dib(HBITMAP hbitmap, LPBITMAPINFOHEADER lpbi);
BOOL				DrawCursor(SRECINFO * psrecInfo, HBITMAP hBitmap, POINT* ptCursor);

BOOL				CaptureScreenFrame(SRECINFO * psrecInfo);

ULONG CALLBACK		VideoRecordProc(LPVOID pParam);

BOOL				InitAudRecording();
MMRESULT			StartAudRecording(SRECINFO * psrecInfo);
ULONG CALLBACK		StopAudioRecording(LPVOID pParam);

void				WaveInErrorMsg(MMRESULT result);
void				DataFromSoundIn(SRECINFO * psrecInfo, char* pBuffer, int length) ;
void CALLBACK		WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance,
										DWORD dwParam1, DWORD dwParam2);
int					AddInputBufferToQueue(SRECINFO * psrecInfo);
void				BuildAudRecordingFormat(VSCWAVEFORMAT& fmt,
											srecAudioEncoder codec, DWORD& AudBufferSize);

void				AviFileCloseAll(SRECINFO * psrecInfo);
BOOL				PrepareAviFile(SRECINFO * psrecInfo);

SRECINFO *			CreateCapture();



// 
// Internal Functions
//

BOOL SetCaptureState( SRECHANDLE hCapture, srecState stat )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_CaptureState = stat;

		if (psrecInfo->m_pStateCBProc)
			psrecInfo->m_pStateCBProc(stat, NULL);

		return TRUE;
	}

	return FALSE;
}

BOOL SetLastError( SRECHANDLE hCapture, srecError err )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_LastError = err;

		if (psrecInfo->m_pErrorCBProc)
			psrecInfo->m_pErrorCBProc(err, NULL);

		return TRUE;
	}

	return FALSE;
}

BOOL Bitmap2Dib(HBITMAP hbitmap, LPBITMAPINFOHEADER lpbi)
{
	if (!lpbi)
		return FALSE;

	//
	// Get the bits from the bitmap and stuff them after the LPBI
	//
	BITMAP bitmap;
	::GetObject(hbitmap, sizeof(BITMAP), &bitmap);
	DWORD dwColSize = sizeof(RGBQUAD)*((bitmap.bmBitsPixel <= 8) ? 1<<bitmap.bmBitsPixel : 0);
	LPBYTE lpBits = (LPBYTE)(lpbi+1)+dwColSize;

	HDC hdc = ::CreateCompatibleDC(NULL);

	::GetDIBits(hdc, hbitmap, 0, bitmap.bmHeight, lpBits,
				(LPBITMAPINFO) lpbi, DIB_RGB_COLORS);

	::DeleteDC(hdc) ;

	return TRUE;
}

BOOL DrawCursor(SRECINFO * psrecInfo, HBITMAP hBitmap, POINT* ptCursor)
{
	BOOL br = TRUE;

	CURSORINFO cinfo;
	ICONINFO iinfo;
	POINT xPoint;
	
	// retreive the cursor infomation
	//

	::memset(&cinfo, 0, sizeof(CURSORINFO));
	cinfo.cbSize = sizeof(CURSORINFO);
	::GetCursorInfo(&cinfo); 
	if (cinfo.flags != CURSOR_SHOWING)	// nothing to do when cursor is invisible state.
		return TRUE;


	if (psrecInfo->m_hCustomCursor)
		cinfo.hCursor = psrecInfo->m_hCustomCursor;

	// retreive hotspot data of the cursor
	//
	br = ::GetIconInfo(cinfo.hCursor, &iinfo);
	if (!br)
		return TRUE;
	
	//need to delete the hbmMask and hbmColor bitmaps
	//otherwise the program will crash after a while after running out of resource
	if (iinfo.hbmMask)  ::DeleteObject(iinfo.hbmMask);
	if (iinfo.hbmColor) ::DeleteObject(iinfo.hbmColor);


	// draw cursor
	//

	HDC hScreenDC = ::GetDC(0);
	HDC hMemDC = ::CreateCompatibleDC(hScreenDC);     

    HBITMAP hbm = hBitmap;
	if (ptCursor)
		xPoint = *ptCursor;
	else
		xPoint = cinfo.ptScreenPos;

	HBITMAP oldbm = (HBITMAP) ::SelectObject(hMemDC, hbm);	 

	// recalc point of cursor with hotspot
	//
	xPoint.x -= iinfo.xHotspot;
	xPoint.y -= iinfo.yHotspot;

	//::DrawIcon( hScreenDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
	::DrawIcon( hMemDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
	
	::SelectObject(hMemDC,oldbm);
	
	::DeleteDC(hMemDC);	
	::ReleaseDC(0, hScreenDC);
	
	return TRUE;
}

BOOL CaptureScreenFrame(SRECINFO * psrecInfo)
{
	int sw = psrecInfo->GetSourceWidth();
	int sh = psrecInfo->GetSourceHeight();
	if (sw <= 0 || sh <= 0)
		return FALSE;

	HDC hScreenDC = ::GetDC(0);

	// if video is paused, use previous image
	//
	if (srecGetCaptureState(psrecInfo) != rstPausedVideo)
	{
		DWORD dwRop;
		if(psrecInfo->m_bRecordLayeredWnd)
			dwRop = SRCCOPY|CAPTUREBLT;
		else
			dwRop = SRCCOPY;

		// Get screen frame
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(psrecInfo->m_hMemDC, psrecInfo->m_hBitmap);
		BOOL br = ::BitBlt(psrecInfo->m_hMemDC, 0, 0, sw, sh,
							hScreenDC, psrecInfo->m_rcSource.left, psrecInfo->m_rcSource.top, dwRop);
		if (!br) 
		{
			::SelectObject(psrecInfo->m_hMemDC, hOldBitmap);
			return br;
		}

		//::StretchBlt(hScreenDC, 0, 0, 1024/4, 768/4, psrecInfo->m_hMemDC[0], 0, 0, 1024, 768, SRCCOPY);
		// Draw Logo
		if(psrecInfo->m_bRecordLogo && psrecInfo->m_hCustomLogo)
		{
			BITMAP bm;
			::GetObject(psrecInfo->m_hCustomLogo, sizeof(BITMAP), &bm);

			HDC hLogoDC = ::CreateCompatibleDC(psrecInfo->m_hMemDC);     
			HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hLogoDC, psrecInfo->m_hCustomLogo);

			::BitBlt(psrecInfo->m_hMemDC, sw-bm.bmWidth, 0, bm.bmWidth, bm.bmHeight,
						hLogoDC, 0, 0, SRCCOPY);

			::SelectObject(hLogoDC, hOldBitmap);
			::DeleteDC(hLogoDC);
		}

		// Draw the Cursor
		//
		if (psrecInfo->m_bRecordCursor)
		{
			CURSORINFO cinfo;
			ICONINFO iinfo;
			POINT xPoint;
			
			// retreive the cursor infomation
			::memset(&cinfo, 0, sizeof(CURSORINFO));
			cinfo.cbSize = sizeof(CURSORINFO);
			::GetCursorInfo(&cinfo); 
			if (cinfo.flags == CURSOR_SHOWING)	// nothing to do when cursor is invisible state.
			{
				if (psrecInfo->m_hCustomCursor)
					cinfo.hCursor = psrecInfo->m_hCustomCursor;

				// retreive hotspot data of the cursor
				::GetIconInfo(cinfo.hCursor, &iinfo);
				//need to delete the hbmMask and hbmColor bitmaps
				//otherwise the program will crash after a while after running out of resource
				if (iinfo.hbmMask)  ::DeleteObject(iinfo.hbmMask);
				if (iinfo.hbmColor) ::DeleteObject(iinfo.hbmColor);


				// recalc point of cursor with hotspot
				xPoint = cinfo.ptScreenPos;

				xPoint.x = xPoint.x - (iinfo.xHotspot + psrecInfo->m_CursorOffsetPoint.x);
				xPoint.y = xPoint.y - (iinfo.yHotspot + psrecInfo->m_CursorOffsetPoint.y);

				//::DrawIcon( hScreenDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
				::DrawIcon( psrecInfo->m_hMemDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
			}
		}

		::SelectObject(psrecInfo->m_hMemDC, hOldBitmap);
	}

	// convert DDI to DIB
	//
	int scans = 0;
	AVBuffer* pavb = psrecInfo->m_avsWriter.LockBuffer(ST_VIDEO);
	if (pavb)
	{
		//memset(pavb->m_pData, 255, pavb->m_size);
		scans = ::GetDIBits(psrecInfo->m_hMemDC, psrecInfo->m_hBitmap,
					0, psrecInfo->GetSourceHeight(),
					pavb->m_pData,
					psrecInfo->GetVidFormat(),
					DIB_RGB_COLORS);


		pavb->m_no   = psrecInfo->m_FrameNo;
		psrecInfo->m_avsWriter.UnlockBuffer(pavb);
	}
	// release gdi resources
	::ReleaseDC(0, hScreenDC);

	return (scans > 0);
}

void AviFileCloseAll(SRECINFO * psrecInfo)
{
	if (psrecInfo)
	{
		if (psrecInfo->m_pVidStream[1])  ::AVIStreamClose(psrecInfo->m_pVidStream[1]);
		if (psrecInfo->m_pVidStream[0])  ::AVIStreamClose(psrecInfo->m_pVidStream[0]);
		if (psrecInfo->m_pAudStream)  ::AVIStreamClose(psrecInfo->m_pAudStream);
		if (psrecInfo->m_pAviFile)    ::AVIFileClose(psrecInfo->m_pAviFile);

		::AVIFileExit();
	}
}

ULONG WINAPI VideoRecordProc(LPVOID pParam)
{
	const DWORD SleepTick = 10;
	DWORD oldFrameNo = 0;

	SRECINFO * psrecInfo = (SRECINFO *) pParam;

	psrecInfo->m_CaptureFrames = 0;
	psrecInfo->m_DroppedFrames = 0;

	// start thread loop
	//
	DWORD InitialTime = ::timeGetTime();
	DWORD AvgTimePerFrame = 1000 / psrecInfo->m_fps;
	SetCaptureState(psrecInfo, rstRecording);
	while (srecGetCaptureState(psrecInfo) != rstStopping
			&& srecGetCaptureState(psrecInfo) != rstStopped
			&& psrecInfo->m_avsWriter.IsAliveWriter())
	{
		// Pause Process
		//
		if (srecGetCaptureState(psrecInfo) == rstPaused)
		{
			DWORD TimePauseStart = ::timeGetTime();
			do {
			::Sleep(SleepTick);
			} while (srecGetCaptureState(psrecInfo) == rstPaused);
			
			InitialTime += ::timeGetTime() - TimePauseStart;
		}
/*		
		while (psrecInfo->m_bDrawing) {
			for (int i=0; i<9; i++)	Sleep(100);
		}
*/		
		Sleep(0);
		
		// capture video frame
		DWORD CaptureStart = ::timeGetTime();

		// duration
		//psrecInfo->m_FrameNo = (CaptureStart - InitialTime) / AvgTimePerFrame;
		psrecInfo->m_FrameNo = (DWORD) ((double) (CaptureStart - InitialTime)/1000.0 * (double) psrecInfo->m_fps);
		psrecInfo->m_Duration = psrecInfo->m_FrameNo / psrecInfo->m_fps;


		DWORD RemainLapse, CaptureLapse;
		if ((psrecInfo->m_FrameNo == 0 || psrecInfo->m_FrameNo > oldFrameNo))// && !psrecInfo->m_bDrawing)
		{
			if (CaptureScreenFrame(psrecInfo))
			{
				oldFrameNo = psrecInfo->m_FrameNo;
				psrecInfo->m_CaptureFrames++;

TTRACE("m_FrameNo = %4d(%7d)\n", psrecInfo->m_FrameNo, psrecInfo->m_CaptureFrames);
			}

			// calculate waiting time
			CaptureLapse = ::timeGetTime() - CaptureStart;
			RemainLapse = (AvgTimePerFrame > CaptureLapse) ? AvgTimePerFrame - CaptureLapse : SleepTick;
			//if (psrecInfo->m_bDrawing)
			//	RemainLapse = 55; // yield enough time to drawing process
		}
		else
		{
			// calculate waiting time
			RemainLapse = AvgTimePerFrame;
		}
//////////////////////////////////////////////////////////////////////////////
// Increase sleeping time for drawing process
			RemainLapse = AvgTimePerFrame;
//////////////////////////////////////////////////////////////////////////////
				
		// delay for next frame
		int nn = RemainLapse / SleepTick;
		for (int i=0; i<nn; i++)
		{
			::Sleep(SleepTick);

			if (srecGetCaptureState(psrecInfo) != rstRecording) break;
		}
		if (srecGetCaptureState(psrecInfo) == rstRecording) {
			::Sleep(RemainLapse-nn*SleepTick);
		}

	} //for loop

	// duration
	psrecInfo->m_Duration = psrecInfo->m_FrameNo / psrecInfo->m_fps;
	psrecInfo->m_DroppedFrames = (psrecInfo->m_Duration * psrecInfo->m_fps) - psrecInfo->m_CaptureFrames;
	if (psrecInfo->m_DroppedFrames < 0) psrecInfo->m_DroppedFrames = 0;

	return 0;
}


///////////
//
// Wave functions
//
///////////////////////////////////////////////////

void WaveInErrorMsg(MMRESULT result)
{
	TCHAR szErrMsg[500+1];
	::waveInGetErrorText(result, szErrMsg, 500);

	::MessageBox(::GetForegroundWindow(), szErrMsg, _T("Audio Error"), MB_OK | MB_ICONEXCLAMATION);
}

MMRESULT StartAudRecording(SRECINFO * psrecInfo)
{
	MMRESULT mmReturn = 0;	

	if (!psrecInfo->m_bAudEnable)
		return MMSYSERR_NOERROR;

	// open wavein device
	//m_AudDeviceID=1;
	mmReturn = ::waveInOpen(&psrecInfo->m_hAudInputHandle, psrecInfo->m_AudDeviceID,
							(LPCWAVEFORMATEX) &psrecInfo->m_AudFormat,
							(DWORD) (void *)WaveInProc, (DWORD) psrecInfo, CALLBACK_FUNCTION);
/*	if (mmReturn == WAVERR_BADFORMAT && psrecInfo->m_AudCodec == acmGSM610)
	{
		psrecInfo->m_AudCodec = acmPCM;			// if audio device doesn't support GSM610 44.1KHz, use just PCM.
		psrecInfo->MakeAudFormat();				// TODO: need some addtional things to fix auto-rebuild audformat algorithm
		return StartAudRecording(psrecInfo);
	}
	else*/ if (mmReturn != MMSYSERR_NOERROR)
	{
		WaveInErrorMsg(mmReturn);
		SetLastError(psrecInfo, serrAudioDeviceOpenFailed);
		return mmReturn;
	}
	else
	{
		// make several input buffers and add them to the input queue
		for(int i=0; i<MAX_AUD_QUEUE_SIZE; i++)
		{
			AddInputBufferToQueue(psrecInfo);
		}
		
		// start recording
		mmReturn = ::waveInStart(psrecInfo->m_hAudInputHandle);
		if(mmReturn )
		{
			WaveInErrorMsg(mmReturn);
			return mmReturn;
		}
	}

	return mmReturn;
}

ULONG WINAPI StopAudioRecording(LPVOID pParam)
{
	SRECINFO * psrecInfo = (SRECINFO *) pParam;
	if (!psrecInfo->m_hAudInputHandle || !psrecInfo->m_bAudEnable)
		return 1;

	MMRESULT mmReturn = MMSYSERR_NOERROR;
	
//printf("\n## before waveInStop\n");
	mmReturn = ::waveInStop(psrecInfo->m_hAudInputHandle);
//printf("\n## after waveInStop\n");

	mmReturn = ::waveInReset(psrecInfo->m_hAudInputHandle);
	mmReturn = ::waveInClose(psrecInfo->m_hAudInputHandle);
	if (mmReturn == WAVERR_STILLPLAYING)
	{
		TTRACE("WAVERR_STILLPLAYING\n");
		//mmReturn = ::waveInReset(psrecInfo->m_hAudInputHandle);
	}

	// cleanup audio queue
	for (int i=0; i < (int)psrecInfo->m_AudQueuedBuffers; i++)
	{
		//::waveInUnprepareHeader(hwi, psrecInfo->m_pAudQueue[i], sizeof(WAVEHDR));
		//::GlobalFree(::GlobalHandle(psrecInfo->m_pAudQueue[i]));
		delete psrecInfo->m_pAudQueue[i];
		psrecInfo->m_pAudQueue[i] = NULL;
	}

	psrecInfo->m_AudQueuedBuffers = 0;
//printf("## exit StopAudioRecording\n");

	return 0;
}

BOOL InitAudRecording()
{
/*	m_AudQueuedBuffers = 0;
	m_hAudInputDevice = NULL;
	
	if (::waveInGetNumDevs())
        m_bEnableAudioDevice = true;
	else
        m_bEnableAudioDevice = false;

	if (m_bEnableAudioDevice)
	{
		BuildAudRecordingFormat();
*/
		/*
		//Create temporary wav file for audio recording
		SAFE_DELETE(m_pAudFile);
		m_pAudFile = new CSoundFile(_T("c:\\vscap.wav"), &m_AudFormat);	

		if  (!(m_pAudFile && m_pAudFile->IsOK()))
		{
			::MessageBox(::GetForegroundWindow(),"Error Creating Sound File","Error",MB_OK | MB_ICONEXCLAMATION);
			//MessageOut(NULL,IDS_STRING_ERRSOUND ,IDS_STRING_NOTE,MB_OK | MB_ICONEXCLAMATION);
			return AVIERR_FILEOPEN;
		}
		*/
//	}
	
	return TRUE;
}

void DataFromSoundIn(SRECINFO * psrecInfo, char* pBuffer, int length) 
{
	// Write WAV data to AVI stream
	HRESULT hr = ::AVIStreamWrite(psrecInfo->m_pAudStream, -1, // m_FrameNo, append at end of stream
									1, pBuffer, length,	AVIIF_KEYFRAME, NULL, NULL);
	if(hr != 0)
	{
		SetCaptureState(psrecInfo, rstStopping);
		SetLastError(psrecInfo, serrAudioWriteFailed);

		return;
	}

/*
	if (m_pAudFile)
	{
		if (!m_pAudFile->Write(buffer))
		{
			StopAudioRecording();
			SAFE_DELETE(m_pAudFile);
			::MessageBox(::GetForegroundWindow(),"Error Writing Sound File", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
	}
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 디바이스 드라이버가 waveInAddBuffer함수를 사용하여 보내어진
// 데이터 블록을 인코딩된 오디오 데이터로 채웠을 때 호출됨.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	MMRESULT mmReturn = 0;
	LPWAVEHDR pHdr = (LPWAVEHDR) dwParam1;
	SRECINFO * psrecInfo = (SRECINFO *) dwInstance;

	if (uMsg != WIM_DATA)
		return;


	TTRACE("WIM_DATA %4d\n", pHdr->dwBytesRecorded);

	if (srecGetCaptureState(psrecInfo) == rstRecording && pHdr->dwBytesRecorded > 0)
	{	
		//Write Data to file
		DataFromSoundIn(psrecInfo, pHdr->lpData, pHdr->dwBytesRecorded);

/*
// DataFromSoundIn's result is a little better than writer thread, so comment out

		AVBuffer* pavb = psrecInfo->m_avsWriter.LockBuffer(ST_AUDIO);
		if (pavb)
		{
			::memcpy(pavb->m_pData, pHdr->lpData, pHdr->dwBytesRecorded);//pavb->m_size);
			pavb->m_no = -1;//frame_no;
			pavb->m_size = pHdr->dwBytesRecorded;
			psrecInfo->m_avsWriter.UnlockBuffer(pavb);

			//frame_no++;// += pHdr->dwBytesRecorded;
		}
*/
	}

	if (srecGetCaptureState(psrecInfo) != rstStopping && srecGetCaptureState(psrecInfo) != rstStopped)
	{	
		// add the input buffer to the queue again
		//::waveInUnprepareHeader(hwi, pHdr, sizeof(WAVEHDR));
		::waveInPrepareHeader(hwi, pHdr, sizeof(WAVEHDR));
		::waveInAddBuffer(hwi, pHdr, sizeof(WAVEHDR));
	}
}

int AddInputBufferToQueue(SRECINFO * psrecInfo)
{
	MMRESULT mmReturn = 0;
	
	if (psrecInfo->m_AudQueuedBuffers > MAX_AUD_QUEUE_SIZE) return psrecInfo->m_AudQueuedBuffers;

	// create the header
	//LPWAVEHDR pHdr = (LPWAVEHDR) ::GlobalLock(::GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, sizeof(WAVEHDR) + psrecInfo->m_AudBufferSize));
	LPWAVEHDR pHdr = (LPWAVEHDR) new char[sizeof(WAVEHDR) + psrecInfo->m_AudBufferSize];
	if(pHdr == NULL) return NULL;

	// new a buffer
	::memset(pHdr, 0, sizeof(WAVEHDR));
	pHdr->lpData = (LPSTR) (pHdr + 1);
	pHdr->dwBufferLength = psrecInfo->m_AudBufferSize;
	pHdr->dwFlags = 0;	// must be zero

	// prepare it
	::waveInPrepareHeader(psrecInfo->m_hAudInputHandle, pHdr, sizeof(WAVEHDR));

	// add the input buffer to the queue
	::waveInAddBuffer(psrecInfo->m_hAudInputHandle, pHdr, sizeof(WAVEHDR));

	// no error
	// increment the number of waiting buffers
	psrecInfo->m_pAudQueue[psrecInfo->m_AudQueuedBuffers] = pHdr;
	return psrecInfo->m_AudQueuedBuffers++;
}


BOOL PrepareAviFile(SRECINFO * psrecInfo)
{
	HRESULT hr;
	AVISTREAMINFO sti;
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};

	// test ensure video compressor
	DWORD compfccHandler = mmioFOURCC('M', 'S', 'V', 'C');	
	switch (psrecInfo->m_VidCodec)
	{
	case DEFAULT_CODEC:
	case vcmMSVC:	// Microsoft Video 1			* Default
		compfccHandler = mmioFOURCC('M', 'S', 'V', 'C');
		break;
	case vcmCVID:	// Cinepak Codec by Radius
		compfccHandler = mmioFOURCC('C', 'V', 'I', 'D');
		break;
	case vcmIV50:	// Indeo Video 5.10
		compfccHandler = mmioFOURCC('I', 'V', '5', '0');
		break;
	case vcmDIVX:	// Divx 5.0.5
		compfccHandler = mmioFOURCC('D', 'I', 'V', 'X');
		break;
	case vcmTSCC:	// for test
		compfccHandler = mmioFOURCC('T', 'S', 'C', 'C');
		break;
	}

	do	// start critical block
	{

	//
	// Open the movie file for writing....
	//
	::AVIFileInit();
	::DeleteFile(psrecInfo->m_szAviFileName);
	hr = ::AVIFileOpen(&psrecInfo->m_pAviFile, psrecInfo->m_szAviFileName, OF_WRITE | OF_CREATE | OF_SHARE_DENY_WRITE, NULL);
	if (hr != AVIERR_OK) 
	{
		SetLastError(psrecInfo, serrAviFileOpenFailed);
		return FALSE;
	}

	// 1. And create the video source stream
	//

	::memset(&sti, 0, sizeof(sti));
	sti.fccType                = streamtypeVIDEO;// stream type
	sti.fccHandler             = 0;
	sti.dwScale                = 1;		// fps = dwRate / dwScale
	sti.dwRate                 = psrecInfo->m_fps;		    
	sti.dwSuggestedBufferSize  = psrecInfo->GetVidBuffSize();
	::SetRect(&sti.rcFrame, 0, 0,		    // rectangle for stream
				psrecInfo->GetSourceWidth(),
				psrecInfo->GetSourceHeight());
	hr = ::AVIFileCreateStream(psrecInfo->m_pAviFile, &psrecInfo->m_pVidStream[0], &sti);
	if (hr != AVIERR_OK)
		break; 
	

	// 2. create the compressed video stream using Get codec and set on stone from video compressor dialog
	//

	// fill in the compress options with default codec when error returned.
	::memset(&opts, 0, sizeof(opts)); 
	aopts[0]->fccType			= streamtypeVIDEO;
	aopts[0]->fccHandler		= compfccHandler;				// mmioFOURCC('M', 'S', 'V', 'C')
	aopts[0]->dwKeyFrameEvery	= psrecInfo->m_KeyFramesEvery;	// keyframe rate 
	aopts[0]->dwQuality			= psrecInfo->m_CompQuality;		// compress quality, 0 - 10,000 
	aopts[0]->dwBytesPerSecond	= 0;							// bytes per second 
	aopts[0]->dwFlags			= AVICOMPRESSF_KEYFRAMES;		// AVICOMPRESSF_INTERLEAVE
	aopts[0]->lpFormat			= 0;							// audio format 
	aopts[0]->cbFormat			= 0;
	aopts[0]->dwInterleaveEvery = 0;

	// popup video compressor dialog
	// it should have only the compressors that can handle the input format appear in the selection list.
	if (psrecInfo->m_bUseVidCompDlg)
	{
		COMPVARS pc;
		memset(&pc, 0, sizeof(pc));
		pc.cbSize = sizeof(pc);
		pc.fccType = aopts[0]->fccType;
		pc.fccHandler = aopts[0]->fccHandler;
		pc.lQ = aopts[0]->dwQuality;
		pc.lKey = aopts[0]->dwKeyFrameEvery;
		if (::ICCompressorChoose(::GetForegroundWindow(),
 							ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE,	// ICMF_CHOOSE_PREVIEW
							psrecInfo->GetVidFormat(),
							0,	//psrecInfo->m_pVidStream[0],
							&pc,
							_T("Video Compressor(Cancel: default Codec)")))
		{
			aopts[0]->fccType			= pc.fccType;
			aopts[0]->fccHandler		= pc.fccHandler;
			aopts[0]->dwKeyFrameEvery	= pc.lKey;
			aopts[0]->dwQuality			= pc.lQ;		// compress quality, 0 - 10,000 
			aopts[0]->dwBytesPerSecond	= pc.lDataRate;
			aopts[0]->dwFlags			= pc.dwFlags;
			aopts[0]->lpFormat			= 0;
			aopts[0]->cbFormat			= 0;
			aopts[0]->dwInterleaveEvery = 0;
		}
		else	// if user press the 'cancel' button, then we just go ahead by default video codec
		{
		}
/*
		//memset(&opts, 0, sizeof(opts));
		if (!::AVISaveOptions(::GetForegroundWindow(),
							ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE |ICMF_CHOOSE_PREVIEW,
							1, &psrecInfo->m_pVidStream[0], &aopts[0]))
		{
			// if user press the 'cancel' button, then we just go ahead by default video codec
		}
*/
	}
	
	// make the compressed video stream with desired codec
	hr = ::AVIMakeCompressedStream(&psrecInfo->m_pVidStream[1], psrecInfo->m_pVidStream[0], &opts, NULL);
	if (hr != AVIERR_OK)
		break; 

	hr = ::AVIStreamSetFormat(
				psrecInfo->m_pVidStream[1],
				0, 
				psrecInfo->GetVidFormat(),			// stream format
				psrecInfo->GetVidFormatSize()		// format size
		);
	if (hr != AVIERR_OK)
		break;	

	// 3. create the audio stream;
	//

	if (psrecInfo->m_bAudEnable)
	{
		::memset(&sti, 0, sizeof(sti));
		sti.fccType                = streamtypeAUDIO;// stream type
		sti.fccHandler             = 0;
		sti.dwStart                = 0;	// delay 0
		sti.dwQuality              = (DWORD)-1;

		sti.dwSuggestedBufferSize  = psrecInfo->m_AudBufferSize;
		sti.dwSampleSize = psrecInfo->m_AudFormat.wfx.nBlockAlign;

		if (psrecInfo->m_AudCodec == acmPCM)
		{
			sti.dwRate  = psrecInfo->m_AudFormat.wfx.nSamplesPerSec;
			sti.dwScale = 1;
		}
		else	// compressed audio format
		{
			sti.dwRate  = psrecInfo->m_AudFormat.wfx.nAvgBytesPerSec;
			sti.dwScale = psrecInfo->m_AudFormat.wfx.nBlockAlign;
			//sti.dwRate = psrecInfo->m_AudFormat.wfx.nAvgBytesPerSec / psrecInfo->m_AudFormat.wfx.nBlockAlign;
		}

		hr = ::AVIFileCreateStream(psrecInfo->m_pAviFile, &psrecInfo->m_pAudStream, &sti);
		if (hr != AVIERR_OK)
			break; 


		if (psrecInfo->m_AudCodec == acmPCM)
			hr = ::AVIStreamSetFormat(psrecInfo->m_pAudStream, 0, &psrecInfo->m_AudFormat, sizeof(WAVEFORMATEX));
		else	// compressed audio format
			hr = ::AVIStreamSetFormat(psrecInfo->m_pAudStream, 0, &psrecInfo->m_AudFormat, sizeof(VSCWAVEFORMAT));
		if (hr != AVIERR_OK)
			break;	
	}

	} while (0);	// end of critical block


	// release resources
	//
	::AVISaveOptionsFree(1, &aopts[0]);

	if (hr != S_OK)
	{
		if (psrecInfo->m_pAviFile)
			AviFileCloseAll(psrecInfo);

		return FALSE;
/*
		switch(hr)
		{
		case AVIERR_NOCOMPRESSOR:
			m_sError+=_T(" A suitable compressor cannot be found.");
				break;
		case AVIERR_MEMORY:
			m_sError+=_T(" There is not enough memory to complete the operation.");
				break; 
		case AVIERR_UNSUPPORTED:
			m_sError+=_T("Compression is not supported for this type of data. "
			"This error might be returned if you try to compress data that is not audio or video.");
			break;
		}
		return FALSE;
*/
	}
	else
	{
		return TRUE;
	}
}

SRECINFO * CreateCapture()
{
	SRECINFO * hCapture = new SRECINFO;
	if (hCapture)
	{
		hCapture->m_VidDeviceID = DEFAULT_VIDEO_DEVICE;

		hCapture->m_AudDeviceID = DEFAULT_AUDIO_DEVICE;

		hCapture->m_hAudInputHandle = NULL;
		hCapture->m_hScreenWnd = NULL;
		hCapture->m_ScreenInput = cinScreen;

		_tcsncpy(hCapture->m_szAviFileName, DEFAULT_AVI_FILENAME, _tcslen(DEFAULT_AVI_FILENAME)+1);

		hCapture->m_hVidRecordThread = NULL;
		hCapture->m_CaptureState = rstStopped;

		//CSoundFile*		m_pAudFile = NULL;
		hCapture->m_pAviFile = NULL;
		hCapture->m_pVidStream[0] = NULL;
		hCapture->m_pVidStream[1] = NULL;
		hCapture->m_pAudStream = NULL;

		// Video properties
		//

		hCapture->m_fps = 15;
		hCapture->m_KeyFramesEvery = hCapture->m_fps;
		hCapture->m_CompQuality = 8500;
		hCapture->m_bRecordCursor = TRUE;
		hCapture->m_hCustomCursor = NULL;
		hCapture->m_bRecordLogo = FALSE;
		hCapture->m_bRecordLayeredWnd = FALSE;

		hCapture->m_CursorOffsetPoint.x = 0;
		hCapture->m_CursorOffsetPoint.y = 0;

		// Audio properties
		//
		
		if (::waveInGetNumDevs())	hCapture->m_bAudEnable = TRUE;
		else						hCapture->m_bAudEnable = FALSE;

		hCapture->m_bAudMute = FALSE;
		//hCapture->m_pAudQueue[MAX_AUD_QUEUE_SIZE];
		hCapture->m_AudQueuedBuffers = 0;

		
		// Codecs
		//

		hCapture->m_VidCodec = DEFAULT_CODEC;
		hCapture->m_AudCodec = DEFAULT_CODEC;

		hCapture->m_FrameNo = 0;	

		// Statistics
		//
		hCapture->m_CaptureFrames = 0;
		hCapture->m_DroppedFrames = 0;
		hCapture->m_Duration = 0;

		// Callback proc.
		//
		hCapture->m_pVideoCBProc = NULL;
		hCapture->m_pStateCBProc = NULL;
		hCapture->m_pErrorCBProc = NULL;

		// Video Frame buffer
		//
		hCapture->m_hMemDC     = NULL;
		hCapture->m_hBitmap    = NULL;
		hCapture->m_hOldBitmap = NULL;

		hCapture->m_hCustomLogo   = NULL;		// logo


		// Etc
		//
		hCapture->m_bValidLicenseKey = FALSE;
		hCapture->m_LastError = serrNone;
	}

	return hCapture;
}


//
// Capture object creation
//

//
// szLicense : SRECSDK00001-147AA447D127-86AE49659587-924D3BEFF365
//
SCRREC_API SRECHANDLE WINAPI  srecCreateCapture( TCHAR * szLicense )
{
	SRECINFO * psrecInfo = NULL;

	CProductCode pc;
	if (pc.CheckCode(szLicense, 12, _tcslen(szLicense)))
	{
		psrecInfo = CreateCapture();
		if (psrecInfo)
		{
			psrecInfo->m_bValidLicenseKey = TRUE;

			// check vfw version
			WORD wVer = HIWORD(::VideoForWindowsVersion());
			if (wVer < 0x010a)
			{
				SetLastError(psrecInfo, serrVFWVersionTooOld);
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
			return NULL;
	}

	// Check settings of the video card
	HDC hScreenDC = ::GetDC(0);
	DWORD bits = ::GetDeviceCaps(hScreenDC, BITSPIXEL);	
	if (bits < 16)	// sorry! still not support less than 16bpp
	{
		::MessageBox(::GetForegroundWindow(),
			"sorry! still not support less than 16bpp screen.",
			"Error", MB_OK | MB_ICONSTOP);

		SetLastError(psrecInfo, serrSystemError);
		return NULL;
	}

	// Prepare frame buffer
	int sx = ::GetSystemMetrics(SM_CXSCREEN);
	int sy = ::GetSystemMetrics(SM_CYSCREEN);
	psrecInfo->m_hMemDC = ::CreateCompatibleDC(hScreenDC);     
	psrecInfo->m_hBitmap = ::CreateCompatibleBitmap(hScreenDC, sx, sy);
	//psrecInfo->m_hOldBitmap = (HBITMAP) ::SelectObject(psrecInfo->m_hMemDC, psrecInfo->m_hBitmap);
	::ReleaseDC(0, hScreenDC);

	psrecInfo->m_bDrawing = FALSE;

	return (SRECHANDLE) psrecInfo;
}

SCRREC_API BOOL     WINAPI  srecDestroyCapture( SRECHANDLE hCapture )
{
	if (hCapture)
	{
		SRECINFO * psrecInfo = (SRECINFO *) hCapture;

		if ( psrecInfo->m_hBitmap )
		{
			::DeleteObject(psrecInfo->m_hBitmap);
			psrecInfo->m_hBitmap = NULL;
//			::SelectObject(psrecInfo->m_hMemDC, psrecInfo->m_hOldBitmap);
		}
//		if ( psrecInfo->m_hCustomLogo )
//		{
//			//::DeleteObject(m_hCustomLogo);		// delete logo by application
//		}

		if ( psrecInfo->m_hMemDC )	::DeleteDC(psrecInfo->m_hMemDC);	

		SAFE_DELETE(hCapture);

		return SCRREC_SUCCESS;
	}
	else
	{
		return SCRREC_FAILED;
	}
}



//
// Capture Methods
//

SCRREC_API BOOL     WINAPI  srecStartCapture( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;
	if (!psrecInfo)
		return SCRREC_FAILED;

	if (srecGetCaptureState(psrecInfo) != rstStopped)
		return SCRREC_FAILED;

	// enter critical block
	//
	do
	{
		SetCaptureState(psrecInfo, rstStarted);

		// make audio and video format
		if (psrecInfo->m_bAudEnable) {
			psrecInfo->MakeAudFormat();
		}
		if (!psrecInfo->MakeVidFormat())
			break;

		// prepare AVI stream
		//
		if (!PrepareAviFile(psrecInfo))
			break;

		// create stream writer for AVI file
		//
		bool br = psrecInfo->m_avsWriter.Init(psrecInfo->m_pAudStream, psrecInfo->GetAudBuffSize(),
											psrecInfo->m_pVidStream[1], psrecInfo->GetVidBuffSize(),
											psrecInfo->m_fps);
		if (!br)
		{
			if (psrecInfo->m_pAviFile)
				AviFileCloseAll(psrecInfo);

			break;
		}
		psrecInfo->m_avsWriter.Run();

		// start audio & video recording
		//
		HRESULT hr = StartAudRecording(psrecInfo);
		if (hr != MMSYSERR_NOERROR) {
			// release file writer buffers
			psrecInfo->m_avsWriter.Stop();
			psrecInfo->m_avsWriter.Destroy();
			if (psrecInfo->m_pAviFile)	AviFileCloseAll(psrecInfo);
			break;
		}
		else
		{
			DWORD dwThreadID;
			psrecInfo->m_hVidRecordThread = ::CreateThread(NULL, 0, VideoRecordProc, psrecInfo,
															CREATE_SUSPENDED, &dwThreadID);
			if (!psrecInfo->m_hVidRecordThread)
				break;

			::SetThreadPriority(psrecInfo->m_hVidRecordThread, THREAD_PRIORITY_LOWEST);
			::ResumeThread(psrecInfo->m_hVidRecordThread);
			TTRACE(_T("## The VideoRecordProc thread(0x%x) has started.\n"), dwThreadID);
		}
		
		return SCRREC_SUCCESS;
	} while (0);

	SetCaptureState(psrecInfo, rstStopped);
    return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecStopCapture( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;
	if (!psrecInfo)
		return SCRREC_FAILED;

	if (srecGetCaptureState(psrecInfo) != rstStopped)
	{
		SetCaptureState(psrecInfo, rstStopping);

		::WaitForSingleObject(psrecInfo->m_hVidRecordThread, INFINITE);
		psrecInfo->m_hVidRecordThread = NULL;

		// stopping audio process
		//
		DWORD dwThreadID;
		HANDLE hThread = ::CreateThread(NULL, 0, StopAudioRecording, psrecInfo,
										0, &dwThreadID);
		if (hThread)
		{
			::WaitForSingleObject(hThread, 3000);
		}

		// release file writer buffers
		psrecInfo->m_avsWriter.Stop();
		psrecInfo->m_avsWriter.Destroy();

		AviFileCloseAll(psrecInfo);

		SetCaptureState(psrecInfo, rstStopped);

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecPauseCapture( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;
	if (!psrecInfo)
		return SCRREC_FAILED;

	if (srecGetCaptureState(psrecInfo) == rstRecording)
	{
		SetCaptureState(psrecInfo, rstPaused);
		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecPauseVideoCapture( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;
	if (!psrecInfo)
		return SCRREC_FAILED;

	if (srecGetCaptureState(psrecInfo) == rstRecording)
	{
		SetCaptureState(psrecInfo, rstPausedVideo);
		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecResumeCapture( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;
	if (!psrecInfo)
		return SCRREC_FAILED;

	if (srecGetCaptureState(psrecInfo) == rstPaused || srecGetCaptureState(psrecInfo) == rstPausedVideo)
	{
		SetCaptureState(psrecInfo, rstRecording);
		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}



//
// Capture Properties
//

SCRREC_API BOOL     WINAPI  srecSetAudioCaptureDevice( SRECHANDLE hCapture, DWORD dwAudioDeviceID )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_AudDeviceID = dwAudioDeviceID;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL WINAPI  srecGetAudioCaptureDevice( SRECHANDLE hCapture, DWORD * pdwAudioDeviceID )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		* pdwAudioDeviceID = psrecInfo->m_AudDeviceID;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL WINAPI  srecSetVideoCaptureDevice( SRECHANDLE hCapture, DWORD dwVideoDeviceID )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_VidDeviceID = dwVideoDeviceID;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL WINAPI srecGetVideoCaptureDevice( SRECHANDLE hCapture, DWORD * pdwVideoDeviceID )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		* pdwVideoDeviceID = psrecInfo->m_VidDeviceID;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL WINAPI srecSetScreenInput( SRECHANDLE hCapture, srecScreenInput scrInput, HWND hWnd, LPRECT pRect )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		switch (scrInput)
		{
		case cinScreen:
			psrecInfo->m_hScreenWnd = ::GetDesktopWindow();
			if (pRect == NULL) {
				::SetRect(&psrecInfo->m_rcSource, 0, 0,
						::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
			}
			else {
				psrecInfo->m_rcSource = *pRect;
			}
			break;
		case cinRegion:
			psrecInfo->m_hScreenWnd = ::GetDesktopWindow();
			if (pRect == NULL) {
				::SetRect(&psrecInfo->m_rcSource, 0, 0,
						::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
			}
			else {
				psrecInfo->m_rcSource = *pRect;
				psrecInfo->m_CursorOffsetPoint.x = pRect->left;
				psrecInfo->m_CursorOffsetPoint.y = pRect->top;
			}
			break;
		case cinWindow:
			psrecInfo->m_hScreenWnd = hWnd;
			::GetWindowRect(hWnd, &psrecInfo->m_rcSource);

			psrecInfo->m_CursorOffsetPoint.x = psrecInfo->m_rcSource.left;
			psrecInfo->m_CursorOffsetPoint.y = psrecInfo->m_rcSource.top;
			break;
		default:
			return SCRREC_FAILED;
		}

		psrecInfo->m_ScreenInput = scrInput;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetScreenInput( SRECHANDLE hCapture, srecScreenInput * scrInput, HWND * phWnd, LPRECT pRect )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*scrInput = (srecScreenInput) psrecInfo->m_ScreenInput;
		*phWnd = psrecInfo->m_hScreenWnd;
		*pRect = psrecInfo->m_rcSource;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}


//
// Advanced Capture Properties
//

SCRREC_API BOOL     WINAPI  srecSetAudioEnable( SRECHANDLE hCapture, BOOL bEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bAudEnable = bEnable;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetAudioEnable( SRECHANDLE hCapture, BOOL * pbEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		 *pbEnable = psrecInfo->m_bAudEnable;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetMuteAudio( SRECHANDLE hCapture, BOOL bMute )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bAudMute = bMute;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetMuteAudio( SRECHANDLE hCapture, BOOL * bMute )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		 *bMute = psrecInfo->m_bAudMute;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetRecordCursor( SRECHANDLE hCapture, BOOL bEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bRecordCursor = bEnable;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetRecordCursor( SRECHANDLE hCapture, BOOL * pbEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		 *pbEnable = psrecInfo->m_bRecordCursor;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetRecordLogo( SRECHANDLE hCapture, BOOL bEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bRecordLogo = bEnable;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}



SCRREC_API BOOL     WINAPI  srecSetRecordLayeredWnd( SRECHANDLE hCapture, BOOL bEnable )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bRecordLayeredWnd = bEnable;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}


SCRREC_API BOOL     WINAPI  srecSetDrawing( SRECHANDLE hCapture, BOOL bDrawing )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_bDrawing = bDrawing;
		return SCRREC_SUCCESS;
	}
	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetCustomCursor( SRECHANDLE hCapture, HCURSOR hCursor )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_hCustomCursor = hCursor;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetCustomCursor( SRECHANDLE hCapture, HCURSOR * hCursor )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		 *hCursor = psrecInfo->m_hCustomCursor;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}


SCRREC_API BOOL     WINAPI  srecSetCustomLogo( SRECHANDLE hCapture, HBITMAP hBitmap )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo && hBitmap)
	{
		psrecInfo->m_hCustomLogo = hBitmap;
		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

//
// Recording Callbacks
//

SCRREC_API BOOL     WINAPI  srecSetStateChangeCallback( SRECHANDLE hCapture, SRECSTATECHANGEPROC pStateChangeProc, DWORD dwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_pStateCBProc = pStateChangeProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetStateChangeCallback( SRECHANDLE hCapture, SRECSTATECHANGEPROC * pStateChangeProc, DWORD * pdwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*pStateChangeProc = psrecInfo->m_pStateCBProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetErrorCallback( SRECHANDLE hCapture, SRECERRORPROC pErrorProc, DWORD dwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_pErrorCBProc = pErrorProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetErrorCallback( SRECHANDLE hCapture, SRECERRORPROC * pErrorProc, DWORD * pdwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*pErrorProc = psrecInfo->m_pErrorCBProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecSetVideoCallback( SRECHANDLE hCapture, SRECVIDEOPROC pVideoProc, BOOL bCursorInfo, DWORD dwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_pVideoCBProc = pVideoProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetVideoCallback( SRECHANDLE hCapture, SRECVIDEOPROC * pVideoProc, BOOL * pbCursorInfo, DWORD * pdwData )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*pVideoProc = psrecInfo->m_pVideoCBProc;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}



//
// Capture runtime statistics
//

SCRREC_API srecState WINAPI  srecGetCaptureState( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		return (srecState) psrecInfo->m_CaptureState;
	}

	return (srecState) 0;
}

SCRREC_API int    WINAPI  srecGetCaptureFrames( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		return psrecInfo->m_CaptureFrames;
	}

	return 0;
}


SCRREC_API double    WINAPI  srecGetActualFrameRate( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		if (psrecInfo->m_Duration > 0)
			return (double) srecGetCaptureFrames( hCapture )/(double) srecGetDuration( hCapture );
		else
			return .0;
	}

	return .0;
}

SCRREC_API int    WINAPI  srecGetDroppedFrames( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		return psrecInfo->m_DroppedFrames;
	}

	return 0;
}

SCRREC_API int    WINAPI  srecGetDuration( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		return psrecInfo->m_Duration;
	}

	return 0;
}



//
// File Properties
//

SCRREC_API BOOL     WINAPI  srecSetFileName( SRECHANDLE hCapture, const char * pszFileName )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		if (pszFileName)
		{
			int len = _tcslen(pszFileName);
			_tcsncpy(psrecInfo->m_szAviFileName, pszFileName, len+1);

			return SCRREC_SUCCESS;
		}
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetFileName( SRECHANDLE hCapture, char * pszFileName )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		if (pszFileName)
		{
			int len = _tcslen(psrecInfo->m_szAviFileName);
			_tcsncpy(pszFileName, psrecInfo->m_szAviFileName, len+1);

			return SCRREC_SUCCESS;
		}
	}

	return SCRREC_FAILED;
}


//
// AVI Options
//

SCRREC_API BOOL     WINAPI srecSetAviOptions( SRECHANDLE hCapture, DWORD dwFps, DWORD dwQuality, DWORD dwKeyFramesEvery )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		if (dwFps < 0) dwFps = 1;

		psrecInfo->m_fps = dwFps;
		psrecInfo->m_CompQuality = dwQuality;
		psrecInfo->m_KeyFramesEvery = dwKeyFramesEvery;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI srecGetAviOptions( SRECHANDLE hCapture, DWORD * dwFps, DWORD * dwQuality, DWORD * dwKeyFramesEvery )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*dwFps = psrecInfo->m_fps;
		*dwQuality = psrecInfo->m_CompQuality;
		*dwKeyFramesEvery = psrecInfo->m_KeyFramesEvery;

		return SCRREC_SUCCESS;
	}

	return SCRREC_FAILED;
}


SCRREC_API BOOL     WINAPI  srecSetAviVideoCodec( SRECHANDLE hCapture, DWORD Codec, bool bUseVidCompDlg )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_VidCodec = Codec;
		psrecInfo->m_bUseVidCompDlg = bUseVidCompDlg;

		return SCRREC_SUCCESS;
	}

    return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetAviVideoCodec( SRECHANDLE hCapture, DWORD * pCodec )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*pCodec = psrecInfo->m_VidCodec;

		return SCRREC_SUCCESS;
	}

    return SCRREC_FAILED;
}


SCRREC_API BOOL     WINAPI  srecSetAviAudioCodec( SRECHANDLE hCapture, DWORD Codec )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		psrecInfo->m_AudCodec = Codec;

		return SCRREC_SUCCESS;
	}

    return SCRREC_FAILED;
}

SCRREC_API BOOL     WINAPI  srecGetAviAudioCodec( SRECHANDLE hCapture, DWORD * pCodec )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		*pCodec = psrecInfo->m_AudCodec;

		return SCRREC_SUCCESS;
	}

    return SCRREC_FAILED;
}


//
// Miscellaneous 
//

SCRREC_API DWORD    WINAPI  srecGetAudioDevices( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		unsigned int numDevs = ::waveInGetNumDevs();

		return numDevs;
	}

    return SCRREC_FAILED;
}

SCRREC_API srecError WINAPI  srecGetLastError( SRECHANDLE hCapture )
{
	SRECINFO * psrecInfo = (SRECINFO *) hCapture;

	if (psrecInfo)
	{
		return psrecInfo->m_LastError;
	}

    return serrInvalidParam;
}

SCRREC_API TCHAR * WINAPI  srecGetLastErrorMsg( srecError err )
{
	switch (err)
	{
	case serrNone:
	   return _T("No errors");
	case serrInvalidState:
	   return NULL;
	case serrInvalidParam:
	   return NULL;
	case serrInvalidTempFolder:
	   return NULL;
	case serrInvalidOutputFile:
	   return NULL;
	case serrInvalidInput:
	   return NULL;
	case serrNoVideoCallback:
	   return NULL;
	case serrNoWaveInDevice:
	   return NULL;
	case serrResourceAllocationFailed:
	   return NULL;
	case serrAudioFormatInvalid:
	   return NULL;
	case serrAudioFormatUnavailable:
	   return NULL;
	case serrAudioDeviceOpenFailed:
		return _T("Can't open audio device with current audio format");
	   return NULL;
	case serrAudioDeviceStartFailed:
	   return NULL;
	case serrAudioWriteFailed:
		return _T("Can't write audio data to the AVI File");
	case serrVideoWriteFailed:
		return _T("Can't write video data to the AVI File");
	case serrAviUnsupported:
	   return _T("Unsupported");
	case serrAviFileOpenFailed:
	   return _T("Can't open the AVI File");
	case serrAviWriteFailed:
	   return _T("Can't write data to the AVI File");
	case serrVFWVersionTooOld:
		return "Video for Windows version too old";

	}

	return NULL;
}



//////////////////////////////////////////////////////////////////////////////////
// DllMain
//////////////////////////////////////////////////////////////////////////////////

static char* _cstr_ = "BCL Screen Recorder SDK V1.0 \n"
					   "Copyright(c) 2003 bclsoft Inc.  All rights reserved. \n";

static WORD _cstr_s0_ = 0xEFFE;
static WORD _cstr_crc16_ = 0x04d9;
static WORD _cstr_s1_ = 0xEFFE;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			CCrc16 crc((unsigned char *) _cstr_, strlen(_cstr_));
			if (crc.m_crc16 != _cstr_crc16_)
			{
				::MessageBox(::GetForegroundWindow(),
					"This DLL file is corrupted by virus or hacker !!",
					"Error", MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

