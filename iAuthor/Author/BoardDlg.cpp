// BoardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "BoardDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoardDlg dialog


CBoardDlg::CBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBoardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBGBitmap = 0;
	m_strImagePath = "";
}


void CBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoardDlg, CDialog)
	//{{AFX_MSG_MAP(CBoardDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern UINT WM_XINK_STARTSTROKE;
extern UINT WM_XINK_STOPSTROKE;
extern UINT WM_XINK_ADDPOINT;

/////////////////////////////////////////////////////////////////////////////
// CBoardDlg message handlers
BOOL CBoardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hCursor = ::LoadCursor(NULL, IDC_WAIT);
	// 다이얼로그 생성시 윈도 Z-Order 지정됨
	//::SetWindowPos(*this, HWND_BOTTOM, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBoardDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//AfxMessageBox(m_strImagePath);
	if (m_pBGBitmap)
	{
		CRect rc;
		GetClientRect(&rc);

		Graphics graphics(dc.m_hDC);	
		graphics.DrawImage(m_pBGBitmap, 0, 0, rc.Width(), rc.Height());
	}

}

BOOL CBoardDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

/////////////////////////////////////////////////////////////////////////////
//
void CBoardDlg::SetBoardStyle(int nBoardStyle)
{	
	switch(nBoardStyle)
	{	
	case	DESKTOP_BOARD:
		m_strImagePath = "";
		SetBoardImageByDesktop();
		Beep(1000,5);
		return;
	case	WHITE_BOARD:
		m_strImagePath.Format("%s\\WhiteBoard.bmp", CAppSettings::GetBoardPath());
		Beep(3000,5);
		break;
	case	BLACK_BOARD:
		m_strImagePath.Format("%s\\BlackBoard.bmp", CAppSettings::GetBoardPath());
		Beep(5000,5);
		break;
	case	MUSIC_BOARD:
		m_strImagePath.Format("%s\\MusicBoard.bmp", CAppSettings::GetBoardPath());
		Beep(7000,5);
		break;
	case	WRITE_BOARD:
		m_strImagePath.Format("%s\\WriteBoard.bmp", CAppSettings::GetBoardPath());
		Beep(9000,5);
		break;
	}

	if (!m_strImagePath.IsEmpty())
	{
		USES_CONVERSION;

		SAFE_DELETE(m_pBGBitmap);
		m_pBGBitmap = new Bitmap(A2W(m_strImagePath), FALSE);
	
		InvalidateRect(0);
	}
}

void CBoardDlg::SetCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
}

void CBoardDlg::SetBoardImage(CString strPath)
{	
	if (!strPath.IsEmpty())
	{
		USES_CONVERSION;

		m_strImagePath = strPath;

		SAFE_DELETE(m_pBGBitmap);
		m_pBGBitmap = new Bitmap(A2W(m_strImagePath), FALSE);
	
		InvalidateRect(0);
	}
}

// Set bgimage to the captured desktop image
void CBoardDlg::SetBoardImageByDesktop()
{
	CWaitCursor wait;

 	// Create a normal DC and a memory DC for the entire screen. The 
 	// normal DC provides a "snapshot" of the screen contents. The 
 	// memory DC keeps a copy of this "snapshot" in the associated 
 	// bitmap. 
	HDC hScreenDC = ::GetDC(0); 

 	// Create a compatible bitmap for hScreenDC.
 	int w = GetDeviceCaps(hScreenDC, HORZRES);
 	int h = GetDeviceCaps(hScreenDC, VERTRES);	

	HBITMAP	hBitmap = ::CreateCompatibleBitmap(hScreenDC, w, h);
	if (hBitmap == 0)
	{
		::ReleaseDC(0, hScreenDC);
 		return;
	}

	// Select the bitmaps into the compatible DC. 
 	HDC hMemDC = ::CreateCompatibleDC(hScreenDC);
	if (hMemDC && !::SelectObject(hMemDC, hBitmap)) 
	{
		::DeleteDC(hMemDC);
 		return;	
	}
 	 
 	// Hide the application window. 
 	//m_pParentWnd->ShowWindow(SW_HIDE); 
 
 	//Copy color data for the entire display into a 
 	//bitmap that is selected into a compatible DC.

	if (::BitBlt(hMemDC, 0,0, w,h, hScreenDC, 0,0, SRCCOPY /*| CAPTUREBLT*/)) 
	{
		SAFE_DELETE(m_pBGBitmap);
		m_pBGBitmap = new Bitmap(hBitmap, NULL);
	}

	// Release
 	::DeleteObject(hBitmap);
 	::DeleteDC(hMemDC);
	::ReleaseDC(0, hScreenDC);

	InvalidateRect(0);
}

BOOL CBoardDlg::PreTranslateMessage(MSG* pMsg) 
{				
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

void CBoardDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	::SetCursor(m_hCursor);
	::PostMessage(*AfxGetMainWnd(), WM_XINK_STARTSTROKE, 0, MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CBoardDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(m_hCursor);
	::PostMessage(*AfxGetMainWnd(), WM_XINK_ADDPOINT, 0, MAKELPARAM(point.x,point.y));
	
	CDialog::OnMouseMove(nFlags, point);
}

void CBoardDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	::SetCursor(m_hCursor);
	::PostMessage(*AfxGetMainWnd(), WM_XINK_STOPSTROKE, 0, MAKELPARAM(point.x,point.y));
	
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CBoardDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(m_hCursor);
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
