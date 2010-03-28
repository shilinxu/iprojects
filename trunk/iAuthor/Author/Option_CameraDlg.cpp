// Option_CameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_CameraDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_CameraDlg dialog


COption_CameraDlg::COption_CameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_CameraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_CameraDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_CameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_CameraDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_CameraDlg, CDialog)
	//{{AFX_MSG_MAP(COption_CameraDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_CAMERA_SIZE0, OnOptCameraSize0)
	ON_BN_CLICKED(IDC_OPT_CAMERA_SIZE1, OnOptCameraSize1)
	ON_BN_CLICKED(IDC_OPT_CAMERA_SIZE2, OnOptCameraSize2)
	ON_BN_CLICKED(IDC_OPT_CAMERA_SIZE3, OnOptCameraSize3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_CameraDlg message handlers

BOOL COption_CameraDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nCurSize = 0;
	ReadOption();

	CButton* pButton;
	switch(m_nCurSize)
	{
	case	1:
		pButton = (CButton*)GetDlgItem(IDC_OPT_CAMERA_SIZE1);
		pButton->SetCheck(1);
		break;
	case	2:
		pButton = (CButton*)GetDlgItem(IDC_OPT_CAMERA_SIZE2);
		pButton->SetCheck(1);
		break;		
	case	3:
		pButton = (CButton*)GetDlgItem(IDC_OPT_CAMERA_SIZE3);
		pButton->SetCheck(1);
		break;		
	default:
		pButton = (CButton*)GetDlgItem(IDC_OPT_CAMERA_SIZE0);
		pButton->SetCheck(1);
		m_nCurSize = 0;
		break;
	}
	
	m_bModifyFlag = false;	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_CameraDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("PC카메라 비디오 화면 사이즈를 조절할 수 있습니다."
				"\r\n모니터 해상도 설정에 따라 적절하게 선택하여 사용하십시오.",
				CRect(10, 210, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject(); 
}

HBRUSH COption_CameraDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_CameraDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_CameraDlg::OnOptCameraSize0() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurSize = 0;
	m_bModifyFlag = true;
}

void COption_CameraDlg::OnOptCameraSize1() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurSize = 1;
	m_bModifyFlag = true;
}

void COption_CameraDlg::OnOptCameraSize2() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurSize = 2;
	m_bModifyFlag = true;
}

void COption_CameraDlg::OnOptCameraSize3() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurSize = 3;
	m_bModifyFlag = true;
}

void COption_CameraDlg::ReadOption()
{
	CString REG_KEY = CAppSettings::GetOptionCameraPath();
	
	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "SIZE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		m_nCurSize = atoi(szCheck);

		RegCloseKey( hKey );
	}
	else
		m_nCurSize = 0;
}

bool COption_CameraDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionCameraPath();
	CString strTemp;
	int Num = 0;

	HKEY hKey;	
	DWORD dwBufLen = MAX_PATH;

	// HKEY_CURRENT_USER
	//const TCHAR* pszRegKeyPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//xIniRegistry reg;
	//if (reg.Open(pszRegKeyPath, HKEY_CURRENT_USER))
	if(ERROR_SUCCESS != RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{
		if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CLASSES_ROOT, REG_KEY, 0, "", 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dwBufLen))
		{
			AfxMessageBox("ERROR! : Option implement.");
			return false;
		}
	}

	strTemp.Format("%d", m_nCurSize);
	RegSetValueEx(hKey, "SIZE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}
