// VSCap.cpp: implementation of the VSCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VidCapTest.h"
#include "VSCap.h"


VSCAPPROC	VSCap::m_pCallBackProc = NULL;
HANDLE		VSCap::m_hVidRecordThread = NULL;

PAVIFILE	VSCap::m_pAviFile = NULL;
PAVISTREAM	VSCap::m_pVidStream = NULL;
PAVISTREAM	VSCap::m_pAudStream = NULL;

TCHAR *		VSCap::m_pszFileName = NULL;
int			VSCap::m_RecordState = VSCap::Stoped;

int			VSCap::m_fps = 15;
int			VSCap::m_Timelapse = 1000/m_fps;
int			VSCap::m_nKeyFramesEvery = VSCap::m_fps;
int			VSCap::m_nCompQuality = 8500;

RECT		VSCap::m_rcSource = {0,0,0,0};
RECT		VSCap::m_rcTarget = {0,0,0,0};

bool			VSCap::m_bEnableAudioDevice = true;
UINT			VSCap::m_AudDeviceID = WAVE_MAPPER;
HWAVEIN			VSCap::m_hAudInputDevice = NULL;
VSCWAVEFORMAT	VSCap::m_AudFormat;
int				VSCap::m_AudBufferSize = 1000;	// samples per callback
LPWAVEHDR		VSCap::m_pAudQueue[MAX_AUD_QUEUE_SIZE];
int				VSCap::m_AudQueuedBuffers=0;
//CSoundFile*	VSCap::m_pAudFile = NULL;

DWORD		VSCap::m_nVidComp = mmioFOURCC('M', 'S', 'V', 'C');
int			VSCap::m_nAudComp = -1;

int			VSCap::m_ScreenBits = 24;
bool		VSCap::m_bRecordCursor = false;

DWORD		VSCap::m_FrameTime = 0;		

void VSCap::Initialize()
{
	UnInitialize();

	m_pCallBackProc = NULL;
	m_hVidRecordThread = NULL;

	m_pAviFile = NULL;
	m_pVidStream = NULL;
	m_pAudStream = NULL;

	m_pszFileName = new TCHAR[MAX_PATH];
	m_RecordState = VSCap::Stoped;

	m_fps = 15;
	m_Timelapse = 1000/m_fps;
	m_nKeyFramesEvery = m_fps;
	m_nCompQuality = 6500;

	::SetRect(&m_rcSource, 0,0,0,0);
	::SetRect(&m_rcTarget, 0,0,0,0);

	m_nVidComp = mmioFOURCC('M', 'S', 'V', 'C');
	m_nAudComp = VSC_ACM_GSM610;;

	m_hAudInputDevice = NULL;

	HDC hScreenDC = ::GetDC(NULL);	
	m_ScreenBits = ::GetDeviceCaps(hScreenDC, BITSPIXEL );	
	//nColors = bits;
	::ReleaseDC(NULL,hScreenDC);		
}

void VSCap::UnInitialize()
{
	if (m_RecordState != VSCap::Stoped)
		RecordStop();

	SAFE_DELETE(m_pszFileName);
}

int VSCap::RecordStart(LPCTSTR pszFileName, int fps, int nKeyFramesEvery, bool bRecordCursor)	// (LPVOID pParam)
{
	if (!pszFileName) return 0;
	if (fps < 15) fps = 15;
	if (fps > 30) fps = 30;

	m_fps = fps;
	m_Timelapse = 1000/m_fps;
	m_nKeyFramesEvery = nKeyFramesEvery;

	// prepare AVI stream
	//
	_tcscpy(m_pszFileName, pszFileName);
	if (!PrepareAviFile())
		return 0;

	DWORD dwThreadID;
	m_hVidRecordThread = ::CreateThread(NULL, 0, VidRecordThread, NULL, CREATE_SUSPENDED, &dwThreadID);
	if (!m_hVidRecordThread)
		return 0;

	m_RecordState = VSCap::Recording;
	m_bRecordCursor = bRecordCursor;

	::ResumeThread(m_hVidRecordThread);

	return (int)m_hVidRecordThread;
}

ULONG WINAPI VSCap::VidRecordThread(LPVOID pParam)
{
	HRESULT hr;

	LPBITMAPINFOHEADER alpbi;

	DWORD TimeExpended, OldFrameTime=0;		
	DWORD InitialTime = ::timeGetTime();	
	bool bInitCapture = true;	

	int nActualFrame = 0;
	int nCurrFrame = 0;
	float fRate = 0.0;
	float fActualRate = 0.0;
	float fTimeLength = 0.0;

	// start thread loop
	//
	while (m_RecordState != VSCap::Stoped)
	{
		// align time flags
		if (bInitCapture)
			m_FrameTime = TimeExpended = 0;
		else
			TimeExpended = ::timeGetTime() - InitialTime;
		
		// capture screen frame or injected frame from callback procedure
		//
		if (m_pCallBackProc)
			m_pCallBackProc(0, 0);
		else
		{
			alpbi = CaptureScreenFrame(m_rcSource.left, m_rcSource.top, m_rcSource.right, m_rcSource.bottom, m_bRecordCursor);
			if (alpbi == NULL)
			{
				::MessageBox(NULL, _T("Screen Capture error!!!"), "Error" , MB_OK|MB_ICONSTOP);
				break;
			}
		}

		// caculate m_FrameTime
		//
		if (!bInitCapture)
		{
			if (m_Timelapse > 1000)
				m_FrameTime++;
			else
				m_FrameTime = (DWORD) (((double) TimeExpended/1000.0 ) * (double) m_fps);	//(1000.0/m_Timelapse));
		}
		else
		{
			bInitCapture = false;
		}
		
		fTimeLength = ((float) TimeExpended) / ((float) 1000.0);
		
		
		// write captured frame into AVI stream if m_FrameTime is different
		//
		if ((m_FrameTime == 0) || (m_FrameTime > OldFrameTime))
		{ 
			//if frametime repeats...the avistreamwrite will cause an error
			hr = ::AVIStreamWrite(m_pVidStream,	// stream pointer
				m_FrameTime,				// time of this frame
				1,						// number to write
				(LPBYTE) alpbi +		// pointer to data
					alpbi->biSize +
					alpbi->biClrUsed * sizeof(RGBQUAD),
					alpbi->biSizeImage,	// size of this frame
				//AVIIF_KEYFRAME,			 // flags....
				0,    //Dependent n previous frame, not key frame
				NULL,
				NULL);
			if (hr != AVIERR_OK)
			{
				m_RecordState = VSCap::Stoped;
				::MessageBox(NULL,"Error Writing Video data", "Error", MB_OK | MB_ICONEXCLAMATION);
				break;
			}

			nActualFrame++ ;
			nCurrFrame = m_FrameTime;
			fRate = ((float) nCurrFrame)/fTimeLength;						
			fActualRate = ((float) nActualFrame)/fTimeLength;
			
			// free memory
			SAFE_RELEASE_GLOBAL(alpbi);

			OldFrameTime = m_FrameTime;

		} // if m_FrameTime is different


		// introduce time lapse
		// maximum lapse when recordstate changes will be less than 100 milliseconds
		int no_iteration = m_Timelapse/50;
		int remainlapse = m_Timelapse - no_iteration*50;		 
		for (int i=0; i<no_iteration; i++)		
		{
			::Sleep(50); //Sleep for 50 milliseconds many times
			if (m_RecordState == VSCap::Stoped) break;
		}
		if (m_RecordState == VSCap::Recording) ::Sleep(remainlapse);

		// Paused Process
		//
		DWORD TimePauseStart = ::timeGetTime();
		while (m_RecordState == VSCap::Paused)
		{
			::Sleep(50);

			// 일시 정지된 시간을 반영
			if (m_RecordState != VSCap::Paused)
			{
				InitialTime += ::timeGetTime() - TimePauseStart;
				break;
			}
		}
	} //for loop

	if (alpbi)
	{
		// free memory
		// 메모리를 한번만 설정해야 함.
		SAFE_RELEASE_GLOBAL(alpbi);
	}

	m_hVidRecordThread = NULL; // dead

	return 0;
}

void VSCap::RecordPause()
{
	if (m_RecordState == VSCap::Recording)
		m_RecordState = VSCap::Paused;
}

void VSCap::RecordResume()
{
	if (m_RecordState == VSCap::Paused)
		m_RecordState = VSCap::Recording;
}


void VSCap::RecordStop()
{
	m_RecordState = VSCap::Stoped;

	::Sleep(200);

	if (m_hVidRecordThread != NULL)
	{
		::TerminateThread(m_hVidRecordThread, 0);
		m_hVidRecordThread = NULL;
	}

	AviFileClose();

	StopAudioRecording();
//		SAFE_DELETE(m_pAudFile);
}

LPBITMAPINFOHEADER VSCap::CaptureScreenFrame(int l, int t, int r, int b, bool bRecordCursor)
{
	HDC hScreenDC = ::GetDC(NULL);

	HDC hMemDC = ::CreateCompatibleDC(hScreenDC);     
	HBITMAP hbm;
	
	int TargetWidth = m_rcTarget.right-m_rcTarget.left;
	int TargetHeight= m_rcTarget.bottom-m_rcTarget.top;

	if (TargetWidth == 0)  TargetWidth = r - l;
	if (TargetHeight == 0) TargetHeight = b - t;

    hbm = ::CreateCompatibleBitmap(hScreenDC, TargetWidth, TargetHeight);
	HBITMAP oldbm = (HBITMAP) ::SelectObject(hMemDC, hbm);	 
	//::BitBlt(hMemDC, 0, 0, m_rcTarget.right-m_rcTarget.left, m_rcTarget.bottom-m_rcTarget.top, hScreenDC, l, t, SRCCOPY);	 	
	BOOL iRet = ::StretchBlt(hMemDC, m_rcTarget.left, m_rcTarget.top, TargetWidth, TargetHeight,
						hScreenDC,	m_rcSource.left, m_rcSource.top,
						m_rcSource.right-m_rcSource.left, m_rcSource.bottom-m_rcSource.top,
						SRCCOPY);
	::ReleaseDC(NULL, hScreenDC);	
	if (!iRet) return NULL;
	
	//Draw the Cursor	
	if (bRecordCursor)
	{
		//Get Cursor Pos
		//HCURSOR hcur = ::GetCursor();	//FetchCursorHandle();
		CURSORINFO cinfo;	
		::memset(&cinfo, 0, sizeof(cinfo));
		cinfo.cbSize = sizeof(CURSORINFO);
		BOOL ret = ::GetCursorInfo(&cinfo); 
		if (ret && cinfo.flags == CURSOR_SHOWING)
		{
			POINT xPoint = cinfo.ptScreenPos;
			xPoint.x -= l;	xPoint.y -= t;

			ICONINFO iinfo;
			ret = ::GetIconInfo(cinfo.hCursor, &iinfo);
			if (ret)
			{
				xPoint.x -= iinfo.xHotspot;
				xPoint.y -= iinfo.yHotspot;

				//need to delete the hbmMask and hbmColor bitmaps
				//otherwise the program will crash after a while after running out of resource
				if (iinfo.hbmMask)  ::DeleteObject(iinfo.hbmMask);
				if (iinfo.hbmColor) ::DeleteObject(iinfo.hbmColor);
			}

			//::DrawIcon( hScreenDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
			::DrawIcon( hMemDC,  xPoint.x,  xPoint.y, cinfo.hCursor);
		}
	}
	//::ReleaseDC(NULL, hScreenDC);	
	
	
	::SelectObject(hMemDC,oldbm);    			
	LPBITMAPINFOHEADER pBM_HEADER = (LPBITMAPINFOHEADER)GlobalLock(Bitmap2Dib(hbm, m_ScreenBits));	
	//LPBITMAPINFOHEADER pBM_HEADER = (LPBITMAPINFOHEADER)GlobalLock(Bitmap2Dib(hbm, 24));	
	if (pBM_HEADER == NULL)
	{
		return NULL;
		//::MessageBox(NULL,"Error reading a frame!","Error",MB_OK | MB_ICONEXCLAMATION);					
		//exit(1);
	}    

	DeleteObject(hbm);			
	DeleteDC(hMemDC);	
	
	
	//if flashing rect
	//if (flashingRect && !tempDisableRect) {
	//	
	//	if (autopan)	{
	//		DrawFlashingRect(FALSE , 1);
	//	}
	//	else
	//		DrawFlashingRect(FALSE , 0);
	//}	
	
	return pBM_HEADER;
}

HANDLE  VSCap::Bitmap2Dib(HBITMAP hbitmap, UINT bits)
{
	HANDLE              hdib ;
	HDC                 hdc ;
	BITMAP              bitmap ;
	UINT                wLineLen ;
	DWORD               dwSize ;
	DWORD               wColSize ;
	LPBITMAPINFOHEADER  lpbi ;
	LPBYTE              lpBits ;
	
	GetObject(hbitmap,sizeof(BITMAP),&bitmap) ;

	//
	// DWORD align the width of the DIB
	// Figure out the size of the colour table
	// Calculate the size of the DIB
	//
	wLineLen = (bitmap.bmWidth*bits+31)/32 * 4;
	wColSize = sizeof(RGBQUAD)*((bits <= 8) ? 1<<bits : 0);
	dwSize = sizeof(BITMAPINFOHEADER) + wColSize +
		(DWORD)(UINT)wLineLen*(DWORD)(UINT)bitmap.bmHeight;

	//
	// Allocate room for a DIB and set the LPBI fields
	//
	hdib = GlobalAlloc(GHND,dwSize);
	if (!hdib)
		return hdib ;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib) ;

	lpbi->biSize = sizeof(BITMAPINFOHEADER) ;
	lpbi->biWidth = bitmap.bmWidth ;
	lpbi->biHeight = bitmap.bmHeight ;
	lpbi->biPlanes = 1 ;
	lpbi->biBitCount = (WORD) bits ;
	lpbi->biCompression = BI_RGB ;
	lpbi->biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize ;
	lpbi->biXPelsPerMeter = 0 ;
	lpbi->biYPelsPerMeter = 0 ;
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;
	lpbi->biClrImportant = 0 ;

	//
	// Get the bits from the bitmap and stuff them after the LPBI
	//
	lpBits = (LPBYTE)(lpbi+1)+wColSize ;

	hdc = CreateCompatibleDC(NULL) ;

	GetDIBits(hdc,hbitmap,0,bitmap.bmHeight,lpBits,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;

	DeleteDC(hdc) ;
	GlobalUnlock(hdib);

	return hdib ;
}

bool VSCap::PrepareAviFile()
{
	HRESULT hr;
	AVISTREAMINFO sti;
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	PAVISTREAM pVidSourceStream = NULL;

	int TargetWidth  = m_rcTarget.right - m_rcTarget.left;
	int TargetHeight = m_rcTarget.bottom - m_rcTarget.top;	
	
	// check vfw version
	WORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a)
	{			 
		::MessageBox(NULL, "Error! Video for Windows version too old!", "Error" , MB_OK|MB_ICONSTOP);
		return false;
	}

	////////////////////////////////////////////////
	// CAPTURE FIRST FRAME
	////////////////////////////////////////////////	
	LPBITMAPINFOHEADER alpbi = NULL;
	alpbi = CaptureScreenFrame(m_rcSource.left, m_rcSource.top, 
								m_rcSource.right, m_rcSource.bottom, m_bRecordCursor);

	DWORD compfccHandler = m_nVidComp;	//mmioFOURCC('M', 'S', 'V', 'C');	

	//Special Cases
	if (compfccHandler == mmioFOURCC('D', 'I', 'V', 'X'))	
	{ //Still Can't Handle DIVX
		compfccHandler = mmioFOURCC('M', 'S', 'V', 'C');	
	}	

	
	if (compfccHandler==mmioFOURCC('I', 'V', '5', '0'))	
	{ //Still Can't Handle Indeo 5.04
		compfccHandler = mmioFOURCC('M', 'S', 'V', 'C');	
	}
	

	do	// start critical block
	{

	//
	// Open the movie file for writing....
	//
	AVIFileInit();
	::DeleteFile(m_pszFileName);
	hr = ::AVIFileOpen(&m_pAviFile, m_pszFileName, OF_WRITE | OF_CREATE | OF_SHARE_DENY_WRITE, NULL);	
	if (hr != AVIERR_OK) 
	{
		::MessageBox(NULL,"Error Creating AVI File","Error",MB_OK | MB_ICONEXCLAMATION);
		break;
	}

	// 1. And create the video source stream
	//

	::memset(&sti, 0, sizeof(sti));
	sti.fccType                = streamtypeVIDEO;// stream type
	//sti.fccHandler             = compfccHandler;
	sti.fccHandler             = 0;
	sti.dwScale                = 1;		// fps = dwRate / dwScale
	sti.dwRate                 = m_fps;		    
	sti.dwSuggestedBufferSize  = alpbi->biSizeImage;
	::SetRect(&sti.rcFrame, 0, 0,		    // rectangle for stream
				(int) alpbi->biWidth,
				(int) alpbi->biHeight);
	hr = ::AVIFileCreateStream(m_pAviFile, &pVidSourceStream, &sti);
	if (hr != AVIERR_OK) break; 
	
	

	// 2. create the compressed video stream;
	//

	// Fill in the compress options
	//
	::memset(&opts, 0, sizeof(opts)); 
	aopts[0]->fccType			= streamtypeVIDEO;
	//aopts[0]->fccHandler		= mmioFOURCC('M', 'S', 'V', 'C');
	aopts[0]->fccHandler		= compfccHandler;
	aopts[0]->dwKeyFrameEvery	= m_nKeyFramesEvery;	// keyframe rate 
	aopts[0]->dwQuality			= m_nCompQuality;		// compress quality 0-10,000 
	aopts[0]->dwBytesPerSecond	= 0;					// bytes per second 
	aopts[0]->dwFlags			= AVICOMPRESSF_VALID | AVICOMPRESSF_KEYFRAMES;	//AVICOMPRESSF_INTERLEAVE;    // flags 		
	aopts[0]->lpFormat			= 0x0;                  // save format 
	aopts[0]->cbFormat			= 0;
	aopts[0]->dwInterleaveEvery = 0;

	//The 1 here indicates only 1 stream
	//if (!AVISaveOptions(NULL, 0, 1, &ps, (LPAVICOMPRESSOPTIONS *) &aopts)) 
	//        goto error;
	
	hr = ::AVIMakeCompressedStream(&m_pVidStream, pVidSourceStream, &opts, NULL);
	if (hr != AVIERR_OK)  break; 

	hr = ::AVIStreamSetFormat(m_pVidStream, 0, 
			       alpbi,	    // stream format
			       alpbi->biSize +   // format size
			       alpbi->biClrUsed * sizeof(RGBQUAD));
	if (hr != AVIERR_OK) break;	

	// 3. create the audio stream;
	//

	InitAudRecording();

	if (m_bEnableAudioDevice)
	{
		::memset(&sti, 0, sizeof(sti));
		sti.fccType                = streamtypeAUDIO;// stream type
		//sti.fccHandler             = compfccHandler;
		sti.fccHandler             = 0;
		sti.dwScale                = 1;
		sti.dwStart                = 0;	// delay 0

		// Specifies how large a buffer should be used to read this stream. Typically,
		// this contains a value corresponding to the largest chunk present in the stream.
		// Using the correct buffer size makes playback more efficient. Use zero if you 
		// do not know the correct buffer size.
		//
		sti.dwSuggestedBufferSize  = m_AudBufferSize;

		// Specifies the size of a single sample of data. This is set to zero if the samples
		// can vary in size. If this number is nonzero, then multiple samples of data can be
		// grouped into a single chunk within the file. If it is zero, each sample of data
		// (such as a video frame) must be in a separate chunk. For video streams, this number
		// is typically zero, although it can be nonzero if all video frames are the same size.
		//
		// For audio streams, this number should be the same as the nBlockAlign member of the
		// WAVEFORMATEX structure describing the audio.
		//
		sti.dwSampleSize = m_AudFormat.wfx.nBlockAlign;

		// Used with dwRate to specify the time scale that this stream will use.
		// Dividing dwRate by dwScale gives the number of samples per second. For video streams, 
		// this is the frame rate. For audio streams, this rate corresponds to the time needed to 
		// play nBlockAlign bytes of audio, which for PCM audio is the just the sample rate.
		//
		if (m_nAudComp == VSC_ACM_PCM)
			sti.dwRate = m_AudFormat.wfx.nSamplesPerSec;
		else	// compressed audio format
			sti.dwRate = m_AudFormat.wfx.nAvgBytesPerSec / m_AudFormat.wfx.nBlockAlign;

		hr = ::AVIFileCreateStream(m_pAviFile, &m_pAudStream, &sti);
		if (hr != AVIERR_OK) break; 


		if (m_nAudComp == VSC_ACM_PCM)
			hr = ::AVIStreamSetFormat(m_pAudStream, 0, &m_AudFormat, sizeof(WAVEFORMATEX));
		else	// compressed audio format
			hr = ::AVIStreamSetFormat(m_pAudStream, 0, &m_AudFormat, sizeof(VSCWAVEFORMAT));
		if (hr != AVIERR_OK) break;	

		hr = StartAudRecording();
	}

	} while (0);	// end of critical block


	// release resources
	//
	SAFE_RELEASE_GLOBAL(alpbi);
	::AVISaveOptionsFree(1, (LPAVICOMPRESSOPTIONS FAR *) &aopts);
	if (pVidSourceStream)	::AVIStreamClose(pVidSourceStream);

	if (hr != S_OK)
	{
		if (m_pAviFile) AviFileClose();

		return false;
	}
	else
		return true;
}

void VSCap::AviFileClose()
{
	if (m_pVidStream)  ::AVIStreamClose(m_pVidStream);
	if (m_pAudStream)  ::AVIStreamClose(m_pAudStream);
	if (m_pAviFile)    ::AVIFileClose(m_pAviFile);

	::AVIFileExit();
}


///////////
//
// Wave functions
//
///////////////////////////////////////////////////

HRESULT VSCap::StartAudRecording()
{
	MMRESULT mmReturn = 0;	

	// open wavein device 
	//m_AudDeviceID=1;
	mmReturn = ::waveInOpen(&m_hAudInputDevice, m_AudDeviceID, (LPCWAVEFORMATEX) &m_AudFormat,
							(DWORD) (void *)WaveInProc, NULL, CALLBACK_FUNCTION);
	if(mmReturn)
	{
		WaveInErrorMsg(mmReturn);
		return mmReturn;
	}
	else
	{
		// make several input buffers and add them to the input queue
		for(int i=0; i<3; i++)
		{
			AddInputBufferToQueue();
		}
		
		// start recording
		mmReturn = ::waveInStart(m_hAudInputDevice);
		if(mmReturn )
		{
			WaveInErrorMsg(mmReturn);
			return mmReturn;
		}
	}

	return S_OK;
}

void VSCap::StopAudioRecording()
{
	if (!m_hAudInputDevice)
		return;

	MMRESULT mmReturn = MMSYSERR_NOERROR;
	
	//mmReturn = ::waveInStop(m_hAudInputDevice);
	//if (mmReturn) WaveInErrorMsg(mmReturn);

	//mmReturn = ::waveInReset(m_hAudInputDevice);
	//if(mmReturn) WaveInErrorMsg(mmReturn);

	mmReturn = ::waveInClose(m_hAudInputDevice);
	if (mmReturn == WAVERR_STILLPLAYING)
	{
		// cleanup audio quque
		for (int i=0; i < m_AudQueuedBuffers; i++)
		{
			if (::waveInUnprepareHeader(m_hAudInputDevice, m_pAudQueue[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
				break;

			::GlobalFree(::GlobalHandle(m_pAudQueue[i]));
			m_pAudQueue[i] = NULL;
		}
	}
	else if (mmReturn)
		WaveInErrorMsg(mmReturn);
	
	m_AudQueuedBuffers = 0;
}

HRESULT VSCap::InitAudRecording()
{
	m_AudQueuedBuffers = 0;
	m_hAudInputDevice = NULL;
	
	if (::waveInGetNumDevs())
        m_bEnableAudioDevice = true;
	else
        m_bEnableAudioDevice = false;

	if (m_bEnableAudioDevice)
	{
		BuildAudRecordingFormat();

		/*
		//Create temporary wav file for audio recording
		SAFE_DELETE(m_pAudFile);
		m_pAudFile = new CSoundFile(_T("c:\\vscap.wav"), &m_AudFormat);	

		if  (!(m_pAudFile && m_pAudFile->IsOK()))
		{
			::MessageBox(NULL,"Error Creating Sound File","Error",MB_OK | MB_ICONEXCLAMATION);
			//MessageOut(NULL,IDS_STRING_ERRSOUND ,IDS_STRING_NOTE,MB_OK | MB_ICONEXCLAMATION);
			return AVIERR_FILEOPEN;
		}
		*/
	}
	
	return AVIERR_OK;
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
void VSCap::BuildAudRecordingFormat()
{
	::memset(&m_AudFormat, 0, sizeof(m_AudFormat));

	switch (m_nAudComp)
	{
	case VSC_ACM_PCM:
		{
		m_AudFormat.wfx.wFormatTag	   = WAVE_FORMAT_PCM;		
		m_AudFormat.wfx.nChannels      = 1;						// mono
		m_AudFormat.wfx.wBitsPerSample = (WORD) 16;
		m_AudFormat.wfx.nSamplesPerSec = 8000;					// 8.0kHz, 11.025kHz, 22.05kHz, 44.1kHz
		m_AudFormat.wfx.nBlockAlign    = (WORD) m_AudFormat.wfx.nChannels * (m_AudFormat.wfx.wBitsPerSample/8);
		m_AudFormat.wfx.nAvgBytesPerSec = m_AudFormat.wfx.nSamplesPerSec * m_AudFormat.wfx.nBlockAlign;
		m_AudFormat.wfx.cbSize = 0;

		m_AudBufferSize = m_AudFormat.wfx.nBlockAlign * m_AudFormat.wfx.nAvgBytesPerSec / 10;
		}
		break;
	case VSC_ACM_ADPCM:
	case VSC_ACM_GSM610:
	default:
		{
		m_AudFormat.wfx.wFormatTag = WAVE_FORMAT_GSM610;
		m_AudFormat.wfx.nChannels = 1;
		m_AudFormat.wfx.nSamplesPerSec = 8000;		// 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz. For non-PCM formats
													// GSM610의 경우 320의 배수여야 함.
		m_AudFormat.wfx.nBlockAlign = 65;			// * fixed transfer buffer size ( 8 KHz일 경우 40ms(1/25 sec) 마다 전송(25번) )
		m_AudFormat.wSamplesPerBlock = 320;			// * fixed sampling size ( 샘플링 단위 320 bytes = 1/8000 sec)
		m_AudFormat.wfx.wBitsPerSample = 0;			// always 0 for compressed audio format.

		int fps = m_AudFormat.wfx.nSamplesPerSec / m_AudFormat.wSamplesPerBlock;	// 8000 Hz / 320 = 25 fps

		m_AudFormat.wfx.nAvgBytesPerSec =
   				fps * m_AudFormat.wfx.nBlockAlign;	// 1625 = nAvgBytesPerSec = (nSamplesPerSec)/320*65
		m_AudFormat.wfx.cbSize = 2;					// a count of extra byte for extension wave formats ( eg. GSM610WAVEFORMAT )

		m_AudBufferSize = m_AudFormat.wfx.nAvgBytesPerSec / fps * 2;
		}
		break;
	}
}

void VSCap::DataFromSoundIn(char* pBuffer, int length) 
{
	// Write WAV data to AVI stream
	HRESULT hr = ::AVIStreamWrite(m_pAudStream, -1, // m_FrameTime, append at end of stream
									1, pBuffer, length,	AVIIF_KEYFRAME, NULL, NULL);
	if(hr != 0)
	{
		m_RecordState = VSCap::Stoped;
		::MessageBox(NULL,"Error Writing Audio data", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

/*
	if (m_pAudFile)
	{
		if (!m_pAudFile->Write(buffer))
		{
			StopAudioRecording();
			SAFE_DELETE(m_pAudFile);
			::MessageBox(NULL,"Error Writing Sound File", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
	}
*/
}


void VSCap::WaveInErrorMsg(MMRESULT result)
{
	TCHAR szErrMsg[500+1];
	::waveInGetErrorText(result, szErrMsg, 500);

	::MessageBox(NULL, szErrMsg, _T("Audio Error"), MB_OK | MB_ICONEXCLAMATION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 디바이스 드라이버가 waveInAddBuffer함수를 사용하여 보내어진
// 데이터 블록을 인코딩된 오디오 데이터로 채웠을 때 호출됨.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK VSCap::WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	MMRESULT mmReturn = 0;
	LPWAVEHDR pHdr = (LPWAVEHDR) dwParam1;

	if (uMsg == WIM_OPEN)
		return;

	if (uMsg != WIM_DATA)
		return;

	TRACE("WIM_DATA %4d bytes data\n", pHdr->dwBytesRecorded);
	
	// When m_RecordState is Recording or Paused
	if (m_RecordState != VSCap::Stoped && pHdr->dwBytesRecorded > 0)
	{	
		//Write Data to file
		if (m_RecordState == VSCap::Recording)
			DataFromSoundIn(pHdr->lpData, pHdr->dwBytesRecorded);

		// reuse the buffer:
		// prepare it again
		mmReturn = ::waveInPrepareHeader(hwi, pHdr, sizeof(WAVEHDR));
		if(mmReturn) WaveInErrorMsg(mmReturn);
		else // no error
		{
			// add the input buffer to the queue again
			mmReturn = ::waveInAddBuffer(hwi, pHdr, sizeof(WAVEHDR));
			if (mmReturn)	WaveInErrorMsg(mmReturn);
			else			return;  // no error
		}
	}
} 

int VSCap::AddInputBufferToQueue()
{
	MMRESULT mmReturn = 0;
	
	if (m_AudQueuedBuffers >= MAX_AUD_QUEUE_SIZE) return m_AudQueuedBuffers;

	// create the header
	LPWAVEHDR pHdr = (LPWAVEHDR) ::GlobalLock(::GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, sizeof(WAVEHDR) + m_AudBufferSize));
	if(pHdr == NULL) return NULL;
	::memset(pHdr, 0, sizeof(WAVEHDR));

	// new a buffer
	pHdr->lpData = (LPSTR) (pHdr + 1);
	pHdr->dwBufferLength = m_AudBufferSize;
	pHdr->dwFlags = 0;

	// prepare it
	::waveInPrepareHeader(m_hAudInputDevice, pHdr, sizeof(WAVEHDR));

	// add the input buffer to the queue
	::waveInAddBuffer(m_hAudInputDevice, pHdr, sizeof(WAVEHDR));

	// no error
	// increment the number of waiting buffers
	m_pAudQueue[m_AudQueuedBuffers] = pHdr;
	return m_AudQueuedBuffers++;
}

