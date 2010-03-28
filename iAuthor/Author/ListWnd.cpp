// ListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "ListWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListWnd

CListWnd::CListWnd()
{
	m_ScrollPosition = 0;
	m_ShiftWndValue = 0;
	m_nPageHeight = 0;
	m_nScrollRange = 0;
}

CListWnd::~CListWnd()
{
}


BEGIN_MESSAGE_MAP(CListWnd, CWnd)
	//{{AFX_MSG_MAP(CListWnd)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListWnd message handlers

BOOL CListWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	m_nID = nID;

	DWORD dwStyle = WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE;
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);	
}


int CListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_ListClipWnd.Create(CRect(0,0,0,0), this);	

	DWORD dwstyle = SBS_VERT|WS_CHILD|WS_VISIBLE|SBS_RIGHTALIGN;
	m_ListScrollbar.Create(dwstyle, CRect(0,0,0,0), this,0);

	m_ScrollInfo.fMask = SIF_ALL;
    m_ScrollInfo.nPage = 0;
	m_ScrollInfo.nMin = 0;
    m_ScrollInfo.nMax = 0;
    m_ScrollInfo.nPos = m_ScrollPosition;	
	m_ListScrollbar.SetScrollInfo(&m_ScrollInfo);

	return 0;
}

void CListWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here	
	if(cx == 0 || cy == 0)
		return;

	CRect	rc = CRect(0,0,0,0);	

	rc.right = cx;
	rc.bottom = cy;
	m_ListClipWnd.MoveWindow(rc);
}


void CListWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if(pScrollBar == (CScrollBar*)&m_ListScrollbar)
    { 
		SCROLLINFO info;		
		m_ListScrollbar.GetScrollInfo(&info,SIF_ALL);

		switch (nSBCode) 
		{
		case SB_LINEUP:			
		case SB_PAGEUP:
			if(m_ScrollPosition > 0)
			{
				m_ScrollPosition--;
				MoveListClipWnd();
			}
			break;
		case SB_PAGEDOWN:
		case SB_LINEDOWN:
			if(m_nScrollRange > m_ScrollPosition)
			{
				m_ScrollPosition++;
				MoveListClipWnd();
			}
			break;
		case SB_THUMBTRACK:// Thumb을 moause down할때.			
		case SB_THUMBPOSITION:// Thumb을 Mouse up할때.
			{
				m_ScrollPosition = info.nTrackPos;
				MoveListClipWnd();
			}
			break;
		default:
			return;
		}
		
		m_ScrollInfo.fMask = SIF_ALL;	
		m_ScrollInfo.nPos = m_ScrollPosition;
		m_ScrollInfo.nTrackPos  = m_ScrollPosition;
		m_ListScrollbar.SetScrollInfo(&m_ScrollInfo);
	}	

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CListWnd::MoveListClipWnd()
{
	CRect rect, rect1;	
	GetClientRect(&rect);
	m_ListClipWnd.GetClientRect(&rect1);
	
	int nHeight = rect1.Height();

	if(m_ScrollPosition == 0)
	{
		rect1.top = 0;
		rect1.bottom = rect1.top + nHeight;
	}
	else if(m_ScrollPosition == m_nScrollRange)
	{
		rect1.top = rect.Height() - nHeight;
		rect1.bottom = rect1.top + nHeight;
	}
	else
	{
		rect1.top = -(m_nPageHeight*m_ScrollPosition);
		rect1.bottom = rect1.top + nHeight;	
	}
	 
	m_ListClipWnd.MoveWindow(rect1);
}



void CListWnd::MoveListWnd_Init(void)
{
	CRect rect, rect1;	
	GetClientRect(&rect);
	m_ListClipWnd.GetClientRect(&rect1);
	
	int nHeight = rect1.Height();
	m_ScrollPosition = m_ScrollInfo.nTrackPos = m_ScrollInfo.nPos = 0;
	m_ListScrollbar.SetScrollInfo(&m_ScrollInfo);
	
	rect1.top = 0;
	rect1.bottom = rect1.top + nHeight;	
	 
	m_ListClipWnd.MoveWindow(rect1);
}


/////////////////////////////////////////////////////////////////////////////
// Scroll bar

void CListWnd::ReLayOut(int nHeight, int nPage)
{
	CRect rc;
	GetClientRect(&rc);	
	
	if(nHeight > rc.Height())
	{
		CRect rc1;
		rc1=rc;

		int	Width;
		Width = GetSystemMetrics(SM_CXHSCROLL);

		rc.right -= (Width+1);
		rc.bottom = rc.top + nHeight;
		m_ListClipWnd.ShowWindow(SW_HIDE);
		m_ListClipWnd.MoveWindow(rc);

		m_ScrollInfo.fMask = SIF_ALL;
		m_ScrollInfo.nMax = nHeight/nPage;
		m_ScrollInfo.nPage = rc1.Height()/nPage;
		m_nScrollRange = m_ScrollInfo.nMax - m_ScrollInfo.nPage+1;
		m_ScrollPosition = m_ScrollInfo.nTrackPos = m_ScrollInfo.nPos = m_nScrollRange;
		
		m_ListScrollbar.SetScrollInfo(&m_ScrollInfo);

		m_nPageHeight = (nHeight - rc1.Height())/m_nScrollRange;
	

		MoveListClipWnd();
		m_ListClipWnd.ShowWindow(SW_SHOW);


		rc1.top +=1; rc1.bottom -=1;
		rc1.left = rc1.right-Width-1 ; rc.right = rc.left + Width;
		
		m_ListScrollbar.MoveWindow(rc1);
		m_ListScrollbar.ShowScrollBar(TRUE);
	}
	else
	{
		m_ListScrollbar.ShowScrollBar(FALSE);
		m_ListClipWnd.MoveWindow(rc);
	}	
}


/////////////////////////////////////////////////////////////////////////////
// Insert Item
BOOL CListWnd::AddSlideItem(CString strPath, int nIndex)
{
	if(strPath == "")
		return FALSE;

	return m_ListClipWnd.AddSlideItem(strPath, nIndex);
}


/////////////////////////////////////////////////////////////////////////////
// Initialize 
void CListWnd::Init(void)
{
	m_ScrollPosition = 0;
	m_ShiftWndValue = 0;
	m_nPageHeight = 0;
	m_nScrollRange = 0;
	
	m_ListClipWnd.Init();	
}


// Return Selected Item Path
CString CListWnd::GetSelteditemPath()
{
	return m_ListClipWnd.GetSelteditemPath();
}

void CListWnd::ReplaceSildeThumbnail(CString strPath)
{
	m_ListClipWnd.ReplaceSildeThumbnail(strPath);
}
