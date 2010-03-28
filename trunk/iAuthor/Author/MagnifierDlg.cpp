// MagnifierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "MagnifierDlg.h"
#include "AppSettings.h"

#include "AuthorDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagnifierDlg dialog


CMagnifierDlg::CMagnifierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagnifierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagnifierDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMagnifierBitmap = NULL;	
	m_ptPrev = CPoint(0,0);
	m_bEnable = false;
	m_pParentWnd = NULL;
}


void CMagnifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagnifierDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMagnifierDlg, CDialog)
	//{{AFX_MSG_MAP(CMagnifierDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagnifierDlg message handlers
int CMagnifierDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	VERIFY(MagniPrevDlg.Create( IDD_MAGNIFIERPREVIEWDLG, this));
	MagniPrevDlg.ShowWindow(SW_HIDE);

	return 0;
}


void CMagnifierDlg::SetParentWnd(CWnd *pWnd)
{
	m_pParentWnd = pWnd;
}


void CMagnifierDlg::SetEnbleMagnifier(bool bEnable)
{
	m_bEnable = bEnable;

	if(m_pMagnifierBitmap != NULL)
	{
		delete m_pMagnifierBitmap;
		m_pMagnifierBitmap = NULL;
	}

	if(bEnable)
	{
		CString strPath;
		strPath.Format("%s\\Magnifier.bmp", CAppSettings::GetTempPath());

		WCHAR wszPath[MAX_PATH];
		MultiByteToWideChar( CP_ACP, 0, strPath, strlen(strPath)+1, wszPath,   
							 sizeof(wszPath)/sizeof(wszPath[0]) );

		m_pMagnifierBitmap = new Bitmap(wszPath,FALSE);

		SetCapture();
		SetCursorPos(m_nWindowSize.cx/2, m_nWindowSize.cy/2);
		MagniPrevDlg.MoveWindow(CRect(m_nWindowSize.cx/2-160, m_nWindowSize.cy/2-120,
										m_nWindowSize.cx/2+160, m_nWindowSize.cy/2+120));
		MagniPrevDlg.SetEnble(true);
		MagniPrevDlg.ShowWindow(SW_SHOW);	
	}
	else
	{
		MagniPrevDlg.ShowWindow(SW_HIDE);
		MagniPrevDlg.SetEnble(false);
	}
}


BOOL CMagnifierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	

	m_nWindowSize.cx = ::GetSystemMetrics(SM_CXSCREEN);
	m_nWindowSize.cy = ::GetSystemMetrics(SM_CYSCREEN);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMagnifierDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_pMagnifierBitmap != NULL)
	{
		CRect rc;
		GetClientRect(&rc);

		Graphics graphics(dc.m_hDC);		
		
		graphics.DrawImage(m_pMagnifierBitmap, 0, 0, rc.Width(), rc.Height());
	}
}


void CMagnifierDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	if(m_pMagnifierBitmap != NULL)
		delete m_pMagnifierBitmap;
}



BOOL CMagnifierDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


//////////////////////////////////////////////////////////////////////////////
//
void CMagnifierDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CMagnifierDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnEscape();
/*
	ReleaseCapture();
	MagniPrevDlg.ShowWindow(SW_HIDE);
	MagniPrevDlg.SetEnble(false);

	((CAuthorDlg*)m_pParentWnd)->OnNotifyZoomEnd(true);	
*/
	CDialog::OnRButtonDown(nFlags, point);
}


void CMagnifierDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetCapture() != this)
		SetCapture();

	if(point.x > m_nWindowSize.cx - 160)
		point.x = m_nWindowSize.cx - 160;

	if(point.y > m_nWindowSize.cy - 120)
		point.y = m_nWindowSize.cy - 120;

	if(point.x < 160)
		point.x = 160;

	if(point.y < 120)
		point.y = 120;

	if(m_ptPrev != point)
	{
		MagniPrevDlg.MoveWindow(CRect(point.x-160, point.y-120, point.x+160, point.y+120));
		MagniPrevDlg.Invalidate();
		m_ptPrev = point;
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CMagnifierDlg::OnEscape()
{
	ReleaseCapture();
	((CAuthorDlg*)m_pParentWnd)->OnNotifyZoomEnd(false);
}

BOOL CMagnifierDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{	
		UINT WPARAM = pMsg->wParam;
		switch(WPARAM)
		{
		case	VK_ESCAPE:		// Esc, Enter key down
			OnEscape();
			break;
		case	107:			// '+'
		case	VK_RIGHT:
		case	VK_UP:
			MagniPrevDlg.ZoomIn();
			break;
		case	109:			// '-'
		case	VK_LEFT:
		case	VK_DOWN:
			MagniPrevDlg.ZoomOut();
			break;
		case	VK_RETURN:		
		case	VK_SPACE:			
			return TRUE;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}
