// Option_LogoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_LogoDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_LogoDlg dialog


COption_LogoDlg::COption_LogoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_LogoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_LogoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_LogoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_LogoDlg)
	DDX_Control(pDX, IDC_OPT_LOGOEDITOR, m_LogoEditor);
	DDX_Control(pDX, IDC_OPT_LOGOBROWBTN, m_LogoBrowBtn);
	DDX_Control(pDX, IDC_OPT_LOGOCHECKBTN, m_LogoChkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_LogoDlg, CDialog)
	//{{AFX_MSG_MAP(COption_LogoDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_LOGOCHECKBTN, OnOptLogocheckbtn)
	ON_BN_CLICKED(IDC_OPT_LOGOBROWBTN, OnOptLogobrowbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_LogoDlg message handlers
BOOL COption_LogoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_bReg = CAppSettings::IsRegistration();
	if(!m_bReg)
	{	
		m_LogoChkBtn.EnableWindow(FALSE);
		m_LogoBrowBtn.EnableWindow(FALSE);
	}
	
	ReadOption();
	m_bModifyFlag = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_LogoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("동영상 녹화 시에 삽입할 로고를 이미지 파일 형식(BMP)으로 불러올 수"
				"\r\n있습니다."
				"\r\n\r\n로고 삽입 설정에서 삽입하고자 하는 로고를 선택하시면 동영상 녹화 시에"
				"\r\n로고가 자동으로 삽입됩니다."
				"\r\n로고의 삽입은 저작권의 보호 및 기타 광고의 용도로 사용하실 수 있습니다.",
				CRect(10, 160, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH COption_LogoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

void COption_LogoDlg::OnOptLogocheckbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_LogoChkBtn.GetCheck())
 		m_LogoBrowBtn.EnableWindow();
 	else
 	{
 		m_LogoBrowBtn.EnableWindow(FALSE);
 		m_LogoEditor.SetWindowText("");
 	}
 
 	m_bModifyFlag = true;
}

void COption_LogoDlg::OnOptLogobrowbtn() 
{
	// TODO: Add your control notification handler code here
	CString strFilter;
 	strFilter = "Logo files (.bmp)|*.bmp||";
 
 	CFileDialog OpenFileDialog(TRUE, "bmp", "Logo",
 		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter, this);	
 	
 	if(OpenFileDialog.DoModal() == IDOK)
 	{
 		m_LogoEditor.SetWindowText(OpenFileDialog.GetPathName());
 
 		m_bModifyFlag = true;
 	}
}


void COption_LogoDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionRecordPath();
	CString strTemp;
	int Num = 0;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "LOGOENABLE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
		{
			m_LogoChkBtn.SetCheck(1);
			m_LogoBrowBtn.EnableWindow();

			memset(szCheck, 0, MAX_PATH );
			DWORD dwBufLen = MAX_PATH;
			RegQueryValueEx( hKey, "LOGOPATH", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
			strTemp.Format("%s", szCheck);
			m_LogoEditor.SetWindowText(strTemp);
		}
		else
			m_LogoBrowBtn.EnableWindow(FALSE);		

		RegCloseKey( hKey );
	}
	else
	{
		m_LogoChkBtn.SetCheck(0);
		m_LogoBrowBtn.EnableWindow(FALSE);
	}

}

bool COption_LogoDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionRecordPath();
	CString strTemp;
	int Num = 0;

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

	strTemp = "FALSE";
	if(m_LogoChkBtn.GetCheck())
	{
		strTemp = "TRUE";
		RegSetValueEx(hKey, "LOGOENABLE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

		m_LogoEditor.GetWindowText(strTemp);
		RegSetValueEx(hKey, "LOGOPATH", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	}
	else
	{
		RegSetValueEx(hKey, "LOGOENABLE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

		strTemp = "";		
		RegSetValueEx(hKey, "LOGOPATH", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	}

	RegCloseKey( hKey );

	return true;

}

BOOL COption_LogoDlg::PreTranslateMessage(MSG* pMsg) 
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
