// ColorSelctDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "ColorSelctDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSelctDlg dialog


CColorSelctDlg::CColorSelctDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorSelctDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorSelctDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTracking = FALSE;
	
	m_EnumColor[0] = RGB(0xFF, 0xFF, 0xFF);
	m_EnumColor[1] = RGB(0x80, 0x80, 0x80);
	m_EnumColor[2] = RGB(0x00, 0x00, 0x00);
	m_EnumColor[3] = RGB(0xFF, 0x00, 0x00);
	m_EnumColor[4] = RGB(0xFF, 0x7D, 0x00);
	m_EnumColor[5] = RGB(0xFF, 0x80, 0x80);
	m_EnumColor[6] = RGB(0xFF, 0xFF, 0x00);
	m_EnumColor[7] = RGB(0x80, 0xFF, 0x80);
	m_EnumColor[8] = RGB(0x00, 0xFF, 0x00);
	m_EnumColor[9] = RGB(0x00, 0xFF, 0xFF);
	m_EnumColor[10] = RGB(0x00, 0x00, 0xFF);
	m_EnumColor[11] = RGB(0x80, 0x00, 0x80);
	m_EnumColor[12] = RGB(0xFF, 0x00, 0xFF);
	m_EnumColor[13] = RGB(0x80, 0x80, 0xFF);
	m_EnumColor[14] = RGB(0x80, 0x80, 0x00);
	m_EnumColor[15] = RGB(0x00, 0x80, 0x80);	

	m_SelectColor = m_EnumColor[2];	
	m_rcSelColor = CRect(53, 19, 69, 32);
	m_rcTempColor = CRect(-10,-10,-5,-5);
}


void CColorSelctDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorSelctDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorSelctDlg, CDialog)
	//{{AFX_MSG_MAP(CColorSelctDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSelctDlg message handlers

BOOL CColorSelctDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CColorSelctDlg::SetEnableTransparent(bool bTrans)
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

void CColorSelctDlg::OnMouseMove(UINT nFlags, CPoint point) 
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
	int nCol, nRow;
	GetClientRect(&rc);

	if(rc.PtInRect(point) && point.y > 18 && point.y < 34)
		nRow = 0;
	else if(rc.PtInRect(point) && point.y > 45 && point.y < 61)
		nRow = 1;		
	else
	{	
		rc = m_rcTempColor;
		rc.InflateRect(3,3,3,3);
		m_rcTempColor = CRect(-10,-10,-5,-5);
		InvalidateRect(&rc);

		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	if(point.x > 10 && point.y < 174)
	{
		rc = m_rcTempColor;
		rc.InflateRect(3,3,3,3);
		m_rcTempColor = CRect(-10,-10,-5,-5);
		InvalidateRect(&rc);

		if((point.x-10)%21 > 18)
		{
			CDialog::OnMouseMove(nFlags, point);
			return;
		}

		nCol = (point.x-10)/21;
		if(nRow == 0)
		{				
			rcTemp.top = 19;				
			rcTemp.bottom = 33;
		}
		else
		{				
 			rcTemp.top = 46;
			rcTemp.bottom = 60;
		}

		rcTemp.left = 11 + nCol*21;
		rcTemp.right = rcTemp.left + 17;

		m_rcTempColor = rcTemp;
		rcTemp.InflateRect(3,3,3,3);
		InvalidateRect(&rcTemp);
	}
	else
	{	
		rc = m_rcTempColor;
		rc.InflateRect(3,3,3,3);
		m_rcTempColor = CRect(-10,-10,-5,-5);
		InvalidateRect(&rc);

		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CColorSelctDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();

	CDialog::OnLButtonDown(nFlags, point);
}


void CColorSelctDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(GetCapture() == this)
	//{
	//	ReleaseCapture();

		CRect rc, rcTemp;
		int nCol, nRow;
		GetClientRect(&rc);

		if(rc.PtInRect(point) && point.y > 18 && point.y < 34)
			nRow = 0;
		else if(rc.PtInRect(point) && point.y > 45 && point.y < 61)
			nRow = 1;		
		else
		{	
			rc = m_rcTempColor;				
			m_rcTempColor = CRect(-10,-10,-5,-5);
			
			CDialog::OnLButtonUp(nFlags, point);
			return;
		}

		if(point.x > 10 && point.y < 174)
		{
			rc = m_rcTempColor;
			m_rcTempColor = CRect(-10,-10,-5,-5);
			
			if((point.x-10)%21 > 18)
			{
				CDialog::OnLButtonUp(nFlags, point);
				return;
			}

			nCol = (point.x-10)/21;
			if(nRow == 0)
			{				
				rcTemp.top = 19;				
				rcTemp.bottom = 33;
			}
			else
			{				
 				rcTemp.top = 46;
				rcTemp.bottom = 60;
			}

			rcTemp.left = 11 + nCol*21;
			rcTemp.right = rcTemp.left + 17;

			m_rcSelColor = rcTemp;
			m_SelectColor = m_EnumColor[nRow*8+nCol];

			GetParent()->SendMessage(WM_COLORSELCT, m_SelectColor, 0);
		}
		else
		{	
			rc = m_rcTempColor;
			m_rcTempColor = CRect(-10,-10,-5,-5);
		
			CDialog::OnLButtonUp(nFlags, point);
			return;
		}
	//}

	CDialog::OnLButtonUp(nFlags, point);
}


LRESULT CColorSelctDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bTracking)
	{
		GetParent()->SendMessage(WM_MOUSE_OUT, -1, 0);
		m_bTracking = FALSE;

		m_rcTempColor = CRect(-10,-10,-5,-5);
	}

	return 0;
} 

void CColorSelctDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	Bitmap, *pBitmap;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	Bitmap.LoadBitmap(IDB_COLORSELBG);
	pBitmap	= (CBitmap*)memDC.SelectObject(&Bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	Bitmap.DeleteObject();

	rc = m_rcSelColor;
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(214,227,247), RGB(214,227,247));	

	rc = m_rcTempColor;	
	dc.Draw3dRect(&rc, RGB(173,170,156), RGB(173,170,156));
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect(&rc, RGB(49,105,198), RGB(49,105,198));	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CColorSelctDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CColorSelctDlg::PreTranslateMessage(MSG* pMsg) 
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
