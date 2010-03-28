// Option_HotkeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_HotkeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_HotkeyDlg dialog


COption_HotkeyDlg::COption_HotkeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_HotkeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_HotkeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_HotkeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_HotkeyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_HotkeyDlg, CDialog)
	//{{AFX_MSG_MAP(COption_HotkeyDlg)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_HotkeyDlg message handlers

HBRUSH COption_HotkeyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_HotkeyDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

void COption_HotkeyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("강의나 저작을 하는 동안 마우스나 전자펜의 불필요한 움직임을 "
				"\r\n방지하기 위해 단축키를 제공합니다.",
				CRect(10, 225, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}
