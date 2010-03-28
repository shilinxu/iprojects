// DBHook.cpp : Defines the entry point for the DLL application.
// 

#include "stdafx.h"
#include "DBHook.h"

#include <Winuser.h>

#pragma comment(linker, "/section:SharedData,rws")
#pragma data_seg("SharedData")

HANDLE g_hModule=NULL;
HWND g_hParentWnd=NULL;
HHOOK g_hHookMouse=NULL;
HHOOK g_hHookJournal=NULL;
bool g_bHookEnable = true;
bool g_bFullWorkArea = false;
bool g_bDrawParentArea = true;
bool g_bSelectRange = false;

HCURSOR g_hCursor=NULL;
RECT g_rcWorkarea={0,0,0,0};

bool   g_bStrokeStart = false;

#pragma data_seg()


// 040513: add changes mousemsgs to custom messages for XInk engine.
//
UINT WM_XINK_STARTSTROKE;
UINT WM_XINK_ADDPOINT;
UINT WM_XINK_STOPSTROKE;


bool HitTestWorkarea(POINT pt);

LRESULT CALLBACK JournalRecordProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LLMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

void MouseEventHandler(WORD id, POINT pt);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hModule = hModule;
			
			// 040513: add changes mousemsgs to custom messages for XInk engine.
			//
			WM_XINK_STARTSTROKE = RegisterWindowMessageW(L"WM_XINK_STARTSTROKE");
			WM_XINK_ADDPOINT	= RegisterWindowMessageW(L"WM_XINK_ADDPOINT");
			WM_XINK_STOPSTROKE	= RegisterWindowMessageW(L"WM_XINK_STOPSTROKE");

			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

void MouseEventHandler(WORD id, POINT pt)
{
	POINT s_ptOld = {0, 0};	

	SHORT KeyStatus = GetKeyState(VK_LBUTTON);
	if (KeyStatus & 0x8000)	// LBUTTON down
	{		
		// hittest for valide region
		if (!g_bStrokeStart)
		{
			g_bStrokeStart = true;
			::PostMessage(g_hParentWnd, WM_XINK_STARTSTROKE, 0, MAKELPARAM(pt.x,pt.y));
		}
		//else if (g_bStrokeStart)
		//{
		//	g_bStrokeStart = false;
		//	::PostMessage(g_hParentWnd, WM_LBUTTONUP, 0xFF, MAKELPARAM(pt.x,pt.y));
		//}
	}

	// post WM_MOUSEMOVE
	if (g_bStrokeStart)
	{
		switch(id)		// hittest for valide region
		{
		case WM_RBUTTONDOWN:			
		case WM_NCRBUTTONDOWN:			
		case WM_LBUTTONUP:			
		case WM_NCLBUTTONUP:
			g_bStrokeStart = false;
			::PostMessage(g_hParentWnd, WM_XINK_STOPSTROKE, 0, MAKELPARAM(pt.x,pt.y));
			break;
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			s_ptOld = pt;
			::PostMessage(g_hParentWnd, WM_XINK_ADDPOINT, 0, MAKELPARAM(pt.x,pt.y));
			break;
		}
		// check point 
		//if (s_ptOld.x == pt.x && s_ptOld.y == pt.y)
		//	continue;
	}
}

// This is the constructor of a class that has been exported.
// see DBHook.h for the class definition
CDBHook::CDBHook()
{ 
	return;
}

DBHOOK_API void DBHSetHookEnable(bool bEnable)
{
	if(g_bFullWorkArea)
	{
		int	cx, cy;
		cx = ::GetSystemMetrics(SM_CXSCREEN);
		cy = ::GetSystemMetrics(SM_CYSCREEN);

		g_rcWorkarea.left = 0;
		g_rcWorkarea.top = 0;
		g_rcWorkarea.right = cx;
		g_rcWorkarea.bottom = cy;

	}
	else
		SystemParametersInfo(SPI_GETWORKAREA, NULL,	&g_rcWorkarea, NULL);

	g_bHookEnable = bEnable;
}

DBHOOK_API RECT DBHGetWorkarea() 
{
	// The size of the work area on the primary display monitor.
	// The work area is the portion of the screen not obscured 
	// by the system taskbar or by application desktop toolbars.
	//int width  = GetSystemMetrics(SM_CXFULLSCREEN);
	//int height = GetSystemMetrics(SM_CYFULLSCREEN);
	if(g_bFullWorkArea)
	{
		int	cx, cy;
		cx = ::GetSystemMetrics(SM_CXSCREEN);
		cy = ::GetSystemMetrics(SM_CYSCREEN);

		g_rcWorkarea.left = 0;
		g_rcWorkarea.top = 0;
		g_rcWorkarea.right = cx;
		g_rcWorkarea.bottom = cy;
	}
	else
		SystemParametersInfo(SPI_GETWORKAREA, NULL,	&g_rcWorkarea, NULL);

	return g_rcWorkarea;
}

DBHOOK_API void DBHSetHookFullWorkarea(bool bFullarea)
{
	g_bFullWorkArea = bFullarea;

	if(g_bFullWorkArea)
	{
		int	cx, cy;
		cx = ::GetSystemMetrics(SM_CXSCREEN);
		cy = ::GetSystemMetrics(SM_CYSCREEN);

		g_rcWorkarea.left = 0;
		g_rcWorkarea.top = 0;
		g_rcWorkarea.right = cx;
		g_rcWorkarea.bottom = cy;
	}
	else
		SystemParametersInfo(SPI_GETWORKAREA, NULL,	&g_rcWorkarea, NULL);
}

DBHOOK_API void DBHSetHookDrawParentarea(bool bDrawParentArea)
{
	g_bDrawParentArea = bDrawParentArea;
}

DBHOOK_API void DBHSetHookSelectRange(bool bSelectRange)
{	
	g_bSelectRange = bSelectRange;
}


DBHOOK_API bool DBHookInstallCheck() 
{
	if (g_hHookMouse && g_hHookMouse)
		return true;

	return false;
}

DBHOOK_API bool DBHookInstall(HWND hWnd, HCURSOR hCursor) 
{
	g_hParentWnd = hWnd;
	g_hCursor = hCursor;

	if (DBHookInstallCheck())
		return false;
	
    //g_hHook = ::SetWindowsHookEx(idHook, lpfn, GetModuleHandle(NULL), 0);	// global scope, use dll
    //g_hHook = ::SetWindowsHookEx(idHook, lpfn, NULL, AfxGetThread()->m_nThreadID);	// thread scope
	//g_hHook = ::SetWindowsHookEx( WH_CALLWNDPROC, CallWndProc, (HINSTANCE)g_hModule/*GetModuleHandle(NULL)*/, 0); 
	//g_hHook = ::SetWindowsHookEx( WH_GETMESSAGE, GetMsgProc, (HINSTANCE)g_hModule/*GetModuleHandle(NULL)*/, 0); 
	//g_hHook = ::SetWindowsHookEx( 14/*WH_MOUSE_LL*/, LLMouseProc, (HINSTANCE)g_hModule/*GetModuleHandle(NULL)*/, 0); 
	g_hHookMouse   = ::SetWindowsHookEx( WH_MOUSE, MouseProc, (HINSTANCE)g_hModule/*GetModuleHandle(NULL)*/, 0); 
	g_hHookJournal = ::SetWindowsHookEx( WH_JOURNALRECORD, JournalRecordProc, (HINSTANCE)g_hModule/*GetModuleHandle(NULL)*/, 0); 

	if (g_hHookMouse == NULL || g_hHookJournal == NULL)
		return false;
	else
	    return true;
}

DBHOOK_API int DBHookUninstall() 
{
	if (!DBHookInstallCheck())
		return ERROR_SUCCESS;

	//MessageBox(NULL, "DBHookUninstall", "aa", MB_OK);

	::UnhookWindowsHookEx(g_hHookMouse);
	::UnhookWindowsHookEx(g_hHookJournal);

	g_hHookMouse = NULL;
	g_hHookJournal = NULL;
	g_hParentWnd = NULL;

	// To obtain an error string for system error codes, use the FormatMessage function. 
	// For a complete list of error codes provided by the operating system,
	// see System Error Codes.
	return GetLastError();
}

DBHOOK_API void DBHookSetCursor(HCURSOR hCursor) 
{
	g_hCursor = hCursor;
}

bool HitTestWorkarea(POINT pt)
{
	if(g_bSelectRange)
		return true;

	RECT rcParentWnd;
	::GetWindowRect(g_hParentWnd, &rcParentWnd);

	if(g_bStrokeStart)
	{
		if(g_bDrawParentArea)
		{
			if (::PtInRect(&g_rcWorkarea, pt))
				return true;
		}
		else
		{
			if(::PtInRect(&g_rcWorkarea, pt) && !::PtInRect(&rcParentWnd, pt))
				return true;
		}
	}
	else if(::PtInRect(&g_rcWorkarea, pt) && !::PtInRect(&rcParentWnd, pt))
	{
		return true;
	}

	return false;
}

/**************************************************************** 
  WH_MOUSE hook procedure 
 ****************************************************************/ 
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	//TRACE("\n********** MouseProc()\n");
	if (nCode == HC_ACTION && g_hParentWnd && g_bHookEnable)
	{
		MOUSEHOOKSTRUCT *pmhs = (MOUSEHOOKSTRUCT *)lParam;

		switch (wParam)
		{
		//case WM_INITMENU:
		//	return 1;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			// hittest for valide region
			if (g_hParentWnd && g_bHookEnable && HitTestWorkarea(pmhs->pt))
			{
				//CallNextHookEx(g_hHook, nCode, wParam, lParam);
				MouseEventHandler(wParam, pmhs->pt);

				return 1;	// break message hook chaine
			}
/*
			{
			point.x = pmhs->pt.x;
			point.y = pmhs->pt.y;

			// hittest for valide region
			if (HitTestWorkarea(point))
			{
				if (g_hCursor)
					::SetCursor(g_hCursor);

				::PostMessage(g_hParentWnd, wParam, 0xFF, MAKELPARAM(point.x, point.y));
				CallNextHookEx(g_hHook, nCode, wParam, lParam);
				return 1;	// break message hook chaine
			}
			else	::PostMessage(g_hParentWnd, WM_LBUTTONUP, 0xFF, MAKELPARAM(point.x,point.y));
			}
			break;
*/
		}
	}

	//::MessageBeep(MB_ICONASTERISK);

    return CallNextHookEx(g_hHookMouse, nCode, wParam, lParam); 
} 


/**************************************************************** 
   WH_JOURNALRECORD hook procedure 
 ****************************************************************/ 
LRESULT CALLBACK JournalRecordProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	if (nCode == HC_ACTION && g_hParentWnd && g_bHookEnable)
	{
		POINT point;	//::GetCursorPos(&point);
		PEVENTMSG pMsg = (PEVENTMSG) lParam;

		point.x = pMsg->paramL;
		point.y = pMsg->paramH;

		switch (pMsg->message)
		{
		//case SC_SCREENSAVE:		// 스크린 세이버 무시 (잘안됨)
		//	return 0L;
/*
		case WM_NCLBUTTONDOWN:
		case WM_NCHITTEST:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCLBUTTONUP:
			if (::GetCapture() == g_hParentWnd)
			{
				::ReleaseCapture();
			}
			break;
*/
		case WM_SETCURSOR:		// 펜모양의 커서로 변경
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			// hittest for valide region
			if (HitTestWorkarea(point))
			{
				if (g_hCursor)
				{
					::SetCursor(g_hCursor);
				}
			}
			break;
		default:
			break;
		}
	}

	return CallNextHookEx(g_hHookJournal, nCode, wParam, lParam); 
} 


/**************************************************************** 
  WH_CALLWNDPROC hook procedure 
 ****************************************************************/ 
/*LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	PCWPSTRUCT pMsg = (PCWPSTRUCT) lParam;

	if (nCode == HC_ACTION && g_hParentWnd && g_bHookEnable)
	{
		switch (pMsg->message)
		{
		//case WM_INITMENU:
		//	return 1;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCMOUSEMOVE:
			{
			POINT point;
			GetCursorPos(&point);

			// hittest for valide region
			if (HitTestWorkarea(point))
			{
				if (g_hCursor)
					::SetCursor(g_hCursor);

				::PostMessage(g_hParentWnd, pMsg->message, 0xFF, pMsg->lParam);
				return 0;	// break message hook chaine
			}
			else	::PostMessage(g_hParentWnd, WM_LBUTTONUP, 0xFF, pMsg->lParam);
			}

			break;
		}
	}

    return CallNextHookEx(g_hHook, nCode, wParam, lParam); 
} */



/**************************************************************** 
  WH_GETMESSAGE hook procedure 
 ****************************************************************/ 
/*LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	PMSG pMsg = (PMSG) lParam;

	if (nCode == HC_ACTION && g_hParentWnd && g_bHookEnable)
	{
		switch (pMsg->message)
		{
		//case WM_INITMENU:
		//	return 1;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCMOUSEMOVE:
			{
			POINT point;
			GetCursorPos(&point);

			// hittest for valide region
			if (HitTestWorkarea(point))
			{
				if (g_hCursor)
					::SetCursor(g_hCursor);

				::PostMessage(g_hParentWnd, pMsg->message, 0xFF, pMsg->lParam);
				return 1;	// break message hook chaine
			}
			else	::PostMessage(g_hParentWnd, WM_LBUTTONUP, 0xFF, pMsg->lParam);
			}

			break;
		}
	}


    return CallNextHookEx(g_hHook, nCode, wParam, lParam); 
} */

/**************************************************************** 
  WH_MOUSE_LL hook procedure 
 ****************************************************************/ 
/*
 * Structure used by WH_MOUSE_LL
 */
/*
typedef struct tagMSLLHOOKSTRUCT {
    POINT   pt;
    DWORD   mouseData;
    DWORD   flags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} MSLLHOOKSTRUCT, FAR *LPMSLLHOOKSTRUCT, *PMSLLHOOKSTRUCT;
LRESULT CALLBACK LLMouseProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	POINT point;	//	::GetCursorPos(&point);
	MSLLHOOKSTRUCT *pmhs = (MSLLHOOKSTRUCT *)lParam;

	//TRACE("\n********** LLMouseProc()\n");
	if (nCode == HC_ACTION && g_hParentWnd && g_bHookEnable)
	{
		switch (wParam)
		{
		//case WM_INITMENU:
		//	return 1;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCMOUSEMOVE:
			{
			point.x = pmhs->pt.x;
			point.y = pmhs->pt.y;

			// hittest for valide region
			if (HitTestWorkarea(point))
			{
				if (g_hCursor)
					::SetCursor(g_hCursor);

				::PostMessage(g_hParentWnd, wParam, 0xFF, MAKELPARAM(point.x, point.y));
			    CallNextHookEx(g_hHook, nCode, wParam, lParam); 
				return 1;	// break message hook chaine
			}
			//else	::PostMessage(g_hParentWnd, WM_LBUTTONUP, 0xFF, MAKELPARAM(point.x,point.y));
			}
			break;
		}
	}

	//::MessageBeep(MB_ICONASTERISK);

    return CallNextHookEx(g_hHook, nCode, wParam, lParam); 
} 
*/