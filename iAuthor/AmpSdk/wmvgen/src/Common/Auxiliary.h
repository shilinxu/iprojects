//------------------------------------------------------------------------------
// File: Auxiliary.h
//
// Desc: Useful auxiliaries for the Win32 Application.
//
// Copyright (c) 2000 - 2005, All rights reserved.
//------------------------------------------------------------------------------

#if !defined(AUXILIARY_H__3F5BAE08_581B_4008_ABB4_0A4D2D3456C6__INCLUDED_)
#define AUXILIARY_H__3F5BAE08_581B_4008_ABB4_0A4D2D3456C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//
// Function prototypes
//

//#ifdef _DEBUG

void _TRACE(LPCTSTR lpszFormat, ...) ;

//#endif


//
// Macros
//

#define SAFE_RELEASE( x )		if(x) { x->Release(); x = NULL; }
#define SAFE_ADDREF( x )		if(x) { x->AddRef(); }
#define SAFE_DELETE( x )		if(x) { delete x; x = NULL; }
#define SAFE_ARRAYDELETE( x )	if(x) { delete[] x; x = NULL; }
#define SAFE_SYSFREESTRING( x )	if(x) { SysFreeString( x ); x = NULL; }
#define SAFE_CLOSEHANDLE( x )	if(x && INVALID_HANDLE_VALUE != x) { CloseHandle( x ); x = NULL; }


#define JIF(x) { if (FAILED(hr=(x))) \
	{ TRACE(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr); return hr; }}

#define LIF(x) { if (FAILED(hr=(x))) \
	{ TRACE(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n"), hr); }}


//
// Useful Defines
//


#endif // !defined(AUXILIARY_H__3F5BAE08_581B_4008_ABB4_0A4D2D3456C6__INCLUDED_)
