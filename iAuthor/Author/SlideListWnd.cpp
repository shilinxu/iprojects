// SlideListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "SlideListWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlideListWnd

CSlideListWnd::CSlideListWnd()
{
	m_nIndex = 0;
	m_strSelectItemPath = "";
}

CSlideListWnd::~CSlideListWnd()
{
}


BEGIN_MESSAGE_MAP(CSlideListWnd, CWnd)
	//{{AFX_MSG_MAP(CSlideListWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSlideListWnd message handlers

BOOL CSlideListWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	
	DWORD dwStyle = WS_CHILD/*|WS_CLIPCHILDREN*/|WS_CLIPSIBLINGS|WS_VISIBLE;
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CSlideListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_CLipListWnd.Create(CRect(0,0,0,0), this, IDW_SLIDELIST_CLIPLIST);	
	
	return 0;
}

BOOL CSlideListWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CSlideListWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	
	// Do not call CWnd::OnPaint() for painting messages
}

void CSlideListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_CLipListWnd.GetSafeHwnd() != NULL)
	{
		CRect rc;
		GetClientRect(&rc);
		m_CLipListWnd.MoveWindow(&rc);
	}
	
}


/////////////////////////////////////////////////////////////////////////////
//
void CSlideListWnd::Init()
{
	m_nIndex = 0;
	m_strSelectItemPath = "";

	m_CLipListWnd.Init();
}


void CSlideListWnd::AddItem(CString strPath)
{
	if(m_CLipListWnd.AddSlideItem(strPath, m_nIndex))
		m_nIndex++;
}

int CSlideListWnd::GetCount()
{
	return m_nIndex;
}


CString CSlideListWnd::GetSelteditemPath()
{
	return m_CLipListWnd.GetSelteditemPath();
}

void CSlideListWnd::ReplaceSildeThumbnail(CString strPath)
{
	m_CLipListWnd.ReplaceSildeThumbnail(strPath);
}
