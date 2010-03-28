// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Author.h"
#include "SplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog


CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

BOOL CSplashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
            return TRUE;
    }
 
	if(!((osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) || // Microsoft Windows 95
        (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) || // Microsoft Windows 98
        (osvi.dwMajorVersion <= 4 && osvi.dwMinorVersion == 90)))  // Microsoft Windows Me
	{
		typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
		SLWA pSetLayeredWindowAttributes = NULL;
		HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
		pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
		HWND hwnd = this->m_hWnd; 
		SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);	
		pSetLayeredWindowAttributes(hwnd, 0, 220, LWA_ALPHA);
	}
	
	SetTimer(1, 1500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSplashDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);

	OnOK();
	CDialog::OnTimer(nIDEvent);
}

BOOL CSplashDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN )
	{
        if(pMsg->wParam == VK_F4)
			return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN)
	{			
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)		// Esc, Enter key down
			return	TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
