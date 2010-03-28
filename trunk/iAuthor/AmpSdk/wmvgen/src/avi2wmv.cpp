// avi2wmv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "assert.h"

#include <dshow.h>		// dx90
#include <atlbase.h>
#include <dshowasf.h>

#include "common/dshowutil.h"

// Global data
DWORD g_dwGraphRegister=0;  // For ROT(running object table)

void ErrorReport(LPCTSTR pszMsg)
{
	printf("%s", pszMsg);
}

//------------------------------------------------------------------------------
// Name: SetNoClock()
// Desc: Prevents an unnecessary clock from being created.
// This speeds up the copying process, since the renderer won't wait
// for the proper time to render a sample; instead, the data will
// be processed as fast as possible.
//------------------------------------------------------------------------------
HRESULT SetNoClock(IFilterGraph *pGraph)
{
    if (!pGraph)
        return E_POINTER;

	HRESULT hr = S_OK;
    CComQIPtr<IMediaFilter> pFilter = pGraph;
    if (pFilter)
    {
        // Set to "no clock"
        hr = pFilter->SetSyncSource(NULL);
    }

    return hr;
}

//------------------------------------------------------------------------------
// Name: ReadProfileAsPRX()
// Desc: Reads a profile from a PRX file.
// Exam:
//------------------------------------------------------------------------------
STDMETHODIMP ReadProfileAsPRX( LPCTSTR tszFilename, LPWSTR* wszProfileData, DWORD& dwProfileDataLength )
{
	HANDLE hFile = NULL;
	HRESULT hr = S_OK;
	DWORD dwBytesRead;
	BOOL bResult;

	assert( tszFilename );
	assert( NULL == *wszProfileData );
	assert( 0 == dwProfileDataLength );

	do
	{
		//
		// Open the file
		//
		hFile = CreateFile( tszFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
		if ( INVALID_HANDLE_VALUE == hFile )
		{
			hr = HRESULT_FROM_WIN32( GetLastError() );
			break;
		}

		//
		// Read the profile data from the file
		//
		dwProfileDataLength = ::GetFileSize(hFile, NULL);
		if (dwProfileDataLength == 0) break;

		*wszProfileData = (WCHAR *) new BYTE[dwProfileDataLength+1];
		memset(*wszProfileData, 0, dwProfileDataLength+1);

		bResult = ReadFile(hFile, *wszProfileData, dwProfileDataLength, &dwBytesRead, NULL);
		if ( !bResult )
		{
			hr = HRESULT_FROM_WIN32( GetLastError() );
			break;
		}
	}
	while ( FALSE );

	//
	// Close the file, if it was opened successfully
	//
	SAFE_CLOSEHANDLE( hFile );

	return hr;
}


//------------------------------------------------------------------------------
// Name: WriteProfileAsPRX()
// Desc: Writes a profile to a PRX file.
// Exam:
//	dwProfileDataLength = 0;
//	hr = pProfileManager->SaveProfile( pProfile, NULL, &dwProfileDataLength );
//	wszProfileData = new WCHAR[ dwProfileDataLength + 1 ];
//	hr = pProfileManager->SaveProfile( pProfile, wszProfileData, &dwProfileDataLength );
//
//	//
//	// Write the profile to a file
//	//
//	hr = WriteProfileAsPRX( cSaveDialog.GetPathName(), wszProfileData,
//								dwProfileDataLength * sizeof( WCHAR ) );
//	if ( FAILED( hr ) )
//	{
//		break;
//	}
//------------------------------------------------------------------------------
STDMETHODIMP WriteProfileAsPRX( LPCTSTR tszFilename, LPCWSTR wszProfileData, DWORD dwProfileDataLength )
{
    HANDLE hFile = NULL;
    HRESULT hr = S_OK;
    DWORD dwBytesWritten;
    BOOL bResult;

    assert( tszFilename );
    assert( wszProfileData );
    assert( 0 != dwProfileDataLength );

    do
    {
        //
        // Create the file, overwriting any existing file
        //
        hFile = CreateFile( tszFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
        if ( INVALID_HANDLE_VALUE == hFile )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            break;
        }

        //
        // Write the profile data to the file
        //
        bResult = WriteFile( hFile, wszProfileData, dwProfileDataLength, &dwBytesWritten, NULL );
        if ( !bResult )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            break;
        }
    }
    while ( FALSE );

    //
    // Close the file, if it was opened successfully
    //
    SAFE_CLOSEHANDLE( hFile );

    return hr;
}

#include "initguid.h"

// cea8deff-0af7-4db9-9a38-fb3c3aefc0de           Avi Stream Splitter2 (when you installed universal codec)
DEFINE_GUID(CLSID_AviSplitter2,
0xcea8deff, 0x0af7, 0x4db9, 0x9a, 0x38, 0xfb, 0x3c, 0x3a, 0xef, 0xc0, 0xde);


HRESULT BuildGraph(LPCTSTR pszAviFileName, LPCTSTR pszWmvFileName, LPCTSTR pszProfileName, IGraphBuilder* pGraph, IBaseFilter** pAviSplitter)
{
	USES_CONVERSION;
	HRESULT hr = S_OK;
	CComPtr<IBaseFilter> pAsyncReader = NULL;
	CComPtr<IBaseFilter> pVidRenderer = NULL;
	CComPtr<IBaseFilter> pAudRenderer = NULL;
    CComPtr<IBaseFilter> pAsfWriter = NULL;
    CComPtr<IBaseFilter> pVidDecoder = NULL;
	CComPtr<IBaseFilter> pAudDecoder = NULL;

	//hr = pAviSplitter.CoCreateInstance(CLSID_AviSplitter);
	hr = CoCreateInstance(CLSID_AviSplitter2, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)pAviSplitter);	
 	if (FAILED(hr))
	hr = CoCreateInstance(CLSID_AviSplitter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)pAviSplitter);	
	if (!FAILED(hr))
	{
		hr = pGraph->AddFilter(*pAviSplitter, NULL);
	}

	// Render media file
	//
	//hr = pGraph->RenderFile(T2W("c:\\aa.avi"), NULL);
	hr = pGraph->AddSourceFilter(T2W(pszAviFileName), L"Source Filter", &pAsyncReader);
    CComPtr<IPin> pSourceOut;
    pSourceOut = GetOutPin(pAsyncReader, 0);
	hr = pGraph->Render(pSourceOut);
	if ( FAILED(hr))
	{
		ErrorReport("Can't open media file");
		return hr;
	}

	// Remove video renderer
	//
	hr = FindVideoRenderer(pGraph, &pVidRenderer);
	if ( FAILED(hr))
	{
		ErrorReport("Can't find Video Renderer filter");
		return hr;
	}
	hr = pGraph->RemoveFilter(pVidRenderer);

	// Remove audio renderer
	//
	hr = FindAudioRenderer(pGraph, &pAudRenderer);
	if ( FAILED(hr))
	{
		ErrorReport("Can't find Audio Renderer filter");
		return hr;
	}
	hr = pGraph->RemoveFilter(pAudRenderer);

	// Add WM AsfWriter filter
	//
	// {7c23220e-55bb-11d3-8b16-00c04fb6bd3d}
	//const CLSID CLSID_WMAWriter = {
	//	0x7c23220e,0x55bb,0x11d3,{0x8b,0x16,0x00,0xc0,0x4f,0xb6,0xbd,0x3d}
	//};
	hr = pAsfWriter.CoCreateInstance(CLSID_WMAsfWriter);
	if (!FAILED(hr))
	{
		hr = pGraph->AddFilter(pAsfWriter, NULL);
		CComQIPtr<IFileSinkFilter2> pSink2 = pAsfWriter;
		hr = pSink2->SetFileName(T2W(pszWmvFileName), NULL);
		hr = pSink2->SetMode(AM_FILE_OVERWRITE);

		CComQIPtr<IConfigAsfWriter> pConfig = pAsfWriter;
		hr = pConfig->SetIndexMode(TRUE);
	}
	else
	{
		ErrorReport("Could not create WMAsfWriter filter.");
		return hr;
	}

    // Connect Video device filter's stream to the ASF MUX/FW
	//
    CComPtr<IPin> pVidIn, pVidOut;
    CComPtr<IPin> pAudIn, pAudOut;
	hr  = pGraph->FindFilterByName(L"AVI Decompressor", &pVidDecoder);
	hr |= pGraph->FindFilterByName(L"ACM Wrapper", &pAudDecoder);
	if (FAILED(hr))
	{
		ErrorReport("Could not find decoder filters.");
		return hr;
	}
    pVidOut = GetOutPin(pVidDecoder, 0); pAudOut = GetOutPin(pAudDecoder, 0);
    pAudIn  = GetInPin(pAsfWriter, 0);   pVidIn   = GetInPin(pAsfWriter, 1);
	if (!pVidOut || !pAudOut || !pAudIn || !pVidIn)
	{
		ErrorReport("Could not find i/o pins.");
		return hr;
	}

	// 32bpp: Color space converter 가 연결된 경우 
	// 문제를 해결하기 위해 미리 연결을 해제
	//
	hr = pVidOut->Disconnect();					//hr = pGraph->Connect(pVidOut, pVidIn);// 필요한 경우 중간 필터가 연결됨
	hr  = pVidOut->Connect(pVidIn, NULL);		// 중간 필터없이 직접 연결
	if (FAILED(hr))
	{
		ErrorReport("Could not connect video decoder filter to the WMAsfWriter filter.");
		//return hr;
	}

	hr = pAudOut->Disconnect();					//hr = pGraph->Connect(pAudOut, pAudIn);
	hr  = pAudOut->Connect(pAudIn, NULL);		// 중간 필터없이 직접 연결
	if (FAILED(hr))
	{
		ErrorReport("Could not connect audio decoder filter to the WMAsfWriter filter.");
		return hr;
	}

	CComPtr<IWMProfileManager>	pProfileManager = NULL;
	CComPtr<IWMProfile> pProfile = NULL;
	hr = WMCreateProfileManager( &pProfileManager );
	if (!FAILED(hr))
	{
		CComQIPtr<IWMProfileManager2> pProfileManager2 = pProfileManager;
		if (pProfileManager2)
		{
			hr = pProfileManager2->SetSystemProfileVersion(WMT_VER_8_0);
			if (FAILED(hr))	hr = pProfileManager2->SetSystemProfileVersion(WMT_VER_7_0);
		
			//
			// Load the profile from XML
			//
			//hr = pProfileManager->LoadSystemProfile( 11L, &pProfile );

			WCHAR* pwszProfileData = NULL;
			DWORD  dwDataLength = 0;

			hr = ReadProfileAsPRX(pszProfileName, &pwszProfileData, dwDataLength);
			if (pwszProfileData == NULL)
			{
				ErrorReport("Error reading profile.");
				return hr;
			}

			hr = pProfileManager->LoadProfileByData(pwszProfileData, &pProfile);
			SAFE_DELETE(pwszProfileData);
		}
	}
	else
	{
		ErrorReport("Failed to Create WMCreateProfileManager");
		return hr;
	}
	

	CComQIPtr<IConfigAsfWriter> pConfigAsf = pAsfWriter;
	hr = pConfigAsf->ConfigureFilterUsingProfile(pProfile);		//ConfigureFilterUsingProfileId(12);
	//WCHAR pszName[100]; DWORD size;
	//pProfile->GetName(pszName, &size);

#ifdef _DEBUG
	// Add our graph to the running object table, which will allow
	// the GraphEdit application to "spy" on our graph
	hr = AddGraphToRot(pGraph, &g_dwGraphRegister);
	if (FAILED(hr))
	{
		g_dwGraphRegister = 0;
		ErrorReport(_T("Failed to register filter graph with ROT!"));
	}
#endif // _DEBUG

    // Set sync source to NULL to encode as fast as possible
    SetNoClock(pGraph);

	return hr;
}

HRESULT TeardownGraph(IGraphBuilder* pGraph)
{
	HRESULT hr = S_OK;
#ifdef _DEBUG
	// Remove filter graph from the running object table   
	RemoveGraphFromRot(g_dwGraphRegister);
#endif // _DEBUG

	pGraph->Release();
	return hr;
}

HRESULT SetRenderRange(IBaseFilter* pAviSplitter, double start, double stop)
{
	HRESULT hr = S_OK;

	CComPtr<IPin> pSplitterOut0;	// video
	CComPtr<IPin> pSplitterOut1;	// audio

	pSplitterOut0 = GetOutPin(pAviSplitter, 0);
	pSplitterOut1 = GetOutPin(pAviSplitter, 1);
	if (!pSplitterOut0 || !pSplitterOut1) return S_FALSE;

	CComQIPtr<IMediaSeeking> pMediaSeeking0 = pSplitterOut0;	//m_pGraph;
	CComQIPtr<IMediaSeeking> pMediaSeeking1 = pSplitterOut1;	//m_pGraph;
	if (!pMediaSeeking0 || !pMediaSeeking1) return S_FALSE;

	hr = pMediaSeeking0->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);	//&TIME_FORMAT_FRAME);
	hr = pMediaSeeking1->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);	//&TIME_FORMAT_FRAME);

	// Set flags for the capabilities you want to check.
	DWORD dwCaps = AM_SEEKING_CanSeekAbsolute;

	hr = pMediaSeeking0->CheckCapabilities(&dwCaps);
	if(FAILED(hr)) 
	{
		printf("The stream cannot seek.\n");
		return S_FALSE;
	}
	else // The stream might have some of the capabilities.
	{
		if (dwCaps & AM_SEEKING_CanSeekAbsolute)
			printf("AM_SEEKING_CanSeekAbsolute\n");
		if (dwCaps & AM_SEEKING_CanSeekForwards)
			printf("AM_SEEKING_CanSeekForwards\n");
		if (dwCaps & AM_SEEKING_CanSeekBackwards)
			printf("AM_SEEKING_CanSeekBackwards\n");
	}

	REFERENCE_TIME Duration = 0;
	hr = pMediaSeeking0->GetDuration(&Duration);

	// Set play range to duration.
	REFERENCE_TIME rtStart = (REFERENCE_TIME) (start * (double)UNITS);
	REFERENCE_TIME rtStop  = (REFERENCE_TIME) (stop * (double)UNITS);
	hr = pMediaSeeking0->SetPositions(&rtStart, AM_SEEKING_AbsolutePositioning, 
									  &rtStop , AM_SEEKING_AbsolutePositioning);

	hr = pMediaSeeking1->SetPositions(&rtStart, AM_SEEKING_AbsolutePositioning, 
									  &rtStop , AM_SEEKING_AbsolutePositioning);

	return hr;
}

/*
int main(int argc, char* argv[])
{
	HRESULT hr = S_OK;

	// Init. the quartz library
    CoInitialize(NULL);

    hr = m_pGraph.CoCreateInstance(CLSID_FilterGraph);

	hr = BuildGraph(m_pGraph);
	if ( FAILED(hr))
	{
		return 1;
	}

	CComQIPtr<IMediaControl> pControl = m_pGraph;
	if (pControl)
	{
		// settings start and stop positions
		//
		hr = SetRenderRange(5.0, 7.0);

		// start the graph
	    hr = pControl->Run();
		if ( FAILED(hr))
		{
			ErrorReport("pMediaControl->Run() Failed");
			return hr;
		}
	}

	getchar();
	pControl->Stop();

	m_pGraph.Release();

	CoUninitialize();

	return 0;
}
*/
