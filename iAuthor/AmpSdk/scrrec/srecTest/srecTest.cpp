// srecTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "../../include/bclscrrec.h"


void main(int argc, char* argv[])
{
	/*
	HDC hdc = ::GetDC(0);
	HDC hmem = ::CreateCompatibleDC(hdc);
	HBITMAP hbm = ::CreateCompatibleBitmap(hdc, 1024, 768);
	HBITMAP hobm = (HBITMAP) ::SelectObject(hmem, hbm);
	::BitBlt(hmem, 0, 0, 1024,768, hdc, 0, 0, SRCCOPY);
	::StretchBlt(hdc, 0, 0, 1024/4, 768/4, hmem, 0, 0, 1024, 768, SRCCOPY);

	::SelectObject(hmem, hobm);
	::DeleteDC(hmem);
	::ReleaseDC(0, hdc);
	return;
	*/

	int fps = 20;
	DWORD vcodec = vcmMSVC;

	_tprintf(_T("\n\nUsage: srecTest {fps(10~20)} {codec(vcmMSVC or vcmTSCC)}"));
	
	if (argc == 2)
	{
		fps = atoi(argv[1]);
	}
	else if (argc == 3)
	{
		fps = atoi(argv[1]);
		vcodec = (_tcscmp(argv[2], _T("vcmTSCC")) == 0) ? vcmTSCC : vcmMSVC;
	}

	_tprintf(_T("\n\nParams: %dfps, %d th video codec\n\n"), fps, vcodec);
	
	// Step 1 - Initializing the Capture Object
	// The first task that must be done is to initialize the capture object.
	// Use srecCreateCapture to obtain an capture handle to a capture object.
	// This handle is required by every recorder DLL API function call.
	// The license key string from the "licensekey.txt" file must be passed as a parameter
	// to srecCreateCapture to license the control.

	SRECHANDLE  hCapture;
	hCapture = srecCreateCapture( "SRECSDK00001-147AA447D127-86AE49659587-924D3BEFF365" );
	if ( hCapture == NULL )
	{
	   return;
	}

	// Step 2 - Setting the Capture Properties
	// The next step is to configure various capture object properties.
	// Most object properties have default values that are used if the property is not specified.
	// You will need to configure different properties based on what you wish to do with 
	// the recorder control.  Please see the function reference for details.  

	// Configure to capture video and audio device identifier
	srecSetAudioCaptureDevice( hCapture, DEFAULT_AUDIO_DEVICE );	// from first wavein device
	srecSetVideoCaptureDevice( hCapture, DEFAULT_VIDEO_DEVICE );	// from screen

	// if video device is screen the configure to capture entire screen or the window
	// and save to an AVI file
	int sx = ::GetSystemMetrics(SM_CXSCREEN);
	int sy = ::GetSystemMetrics(SM_CYSCREEN);
	if (sx < 1024)
	{
		srecSetScreenInput( hCapture, cinScreen, NULL, NULL );
	}
	else
	{
		RECT rc = {0,0, 1024, 768};
		srecSetScreenInput( hCapture, cinScreen, NULL, &rc );
	}

	// Configure to capture AVI Options
	DWORD dwFps = fps;
	DWORD dwQuality = 8000; // 0 ~ 10000
	DWORD dwKeyFramesEvery = fps*5;
	srecSetAviOptions( hCapture, dwFps, dwQuality, dwKeyFramesEvery );

	// Configure to capture Audio and Video Codecs
	srecSetAviVideoCodec( hCapture, vcodec );
	srecSetAviAudioCodec( hCapture, acmGSM610 );

	// Toggle enable audio
	srecSetAudioEnable( hCapture, TRUE);//TRUE FALSE );

	// Recording cursor shape ?
	srecSetRecordCursor( hCapture, TRUE );

	// An output file name MUST be specified to create an output file
	char* pszFile = "ScrTest.avi";
	srecSetFileName( hCapture, pszFile);


	// Step 3 - Controlling the Capture Session
	// Once the capture object is configured correctly, start the capture session.
	// You have complete control to Pause, Resume, Stop or Abort using the capture methods.
	// See the programming reference for a complete list.

	if ( srecStartCapture( hCapture ) != SCRREC_SUCCESS )
	{
		printf("\n\nError: %s\n", srecGetLastErrorMsg ( srecGetLastError( hCapture ) ));
		srecDestroyCapture( hCapture ); 
		return;
	}



	DWORD RecTime = 1000*5;
	DWORD StartTime = ::timeGetTime();
	SYSTEMTIME locTime;
	GetLocalTime(&locTime);
	printf("[%02d/%02d/%02d, %02d:%02d:%02d] - recording started during %d seconds...\n\n",
		locTime.wYear, locTime.wMonth, locTime.wDay,
		locTime.wHour, locTime.wMinute, locTime.wSecond,
		RecTime / 1000);

	while (srecGetCaptureState( hCapture ) != rstStopped)
	{
		DWORD elapse = ::timeGetTime() - StartTime;

		double afps = 0.0;
		int    elsec = elapse/1000;
		if (elsec > 0)
			afps = (double) srecGetCaptureFrames( hCapture )/(double) elsec;

		printf("\r## %d secs elapsed... (%5.2f fps)", elsec, afps);

		if (elapse > RecTime)
			break;
		Sleep(500);
	}


	// Finish the capture session
	printf("\n\n## Stopping progress... ");
	srecStopCapture( hCapture );


	// Note that the srecSetStateChangeCallback can be used to automatically obtain
	// the capture state in a state change event callback function.

   	// Display capture statistics¡¦
	// The capture statistics will be reset when a new capture session is started
	// (  i.e. the statistics will persist until  srecStartCapture is called again ).
   	printf("\n\n\n");
   	printf("==============================================================\n");
   	printf("**File('%s') Capture statistics...\n", pszFile);
   	printf("--------------------------------------------------------------\n");
	printf("## %d Frames Captured, \t duration is %d secs\n", srecGetCaptureFrames( hCapture ), srecGetDuration( hCapture ));
	printf("## %d Frames Dropped\n", srecGetDroppedFrames( hCapture ));
	printf("## Actual frame rate = %5.2f fps\n", srecGetActualFrameRate( hCapture ));
	printf("\n## Last Error Message: %s\n\n\n", srecGetLastErrorMsg( srecGetLastError( hCapture ) ));


	// Step 5 - Destroying the Capture Object
	// After using srecStopCapture to finish a capture session to discard a capture session,
	// you may continue to use the capture handle to reconfigure and begin a new capture session.
	// If you not longer need to capture the screen, be sure to destroy the capture object
	// by calling srecDestroyCapture.

	srecDestroyCapture( hCapture ); 

	//printf("Press [Enter] key to continue...");
	//getchar();

	return;
}
