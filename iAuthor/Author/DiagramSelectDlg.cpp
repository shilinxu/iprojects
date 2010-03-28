// DiagramSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "DiagramSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramSelectDlg dialog


CDiagramSelectDlg::CDiagramSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagramSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiagramSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTracking = FALSE;

	m_nSelectDiagram = DIAGRAM_RECT;
	
	m_rcDiagram[0] = CRect(0,13,188,39);
	m_rcDiagram[1] = CRect(0,39,188,66);
	m_rcDiagram[2] = CRect(0,66,188,93);

	m_rcSelDiagram = m_rcDiagram[1];
	m_rcTempDiagram = CRect(0,0,0,0);
}


void CDiagramSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagramSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiagramSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CDiagramSelectDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramSelectDlg message handlers

BOOL CDiagramSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDiagramSelectDlg::SetEnableTransparent(bool bTrans)
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


void CDiagramSelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
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

	if(rc.PtInRect(point))
	{		
		if(m_rcDiagram[0].PtInRect(point))
		{
			if(m_rcTempDiagram == m_rcDiagram[0])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempDiagram;
			m_rcTempDiagram = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempDiagram = m_rcDiagram[0];
			InvalidateRect(m_rcTempDiagram);
		}
		else if(m_rcDiagram[1].PtInRect(point))
		{
			if(m_rcTempDiagram == m_rcDiagram[1])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempDiagram;
			m_rcTempDiagram = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempDiagram = m_rcDiagram[1];
			InvalidateRect(m_rcTempDiagram);
		}
		else if(m_rcDiagram[2].PtInRect(point))
		{
			if(m_rcTempDiagram == m_rcDiagram[2])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempDiagram;
			m_rcTempDiagram = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempDiagram = m_rcDiagram[2];
			InvalidateRect(m_rcTempDiagram);
		}
		else
		{
			if(m_rcTempDiagram.Width() == 0)
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempDiagram;
			m_rcTempDiagram = CRect(0,0,0,0);
			InvalidateRect(rcTemp);
		}
	}
	else
	{
		if(m_rcTempDiagram.Width() == 0)
		{
			CDialog::OnMouseMove(nFlags, point);
			return;
		}

		rcTemp = m_rcTempDiagram;
		m_rcTempDiagram = CRect(0,0,0,0);
		InvalidateRect(rcTemp);
	}
	CDialog::OnMouseMove(nFlags, point);
}


void CDiagramSelectDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CDiagramSelectDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(GetCapture() == this)
	//{
	//	ReleaseCapture();
	
		CRect rc;
		GetClientRect(&rc);

		if(rc.PtInRect(point))
		{
			if(m_rcDiagram[0].PtInRect(point))
			{
				m_nSelectDiagram = WHITE_BOARD;
				m_rcSelDiagram = m_rcDiagram[0];

				GetParent()->SendMessage(WM_DIAGRAMSELCT, DIAGRAM_ELLIPSE, 0);
			}
			else if(m_rcDiagram[1].PtInRect(point))
			{
				m_nSelectDiagram = WHITE_BOARD;
				m_rcSelDiagram = m_rcDiagram[1];

				GetParent()->SendMessage(WM_DIAGRAMSELCT, DIAGRAM_RECT, 0);
			}
			else if(m_rcDiagram[2].PtInRect(point))
			{
				m_nSelectDiagram = WHITE_BOARD;
				m_rcSelDiagram = m_rcDiagram[2];

				GetParent()->SendMessage(WM_DIAGRAMSELCT, DIAGRAM_LINE, 0);
			}
		}

		m_rcTempDiagram = CRect(0,0,0,0);
	//}

	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CDiagramSelectDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bTracking)
	{
		GetParent()->SendMessage(WM_MOUSE_OUT, -1, 0);
		m_bTracking = FALSE;
	}

	return 0;
} 

void CDiagramSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	Bitmap, *pBitmap;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	Bitmap.LoadBitmap(IDB_DIAGRAMSELBG);
	pBitmap	= (CBitmap*)memDC.SelectObject(&Bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	Bitmap.DeleteObject();

	rc = m_rcSelDiagram;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(214,227,247), RGB(214,227,247));

	rc = m_rcTempDiagram;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDiagramSelectDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CDiagramSelectDlg::PreTranslateMessage(MSG* pMsg) 
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
