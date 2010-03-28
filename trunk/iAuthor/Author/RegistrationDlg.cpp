// RegistrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "RegistrationDlg.h"

#include "ProductCode.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg dialog
CRegistrationDlg::CRegistrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegistrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegistrationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strUserName = "";
	m_strPCode = "";
	m_strRCode = "";
}


void CRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegistrationDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_RegBtn);
	DDX_Control(pDX, IDC_REGDLG_USERNAMEEDITOR, m_UserNameEditor);
	DDX_Control(pDX, IDC_REGDLG_REGCODEEDITOR, m_RegCodeEditor);
	DDX_Control(pDX, IDC_REGDLG_PRODUCTCODEEDITOR, m_ProductCodeEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegistrationDlg, CDialog)
	//{{AFX_MSG_MAP(CRegistrationDlg)
	ON_BN_CLICKED(IDOK, OnRegistration)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg message handlers

BOOL CRegistrationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_Brush.CreateSolidBrush(RGB(255,255,255));

	CRect rc;
	GetWindowRect(&rc);
	int nWidth, nHeight, cx, cy;
	nWidth = rc.Width();
	nHeight = rc.Height();
	cx = ::GetSystemMetrics(SM_CXSCREEN);
	cy = ::GetSystemMetrics(SM_CYSCREEN);

	rc.left = (cx-nWidth)/2;
	rc.right = rc.left + nWidth;
	rc.top = (cy - nHeight - 160)/2;
	rc.bottom = rc.top + nHeight;

	MoveWindow(&rc);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRegistrationDlg::OnRegistration() 
{
	// TODO: Add your control notification handler code here
	CProductCode pc;
	CString strSerialCode;

	m_UserNameEditor.GetWindowText(m_strUserName);	
	m_ProductCodeEditor.GetWindowText(m_strPCode);
	m_RegCodeEditor.GetWindowText(m_strRCode);

	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox("사용자 이름 을 입력하세요!", MB_ICONERROR);

		m_UserNameEditor.SetSel(0, -1);
		return;
	}

	if (m_strPCode.IsEmpty())
	{
		AfxMessageBox("제품 코드를 입력하세요!", MB_ICONERROR);

		m_ProductCodeEditor.SetSel(0, -1);
		return;
	}

	if (m_strRCode.IsEmpty())
	{
		AfxMessageBox("등록 코드를 입력하세요!", MB_ICONERROR);

		m_RegCodeEditor.SetSel(0, -1);
		return;
	}

	m_strPCode.TrimRight();
	m_strPCode.MakeUpper();
	if (m_strPCode.Find(_T("APREG"), 0) == -1)
	{
		AfxMessageBox("제품 코드가 일치하지 않습니다!", MB_ICONERROR);
		m_ProductCodeEditor.SetSel(0, -1);
		return;
	}	

	m_strUserName.TrimRight();
	m_strRCode.TrimRight();
	if(CAppSettings::SetRegistration((LPCTSTR) m_strUserName, (LPCTSTR) m_strPCode, (LPCTSTR) m_strRCode))
	{
		AfxMessageBox("등록이 완료되었습니다.\n프로그램을 다시 시작하십시오.", MB_OK);
		OnOK();
	}
	else
	{
		AfxMessageBox("제품 코드와 등록 코드가 일치하지 않습니다!\n정확한 제품 코드와 등록 코드를 입력하세요!", MB_ICONERROR);
		m_ProductCodeEditor.SetSel(0, -1);
	}
}

void CRegistrationDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CRegistrationDlg::PreTranslateMessage(MSG* pMsg) 
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

void CRegistrationDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();	

	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);

	dc.SetBkMode(TRANSPARENT);
	rc = CRect(10,102, 134, 120);
	dc.DrawText("User Name  : ", &rc, DT_RIGHT);
	
	rc = CRect(10,132, 134, 150);
	dc.DrawText("Product Code  : ", &rc, DT_RIGHT);

	rc = CRect(10,162, 134, 180);
	dc.DrawText("Registration Code  : ", &rc, DT_RIGHT);

	rc = CRect(170,12, 300, 30);
	dc.DrawText("iAuthor(R)", &rc, DT_LEFT);

	rc = CRect(170,32, 430, 50);
	dc.DrawText("Copyright(C) 2004 MM coporation.", &rc, DT_LEFT);

	rc = CRect(170,52, 430, 70);
	dc.DrawText("All rights reserved.", &rc, DT_LEFT);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();	
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CRegistrationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	hbr =  m_Brush;
	return hbr;
}
