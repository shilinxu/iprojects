// EraserSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "EraserSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEraserSelectDlg dialog


CEraserSelectDlg::CEraserSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEraserSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEraserSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTracking = FALSE;

	m_nSelectEraser = ERASER;

	m_rcEraser[0] = CRect(0,13,188,39);
	m_rcEraser[1] = CRect(0,39,188,66);
	m_rcEraser[2] = CRect(0,66,188,93);
	m_rcEraser[3] = CRect(0,93,188,120);

	m_rcSelEraser = m_rcEraser[3];
	m_rcTempEraser = CRect(0,0,0,0);
}


void CEraserSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEraserSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEraserSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CEraserSelectDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEraserSelectDlg message handlers

BOOL CEraserSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CEraserSelectDlg::SetEnableTransparent(bool bTrans)
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


void CEraserSelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
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
		if(m_rcEraser[0].PtInRect(point))
		{
			if(m_rcTempEraser == m_rcEraser[0])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempEraser;
			m_rcTempEraser = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempEraser = m_rcEraser[0];
			InvalidateRect(m_rcTempEraser);
		}
		else if(m_rcEraser[1].PtInRect(point))
		{
			if(m_rcTempEraser == m_rcEraser[1])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempEraser;
			m_rcTempEraser = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempEraser = m_rcEraser[1];
			InvalidateRect(m_rcTempEraser);
		}
		else if(m_rcEraser[2].PtInRect(point))
		{
			if(m_rcTempEraser == m_rcEraser[2])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempEraser;
			m_rcTempEraser = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempEraser = m_rcEraser[2];
			InvalidateRect(m_rcTempEraser);
		}
		else if(m_rcEraser[3].PtInRect(point))
		{
			if(m_rcTempEraser == m_rcEraser[3])
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempEraser;
			m_rcTempEraser = CRect(0,0,0,0);
			InvalidateRect(rcTemp);

			m_rcTempEraser = m_rcEraser[3];
			InvalidateRect(m_rcTempEraser);
		}
		else
		{
			if(m_rcTempEraser.Width() == 0)
			{
				CDialog::OnMouseMove(nFlags, point);
				return;
			}

			rcTemp = m_rcTempEraser;
			m_rcTempEraser = CRect(0,0,0,0);
			InvalidateRect(rcTemp);
		}
	}
	else
	{
		if(m_rcTempEraser.Width() == 0)
		{
			CDialog::OnMouseMove(nFlags, point);
			return;
		}

		rcTemp = m_rcTempEraser;
		m_rcTempEraser = CRect(0,0,0,0);
		InvalidateRect(rcTemp);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CEraserSelectDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CEraserSelectDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(GetCapture() == this)
	//{
	//	ReleaseCapture();

		CRect rc;
		GetClientRect(&rc);

		if(rc.PtInRect(point))
		{
			if(m_rcEraser[0].PtInRect(point))
			{
				m_nSelectEraser = UNDO;
				m_rcSelEraser = m_rcEraser[0];

				GetParent()->SendMessage(WM_ERASERSELCT, UNDO, 0);
			}
			else if(m_rcEraser[1].PtInRect(point))
			{
				m_nSelectEraser = ERASER_ALL;
				m_rcSelEraser = m_rcEraser[1];

				GetParent()->SendMessage(WM_ERASERSELCT, ERASER_ALL, 0);
			}
			else if(m_rcEraser[2].PtInRect(point))
			{
				m_nSelectEraser = ERASER_RECT;
				m_rcSelEraser = m_rcEraser[2];

				GetParent()->SendMessage(WM_ERASERSELCT, ERASER_RECT, 0);
			}
			else if(m_rcEraser[3].PtInRect(point))
			{
				m_nSelectEraser = ERASER;
				m_rcSelEraser = m_rcEraser[3];

				GetParent()->SendMessage(WM_ERASERSELCT, ERASER, 0);
			}		
		}

		m_rcTempEraser = CRect(0,0,0,0);
	//}
	
	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CEraserSelectDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bTracking)
	{
		GetParent()->SendMessage(WM_MOUSE_OUT, -1, 0);
		m_bTracking = FALSE;
		m_rcTempEraser = CRect(0,0,0,0);
	}

	return 0;
} 

void CEraserSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	Bitmap, *pBitmap;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	Bitmap.LoadBitmap(IDB_ERASERSELBG);
	pBitmap	= (CBitmap*)memDC.SelectObject(&Bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	Bitmap.DeleteObject();

	rc = m_rcSelEraser;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(214,227,247), RGB(214,227,247));

	rc = m_rcTempEraser;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CEraserSelectDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CEraserSelectDlg::PreTranslateMessage(MSG* pMsg) 
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
