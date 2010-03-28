// ListItem.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "ListItem.h"
#include "ListClipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListItem

CListItem::CListItem()
{
	m_bSelect = false;
	m_pBitmap = NULL;
	m_Index = 0;
	m_strPath = "";	
	m_strName = "";

	m_rcImageRect = CRect(0,0,0,0);
}

CListItem::~CListItem()
{
}


BEGIN_MESSAGE_MAP(CListItem, CWnd)
	//{{AFX_MSG_MAP(CListItem)
	ON_WM_PAINT()	
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListItem message handlers
BOOL CListItem::Create(CWnd* pWnd, UINT nID, CRect rect)
{
	ASSERT(pWnd);

	BOOL bRet = CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
								rect, pWnd, nID);
	
	return	bRet;
}

void CListItem::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	
	Graphics graphics(dc.m_hDC);
	graphics.Clear(Color(255,0,0,0));		
	if(m_pBitmap != NULL)
		graphics.DrawImage(m_pBitmap, 0, 0);

	Pen pen(Color(255,192,192,192), 2.0f);
	graphics.DrawRectangle(&pen, Rect(m_rcImageRect.left, m_rcImageRect.top,
								 m_rcImageRect.Width(), m_rcImageRect.Height()));

	CRect rc;
	GetClientRect(rc); 
//	rc.DeflateRect(1,1,1,1);
	dc.Draw3dRect(rc, RGB(100,100,100), RGB(100,100,100));
}

/////////////////////////////////////////////////////////////////////////////
//
void CListItem::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bSelect)
	{
		((CListClipWnd*)GetParent())->OnSelectChangeItem(m_Index);
		m_bSelect = true;
	}	

	CWnd::OnLButtonDown(nFlags, point);
}


void CListItem::SetSelection(bool bSelect)
{
	m_bSelect = bSelect;
}
