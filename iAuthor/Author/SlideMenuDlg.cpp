// SlideMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "SlideMenuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlideMenuDlg dialog


CSlideMenuDlg::CSlideMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSlideMenuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlideMenuDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_strSelectSlide = "";
}

void CSlideMenuDlg::ArrangeControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x = 10;
	int y = rc.bottom - 57;

	//
	m_OpenBtn.SetWindowPos(0,   x,y,       0,0, SWP_NOSIZE | SWP_NOZORDER);	x += 70;
	m_SelBtn.SetWindowPos(0,    x+20,y,    0,0, SWP_NOSIZE | SWP_NOZORDER);	x += 40;
	m_CancelBtn.SetWindowPos(0, x+40,y,    0,0, SWP_NOSIZE | SWP_NOZORDER);
}

void CSlideMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlideMenuDlg)
	DDX_Control(pDX, IDC_SLIDE_SELECTBTN, m_SelBtn);
	DDX_Control(pDX, IDC_SLIDE_OPENBTN, m_OpenBtn);
	DDX_Control(pDX, IDC_SLIDE_CANCELBTN, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSlideMenuDlg, CDialog)
	//{{AFX_MSG_MAP(CSlideMenuDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SLIDE_SELECTBTN, OnSlideSelectbtn)
	ON_BN_CLICKED(IDC_SLIDE_CANCELBTN, OnSlideCancelbtn)
	ON_BN_CLICKED(IDC_SLIDE_OPENBTN, OnSlideOpenbtn)
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlideMenuDlg message handlers
int CSlideMenuDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_SlideListWnd.Create(CRect(3,3,217,161), this, IDW_SLIDELIST);

	return 0;
}


BOOL CSlideMenuDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	// TODO: Add extra initialization here	
	CRect rc;

	m_SelBtn.DrawBorder(FALSE);
	m_SelBtn.SetBitmaps(IDB_SELBTN, RGB(255,0,255));	
	m_SelBtn.DrawTransparent();

	m_OpenBtn.DrawBorder(FALSE);
	m_OpenBtn.SetBitmaps(IDB_BROSERBTN, RGB(255,0,255));	
	m_OpenBtn.DrawTransparent();

	m_CancelBtn.DrawBorder(FALSE);
	m_CancelBtn.SetBitmaps(IDB_CANCELBTN, RGB(255,0,255));	
	m_CancelBtn.DrawTransparent();

	SetFocus();
	
	ArrangeControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSlideMenuDlg::SetEnableTransparent(bool bTrans)
{
	if(bTrans)
	{
		typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
		SLWA pSetLayeredWindowAttributes = NULL;
		HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
		pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
		HWND hwnd = this->m_hWnd; 
		SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		pSetLayeredWindowAttributes(hwnd, RGB(255,0,255), 255, LWA_ALPHA);
	}
	else
	{
		ModifyStyleEx(WS_EX_LAYERED, 0);
	}
}


void CSlideMenuDlg::OnSlideOpenbtn() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = "이미지 파일 (*.bmp;*.gif;*.jpeg;*.jpg;*.png;*.tiff;*.wmf;*emf;)|*.bmp;*.gif;*.jpeg;*.jpg;*.png;*.tiff;*.wmf;*emf;|";
	CFileDialog OpenFileDialog( TRUE, "Save file", "",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter, GetDesktopWindow());

	OpenFileDialog.m_ofn.lpstrTitle = "Open Image file";
	
	if(OpenFileDialog.DoModal() == IDOK)
	{		
		CString strSlide = OpenFileDialog.GetPathName();

		AddSlide(strSlide);
	}	
}


void CSlideMenuDlg::OnSlideSelectbtn() 
{
	// TODO: Add your control notification handler code here	
	ShowWindow(SW_HIDE);
	m_strSelectSlide = m_SlideListWnd.GetSelteditemPath();

	if(!m_strSelectSlide.IsEmpty())
		GetParent()->SendMessage(WM_SLIDEMENUSELCT, TRUE, 0);
	else
		GetParent()->SendMessage(WM_SLIDEMENUSELCT, FALSE, 0);
}

void CSlideMenuDlg::OnSlideCancelbtn() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	GetParent()->SendMessage(WM_SLIDEMENUSELCT, FALSE, 0);
}


void CSlideMenuDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_SLIDEMENUBG);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();
}

BOOL CSlideMenuDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}


/////////////////////////////////////////////////////////////////////////////
//
void CSlideMenuDlg::AddSlide(CString strSlide)
{	
	m_SlideListWnd.AddItem(strSlide);
}

CString CSlideMenuDlg::GetSelectSlide()
{
	return m_strSelectSlide;
}


void CSlideMenuDlg::ReplaceSildeThumbnail(CString strPath)
{
	m_SlideListWnd.ReplaceSildeThumbnail(strPath);
}


/////////////////////////////////////////////////////////////////////////////
//
BOOL CSlideMenuDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN )
	{
        if(pMsg->wParam == VK_F4)
			return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)		// Esc to cancel
		{
			OnSlideCancelbtn();
			return	TRUE;
		}
		
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)		// Esc, Enter key down
			return	TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}




void CSlideMenuDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnSlideCancelbtn();
	
	CDialog::OnRButtonUp(nFlags, point);
}
