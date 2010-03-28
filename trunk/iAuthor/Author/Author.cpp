// Author.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Author.h"
#include "AuthorDlg.h"
#include "SplashDlg.h"
#include "AppSettings.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuthorApp

BEGIN_MESSAGE_MAP(CAuthorApp, CWinApp)
	//{{AFX_MSG_MAP(CAuthorApp)	
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuthorApp construction

CAuthorApp::CAuthorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAuthorApp object
ULONG_PTR g_gdiplusToken;
CAuthorApp theApp;

// For avoiding Multiple Instances of an Application
// This can be done by creating a shared data segment.
// g_hMainWnd is set when the main window is created.
//
#pragma comment(linker, "/SECTION:.shr,RWS")
#pragma data_seg(".shr")
HWND g_hMainWnd = NULL;
#pragma data_seg()

/////////////////////////////////////////////////////////////////////////////
// CAuthorApp initialization

BOOL CAuthorApp::InitInstance()
{
	////////////////////////////////////////////////////////////////////////////
	// Avoiding Multiple Instances of an Application
	bool AlreadyRunning = (g_hMainWnd != NULL);
	if (AlreadyRunning)
	{
		::SetForegroundWindow(g_hMainWnd);

		if (IsIconic(g_hMainWnd))
		{
			::ShowWindow(g_hMainWnd, SW_RESTORE);
		}

		return FALSE; // terminates the creation
	}
	// Avoiding Multiple Instances of an Application end
	////////////////////////////////////////////////////////////////////////////
	
	if(!AfxOleInit())
    {
        AfxMessageBox("Cannot initialize COM services.");
        return FALSE;
    }

	AfxEnableControlContainer();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	CAppSettings::Init();

	if (CAppSettings::CheckVistaDWM4Restart())
	{
		AfxMessageBox("비스타 호환성 설정을 변경하고 종료합니다.\n다시 시작해 주세요..");
		return TRUE;
	}

	CSplashDlg SplashDlg;
	SplashDlg.DoModal();

	CAuthorDlg dlg;
	m_pMainWnd = &dlg;

	// For avoid multiple instance.
	g_hMainWnd = m_pMainWnd->GetSafeHwnd();
	
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CAuthorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	GdiplusShutdown(g_gdiplusToken);
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAuthorApp message handlers

