// ListClipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "ListClipWnd.h"
#include "ListWnd.h"

#include "MainFrm.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListClipWnd

CListClipWnd::CListClipWnd()
{
	m_IDSelectItem = -1;
	m_strSelItem = "";
	m_CurColumn = 0;
	m_CurRow = 0;

	m_nColumn = 0;
	m_ItemWidth = 80;
	m_ItemHeight = 60;	
	m_rcImageRect.right = m_ItemWidth;
	m_rcImageRect.bottom = m_ItemHeight;
	m_HSpace_Item = 15;	
	m_ptStart_Item.x = 20;
	m_ptStart_Item.y = 15;
	m_WSpace_Item = 15;

	m_rcSelectItem = CRect(0,0,0,0);
	m_rcImageRect = CRect(0,0,0,0);
}

CListClipWnd::~CListClipWnd()
{
}


BEGIN_MESSAGE_MAP(CListClipWnd, CWnd)
	//{{AFX_MSG_MAP(CListClipWnd)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListClipWnd message handlers
BOOL CListClipWnd::Create(const RECT& rect, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	DWORD dwStyle = WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE;

	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, 0);	
}

void CListClipWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(cx == 0 || cy == 0)
		return;

	m_nColumn = (cx+m_WSpace_Item)/(m_ItemWidth+m_WSpace_Item);
	int nSpace = cx - m_ItemWidth*m_nColumn;

	m_WSpace_Item = m_ptStart_Item.x = nSpace/(m_nColumn+1);

	int nHeight = GetSize();
	if(nHeight > 0)
	{
		((CListWnd*)GetParent())->ReLayOut(nHeight, (m_ItemHeight+m_HSpace_Item));
		((CListWnd*)GetParent())->MoveListWnd_Init();
	}

	LayOut();
}


void CListClipWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect	rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(255,255,255));

	if(m_IDSelectItem != -1)
	{
		if(m_rcSelectItem.Width() > 0)
		{			
			rc = m_rcSelectItem;
			rc.InflateRect(1,1,1,1);
			
			COLORREF Color = RGB(215, 226, 243);
			dc.Draw3dRect(&m_rcSelectItem, Color, Color);

			Color = ::GetSysColor(COLOR_MENUHILIGHT);
			rc.InflateRect(1,1,1,1);
			dc.Draw3dRect(&rc, Color, Color);		
		}
	}
}


void CListClipWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	POSITION pos = m_ItemList.GetHeadPosition();
	while( pos )
	{
		CListItem* pItem = (CListItem*)m_ItemList.GetNext(pos);		
		delete pItem->m_pBitmap;		
		pItem->DestroyWindow();
		delete pItem;
	}
	m_ItemList.RemoveAll();
}



// Initialize
void CListClipWnd::Init(void)
{
	m_IDSelectItem = -1;
	m_strSelItem = "";
	m_CurColumn = 0;
	m_CurRow = 0;
	m_rcSelectItem = CRect(0,0,0,0);

	POSITION pos = m_ItemList.GetHeadPosition();
	while( pos )
	{
		CListItem* pItem = (CListItem*)m_ItemList.GetNext(pos);	
		delete pItem->m_pBitmap;
		pItem->DestroyWindow();
		delete pItem;
	}
	m_ItemList.RemoveAll();	

	((CListWnd*)GetParent())->ReLayOut(0, (m_ItemHeight+m_HSpace_Item));

	Invalidate();
}


bool CListClipWnd::IsValidSlide(CString strPath)
{
	CListItem* pItem;
	POSITION pos;

	for(pos = m_ItemList.GetHeadPosition(); pos != NULL;)
	{
		pItem = (CListItem*)m_ItemList.GetNext(pos);

		if(pItem->m_strPath == strPath)
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// Add item
BOOL CListClipWnd::AddSlideItem(CString strPath, int nIndex)
{
	if(!IsValidSlide(strPath))
	{
		::MessageBox(NULL, "이미 불러들인 슬라이드입니다.", "Slide Message", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
		return FALSE;
	}

	Bitmap* pImg = NULL;
	Bitmap* pThumbnail;

	WCHAR wszPath[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, strPath, strlen(strPath)+1, wszPath,   
							 sizeof(wszPath)/sizeof(wszPath[0]) );

	pImg = Bitmap::FromFile(wszPath);

	if ( pImg == NULL || pImg->GetWidth() <= 0 || pImg->GetHeight() <= 0 )
	{
		delete pImg;
		::MessageBox(NULL, "불러들일 수 없는 파일 입니다.", "Slide Message", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		return FALSE;
	}

	Graphics graphics(this->m_hWnd);
	pThumbnail = new Bitmap(m_ItemWidth, m_ItemHeight, &graphics);

	Graphics* pThumbGraphics = new Graphics(pThumbnail);
	pThumbGraphics->Clear(Color(255,255,255,255));
	pThumbGraphics->DrawImage(pImg, 0, 0, m_ItemWidth, m_ItemHeight);

	delete pImg;
	delete pThumbGraphics;

	CListItem* pItem = new CListItem();	
	pItem->Create(this);	
	pItem->m_Index  = nIndex;	
	pItem->m_pBitmap = pThumbnail;	
	pItem->m_strPath = strPath;
	pItem->m_rcImageRect = m_rcImageRect;	

	m_ItemList.AddTail(pItem);	

	int nHeight = GetSize();

	if(nHeight > 0)
	{
		((CListWnd*)GetParent())->ReLayOut(nHeight, (m_ItemHeight+m_HSpace_Item));
		LayOut();
	}
	else
	{
		CRect rc = CRect(0,0,0,0);
		rc.left = m_ptStart_Item.x + (m_ItemWidth + m_WSpace_Item)*m_CurColumn;
		rc.top = m_ptStart_Item.y + (m_ItemHeight + m_HSpace_Item)*m_CurRow;
		rc.right =rc.left + m_ItemWidth;
		rc.bottom = rc.top + m_ItemHeight;

		pItem->MoveWindow(rc);

		m_CurColumn++;
		if(m_CurColumn == m_nColumn)
		{
			m_CurColumn = 0;
			m_CurRow++;
		}
	}

	return TRUE;
}


// Rearrange item position
void CListClipWnd::LayOut(void)
{
	CRect rc = CRect(0,0,0,0);
	POSITION pos = m_ItemList.GetHeadPosition();
	if( pos!=NULL )
	{
		m_CurColumn = 0;
		m_CurRow = 0;

		CListItem* pItem;
		for(;;m_CurColumn++)
		{
			if(m_CurColumn == m_nColumn)
			{
				m_CurColumn = 0;
				m_CurRow++;
			}
			pItem	= (CListItem*)m_ItemList.GetNext(pos);

			rc.left = m_ptStart_Item.x + (m_ItemWidth + m_WSpace_Item)*m_CurColumn;
			rc.right =rc.left + m_ItemWidth;

			rc.top = m_ptStart_Item.y + (m_ItemHeight + m_HSpace_Item)*m_CurRow;			
			rc.bottom = rc.top + m_ItemHeight;

			pItem->MoveWindow(rc);
			if( m_ItemList.IsEmpty() || pos==NULL ) break;
		}

		m_CurColumn++;
		if(m_CurColumn == m_nColumn)
		{
			m_CurColumn = 0;
			m_CurRow++;
		}
	}
}



// calcuate List window size
int CListClipWnd::GetSize(void)
{
	CRect rc;
	GetClientRect(&rc);

	int nCount = 0;
	for( POSITION pos = m_ItemList.GetHeadPosition(); pos != NULL; ) 
	{	
		m_ItemList.GetNext(pos);
		nCount++;
	}

	if(nCount > 0 && m_nColumn > 0)
	{
		int nHeight;
		if(nCount%m_nColumn)
			nHeight = m_ptStart_Item.y*2 + m_ItemHeight*((nCount/m_nColumn)+1) + m_HSpace_Item*(nCount/m_nColumn);
		else
				nHeight = m_ptStart_Item.y*2 + m_ItemHeight*(nCount/m_nColumn) + m_HSpace_Item*((nCount/m_nColumn)-1);

		if(rc.Height() != nHeight)
			return nHeight;
	}

	return 0;
}



// Change slide
void CListClipWnd::OnSelectChangeItem(int nIndex)
{		
	CWaitCursor wait;

	CString strPath;
	int nPrevIndex = m_IDSelectItem;
	m_strSelItem = "";

	if(nIndex != -1)
	{
		CListItem* pItem;
		POSITION pos;
		if(m_IDSelectItem != -1)
		{
			for(pos = m_ItemList.GetHeadPosition(); pos != NULL;)
			{
				pItem = (CListItem*)m_ItemList.GetNext(pos);

				if(pItem->m_Index == m_IDSelectItem)
				{
					pItem->SetSelection(false);
					break;
				}
			}
		}
	
		for(pos = m_ItemList.GetHeadPosition(); pos != NULL;)
		{
			pItem = (CListItem*)m_ItemList.GetNext(pos);

			if(pItem->m_Index == nIndex)
			{
				CRect rc;
				rc = m_rcSelectItem;
				m_rcSelectItem = CRect(0,0,0,0);
				rc.InflateRect(3,3,3,3);
				InvalidateRect(&rc);

				pItem->GetWindowRect(&m_rcSelectItem);
				ScreenToClient(&m_rcSelectItem);

				rc = m_rcSelectItem;
				rc.InflateRect(3,3,3,3);
				InvalidateRect(&rc);
				m_IDSelectItem = nIndex;

				m_strSelItem = pItem->m_strPath;				

				break;
			}		
		}
	}	
}


void CListClipWnd::ReplaceSildeThumbnail(CString strPath)
{
	CListItem* pItem;
	for(POSITION pos = m_ItemList.GetHeadPosition(); pos != NULL;)
	{
		pItem = (CListItem*)m_ItemList.GetNext(pos);

		if(pItem->m_strPath == strPath)
		{			
			WCHAR wszPath[MAX_PATH];
			MultiByteToWideChar( CP_ACP, 0, strPath, strlen(strPath)+1, wszPath,   
									 sizeof(wszPath)/sizeof(wszPath[0]) );

			Bitmap* pImg = Bitmap::FromFile(wszPath);
			if ( pImg == NULL )
			{
				return;
			}

			Graphics graphics(this->m_hWnd);
			Bitmap* pThumbnail = new Bitmap(m_ItemWidth, m_ItemHeight, &graphics);

			Graphics* pThumbGraphics = new Graphics(pThumbnail);
			pThumbGraphics->Clear(Color(255,255,255,255));
			pThumbGraphics->DrawImage(pImg, 0, 0, m_ItemWidth, m_ItemHeight);

			delete pThumbGraphics;				
			delete pImg;
			delete pItem->m_pBitmap;

			pItem->m_pBitmap = pThumbnail;
			pItem->Invalidate();

			break;
		}
	}
}


// Return Selected Item Path
CString CListClipWnd::GetSelteditemPath()
{	
	return m_strSelItem;
}
