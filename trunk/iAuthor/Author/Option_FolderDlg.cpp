// Option_FolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_FolderDlg.h"
#include "AppSettings.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_FolderDlg dialog


COption_FolderDlg::COption_FolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_FolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_FolderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_FolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_FolderDlg)
	DDX_Control(pDX, IDC_OPT_CONTENTSEDITOR, m_ContetnsEditor);
	DDX_Control(pDX, IDC_OPT_SLIDEBROWBTN, m_SlideBrowBtn);
	DDX_Control(pDX, IDC_OPT_SLIDEEDITOR, m_SlideEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_FolderDlg, CDialog)
	//{{AFX_MSG_MAP(COption_FolderDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_CONTENTSBROWBTN, OnOptContentsbrowbtn)
	ON_BN_CLICKED(IDC_OPT_SLIDEBROWBTN, OnOptSlidebrowbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_FolderDlg message handlers

BOOL COption_FolderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_bModifyFlag = false;

	if(!CAppSettings::IsEnableSliderFolderChange())
	{
		m_SlideEditor.EnableWindow(FALSE);
		m_SlideBrowBtn.EnableWindow(FALSE);
	}

	m_ContetnsEditor.SetWindowText(CAppSettings::GetContentsPath());
	m_SlideEditor.SetWindowText(CAppSettings::GetSlidePath());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_FolderDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("저작한 동영상(AVI)의 저장을 위해 저장 폴더의 위치를 사용자 편의대로"
				"\r\n설정할 수 있습니다."
				"\r\n\r\n저작한 슬라이드의 저장 위치를 사용자 편의대로 설정할 수 있습니다.",
				CRect(10, 180, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject(); 
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH COption_FolderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

void COption_FolderDlg::OnOptContentsbrowbtn() 
{
	// TODO: Add your control notification handler code here
 	CString strTemp;
 	m_ContetnsEditor.GetWindowText(strTemp);
 
 	CFolderDialog OpenFolderDlg(strTemp);
 	if(OpenFolderDlg.DoModal() == IDOK)
 	{	
 		strTemp = OpenFolderDlg.GetCurDirectory();
 		m_ContetnsEditor.SetWindowText(strTemp);	
 	
 		CAppSettings::SetContentsPath(strTemp);	
		WriteOption();
 	}
}

void COption_FolderDlg::OnOptSlidebrowbtn() 
{
	// TODO: Add your control notification handler code here
 	CString strTemp;
 	m_SlideEditor.GetWindowText(strTemp);
 
 	CFolderDialog OpenFolderDlg(strTemp);
 	if(OpenFolderDlg.DoModal() == IDOK)
 	{	
 		strTemp = OpenFolderDlg.GetCurDirectory();
 		m_SlideEditor.SetWindowText(strTemp);
 		
 		CAppSettings::SetSlidePath(strTemp);
		WriteOption();
 	}	
}

BOOL COption_FolderDlg::PreTranslateMessage(MSG* pMsg) 
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

bool COption_FolderDlg::WriteOption()
{
	CString REG_KEY  = CAppSettings::GetOptionFolderPath();
	CString strTemp;
	
	HKEY hKey;	
	DWORD dwBufLen = MAX_PATH;

	if(ERROR_SUCCESS != RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{
		if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CLASSES_ROOT, REG_KEY, 0, "", 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dwBufLen))
		{
			AfxMessageBox("ERROR! : Option implement.");
			return false;
		}
	}

	m_SlideEditor.GetWindowText(strTemp);
	RegSetValueEx(hKey, "SLIDDE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	m_ContetnsEditor.GetWindowText(strTemp);
	RegSetValueEx(hKey, "CONTENTS", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}