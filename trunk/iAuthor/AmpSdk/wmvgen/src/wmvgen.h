/////////////////////////////////////////////////////////////////////
//	WmvGen.h - Header File for the BCL WMV Generator DLL API
//
//  Input : AVI File & WMV Profile
//  Output: WMV File
//
//
//	Copyright(c) 2003 bclsoft Inc.  All rights reserved.
/////////////////////////////////////////////////////////////////////
#ifndef __WMVGEN_H__
#define __WMVGEN_H__


#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
// Import declaration
//

#ifdef WMVGEN_EXPORTS
#define WMVGEN_API __declspec(dllexport)
#else
#define WMVGEN_API __declspec(dllimport)
#endif


////////////////////////////////////////////////////////////////////////////////
// Constants
//

// General Function return values
#define WMVGEN_SUCCESS                   TRUE     // General error - success
#define WMVGEN_FAILED                    FALSE    // General error - failure


////////////////////////////////////////////////////////////////////////////////
// Data types
//

typedef	HANDLE WGENHANDLE;

// srecGetLastError values
typedef enum
{
	wgenNone  = 0,
	wgenOutOfMemory,
	wgenInvalidState,
	wgenInvalidParam,
	wgenInvalidTempFolder,
	wgenInvalidInputFile,
	wgenInvalidOutputFile,
	wgenInvalidWmvProfileName,
	wgenAviFileOpenFailed,
	wgenWmvRenderFailed,
	wgenCreateThreadFailed
}  wgenError;                             

/////////////////////////////////////////////////////////////////////
// Function Prototypes

// Callback functions
typedef BOOL (CALLBACK* WGENPROGRESSPROC)	( double rtCurrnet, VOID * pData );
typedef BOOL (CALLBACK* WGENERRORPROC)		( wgenError Error, VOID * pData );


// Instance creation
WMVGEN_API WGENHANDLE   WINAPI wgenCreateInstance( TCHAR * szLicense );
WMVGEN_API BOOL         WINAPI wgenDestroyInstance( WGENHANDLE hInst );

// Public Methods
//

WMVGEN_API BOOL			WINAPI wgenSetAviFileName( WGENHANDLE hInst, LPCTSTR pszAviFileName );
WMVGEN_API BOOL			WINAPI wgenSetWmvFileName( WGENHANDLE hInst, LPCTSTR pszWmvFileName );
WMVGEN_API BOOL			WINAPI wgenSetWmvProfileName( WGENHANDLE hInst, LPCTSTR pszProfileName );
WMVGEN_API BOOL			WINAPI wgenSetRenderRange( WGENHANDLE hInst, double tStart, double tStop );

WMVGEN_API BOOL			WINAPI wgenStartRender( WGENHANDLE hInst );
WMVGEN_API BOOL			WINAPI wgenStopRender( WGENHANDLE hInst );

WMVGEN_API BOOL			WINAPI wgenSetProgressCallback( WGENHANDLE hInst, WGENPROGRESSPROC pCBProc, DWORD dwData );
WMVGEN_API BOOL			WINAPI wgenSetErrorCallback( WGENHANDLE hInst, WGENERRORPROC pErrorProc, DWORD dwData );
WMVGEN_API BOOL			WINAPI wgenSetLastError( WGENHANDLE hInst, wgenError err );
WMVGEN_API wgenError	WINAPI wgenGetLastError();

// Internal Methods

#ifdef __cplusplus
} // extern "C" 
#endif


#endif //__WMVGEN_H__