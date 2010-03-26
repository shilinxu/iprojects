//============================================================================
//
//  (@)HMPlayer.h v1.1 by guru21
//
//  The DirectShow based audio/video media player.
//  Play, pause, stop, mute, and fullscreen mode toggle can
//  be performed via exposed interface.
//----------------------------------------------------------------------------
//  Revision history:
//
//  2000/05/04 Created by Moonmok Park, it called CDirectShowManager.
//  2001/01/15 Change name, modify for dshow 8.0
//             and remove MFC dependents.
//
//----------------------------------------------------------------------------
//  Copyright (c) 2001- Mointek Co., Ltd. All rights reserved.
//============================================================================

#ifndef __HM_PLAYER_H__
#define __HM_PLAYER_H__

#include "dshow.h"

//
// Macros
//
#define UNITS (10000)
//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// WM_USER through 0x7FFF Integer messages for use by private window classes. 
#define WM_HM_GRAPHNOTIFY	(WM_USER+0x1001)


//----------------------------------------------------------------------------
// MediaPlayer
class CHMPlayer
{
public:
	// The state of current activity.
	enum _State {
        Uninitialized=0,
        Stopped,
        Paused,
        Playing,
        TermPlaying
    };


	CHMPlayer();
	virtual ~CHMPlayer();


    // state methods
    bool IsValid(){ return this && m_State!=Uninitialized; }
    int  GetState();

    // media open and close
    bool Open(LPCTSTR pszFile, HWND hWnd);
	bool SetFilterGraph(IGraphBuilder *pGB);
    void Close();

	//-----------------------------------------------------------------------
	// Sample application's message handler fo Graph Notify
	//-----------------------------------------------------------------------
	// void CPlayerDlg::OnGraphNotify()
	// {
	// 	if (m_pPlayer == NULL)
	// 		return;
	// 
	//  // Event Notification Codes (; ref. DirectX help)
	// 	long ev_code = m_pPlayer->HandleGraphEvent();
	// 
	// 	if (m_pPlayer->GetState() == CHMPlayer::Stopped)
	// 		AfxMessageBox("media stopped");
	// 
	// }
	bool SetGraphNotify(HWND hWnd);		// call this after open operation
	long HandleGraphEvent();			// insert this in your application message loop
										// or message hander (ex: OnGraphNotify())

    // play window control
    void SetOwnerWindow(HWND hOwnerWnd);
    bool SetMessageDrain(HWND hWnd);
    bool SetWindowPosition(const long x, const long y, const long width, const long height);
    bool GetWindowPosition(long* x, long* y, long* width, long* height);
	bool SetWindowState(const long WindowState);
	bool GetWindowState(long* pWindowState);
	void Refresh();
    bool SetFullScreenMode(const bool isFull);
    bool SetVisible(const bool bVisible);
	bool GetVisible(bool* pbVisible);

    // navigation
	HRESULT SeekTime(const REFTIME refTime);			// random access by time
	HRESULT SeekFrame(long nFrame, double fps=30.0);	// random access by frame
	HRESULT SeekNextFrame(double fps=12.0);				// don't need fps any more!!! 01/05/29
	HRESULT SeekPrevFrame(double fps=12.0);

    // set play rate
    double GetPlayRate();
    void   SetPlayRate(const double dRate);
    void   ResetPlayRate();
    void   IncPlayRate();
    void   DecPlayRate();

    // audio control
    bool ToggleMute();
    bool SetAudioVolume(long Volume);		// The allowable input range is -10,000 to 0.
    long GetAudioVolume();				// Divide by 100 to get equivalent decibel value (for example -10,000 = -100 dB)
    bool SetAudioBalance(long Balance);	// The allowable input range is -10,000 to 10,000
    long GetAudioBalance();	// For example, a value of 1000 indicates -10 dB on the right channel and -90 dB on the left channel

    // play control
    bool Play();
    bool PlayFromTo(const REFTIME tStartTime, const REFTIME tStopTime);
    bool Pause();
    bool Stop();
    bool PutStopTime(REFTIME tStopTime);
    
	// conversion frame and reference time
	long    Time2Frame(const double refTime);
	double  Frame2Time(const long nFrame);

    // get video informations
	LPCTSTR GetFileName() { return m_szFile; }
    REFTIME GetCurrentPosition();
    REFTIME GetDuration();
	long    GetWidth()		{ return m_Width; }
	long    GetHeight()		{ return m_Height;}

protected:
    bool canStop()   { return m_State==Playing || m_State==TermPlaying || m_State==Paused; }
    bool canPlay()   { return m_State==Stopped || m_State==Paused; }
    bool canPause()  { return m_State==Playing || m_State==TermPlaying || m_State==Stopped; }
    bool canRefresh(){ return m_State==Paused; }

    // Create or release members.
    void createContents(LPCTSTR pszFile);
    void createContents(IGraphBuilder *pGB);
    void releaseContents();

	// Etc...
	void resetPlayer();
    void checkAudioOnly();

	// Attributes
	_State m_State;
	TCHAR  m_szFile[MAX_PATH];		// Default media file name to playback

	long   m_Width;
	long   m_Height;
	long   m_lVolume;
	bool   m_bAudioOnly;
	double m_AvgTimePerFrame;
    double m_dPlayRate;

	// DirectShow interfaces
	IGraphBuilder  *m_pGB;
	IMediaControl  *m_pMC;
	IMediaEventEx  *m_pME;
	IVideoWindow   *m_pVW;
	IBasicAudio    *m_pBA;
	IBasicVideo    *m_pBV;
	IMediaPosition *m_pMP;
	IMediaSeeking  *m_pMS;

};

/////////////////////////////////////////////////////////////////////////////

#endif // __HM_PLAYER_H__
