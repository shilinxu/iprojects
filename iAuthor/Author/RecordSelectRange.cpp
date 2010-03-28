// RecordSelectRange.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "RecordSelectRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		RECORD_FULLSCREEN		1
#define		RECORD_WINDOW			2
#define		RECORD_AREA				3

/////////////////////////////////////////////////////////////////////////////
// CRecordSelectRange dialog


CRecordSelectRange::CRecordSelectRange(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordSelectRange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordSelectRange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nRangeState = RECORD_FULLSCREEN;
}


void CRecordSelectRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordSelectRange)
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordSelectRange, CDialog)
	//{{AFX_MSG_MAP(CRecordSelectRange)
	ON_BN_CLICKED(IDC_RECORD_RANGE_AREA, OnRecordRangeArea)
	ON_BN_CLICKED(IDC_RECORD_RANGE_FULL, OnRecordRangeFull)
	ON_BN_CLICKED(IDC_RECORD_RANGE_WINDOW, OnRecordRangeWindow)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordSelectRange message handlers
BOOL CRecordSelectRange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brush.CreateSolidBrush(RGB(203,202,206));

	CRect rc;
	GetWindowRect(&rc);

	int nTop, nLeft;
	nLeft = (::GetSystemMetrics(SM_CXSCREEN) - rc.Width())/2;
	nTop = (::GetSystemMetrics(SM_CYSCREEN) - rc.Height() - 200)/2;

	MoveWindow(nLeft, nTop, rc.Width(), rc.Height(), TRUE);


	m_OkBtn.DrawBorder(FALSE);
	m_OkBtn.SetBitmaps(IDB_OKBTN, RGB(255,0,255));	
	m_OkBtn.DrawTransparent();

	m_CancelBtn.DrawBorder(FALSE);
	m_CancelBtn.SetBitmaps(IDB_CANCELBTN, RGB(255,0,255));	
	m_CancelBtn.DrawTransparent();	

	int nID;
	switch(m_nRangeState)
	{	
	case	RECORD_WINDOW:
		nID = IDC_RECORD_RANGE_WINDOW;
		break;
	case	RECORD_AREA:
		nID = IDC_RECORD_RANGE_AREA;
		break;
	default:		
		nID = IDC_RECORD_RANGE_FULL;
		break;
	}

	CButton* pButton = (CButton*)GetDlgItem(nID);
	pButton->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}


void CRecordSelectRange::SetRecordRangeState(int nRecordRangeState)
{
	m_nRangeState = nRecordRangeState;
}

/////////////////////////////////////////////////////////////////////////////
//
void CRecordSelectRange::OnRecordRangeArea() 
{
	// TODO: Add your control notification handler code here
	m_nRangeState = RECORD_AREA;
}

void CRecordSelectRange::OnRecordRangeFull() 
{
	// TODO: Add your control notification handler code here
	m_nRangeState = RECORD_FULLSCREEN;
}

void CRecordSelectRange::OnRecordRangeWindow() 
{
	// TODO: Add your control notification handler code here
	m_nRangeState = RECORD_WINDOW;
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CRecordSelectRange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->m_hWnd != m_OkBtn.m_hWnd && pWnd->m_hWnd != m_CancelBtn.m_hWnd)
	{
		pDC->SetBkColor(RGB(203,202,206));
		hbr =  m_brush;
	}

	return hbr;
}

void CRecordSelectRange::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_RECORDRANGE);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();
}
