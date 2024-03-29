//------------------------------------------------------------------------------
// File: DShowUtil.h
//
// Desc: DirectShow sample code - prototypes for utility functions
//
// Copyright (c) 2000-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef __DSHOW_UTIL_H__
#define __DSHOW_UTIL_H__


// Paste from baseclass(reftime.h)
const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

// Paste from baseclass(mtype.cpp)
void WINAPI DeleteMediaType(AM_MEDIA_TYPE *pmt);
AM_MEDIA_TYPE * WINAPI CreateMediaType(AM_MEDIA_TYPE const *pSrc);
HRESULT WINAPI CopyMediaType(AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource);
void WINAPI FreeMediaType(AM_MEDIA_TYPE& mt);

//
// Function prototypes for DirectShow applications
//
HRESULT GetPin(IBaseFilter * pFilter, PIN_DIRECTION dirrequired, 
               int iNum, IPin **ppPin);
    
HRESULT FindOtherSplitterPin(IPin *pPinIn, GUID guid, 
               int nStream, IPin **ppSplitPin);
    
HRESULT SeekNextFrame(IMediaSeeking * pSeeking, double FPS, long Frame );
    
IPin * GetInPin ( IBaseFilter *pFilter, int Num );
IPin * GetOutPin( IBaseFilter *pFilter, int Num );

HRESULT CountFilterPins(IBaseFilter *pFilter, ULONG *pulInPins, ULONG *pulOutPins);
HRESULT CountTotalFilterPins(IBaseFilter *pFilter, ULONG *pulPins);

// Find a renderer filter in an existing graph
HRESULT FindRenderer(IGraphBuilder *pGB, const GUID *mediatype, IBaseFilter **ppFilter);
HRESULT FindAudioRenderer(IGraphBuilder *pGB, IBaseFilter **ppFilter);
HRESULT FindVideoRenderer(IGraphBuilder *pGB, IBaseFilter **ppFilter);

// Finds a filter's server name
void ShowFilenameByCLSID(REFCLSID clsid, TCHAR *szFilename);

// Media file information
HRESULT GetFileDurationString(IMediaSeeking *pMS, TCHAR *szDuration);

// Property pages and capabilities
BOOL SupportsPropertyPage(IBaseFilter *pFilter);
HRESULT ShowFilterPropertyPage(IBaseFilter *pFilter, HWND hwndParent);
BOOL CanFrameStep(IGraphBuilder *pGB);

// Debug helper functions
void TurnOnDebugDllDebugging( );
void DbgPrint( char * pText );
void ErrPrint( char * pText );

// Adds/removes a DirectShow filter graph from the Running Object Table,
// allowing GraphEdit to "spy" on a remote filter graph if enabled.
HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveGraphFromRot(DWORD pdwRegister);


#endif	// __DSHOW_UTIL_H__
