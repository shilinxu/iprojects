// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__28D6F603_00FA_48B5_9A59_666A4023BA22__INCLUDED_)
#define AFX_STDAFX_H__28D6F603_00FA_48B5_9A59_666A4023BA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxpriv.h>       //SK: makes A2W and other spiffy AFX macros work

//#include "vld/vld.h"		// The visual memory leak detector
#include "XInk/XInk.h"
#include "mmsystem.h"
#include "vfw.h"
#include <Commctrl.h>

#pragma comment(lib, "winmm")

#include <gdiplus.h>
using namespace Gdiplus;

#define WS_EX_LAYERED 0x00080000
#define LWA_ALPHA     0x00000002

#define	WM_MOUSE_OVER		WM_USER + 100
#define	WM_MOUSE_OUT		WM_USER + 101

#define WM_PENSELCT			WM_USER + 102
#define WM_ERASERSELCT		WM_USER + 103
#define WM_COLORSELCT		WM_USER + 104
#define WM_LINEWEIGHTSELCT	WM_USER + 105
#define WM_DIAGRAMSELCT		WM_USER + 106
#define WM_BOARDSELCT		WM_USER + 107
#define WM_SLIDEMENUSELCT	WM_USER + 108
#define WM_SNAPSHOT			WM_USER + 109

#define	DESKTOP_BOARD		0
#define	WHITE_BOARD			1
#define	BLACK_BOARD			2
#define	MUSIC_BOARD			3
#define	WRITE_BOARD			4
#define	IMAGE_BOARD			5

#define	PENCIL				1
#define	BRUSH				2
#define	ERASER				3
#define	ERASER_RECT			4
#define	ERASER_ALL			5
#define	UNDO				6
#define	DIAGRAM_ELLIPSE		7
#define	DIAGRAM_RECT		8
#define	DIAGRAM_LINE		9
#define	HIGHLIGHTPEN		10

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__28D6F603_00FA_48B5_9A59_666A4023BA22__INCLUDED_)
