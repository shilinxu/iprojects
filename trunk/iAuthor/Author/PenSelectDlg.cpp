// PenSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "PenSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenSelectDlg dialog


CPenSelectDlg::CPenSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPenSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPenSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTracking = FALSE;

	m_rcPen[0] = CRect(0,14,187,41);
	m_rcPen[1] = CRect(0,41,187,68);
	m_rcPen[2] = CRect(0,68,187,95);

	m_nSelectPen = PENCIL;
	m_rcSelPen = m_rcPen[2];
	m_rcCurPen = CRect(0,0,0,0);
}


void CPenSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPenSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPenSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CPenSelectDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenSelectDlg message handlers

BOOL CPenSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPenSelectDlg::SetEnableTransparent(bool bTrans)
{
	if(bTrans)
	{
		typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
		SLWA pSetLayeredWindowAttributes = NULL;
		HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
		pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
		HWND hwnd = this->m_hWnd; 
		SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		pSetLayeredWindowAttributes(hwnd, RGB(255,0,255), 230, LWA_ALPHA);
	}
	else
	{
		ModifyStyleEx(WS_EX_LAYERED, 0);
	}
}


void CPenSelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE;
        tme.dwHoverTime = 1;
        m_bTracking = _TrackMouseEvent(&tme);
    }    

	CRect rc, rcTemp;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{		
		for (int i=0; i<3; i++)
		{
			if (m_rcPen[i].PtInRect(point))
			{
				InvalidateRect(m_rcCurPen);
				m_rcCurPen = m_rcPen[i];
				InvalidateRect(m_rcCurPen);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CPenSelectDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}


void CPenSelectDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(GetCapture() == this)
	//{
		//ReleaseCapture();

		if(m_rcPen[0].PtInRect(point))
		{
			m_nSelectPen = HIGHLIGHTPEN;
			m_rcSelPen = m_rcPen[0];

			GetParent()->SendMessage(WM_PENSELCT, HIGHLIGHTPEN, 0);				
		}
		else if(m_rcPen[1].PtInRect(point))
		{
			m_nSelectPen = BRUSH;
			m_rcSelPen = m_rcPen[1];

			GetParent()->SendMessage(WM_PENSELCT, BRUSH, 0);
		}
		else if(m_rcPen[2].PtInRect(point))
		{
			m_nSelectPen = PENCIL;
			m_rcSelPen = m_rcPen[2];

			GetParent()->SendMessage(WM_PENSELCT, PENCIL, 0);				
		}

		m_rcCurPen = CRect(0,0,0,0);
	//}

	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CPenSelectDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bTracking)
	{
		GetParent()->SendMessage(WM_MOUSE_OUT, -1, 0);
		m_bTracking = FALSE;
		m_rcCurPen = CRect(0,0,0,0);
	}

	return 0;
} 

void CPenSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	Bitmap, *pBitmap;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	Bitmap.LoadBitmap(IDB_PENSELBG);
	pBitmap	= (CBitmap*)memDC.SelectObject(&Bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	Bitmap.DeleteObject();

	rc = m_rcSelPen;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(214,227,247), RGB(214,227,247));

	rc = m_rcCurPen;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CPenSelectDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPenSelectDlg::PreTranslateMessage(MSG* pMsg) 
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
