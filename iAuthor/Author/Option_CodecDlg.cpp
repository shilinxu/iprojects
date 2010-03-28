// Option_CodecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_CodecDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_CodecDlg dialog


COption_CodecDlg::COption_CodecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_CodecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_CodecDlg)
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_CodecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_CodecDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_CodecDlg, CDialog)
	//{{AFX_MSG_MAP(COption_CodecDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_VIDEOCODEC_CHECK, OnOptVideocodecCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_CodecDlg message handlers

BOOL COption_CodecDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadOption();
	m_bModifyFlag = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_CodecDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("비디오 압축 코덱을 선택 하시면 녹화 시 시스템에 설치된 비디오 코덱을"
				"\r\n선택하여 사용할 수 있습니다.",
				CRect(10, 110, 406, 180), DT_LEFT);

	if(m_bSelect)
	{
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText(" - 주의 : 선택하신 코덱을 사용할 경우 결과물의 품질 및 완성도는"
					"\r\n              본 프로그램과 완벽하게 호환 될 수 없습니다."
					"\r\n              또한 유료로 배포되는 일부 코덱은 본 프로그램에서 사용"
					"\r\n              할 수 없습니다.",
					CRect(10, 190, 406, 265), DT_LEFT);
	}
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject(); 
}

HBRUSH COption_CodecDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}


void COption_CodecDlg::OnOptVideocodecCheck() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_VIDEOCODEC_CHECK);
	if(pButton->GetCheck())
		m_bSelect = true;
	else
		m_bSelect = false;
	
	InvalidateRect(CRect(10, 190, 406, 265));

	m_bModifyFlag = true;
}



BOOL COption_CodecDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_CodecDlg::ReadOption()
{
	CString REG_KEY = CAppSettings::GetOptionCodecPath();

	CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_VIDEOCODEC_CHECK);
	
	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	CString strTemp;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "CHECK", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp = szCheck;

		if(strTemp == "TRUE")
		{
			pButton->SetCheck(1);
			m_bSelect = true;
		}
		else
		{
			pButton->SetCheck(0);
			m_bSelect = false;
		}

		RegCloseKey( hKey );
	}
	else
	{
		pButton->SetCheck(0);
		m_bSelect = false;
	}
}

bool COption_CodecDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionCodecPath();

	HKEY hKey;	
	DWORD dwBufLen = MAX_PATH;
	CString strTemp;

	if(ERROR_SUCCESS != RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{
		if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CLASSES_ROOT, REG_KEY, 0, "", 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dwBufLen))
		{
			AfxMessageBox("ERROR! : Option implement.");
			return false;
		}
	}

	CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_VIDEOCODEC_CHECK);
	if(pButton->GetCheck())
		strTemp = "TRUE";
	else
		strTemp = "FALSE";

	RegSetValueEx(hKey, "CHECK", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	RegCloseKey( hKey );

	return true;

}
