// LineWeightSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "LineWeightSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineWeightSelectDlg dialog


CLineWeightSelectDlg::CLineWeightSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineWeightSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineWeightSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTracking = FALSE;

	m_SelectWidth = 5;	
	m_rcSelWidth = CRect(0, 66, 94, 93);
	m_rcTempWidth = CRect(0,0,0,0);
}


void CLineWeightSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineWeightSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineWeightSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CLineWeightSelectDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineWeightSelectDlg message handlers

BOOL CLineWeightSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLineWeightSelectDlg::SetEnableTransparent(bool bTrans)
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


void CLineWeightSelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
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

	if(rc.PtInRect(point) && point.y > 11 && point.y < 119)
	{	
		rc = m_rcTempWidth;		
		m_rcTempWidth = CRect(-10,-10,-5,-5);
		InvalidateRect(&rc);

		int nCol = point.x/94;
		int nRow = (point.y-11)/27;

		rcTemp.top = 12 + nRow*27;				
		rcTemp.bottom = rcTemp.top + 27;

		rcTemp.left = nCol*94;
		rcTemp.right =(nCol+1)*94;

		m_rcTempWidth = rcTemp;
		InvalidateRect(&m_rcTempWidth);
	}
	else
	{	
		rc = m_rcTempWidth;		
		m_rcTempWidth = CRect(-10,-10,-5,-5);
		InvalidateRect(&rc);
	}


	CDialog::OnMouseMove(nFlags, point);
}



void CLineWeightSelectDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CLineWeightSelectDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(GetCapture() == this)
	//{
	//	ReleaseCapture();

		m_SelectWidth = 5;	
		m_rcSelWidth = CRect(68, 4, 136, 23);
		m_rcTempWidth = CRect(0,0,0,0);

		CRect rc, rcTemp;
		GetClientRect(&rc);

		if(rc.PtInRect(point) && point.y > 11 && point.y < 119)
		{	
			int nCol = point.x/94;
			int nRow = (point.y-11)/27;

			m_rcSelWidth.top = 12 + nRow*27;				
			m_rcSelWidth.bottom = m_rcSelWidth.top + 27;

			m_rcSelWidth.left = nCol*94;
			m_rcSelWidth.right =(nCol+1)*94;

			m_SelectWidth = nCol*8 + nRow*2+1;

			GetParent()->SendMessage(WM_LINEWEIGHTSELCT, m_SelectWidth, 0);
		}

		m_rcTempWidth = CRect(0,0,0,0);
	//}
	
	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CLineWeightSelectDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bTracking)
	{
		GetParent()->SendMessage(WM_MOUSE_OUT, -1, 0);
		m_bTracking = FALSE;

		m_rcTempWidth = CRect(0,0,0,0);
	}

	return 0;
} 

void CLineWeightSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	Bitmap, *pBitmap;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	Bitmap.LoadBitmap(IDB_LINEWEIGHTSELBG);
	pBitmap	= (CBitmap*)memDC.SelectObject(&Bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	Bitmap.DeleteObject();

	rc = m_rcSelWidth;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(214,227,247), RGB(214,227,247));

	rc = m_rcTempWidth;		
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CLineWeightSelectDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CLineWeightSelectDlg::PreTranslateMessage(MSG* pMsg) 
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
