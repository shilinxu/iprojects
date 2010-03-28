/////////////////////////////////////////////////////////////////////
//	avi2wmv.h - Header File for core of the BCL WMV Generator
//
//	Copyright(c) 2003 bclsoft Inc.  All rights reserved.
/////////////////////////////////////////////////////////////////////
#ifndef __AVI2WMV_H__
#define __AVI2WMV_H__


void ErrorReport(LPCTSTR pszMsg);

HRESULT BuildGraph(LPCTSTR pszAviFileName, LPCTSTR pszWmvFileName, LPCTSTR pszProfileName, IGraphBuilder* pGraph, IBaseFilter** pAviSplitter);
HRESULT SetRenderRange(IBaseFilter* pAviSplitter, double start, double stop);
HRESULT TeardownGraph(IGraphBuilder* pGraph);


#endif //__AVI2WMV_H__
