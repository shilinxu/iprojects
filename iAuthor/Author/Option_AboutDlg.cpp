// Option_AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_AboutDlg.h"
#include "AppSettings.h"
#include "OptionDlg.h"

#include "RegistrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_AboutDlg dialog
COption_AboutDlg::COption_AboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_AboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_AboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(255,255,255));
}


void COption_AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_AboutDlg)
	DDX_Control(pDX, IDC_OPTION_REGISTRATION, m_OptionRegBtn);
	DDX_Control(pDX, IDC_OPT_WARNINGEDITOR, m_WarningEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_AboutDlg, CDialog)
	//{{AFX_MSG_MAP(COption_AboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPTION_REGISTRATION, OnOptionRegistration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_AboutDlg message handlers

HBRUSH COption_AboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkColor(RGB(255,255,255));

	return m_brush;
}

BOOL COption_AboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(CAppSettings::IsRegistration())
	{
		m_OptionRegBtn.ShowWindow(SW_HIDE);

		CString temp, temp1;
		temp = CAppSettings::GetUserName();
		temp1 = CAppSettings::GetPCode();
		m_strInfo.Format("User : %s \r\nProduct Code : %s",temp, temp1);
	}
	else
		m_strInfo = "평가판 사용 중 입니다.\r\n등록 후 사용하십시오.";

	m_rcInfo = CRect(18,135,275,170);

	CString strTemp = _T("주의: \r\n")
				  _T("이 컴퓨터 프로그램은 저작권법과 국제 협약의 보호를 받습니다. ")
				  _T("이 프로그램의 전부 또는 일부를 무단으로 복제, 배포하는 행위는 ")
				  _T("민사 및 형사법에 의해 엄격히 규제되어 있으며, 기소 사유가 됩니다.");

	m_WarningEditor.SetWindowText(strTemp);

	m_bModifyRegistration = false;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_AboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RGB(255,255,255));

	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_LOGO);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(10, 40, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

	
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();	

	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);

	dc.SetBkMode(TRANSPARENT);
	rc = CRect(170,52, 310, 70);
	dc.DrawText("iAuthor(R)", &rc, DT_LEFT);

	rc = CRect(170,72, 400, 90);
	dc.DrawText("Copyright(C) 2004 MM coporation.", &rc, DT_LEFT);


	rc = CRect(170,92, 400, 110);
	dc.DrawText("All rights reserved.", &rc, DT_LEFT);

	dc.DrawText(m_strInfo, &m_rcInfo, DT_LEFT);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void COption_AboutDlg::OnOptionRegistration() 
{
	// TODO: Add your control notification handler code here
	CRegistrationDlg RegDlg;

	if(RegDlg.DoModal() == IDOK)
		m_bModifyRegistration = true;
}

BOOL COption_AboutDlg::PreTranslateMessage(MSG* pMsg) 
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
