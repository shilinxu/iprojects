// wmvgen.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "./wmvgen.h"

#include "../../prodcode/ProductCode.h"
#include "avi2wmv.h"

#define SAFE_DELETE(p)			if(p) { delete p; p = NULL; }
#define SAFE_RELEASE(p)			if(p) { p->Release(); p = NULL; }
#define SAFE_RELEASE_GLOBAL(p)	if(p) { GlobalFreePtr(p); p = NULL; }

#define DEFAULT_AVI_FILENAME	_T("C:\\srec_def.avi")
#define DEFAULT_WMV_FILENAME	_T("C:\\srec_def.wmv")

// OIIB: Object Instance Inforamtion Block
//
typedef struct
{
	IGraphBuilder*	m_pGraph;
	IBaseFilter*	m_pAviSplitter;

	TCHAR	m_szAviFileName[MAX_PATH];
	TCHAR	m_szWmvFileName[MAX_PATH];
	TCHAR	m_szWmvProfileName[MAX_PATH];

	double	m_tRenderStart;
	double	m_tRenderStop;

	HANDLE  m_hRenderWaitThread;
	bool    m_bRenderWaitThreadAlive;

	// Callback proc.
	//
	DWORD				m_dwProgressCBData;
	WGENPROGRESSPROC	m_pProgressCBProc;
	WGENERRORPROC		m_pErrorCBProc;

	
	// Etc
	BOOL		m_bValidLicenseKey;
} OIIB;

wgenError	g_LastError;

//
// Create OIIB(Object Instance Inforamtion Block)
//
OIIB * CreateOIIB()
{
	OIIB * pOiib = new OIIB;
	if (!pOiib)
	{
		wgenSetLastError(pOiib, wgenOutOfMemory);
		return NULL;
	}

	pOiib->m_pGraph		= NULL;
	pOiib->m_pAviSplitter	= NULL;

	_tcsncpy(pOiib->m_szAviFileName, DEFAULT_AVI_FILENAME, _tcslen(DEFAULT_AVI_FILENAME)+1);
	_tcsncpy(pOiib->m_szWmvFileName, DEFAULT_WMV_FILENAME, _tcslen(DEFAULT_WMV_FILENAME)+1);
	ZeroMemory(pOiib->m_szWmvProfileName, MAX_PATH);

	pOiib->m_tRenderStart = 0.0;	// entire range: 0.0 ~ 0.0
	pOiib->m_tRenderStop = 0.0;

	pOiib->m_hRenderWaitThread = NULL;
	pOiib->m_bRenderWaitThreadAlive = false;

	pOiib->m_dwProgressCBData = 0;
	pOiib->m_pProgressCBProc = NULL;
	pOiib->m_pErrorCBProc = NULL;

	pOiib->m_bValidLicenseKey = FALSE;

	wgenSetLastError(pOiib, wgenNone);

	return pOiib;
}


//
// szLicense : WGENSDK00001-5638D00FCB74-96B64B349A86-CD9680064CA1
//
WMVGEN_API WGENHANDLE WINAPI wgenCreateInstance( TCHAR * szLicense )
{
	// Init. the quartz library
    ::CoInitialize(NULL);

	OIIB * pOiib = NULL;

	CProductCode pc;
	if (pc.CheckCode(szLicense, 12, _tcslen(szLicense)))
	{
		pOiib = CreateOIIB();
		if (pOiib)
		{
			pOiib->m_bValidLicenseKey = TRUE;
		}
		else
		{
			return NULL;
		}
	}

	return (WGENHANDLE) pOiib;
}

WMVGEN_API BOOL WINAPI wgenDestroyInstance( WGENHANDLE hInst )
{
	SAFE_DELETE(hInst);
	
	::CoUninitialize();

	return WMVGEN_SUCCESS;
}

WMVGEN_API BOOL WINAPI wgenSetAviFileName( WGENHANDLE hInst, LPCTSTR pszAviFileName )
{
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	if (!pszAviFileName)
	{
		wgenSetLastError(hInst, wgenInvalidInputFile);
		return FALSE;
	}

	OIIB * pOiib = (OIIB *) hInst;
	_tcsncpy(pOiib->m_szAviFileName, pszAviFileName, _tcslen(pszAviFileName)+1);

	return TRUE;
}

WMVGEN_API BOOL WINAPI wgenSetWmvFileName( WGENHANDLE hInst, LPCTSTR pszWmvFileName )
{
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	if (!pszWmvFileName)
	{
		wgenSetLastError(hInst, wgenInvalidOutputFile);
		return FALSE;
	}

	OIIB * pOiib = (OIIB *) hInst;
	_tcsncpy(pOiib->m_szWmvFileName, pszWmvFileName, _tcslen(pszWmvFileName)+1);

	return TRUE;
}


WMVGEN_API BOOL WINAPI wgenSetWmvProfileName( WGENHANDLE hInst, LPCTSTR pszProfileName )
{
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	if (!pszProfileName)
	{
		wgenSetLastError(hInst, wgenInvalidWmvProfileName);
		return FALSE;
	}

	OIIB * pOiib = (OIIB *) hInst;
	_tcsncpy(pOiib->m_szWmvProfileName, pszProfileName, _tcslen(pszProfileName)+1);

	return TRUE;
}

WMVGEN_API BOOL WINAPI wgenSetRenderRange( WGENHANDLE hInst, double tStart, double tStop )
{
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	OIIB * pOiib = (OIIB *) hInst;
	pOiib->m_tRenderStart = (tStart < 0.0) ? 0.0 : tStart;
	pOiib->m_tRenderStop  = (tStop < 0.0) ? 0.0 : tStop;

	return TRUE;
}

ULONG CALLBACK RenderWaitProc(WGENHANDLE hInst)
{
	if (!hInst)
		return 0;

	OIIB * pOiib = (OIIB *) hInst;

	CComQIPtr<IMediaControl> pControl = pOiib->m_pGraph;
	CComQIPtr<IMediaSeeking> pSeek = pOiib->m_pGraph;
	CComQIPtr<IMediaEvent> pEvent = pControl;

	if (!pControl || !pSeek || !pEvent)
	{
		wgenSetLastError(hInst, wgenInvalidState);
		return 0;
	}

	HRESULT hr = S_OK;
	LONG levCode;
	double  nErrorCode = 0;
	pOiib->m_bRenderWaitThreadAlive = true;
	while (pOiib->m_bRenderWaitThreadAlive)
	{
		hr = pEvent->WaitForCompletion(100, &levCode);
		if (hr == S_OK)
		{
			if (levCode == EC_ERRORABORT)
				nErrorCode = -3.0;
			else if (levCode == EC_USERABORT)
				nErrorCode = -2.0;
			else // (levCode == EC_COMPLETE)
				nErrorCode = -1.0;

			pOiib->m_pProgressCBProc(nErrorCode, (void *) pOiib->m_dwProgressCBData);	// terminate as Error state
			pOiib->m_bRenderWaitThreadAlive = false;
		}
		else if (hr == E_ABORT)	// timeout
		{				
			LONGLONG tCurrent;

			pSeek->GetCurrentPosition(&tCurrent);

			double refCurrent = (double)tCurrent / (double)10000000;
			pOiib->m_pProgressCBProc(refCurrent, (void *) pOiib->m_dwProgressCBData);
		}
		else if (hr == VFW_E_WRONG_STATE)
		{				
			pOiib->m_pProgressCBProc(-3.0, (void *) pOiib->m_dwProgressCBData);	// terminate as Error state
			pOiib->m_bRenderWaitThreadAlive = false;
		}
		
	}	// end of loop
	
	SAFE_RELEASE(pControl.p);
	SAFE_RELEASE(pSeek.p);
	SAFE_RELEASE(pEvent.p);
	SAFE_RELEASE(pOiib->m_pAviSplitter);
	TeardownGraph(pOiib->m_pGraph);

	return 0;
}

WMVGEN_API BOOL WINAPI wgenStartRender( WGENHANDLE hInst )
{
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	HRESULT hr = S_OK;

	OIIB * pOiib = (OIIB *) hInst;
	
    hr = ::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IFilterGraph, (void **)&pOiib->m_pGraph);

	hr = BuildGraph(pOiib->m_szAviFileName, pOiib->m_szWmvFileName, pOiib->m_szWmvProfileName, pOiib->m_pGraph, &pOiib->m_pAviSplitter);
	if ( FAILED(hr))
	{
		wgenSetLastError(hInst, wgenWmvRenderFailed);
		return FALSE;
	}

	CComQIPtr<IMediaControl> pControl = pOiib->m_pGraph;
	if (pControl)
	{
		// settings start and stop positions
		//
		hr = SetRenderRange(pOiib->m_pAviSplitter, pOiib->m_tRenderStart, pOiib->m_tRenderStop);

		// start the graph
	    hr = pControl->Run();
		if ( FAILED(hr) )
		{
			wgenSetLastError(hInst, wgenWmvRenderFailed);
			return FALSE;
		}
		else
		{
			DWORD dwThreadID;
			pOiib->m_hRenderWaitThread = ::CreateThread(NULL, 0, RenderWaitProc, pOiib, 0, &dwThreadID);
			if (!pOiib->m_hRenderWaitThread)
			{
				wgenSetLastError(hInst, wgenCreateThreadFailed);
				return FALSE;
			}
		}
	}

	return TRUE;
}

WMVGEN_API BOOL WINAPI wgenStopRender( WGENHANDLE hInst )
{
	OIIB * pOiib = (OIIB *) hInst;
	if (!hInst)
	{
		wgenSetLastError(hInst, wgenInvalidParam);
		return FALSE;
	}

	BOOL bResult = TRUE;
	pOiib->m_bRenderWaitThreadAlive = false;
	Sleep(0);

	CComQIPtr<IMediaControl> pControl = pOiib->m_pGraph;
	if (pControl)
	{
		// start the graph
		HRESULT hr = pControl->Stop();
		if ( FAILED(hr) )
			wgenSetLastError(hInst, wgenWmvRenderFailed);

		//pControl.p->Release();
	}

	::CloseHandle(pOiib->m_hRenderWaitThread);
	pOiib->m_hRenderWaitThread = NULL;

	return bResult;
}

WMVGEN_API BOOL WINAPI wgenSetProgressCallback( WGENHANDLE hInst, WGENPROGRESSPROC pCBProc, DWORD dwData )
{
	OIIB * pOiib = (OIIB *) hInst;

	if (pOiib)
	{
		pOiib->m_pProgressCBProc = pCBProc;
		pOiib->m_dwProgressCBData = dwData;

		return WMVGEN_SUCCESS;
	}

	return WMVGEN_FAILED;
}

WMVGEN_API BOOL WINAPI  wgenSetErrorCallback( WGENHANDLE hInst, WGENERRORPROC pCBProc, DWORD dwData )
{
	OIIB * pOiib = (OIIB *) hInst;

	if (pOiib)
	{
		pOiib->m_pErrorCBProc = pCBProc;

		return WMVGEN_SUCCESS;
	}

	return WMVGEN_FAILED;
}

WMVGEN_API BOOL WINAPI wgenSetLastError( WGENHANDLE hInst, wgenError err )
{
	g_LastError = err;

	OIIB * pOiib = (OIIB *) hInst;

	if (pOiib)
	{
		if (pOiib->m_pErrorCBProc)
			pOiib->m_pErrorCBProc(err, NULL);
	}

	return TRUE;
}

WMVGEN_API wgenError WINAPI wgenGetLastError()
{
	return g_LastError;
}


/////////////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
