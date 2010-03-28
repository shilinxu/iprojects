// MagnifierPrevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "MagnifierPrevDlg.h"
#include "AppSettings.h"

#include "MagnifierDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagnifierPrevDlg dialog


CMagnifierPrevDlg::CMagnifierPrevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagnifierPrevDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagnifierPrevDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nZoomRate = 2;
	m_pMagnifierBitmap = NULL;
}


void CMagnifierPrevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagnifierPrevDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMagnifierPrevDlg, CDialog)
	//{{AFX_MSG_MAP(CMagnifierPrevDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagnifierPrevDlg message handlers

BOOL CMagnifierPrevDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMagnifierPrevDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_pMagnifierBitmap != NULL)
	{	
		CRect rc;
		GetWindowRect(&rc);

		Graphics graphics(dc.m_hDC);

		Rect destRect(0, 0, 320, 240);
		int srcX = rc.left;
		int srcY = rc.top;
		int srcWidth = 320/m_nZoomRate;
		int srcHeight = 240/m_nZoomRate;

		graphics.DrawImage(m_pMagnifierBitmap, destRect, srcX, srcY, 
			srcWidth, srcHeight, UnitPixel, NULL, NULL, NULL);

		rc = CRect(0,0,320,240);
		dc.Draw3dRect(rc,RGB(100,100,100), RGB(100,100,100));
		rc.DeflateRect(1,1,1,1);
		dc.Draw3dRect(rc,RGB(100,100,100), RGB(100,100,100));
	}
}


void CMagnifierPrevDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pMagnifierBitmap != NULL)
		delete m_pMagnifierBitmap;	
}

BOOL CMagnifierPrevDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}


void CMagnifierPrevDlg::ZoomIn()
{
	if(m_nZoomRate == 4)
		return;

	m_nZoomRate++;	

	Invalidate();
}

void CMagnifierPrevDlg::ZoomOut()
{
	if(m_nZoomRate == 2)
		return;

	m_nZoomRate--;
	Invalidate();
}

void CMagnifierPrevDlg::SetEnble(bool bEnable)
{
	if(m_pMagnifierBitmap != NULL)
	{
		delete m_pMagnifierBitmap;
		m_pMagnifierBitmap = NULL;
	}

	if(bEnable)
	{
		CString strPath;
		strPath.Format("%s\\Magnifier.bmp", CAppSettings::GetTempPath());

		WCHAR wszPath[MAX_PATH];
		MultiByteToWideChar( CP_ACP, 0, strPath, strlen(strPath)+1, wszPath,   
							 sizeof(wszPath)/sizeof(wszPath[0]) );

		m_pMagnifierBitmap = new Bitmap(wszPath,FALSE);
	}
}
