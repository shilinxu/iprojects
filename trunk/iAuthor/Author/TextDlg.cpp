// TextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "TextDlg.h"

#include "AuthorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog


CTextDlg::CTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextDlg)
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_bgColor = RGB(255,255,255);
	m_TextColor = RGB(0,0,0);
	m_nFontSize = 10;	
	m_bInputComplete = false;
	m_nEdgeState = 0;
	m_hCursor = NULL;
}


void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialog)
	//{{AFX_MSG_MAP(CTextDlg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDlg message handlers

void CTextDlg::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}


void CTextDlg::SetProperty(COLORREF bgColor, COLORREF TextColor, int nFont)
{
	m_bgColor = bgColor;
	m_TextColor = TextColor;
	m_nFontSize = nFont;

	m_Edit.GetDefaultCharFormat(m_EditFont);
		
	m_EditFont.dwMask |= CFM_CHARSET|CFM_COLOR;
	m_EditFont.dwEffects &= (~CFE_AUTOCOLOR);
	m_EditFont.dwEffects=0;
	m_EditFont.yHeight=(m_nFontSize+10)*20;
	m_EditFont.yOffset=0;
	m_EditFont.crTextColor = m_TextColor;
	m_EditFont.bCharSet = 0;
	m_EditFont.bPitchAndFamily = 34;
	memcpy(m_EditFont.szFaceName, "Aria", 6);
		
	m_Edit.SetDefaultCharFormat(m_EditFont);

	m_Edit.SetBackgroundColor(FALSE, m_bgColor);
}


int CTextDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD Style = ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_VSCROLL;
	BOOL bRet =	m_Edit.Create(Style, CRect(0,0,100,100), this, 0);
	m_Edit.ShowWindow(SW_SHOW);	
	
	return 0;
}

BOOL CTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTextDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_bInputComplete)
	{
		m_rcClient = CRect(0,cy-14,cx,cy);
		m_Edit.MoveWindow(2,2,cx-4,cy-18);		
		m_rcClose.SetRect(cx-16,cy-12,cx-6,cy-2);
	}
	else
	{
		m_rcClient = CRect(0,cy-22,cx,cy);
		m_Edit.MoveWindow(2,2,cx-4,cy-24);
		m_rcInputComplete.SetRect(cx-48, cy-20, cx-30, cy-2);
		m_rcClose.SetRect(cx-24,cy-20,cx-6,cy-2);
	}

	Invalidate();
}



BOOL CTextDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN )
	{
        if(pMsg->wParam == VK_F4)
			return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN)
	{	
		if(pMsg->wParam == VK_ESCAPE)
			return	TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			m_Edit.ReplaceSel("\t");
			return TRUE;
		}
	}		

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTextDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc, rc1;
	CPoint pt;
	GetWindowRect(&rc);
	rc1 = rc;
	rc1.DeflateRect(2,2,2,2);
	GetCursorPos(&pt);

	if(rc.PtInRect(pt) && !rc1.PtInRect(pt))
	{
		rc1.SetRect(rc.right-3, rc.bottom-3, rc.right, rc.bottom);
		if(rc1.PtInRect(pt))
		{
			m_nEdgeState = 5; //NWSE
			m_hCursor = ::LoadCursor(NULL, IDC_SIZENWSE);
			::SetCursor(m_hCursor);
			return TRUE;
		}

		rc1.SetRect(rc.left, rc.top, rc.left+2, rc.bottom);
		if(rc1.PtInRect(pt))
		{
			m_nEdgeState = 1; //Left
			m_hCursor = ::LoadCursor(NULL, IDC_SIZEWE);
			::SetCursor(m_hCursor);
			return TRUE;
		}

		rc1.SetRect(rc.right-2, rc.top, rc.right, rc.bottom);
		if(rc1.PtInRect(pt))
		{
			m_nEdgeState = 2; //right
			m_hCursor = ::LoadCursor(NULL, IDC_SIZEWE);
			::SetCursor(m_hCursor);
			return TRUE;
		}

		rc1.SetRect(rc.left, rc.top, rc.right, rc.top+2);
		if(rc1.PtInRect(pt))
		{
			m_nEdgeState = 3; //top
			m_hCursor = ::LoadCursor(NULL, IDC_SIZENS);
			::SetCursor(m_hCursor);
			return TRUE;
		}

		rc1.SetRect(rc.left, rc.bottom-2, rc.right, rc.bottom);
		if(rc1.PtInRect(pt))
		{
			m_nEdgeState = 4; //bottom	
			m_hCursor = ::LoadCursor(NULL, IDC_SIZENS);
			::SetCursor(m_hCursor);
			return TRUE;
		}
	}
	else
	{
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
		::SetCursor(m_hCursor);
		m_nEdgeState = 0;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CTextDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	if(m_rcClient.PtInRect(point) && m_nEdgeState == 0)
	{		
		if(m_bInputComplete)
		{
			if(m_rcClose.PtInRect(point))
			{				
				m_pParent->PostMessage(WM_MESSAGE_TEXT_DELETE, (WPARAM)this, 0);				
				return;
			}
			else
			{			
				this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
				return;
			}
		}
		else
		{
			if(m_rcInputComplete.PtInRect(point))
			{
				((CAuthorDlg*)m_pParent)->OnNotifyTextInputComplete();
				m_bInputComplete = true;

				CRect rc;
				GetWindowRect(&rc);
				rc.bottom = rc.bottom - 8;
				MoveWindow(&rc);
			}
			else if(m_rcClose.PtInRect(point))
			{				
				m_pParent->PostMessage(WM_MESSAGE_TEXT_DELETE, (WPARAM)this, 0);				
				return;
			}
			else
			{			
				this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
				return;
			}
		}		
	}
	else if(m_nEdgeState)
		SetCapture();	

	CDialog::OnLButtonDown(nFlags, point);
}


void CTextDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON )
	{
		if(m_nEdgeState)
		{			
			ClientToScreen(&point);
			CRect frame;
			GetWindowRect(&frame);

			int nW, nH;
			switch(m_nEdgeState)
			{
			case	1:
				frame.left = point.x;
				if((frame.right - frame.left) > 80)
					MoveWindow(&frame);
				break;
			case	2:
				frame.right = point.x;
				if((frame.right - frame.left) > 80)
					MoveWindow(&frame);
				break;
			case	3:
				frame.top = point.y;
				if((frame.bottom - frame.top) > 50)
					MoveWindow(&frame);
				break;
			case	4:
				frame.bottom = point.y;
				if((frame.bottom - frame.top) > 50)
					MoveWindow(&frame);
				break;
			case	5:
				nW = point.x;
				nH = point.y;
				if((nW - frame.left) > 80)
					frame.right = point.x;
				if((nH - frame.top) > 50)				
					frame.bottom = point.y;

				MoveWindow(&frame);
				break;
			}			
		}
		else if(m_rcClient.PtInRect(point))
		{
			this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
			return;
		}		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CTextDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}


void CTextDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(154,149,200));
		
	dc.FillSolidRect(m_rcClient, RGB(200,149,154));
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
	
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_CLOSE);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);

	if(!m_bInputComplete)
		dc.BitBlt(m_rcClient.right-48, m_rcClient.bottom - 20, 42, bm.bmHeight, &memDC, 0, 0, SRCCOPY);	
	else
		dc.BitBlt(m_rcClient.right-16, m_rcClient.bottom - 12, 10, 10, &memDC, 42, 0, SRCCOPY);	
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();	
}