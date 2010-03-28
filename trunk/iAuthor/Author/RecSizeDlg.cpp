// RecSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "RecSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecSizeDlg dialog


CRecSizeDlg::CRecSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecSizeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_rcRec = CRect(0,0,0,0);
	m_strSize = "0 x 0";
}


void CRecSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecSizeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CRecSizeDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecSizeDlg message handlers

void CRecSizeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);

	dc.SetTextColor(RGB(100,100,100));
	dc.DrawText(m_strSize, CRect(0,0,75,20), DT_CENTER);

	dc.SelectObject(pOldFont);
	Font.DeleteObject(); 
	// Do not call CDialog::OnPaint() for painting messages
}

void CRecSizeDlg::SetStartPosition(int ptX, int ptY)
{
	m_rcRec.left = m_rcRec.right = ptX;
	m_rcRec.top = m_rcRec.bottom = ptY;

	m_strSize = "0 x 0";
}

void CRecSizeDlg::SetPosition(int ptX, int ptY)
{
	m_rcRec.right = ptX;
	m_rcRec.bottom = ptY;

	int nWidth, nHeight;

	if(m_rcRec.left <= m_rcRec.right)
		nWidth = m_rcRec.right - m_rcRec.left;
	else
		nWidth = m_rcRec.left - m_rcRec.right;

	if(m_rcRec.top <= m_rcRec.bottom)
		nHeight = m_rcRec.bottom - m_rcRec.top;
	else
		nHeight = m_rcRec.top - m_rcRec.bottom;

	m_strSize.Format("%d x %d", nWidth, nHeight);

	Invalidate();
}

