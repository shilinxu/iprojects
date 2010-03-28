/////////////////////////////////////////////////////////////////////
//	BclScrRec.h - Header File for the BCL Screen Recorder DLL API
//
//	Copyright(c) 2003 bclsoft Inc.  All rights reserved.
/////////////////////////////////////////////////////////////////////
#ifndef __BCLSCRREC_H__
#define __BCLSCRREC_H__


#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Import declaration
//

#ifdef SCRREC_EXPORTS
#define SCRREC_API __declspec(dllexport)
#else
#define SCRREC_API __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants
//

// General Function return values
#define SCRREC_SUCCESS                   TRUE     // General error - success
#define SCRREC_FAILED                    FALSE    // General error - failure

#define DEFAULT_AUDIO_DEVICE	(-1)	//WAVE_MAPPER
#define DEFAULT_VIDEO_DEVICE	(-1)
#define DEFAULT_CODEC			(-1)

////////////////////////////////////////////////////////////////////////////////
// Data types
//

typedef	HANDLE SRECHANDLE;

// Video callback parameters
typedef struct
{
   HBITMAP      hBitmap;
   POINT        ptCursor;
} srecVideoCallbackParams;

typedef enum
{
   vcdScreenCam = 0,
   vcdVideoCam,
}  srecVideoCaptureDevice;

typedef enum
{
   cinScreen = 0,
   cinWindow,
   cinRegion
}  srecScreenInput;

typedef enum
{
	acmGSM610 = 1,		// * Default
	acmPCM,
	acmADPCM
} srecAudioEncoder;

typedef enum
{
	vcmMSVC = 1,		// Microsoft Video 1			* Default
	vcmCVID,			// Cinepak Codec by Radius
	vcmIV50,			// Indeo Video 5.10
	vcmDIVX,			// Divx 5.0.5
	vcmTSCC				// Tech Smith Screen Codec - lossless, best performance
} srecVideoEncoder;

typedef enum
{
	rstStarted = 1,
	rstRecording,
	rstPaused,
	rstPausedVideo,
	rstStopping,
	rstStopped
}  srecState;

// srecGetLastError values
typedef enum
{
   serrNone  = 0,
   serrSystemError,
   serrInvalidState,
   serrInvalidParam,
   serrInvalidTempFolder,
   serrInvalidOutputFile,
   serrInvalidInput,
   serrNoVideoCallback,
   serrNoWaveInDevice,
   serrResourceAllocationFailed,
   serrAudioFormatInvalid,
   serrAudioFormatUnavailable,
   serrAudioDeviceOpenFailed,
   serrAudioDeviceStartFailed,
   serrAudioWriteFailed,
   serrVideoWriteFailed,
   serrAviUnsupported,
   serrAviFileOpenFailed,
   serrAviWriteFailed,
   serrVFWVersionTooOld
}  srecError;                             

/////////////////////////////////////////////////////////////////////
// Function Prototypes

// Callback functions
typedef BOOL (CALLBACK* SRECVIDEOPROC)( srecVideoCallbackParams * pCallbackParams, VOID * pData );
typedef BOOL (CALLBACK* SRECSTATECHANGEPROC)( srecState State, VOID * pData );
typedef BOOL (CALLBACK* SRECERRORPROC)( srecError Error, VOID * pData );

// Capture object creation
SCRREC_API SRECHANDLE   WINAPI  srecCreateCapture( TCHAR * szLicense );
SCRREC_API BOOL			WINAPI  srecDestroyCapture( SRECHANDLE hCapture );

// Capture Methods
SCRREC_API BOOL     WINAPI  srecStartCapture( SRECHANDLE hCapture );
SCRREC_API BOOL     WINAPI  srecStopCapture( SRECHANDLE hCapture );
SCRREC_API BOOL     WINAPI  srecPauseCapture( SRECHANDLE hCapture );
SCRREC_API BOOL     WINAPI  srecPauseVideoCapture( SRECHANDLE hCapture );
SCRREC_API BOOL     WINAPI  srecResumeCapture( SRECHANDLE hCapture );

// Capture Properties
SCRREC_API BOOL     WINAPI  srecSetAudioCaptureDevice( SRECHANDLE hCapture, DWORD dwAudioDeviceID );
SCRREC_API BOOL     WINAPI  srecGetAudioCaptureDevice( SRECHANDLE hCapture, DWORD * pdwAudioDeviceID );
SCRREC_API BOOL     WINAPI  srecSetVideoCaptureDevice( SRECHANDLE hCapture, DWORD dwVideoDeviceID );
SCRREC_API BOOL     WINAPI  srecGetVideoCaptureDevice( SRECHANDLE hCapture, DWORD * pdwVideoDeviceID );
SCRREC_API BOOL     WINAPI  srecSetScreenInput( SRECHANDLE hCapture, srecScreenInput scrInput, HWND hWnd = NULL, LPRECT pRect = NULL);
SCRREC_API BOOL     WINAPI  srecGetScreenInput( SRECHANDLE hCapture, srecScreenInput * scrInput, HWND * phWnd, LPRECT pRect );

// Advanced Capture Properties
SCRREC_API BOOL     WINAPI  srecSetAudioEnable( SRECHANDLE hCapture, BOOL bEnable );
SCRREC_API BOOL     WINAPI  srecGetAudioEnable( SRECHANDLE hCapture, BOOL * pbEnable );
SCRREC_API BOOL     WINAPI  srecSetMuteAudio( SRECHANDLE hCapture, BOOL bMute );
SCRREC_API BOOL     WINAPI  srecGetMuteAudio( SRECHANDLE hCapture, BOOL * bMute );
SCRREC_API BOOL     WINAPI  srecSetRecordCursor( SRECHANDLE hCapture, BOOL bEnable );
SCRREC_API BOOL     WINAPI  srecGetRecordCursor( SRECHANDLE hCapture, BOOL * pbEnable );
SCRREC_API BOOL     WINAPI  srecSetCustomCursor( SRECHANDLE hCapture, HCURSOR hCursor );
SCRREC_API BOOL     WINAPI  srecGetCustomCursor( SRECHANDLE hCapture, HCURSOR * hCursor );
SCRREC_API BOOL     WINAPI  srecSetRecordLogo( SRECHANDLE hCapture, BOOL bEnable );
SCRREC_API BOOL     WINAPI  srecSetCustomLogo( SRECHANDLE hCapture, HBITMAP hBitmap );
SCRREC_API BOOL     WINAPI  srecSetDrawing( SRECHANDLE hCapture, BOOL bDrawing );

SCRREC_API BOOL     WINAPI  srecSetRecordLayeredWnd( SRECHANDLE hCapture, BOOL bEnable );

// Capture Callbacks
SCRREC_API BOOL     WINAPI  srecSetStateChangeCallback( SRECHANDLE hCapture, SRECSTATECHANGEPROC pStateChangeProc, DWORD dwData );
SCRREC_API BOOL     WINAPI  srecGetStateChangeCallback( SRECHANDLE hCapture, SRECSTATECHANGEPROC * pStateChangeProc, DWORD * pdwData );
SCRREC_API BOOL     WINAPI  srecSetErrorCallback( SRECHANDLE hCapture, SRECERRORPROC pErrorProc, DWORD dwData );
SCRREC_API BOOL     WINAPI  srecGetErrorCallback( SRECHANDLE hCapture, SRECERRORPROC * pErrorProc, DWORD * pdwData );
SCRREC_API BOOL     WINAPI  srecSetVideoCallback( SRECHANDLE hCapture, SRECVIDEOPROC pVideoProc, BOOL bCursorInfo, DWORD dwData );
SCRREC_API BOOL     WINAPI  srecGetVideoCallback( SRECHANDLE hCapture, SRECVIDEOPROC * pVideoProc, BOOL * pbCursorInfo, DWORD * pdwData );

// Capture runtime statistics
SCRREC_API srecState WINAPI srecGetCaptureState( SRECHANDLE hCapture ); 
SCRREC_API int		WINAPI  srecGetCaptureFrames( SRECHANDLE hCapture );
SCRREC_API int		WINAPI  srecGetDroppedFrames( SRECHANDLE hCapture );
SCRREC_API int		WINAPI  srecGetDuration( SRECHANDLE hCapture );			// in seconds
SCRREC_API double   WINAPI  srecGetActualFrameRate( SRECHANDLE hCapture );

// File Properties
SCRREC_API BOOL     WINAPI  srecSetFileName( SRECHANDLE hCapture, const char * pszFileName );
SCRREC_API BOOL     WINAPI  srecGetFileName( SRECHANDLE hCapture, char * pszFileName );

// AVI Options
SCRREC_API BOOL     WINAPI  srecSetAviOptions( SRECHANDLE hCapture, DWORD dwFps, DWORD dwQuality, DWORD dwKeyFramesEvery );
SCRREC_API BOOL     WINAPI  srecGetAviOptions( SRECHANDLE hCapture, DWORD * dwFps, DWORD * dwQuality, DWORD * dwKeyFramesEvery );

SCRREC_API BOOL     WINAPI  srecSetAviVideoCodec( SRECHANDLE hCapture, DWORD Codec, bool bUseVidCompDlg=true );
SCRREC_API BOOL     WINAPI  srecGetAviVideoCodec( SRECHANDLE hCapture, DWORD * pCodec );

SCRREC_API BOOL     WINAPI  srecSetAviAudioCodec( SRECHANDLE hCapture, DWORD Codec );
SCRREC_API BOOL     WINAPI  srecGetAviAudioCodec( SRECHANDLE hCapture, DWORD * pCodec );

// Miscellaneous 
SCRREC_API DWORD     WINAPI  srecGetAudioDevices( SRECHANDLE hCapture );
SCRREC_API srecError WINAPI  srecGetLastError( SRECHANDLE hCapture );
SCRREC_API TCHAR *	 WINAPI  srecGetLastErrorMsg( srecError err );

#ifdef __cplusplus
} // extern "C" 
#endif

#endif //  __BCLSCRREC_H__

/////////////////////////////////////////////////////////////////////
// END OF FILE
/////////////////////////////////////////////////////////////////////
