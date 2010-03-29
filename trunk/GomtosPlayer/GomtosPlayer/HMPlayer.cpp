//============================================================================
//
//  (@)HMPlayer.cpp v1.1 by guru21
//
//----------------------------------------------------------------------------
//
//  The DirectShow based audio/video Media Player.
//
//
//  Copyright (c) 2001-   Mointek Co., Ltd.
//
//============================================================================

#include "stdafx.h"
#include "HMPlayer.h"
//#include "hmedia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// DirectShow 라이브러리 링크
#pragma comment(lib, "strmiids.lib")

// 멀티미디어 라이브러리 링크
#pragma comment(lib, "winmm.lib")

//
// Constants
//
#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L

// Defaults used with audio-only files
#define DEFAULT_AUDIO_WIDTH     240
#define DEFAULT_AUDIO_HEIGHT    120
#define DEFAULT_VIDEO_WIDTH     320
#define DEFAULT_VIDEO_HEIGHT    240

// Defaults used with play rate
#define PLAY_RATE_NORMAL    (1.0)
#define PLAY_RATE_MAX		(2.0)
#define PLAY_RATE_MIN       (0.0)
#define PLAY_RATE_FACTOR    (.1)


/////////////////////////////////////////////////////////////////////////////
// MediaPlayer

CHMPlayer::CHMPlayer()
{
//    // Init. the quartz library
//    // OLE subsystem requires applications to initialize things first!
//    HRESULT hr = CoInitialize(NULL);
//    if (!SUCCEEDED(hr))
//        AfxMessageBox("Initialize OLE subsystem failed!!!");

	// DirectShow interfaces
	m_pGB = NULL;
	m_pMC = NULL;
	m_pME = NULL;
	m_pVW = NULL;
	m_pBA = NULL;
	m_pBV = NULL;
	m_pMP = NULL;
	m_pMP = NULL;
	
    m_State = Uninitialized;

    // set normal play rate
    m_dPlayRate = PLAY_RATE_NORMAL;

	m_szFile[0] = '\0';
}



CHMPlayer::~CHMPlayer()
{
    Close();
}



int CHMPlayer::GetState()
{
    if (m_pMC == NULL)
    	return Uninitialized;
    
    OAFilterState   state;
    m_pMC->GetState(INFINITE, &state);
    switch (state) {
    case State_Running:		// in this case, m_State is Playing or TermPlaying
         break;
    case State_Stopped:
         m_State = Stopped;
         break;
    case State_Paused:
         m_State = Paused;
         break;
    }

    return m_State;
}



void CHMPlayer::checkAudioOnly()
{
    m_bAudioOnly = false;

    if (m_pVW == NULL || m_pBV == NULL)
    {
        m_bAudioOnly = true;
        //HMMsg(TEXT("No video interface.  Assuming audio/MIDI file or unsupported video codec.\r\n"));
        return;
    }

//    HRESULT hr;
//    long lVisible;
//
//    hr = m_pVW->get_Visible(&lVisible);
//    if (FAILED(hr))
//    {
//        // If this is an audio-only clip, get_Visible() won't work.
//        //
//        // Also, if this video is encoded with an unsupported codec,
//        // we won't see any video, although the audio will work if it is
//        // of a supported format.
//        //
//        m_bAudioOnly = true;
//        if (hr != E_NOINTERFACE)
//        {
//            gHMMsg(TEXT("Failed(%08lx) in pVW->get_Visible()!\r\n"), hr);
//
//        }
//    }
}


void CHMPlayer::createContents(LPCTSTR pszFile)
{
    WCHAR wFile[MAX_PATH];
    HRESULT hr;

#ifndef UNICODE
    MultiByteToWideChar(CP_ACP, 0, pszFile, -1, wFile, MAX_PATH);
#endif

	_tcscpy_s(m_szFile, _tcslen(pszFile), pszFile);

    // Get the interface for DirectShow's GraphBuilder
	IGraphBuilder *pGB;
    hr = CoCreateInstance(
			CLSID_FilterGraph,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder,
			(void **)&pGB
		);

    if(SUCCEEDED(hr))
    {
        // Have the graph construct its the appropriate graph automatically
        pGB->RenderFile(wFile, NULL);

		createContents(pGB);
    }
}



void CHMPlayer::createContents(IGraphBuilder *pGB)
{
	HRESULT hr;

    _ASSERT( pGB );

	m_pGB = pGB;

    // QueryInterface for DirectShow interfaces
    m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
    m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pME);
    m_pGB->QueryInterface(IID_IMediaPosition,(void **)&m_pMP);
    m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS);

    // Query for video interfaces, which may not be relevant for audio files
    m_pGB->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);
    m_pGB->QueryInterface(IID_IBasicVideo, (void **)&m_pBV);

    // Query for audio interfaces, which may not be relevant for video-only files
    m_pGB->QueryInterface(IID_IBasicAudio, (void **)&m_pBA);

    // Is this an audio-only file (no video component)?
    checkAudioOnly();

    // We'll manually set the video to be visible, so disable autoshow
	if (m_bAudioOnly)
	{
		m_pVW->put_AutoShow(OAFALSE);
		m_AvgTimePerFrame = 1.0 / 30.0;
	}
	else
	{
		hr = m_pBV->get_AvgTimePerFrame(&m_AvgTimePerFrame);
		if (m_AvgTimePerFrame <= 0.0)
			m_AvgTimePerFrame = 1.0 / 12.0;
	}

	// Set play range to duration.
    REFERENCE_TIME Start = 0;
    REFERENCE_TIME Duration = 0;
    m_pMS->GetDuration( &Duration );
    m_pMS->SetPositions( &Start,    AM_SEEKING_AbsolutePositioning, 
                                        &Duration, AM_SEEKING_AbsolutePositioning );

	// Change state to paused.
	//

	//m_pVW->put_Visible(OAFALSE);
	// Move play window to unvisible area.
    hr = m_pVW->SetWindowPosition(-100, -100, 1, 1);	// If wav file then got invalid interface error.
	//m_pVW->put_Owner(NULL);
    //m_pMC->Run();
    m_pMC->Stop();

    m_State = Stopped;
}



void CHMPlayer::releaseContents()
{
    SAFE_RELEASE(m_pME);
    SAFE_RELEASE(m_pMP);
    SAFE_RELEASE(m_pMS);
    SAFE_RELEASE(m_pMC);
    SAFE_RELEASE(m_pBA);
    SAFE_RELEASE(m_pBV);
    SAFE_RELEASE(m_pVW);
    SAFE_RELEASE(m_pGB);
}



void CHMPlayer::resetPlayer()
{
    // reset play rate to normal speed
    ResetPlayRate();

	// get volume
    m_pBA->get_Volume(&m_lVolume);
    
    // Read the default video size
    if (m_pBV)
    {
    	m_pBV->GetVideoSize(&m_Width, &m_Height);
    }
}



bool CHMPlayer::SetGraphNotify(HWND hWnd)
{
    if (m_pME == NULL)
        return false;

    // Have the graph signal event via window callbacks for performance
    m_pME->SetNotifyWindow((OAHWND)hWnd, WM_HM_GRAPHNOTIFY, 0);

	return true;
}



long CHMPlayer::HandleGraphEvent()
{
    LONG evCode, evParam1, evParam2;
	if (!IsValid())
		return 0;

    while(SUCCEEDED(m_pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Spin through the events
        HRESULT hr = m_pME->FreeEventParams(evCode, evParam1, evParam2);

        if(evCode == EC_COMPLETE || evCode == EC_USERABORT || evCode == EC_STREAM_ERROR_STOPPED)
        {
			//SeekTime(GetDuration());
			//Pause();  디스크 R/W 폭주 현상 
			m_pMC->Stop();
		    m_State = Stopped;
/*        	
            LONGLONG pos=0;

            // Reset to first frame of movie
            hr = m_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                     NULL, AM_SEEKING_NoPositioning);
            if (FAILED(hr))
            {
                // Some custom filters (like the Windows CE MIDI filter) 
                // may not implement seeking interfaces (IMediaSeeking)
                // to allow seeking to the start.  In that case, just stop 
                // and restart for the same effect.  This should not be
                // necessary in most cases.
                if (FAILED(hr = m_pMC->Stop()))
                {
                    HMMsg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
                    break;
                }

                if (FAILED(hr = m_pMC->Run()))
                {
                    HMMsg(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr);
                    break;
                }
            }
*/            
        }
    }

    return evCode;
}



//===========================================================================
// Open(): ready for play media file
//===========================================================================
bool CHMPlayer::Open(LPCTSTR pszFile, HWND hWnd)
{
	_ASSERT(!IsValid());
    if (IsValid())
        return false;

	// create filter graph by given media file
    createContents(pszFile);

	// set owner window and window style of dshow palyer
    if (hWnd != NULL)
	    SetOwnerWindow(hWnd);

	// reset player's attributes.
	resetPlayer();

    return true;
}



//===========================================================================
// SetFilterGraph(): ready for given filter graph
//===========================================================================
bool CHMPlayer::SetFilterGraph(IGraphBuilder *pGB)
{
	_ASSERT(!IsValid());
    if (IsValid())
        return false;

	// Increase ref. counter.
	pGB->AddRef();

	// create members by given filter graph.
    createContents(pGB);
    
	// reset player's attributes.
	resetPlayer();

    return true;
}



//===========================================================================
// Close(): release dshow's filter graph and init player state
//===========================================================================
void CHMPlayer::Close()
{
    if (!IsValid())
        return;

	// Change state to uninit.
    m_State = Uninitialized;

	// Stop receiving event messages.
    m_pME->SetNotifyWindow((OAHWND)NULL, 0, 0);
	m_pVW->put_Visible(OAFALSE);
	SetWindowPosition(0, 0, 0, 0); 

	// Stop to entire filter graph
    m_pMC->Stop();	//m_pMC->StopWhenReady();
	OAFilterState   state;
    m_pMC->GetState(INFINITE, &state);

	// Detach owner window
	m_pVW->put_Owner(NULL);

    releaseContents();

    // Clear file name to allow selection of new file with open dialog
    m_szFile[0] = '\0';

    // Uninit. the quartz library
	//CoUninitialize();
}



//===========================================================================
// SetOwnerWindow()
//===========================================================================
void CHMPlayer::SetOwnerWindow(HWND hOwnerWnd)
{
	// If this is an audio-only file, then there won't be a video interface.
    if (hOwnerWnd != NULL && !m_bAudioOnly)
	{
	    // set Message Drain
		SetMessageDrain(hOwnerWnd);

		// set owner window and window's styles
		m_pVW->put_Owner((OAHWND)hOwnerWnd);
	    m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		// set default window position
		RECT rect;
		::GetClientRect(hOwnerWnd, &rect);
		SetWindowPosition(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top); 

	    // Make the video window visible within the screen window.
        m_pVW->put_Visible(OATRUE);
        m_pVW->SetWindowForeground(-1);

		// Let's get ready to rumble!
		::UpdateWindow(hOwnerWnd);
    }
}




//===========================================================================
// SetMessageDrain()
//===========================================================================
bool CHMPlayer::SetMessageDrain(HWND hWnd)
{
    if (m_pVW == NULL)
    	return false;

    // set window handle of the window specified as the message drain
    HRESULT hr = m_pVW->put_MessageDrain((OAHWND)hWnd);
    
    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::SetWindowPosition(const long x, const long y, const long width, const long height)
{
    if (m_pVW == NULL)
    	return false;
    
    HRESULT hr = m_pVW->SetWindowPosition(x, y, width, height);

    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::GetWindowPosition(long* x, long* y, long* width, long* height)
{
    if (m_pVW == NULL)
    	return false;
    
    HRESULT hr = m_pVW->GetWindowPosition(x, y, width, height);

    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::SetWindowState(const long WindowState)
{
    if (m_pVW == NULL)
    	return false;
    
    HRESULT hr = m_pVW->put_WindowState(WindowState);

    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::GetWindowState(long* pWindowState)
{
    if (m_pVW == NULL)
    	return false;
    
    HRESULT hr = m_pVW->get_WindowState(pWindowState);

    if (FAILED(hr))
    	return false;
    else
    	return true;

}



void CHMPlayer::Refresh()
{
    if (m_pVW == NULL || m_bAudioOnly)
    	return;
    
	m_pVW->SetWindowForeground(OATRUE);
//    m_pVW->put_WindowState(SW_HIDE);
//    m_pVW->put_WindowState(SW_SHOW);

}



bool CHMPlayer::SetVisible(const bool bVisible)
{
    if (m_pVW == NULL)
    	return false;

    HRESULT hr = m_pVW->put_Visible(bVisible ? OATRUE : OAFALSE);
    
    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::GetVisible(bool* pbVisible)
{
    if (m_pVW == NULL)
    	return false;

    long lVisible;
    HRESULT hr = m_pVW->get_Visible(&lVisible);
    
    *pbVisible = (lVisible == OATRUE);

    if (FAILED(hr))
    	return false;
    else
    	return true;

}



bool CHMPlayer::Play()
{
    if (!canPlay())
        return false;

    HRESULT hr;

	if (m_State == TermPlaying)
	{
		PutStopTime(GetDuration());
	}
	
    hr = m_pMC->Run();
    if(SUCCEEDED(hr))
    {
        m_State = Playing;
        return true;
    }
    else {
        return false;
    }

}



bool  CHMPlayer::Pause()
{
    if (!canPause())
        return false;

    OAFilterState   state;

    HRESULT hr;
	hr  = m_pMC->Pause();
    hr |= m_pMC->GetState(INFINITE, &state);
    if(SUCCEEDED(hr))
    {
	    m_State = Paused;
        return true;
    }
    else {
        return false;
    }
}



//
// In this mode, filters release resources and no data is processed. 
//
bool  CHMPlayer::Stop()
{
    if (!canStop())
        return false;

	m_pMC->Stop();
    m_State = Stopped;

    // if we want always to play from the beginning
    // then we should seek back to the start here
    // (on app or user stop request, and also after EC_COMPLETE).
    //SeekTime(0.0);

	return true;
}



bool CHMPlayer::PutStopTime(REFTIME tStopTime)
{
    if (m_pMP == NULL)
    	return false;
    	
    HRESULT hr = m_pMP->put_StopTime(tStopTime);
    if(SUCCEEDED(hr))
        return true;
    else
        return false;
}



double CHMPlayer::Frame2Time(const long nFrame)
{
	double refTime = m_AvgTimePerFrame * nFrame;

	return refTime;
}



long CHMPlayer::Time2Frame(const double refTime)
{
	long nFrame = (long) (refTime / m_AvgTimePerFrame);

	return nFrame;
}



bool CHMPlayer::PlayFromTo(const REFTIME tStartTime, const REFTIME tStopTime)
{
    if (!canPlay())
        return false;

	HRESULT hr;

	// Set play range to duration.
    REFERENCE_TIME Start = (REFERENCE_TIME) (tStartTime * (double)UNITS);
    REFERENCE_TIME Stop  = (REFERENCE_TIME) (tStopTime * (double)UNITS);
    hr = m_pMS->SetPositions( &Start, AM_SEEKING_AbsolutePositioning, 
                              &Stop , AM_SEEKING_AbsolutePositioning );
    hr |= m_pMC->Run();
    if(SUCCEEDED(hr))
    {
        m_State = TermPlaying;
    	return true;
    }
    else {
        return false;
    }
	
}



bool CHMPlayer::SetFullScreenMode(const bool isFull)
{
    if (m_pVW == NULL)
    	return false;
    	
    HRESULT hr = m_pVW->put_FullScreenMode(isFull ? OATRUE : OAFALSE);
    
    if (FAILED(hr))
    	return false;
    else
    	return true;
}



HRESULT CHMPlayer::SeekTime(const REFTIME refTime)
{
    HRESULT hr;

	if (m_pMP == NULL)
		return E_UNEXPECTED;
	
	hr = m_pMP->put_CurrentPosition(refTime);

/*
	if (m_pMS == NULL)
		return E_UNEXPECTED;

    HRESULT hr;
	REFERENCE_TIME rtNow;

	if (m_State == Stopped )
		Pause();

	rtNow = (REFERENCE_TIME) (refTime * (double)UNITS);
	hr = m_pMS->SetPositions(
				&rtNow,  AM_SEEKING_AbsolutePositioning, 
				NULL,    AM_SEEKING_NoPositioning
		 );
*/	

	return hr;
}



HRESULT CHMPlayer::SeekFrame(long nFrame, double fps)
{
	HRESULT hr;
    REFTIME refTime;

    refTime = double(nFrame) * m_AvgTimePerFrame;

    hr = SeekTime(refTime);

    return hr;
}


/*
HRESULT CHMPlayer::SeekNextFrame(double fps)
{
    // try seeking by frames first
    //
    //HRESULT hr = m_pMS->SetTimeFormat(&TIME_FORMAT_FRAME);
    //REFERENCE_TIME Pos = 0;
    //if (!FAILED(hr))
    //{
    //    m_pMS->GetCurrentPosition(&Pos);
    //    Pos++;
    //}
    //else
    //{
	//}

	HRESULT hr;
    REFTIME refTime;

	// modified by raguru.
    hr = m_pMP->get_CurrentPosition(&refTime);
	if (FAILED(hr))
		return hr;

    // couldn't seek by frames, use fps to calculate time
    //
    refTime += 1.0 / fps;

    // add a half-frame to seek to middle of the frame
    //
    refTime += 0.5 / fps;

    hr = SeekTime(refTime);

    return hr;
}
*/


HRESULT CHMPlayer::SeekNextFrame(double fps)
{
	HRESULT hr;
    REFTIME refTime;

	// modified by raguru.
    hr = m_pMP->get_CurrentPosition(&refTime);
	if (FAILED(hr))
		return hr;

    // couldn't seek by frames, use m_AvgTimePerFrame to calculate time
    //
    //refTime = refTime + m_AvgTimePerFrame + (m_AvgTimePerFrame * .5);
    refTime = refTime + m_AvgTimePerFrame;

    hr = SeekTime(refTime);

    return hr;
}



HRESULT CHMPlayer::SeekPrevFrame(double fps)
{
	HRESULT hr;
    REFTIME refTime;

	// modified by raguru.
    hr = m_pMP->get_CurrentPosition(&refTime);
	if (FAILED(hr))
		return hr;

    // couldn't seek by frames, use m_AvgTimePerFrame to calculate time
    //
    refTime = refTime - m_AvgTimePerFrame;

    hr = SeekTime(refTime);

    return hr;
}


// returns milli-seconds
REFTIME CHMPlayer::GetDuration()
{
	//if (m_pMP == NULL)
	//	return -1;

    REFTIME duration = 0.0;
	REFERENCE_TIME rtTotalTime;
	//DWORD caps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration; 
	DWORD caps = AM_SEEKING_CanGetDuration; 

	if (m_pMS && S_OK == m_pMS->CheckCapabilities(&caps)) 
	{
		m_pMS->GetDuration(&rtTotalTime);
		duration = (double)rtTotalTime / (double)UNITS;
	}
	else
	{
		m_pMP->get_Duration(&duration);
		duration *= 1000.0;	// in seconds
	}

    
    return duration;
}



REFTIME CHMPlayer::GetCurrentPosition()
{
	if (m_pMP == NULL)
		return -1;

    REFTIME cur_pos;

    m_pMP->get_CurrentPosition(&cur_pos);
    
    return cur_pos;
}



double  CHMPlayer::GetPlayRate()
{
	if (m_pMP == NULL)
		return -1;

    double dRate;
    m_pMP->get_Rate(&dRate);
    return dRate;
}


void  CHMPlayer::SetPlayRate(const double dRate)
{
	if (m_pMP == NULL)
		return;

    m_dPlayRate = dRate;
    m_pMP->put_Rate(m_dPlayRate);
}


void  CHMPlayer::ResetPlayRate()
{
    SetPlayRate(PLAY_RATE_NORMAL);
}


void  CHMPlayer::IncPlayRate()
{
    m_dPlayRate += PLAY_RATE_FACTOR;
    if (m_dPlayRate >= PLAY_RATE_MAX)
        m_dPlayRate = PLAY_RATE_MAX;
    
    SetPlayRate(m_dPlayRate);
}



void  CHMPlayer::DecPlayRate()
{
    m_dPlayRate -= PLAY_RATE_FACTOR;
    if (m_dPlayRate <= PLAY_RATE_MIN)
        m_dPlayRate = PLAY_RATE_MIN;
    
    SetPlayRate(m_dPlayRate);
}



bool CHMPlayer::ToggleMute()
{
    if (!m_pBA)
        return false;

    HRESULT hr;
    long Volume;

    // Read current volume
    hr = m_pBA->get_Volume(&Volume);
    if (hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        return false;
    }
    else if (FAILED(hr))
    {
        //HMMsg(TEXT("Failed to read audio volume!  hr=0x%x\r\n"), hr);
        return false;
    }

    // Switch volume levels
    if (Volume == VOLUME_SILENCE)
    {
	    // Set volume
    	m_pBA->put_Volume(m_lVolume);
    }
    else
    {
    	Volume = VOLUME_SILENCE;
    	m_pBA->put_Volume(m_lVolume);
    }

    return true;
}


bool  CHMPlayer::SetAudioVolume(long Volume)
{
    if (!m_pBA)
        return false;
		
	HRESULT hr = m_pBA->put_Volume(Volume);

    m_lVolume = Volume;

	if (hr == E_INVALIDARG || hr == E_FAIL)
		return false;
	else
		return true;
}



long  CHMPlayer::GetAudioVolume()
{
    if (!m_pBA)
        return false;

	long Volume;
	HRESULT hr = m_pBA->get_Volume(&Volume);

    m_lVolume = Volume;

	// Divide by 100 to get equivalent decibel value (for example -10,000 = -100 dB)
	return Volume;
}



bool  CHMPlayer::SetAudioBalance(long Balance)
{
    if (!m_pBA)
        return false;

	HRESULT hr = m_pBA->put_Balance(Balance);

	if (hr == E_INVALIDARG || hr == E_FAIL)
		return false;
	else
		return true;
}


// The Balance property is a value between -10,000 and 10,000.
// A value of -10,000 indicates that the right speaker has been disabled
// and only the left speaker is receiving an audio signal. A value of 0
// indicates that both speakers are receiving equivalent audio signals.
// A value of 10,000 indicates that the left speaker has been disabled 
// and only the right speaker is receiving an audio signal. 
//
// Note that not all devices support 10,000 distinguishable steps. 
long  CHMPlayer::GetAudioBalance()
{
    if (!m_pBA)
        return false;

	long Balance;
	HRESULT hr = m_pBA->get_Balance(&Balance);

	return Balance;
}




