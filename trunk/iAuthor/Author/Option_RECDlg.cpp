// Option_RECDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_RECDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_RECDlg dialog


COption_RECDlg::COption_RECDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_RECDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_RECDlg)
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));

	m_strInfo = "저작도구 녹화를 체크하시면 화면과 함께 저작도구도 녹화됩니다."
				"\r\n마우스 커서 녹화는 화면과 함께 마우스의 움직임을 녹화합니다."
				"\r\n프레임 비율 설정은 PC의 속도와 그래픽 카드, 그래픽 가속 여부에"
				"\r\n따라 적절하게 조절하여 사용하십시오.";
}


void COption_RECDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_RECDlg)
	DDX_Control(pDX, IDC_OPT_TOOLRECCHECKBTN, m_ToolRecChkBtn);
	DDX_Control(pDX, IDC_OPT_QUALITYCOMBO, m_QualityCombo);
	DDX_Control(pDX, IDC_OPT_FPSCOMBO, m_FpsCombo);
	DDX_Control(pDX, IDC_OPT_CURSORCOMBO, m_CursorCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_RECDlg, CDialog)
	//{{AFX_MSG_MAP(COption_RECDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_OPT_CURSORCOMBO, OnSelchangeOptCursorcombo)
	ON_CBN_SELCHANGE(IDC_OPT_FPSCOMBO, OnSelchangeOptFpscombo)
	ON_CBN_SELCHANGE(IDC_OPT_QUALITYCOMBO, OnSelchangeOptQualitycombo)
	ON_BN_CLICKED(IDC_OPT_TOOLRECCHECKBTN, OnOptToolreccheckbtn)
	ON_BN_CLICKED(IDC_EXCLUSIVE_RECMODE, OnExclusiveRecmode)
	ON_BN_CLICKED(IDC_NORMAL_RECMODE, OnNormalRecmode)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_RECDlg message handlers

HBRUSH COption_RECDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_RECDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bReg = CAppSettings::IsRegistration();
	if(!m_bReg)
		m_ToolRecChkBtn.EnableWindow(FALSE);
	else
	{
		if(!CAppSettings::IsLayeredWndEnable())
		{
			m_ToolRecChkBtn.EnableWindow(FALSE);
			m_ToolRecChkBtn.SetCheck(1);
		}
	}
	
	ReadOption();
	m_bModifyFlag = false;

	int nLevel = GetAccelerationLevel();

	if(nLevel == 5)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_EXCLUSIVE_RECMODE);
		pButton->SetCheck(1);
	}
	else 
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_NORMAL_RECMODE);
		pButton->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int COption_RECDlg::GetAccelerationLevel()
{
	HKEY hk;
	BYTE buff[1024];
	DWORD buff_siz=1024;
	DWORD curr_level;
	LONG ret;

	
	// Find out the position of the current video device, information on this may be found
	// in the registry, here: "\HKLM\HARDWARE\DEVICEMAP\VIDEO\Device\Video0"
	// There you will find a LPSTR type of entry, pointing to a registry key for the current
	// video device, e.g. "\Registry\Machine\System\ControlSet001\Services\G400\Device0"
	//
	buff_siz = sizeof(buff);
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\VIDEO"), 0, KEY_ALL_ACCESS, &hk);
	ret = RegQueryValueEx(hk, _T("\\Device\\Video0"), 0, 0, buff, &buff_siz) != ERROR_SUCCESS;
	RegCloseKey(hk);
	if (ret != ERROR_SUCCESS || buff_siz == 0)
	{
		return 0;	// If the value does not exists, hardware acceleration is set to "Full"
	}
	else
	{
		buff[buff_siz] = 0;
		buff[1023] = 0;
	}


	// Parse that string, replacing "\Registry\Machine" with HKLM (HKEY_LOCAL_MACHINE)
	// and open the corresponding registry key
	//
	CString videoKey = buff;
	if(videoKey.Replace(_T("\\Registry\\Machine\\"), _T("")) == 0)
		videoKey.Replace(_T("\\REGISTRY\\Machine\\"), _T(""));


	// Look for a DWORD value "Acceleration.Level".
	// If the value does not exists, hardware acceleration is set to "Full".
	// Otherwise, "Acceleration.Level" may be one of 0x1 to 0x5, 0x5 meaning "No Acceleration",
	// and 0x1 meaning "All but cursor acceleration" (see "Extended Display Settings").
	//
	// Set the desired acceleration level, or delete the entry to set "Full" acceleration mode.
	buff_siz = sizeof(curr_level);
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR) videoKey, 0, KEY_ALL_ACCESS, &hk);
	ret = RegQueryValueEx(hk, _T("Acceleration.Level"), 0, 0, (LPBYTE) &curr_level, &buff_siz) != ERROR_SUCCESS;
	RegCloseKey(hk);
	if (ret != ERROR_SUCCESS && buff_siz == 0)
	{
		return 0;	// If the value does not exists, hardware acceleration is set to "Full"
	}
	if (ret != ERROR_SUCCESS || buff_siz == 0)
	{
		return -1;
	}
	
	return curr_level;
}


void COption_RECDlg::OnOptToolreccheckbtn() 
{
	// TODO: Add your control notification handler code here

	m_strInfo = "저작도구 녹화를 체크하시면 화면과 함께저 작도구도 녹화됩니다."
				"\r\n마우스 커서 녹화는 화면과 함께 마우스의 움직임을 녹화합니다."
				"\r\n프레임 비율 설정은 PC의 속도와 그래픽 카드, 그래픽 가속"
				"\r\n여부에 따라 적절하게 조절하여 사용하십시오.";

	InvalidateRect(CRect(10, 205, 406, 265));

	m_bModifyFlag = true;	
}

void COption_RECDlg::OnExclusiveRecmode() 
{
	// TODO: Add your control notification handler code here
	SetModeChange(false);

	m_strInfo = "품질은 설정은 각 프레임의 품질을 결정하는 요소로 느린 PC일수록"
				"\r\n낮게 설정하는 것이 좋습니다. 동영상 전용모드 녹화는 동영상 처리를"
				"\r\n제대로 수행하기 위해 비디오 가속을 자동으로 설정해줍니다.";

	InvalidateRect(CRect(10, 205, 406, 265));

}

void COption_RECDlg::OnNormalRecmode() 
{
	// TODO: Add your control notification handler code here
	SetModeChange(true);

	m_strInfo = "품질은 설정은 각 프레임의 품질을 결정하는 요소로 느린 PC일수록"
				"\r\n낮게 설정하는 것이 좋습니다. 동영상 전용모드 녹화는 동영상 처리를"
				"\r\n제대로 수행하기 위해 비디오 가속을 자동으로 설정해줍니다.";

	InvalidateRect(CRect(10, 205, 406, 265));
}


void COption_RECDlg::OnSelchangeOptCursorcombo() 
{
	// TODO: Add your control notification handler code here
	m_strInfo = "저작도구 녹화를 체크하시면 화면과 함께저 작도구도 녹화됩니다."
				"\r\n마우스 커서 녹화는 화면과 함께 마우스의 움직임을 녹화합니다."
				"\r\n프레임 비율 설정은 PC의 속도와 그래픽 카드, 그래픽 가속"
				"\r\n여부에 따라 적절하게 조절하여 사용하십시오.";

	InvalidateRect(CRect(10, 205, 406, 265));

	m_bModifyFlag = true;
}

void COption_RECDlg::OnSelchangeOptFpscombo() 
{
	// TODO: Add your control notification handler code here
	m_strInfo = "저작도구 녹화를 체크하시면 화면과 함께저 작도구도 녹화됩니다."
				"\r\n마우스 커서 녹화는 화면과 함께 마우스의 움직임을 녹화합니다."
				"\r\n프레임 비율 설정은 PC의 속도와 그래픽 카드, 그래픽 가속"
				"\r\n여부에 따라 적절하게 조절하여 사용하십시오.";

	InvalidateRect(CRect(10, 205, 406, 265));

	m_bModifyFlag = true;
}

void COption_RECDlg::OnSelchangeOptQualitycombo() 
{
	// TODO: Add your control notification handler code here

	m_strInfo = "품질은 설정은 각 프레임의 품질을 결정하는 요소로 느린 PC일수록"
				"\r\n낮게 설정하는 것이 좋습니다. 동영상 전용모드 녹화는 동영상 처리를"
				"\r\n제대로 수행하기 위해 비디오 가속을 자동으로 설정해줍니다.";

	InvalidateRect(CRect(10, 205, 406, 265));

	m_bModifyFlag = true;
}

/////////////////////////////////////////////////////////////////////////////
//

void COption_RECDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionRecordPath();
	CString strTemp;
	int Num = 0;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{			
		if(CAppSettings::IsLayeredWndEnable())
		{
			memset(szCheck, 0, MAX_PATH );
			dwBufLen = MAX_PATH;
			RegQueryValueEx( hKey, "TOOLREC", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
			strTemp.Format("%s", szCheck);
			if(strTemp == "TRUE")
				m_ToolRecChkBtn.SetCheck(1);
		}

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "CURSOR", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		m_CursorCombo.SetCurSel(Num);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "FPS", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		m_FpsCombo.SetCurSel(Num);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "QUALITY", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		m_QualityCombo.SetCurSel(Num);

		RegCloseKey( hKey );
	}
	else
	{
		if(!CAppSettings::IsLayeredWndEnable())
			m_ToolRecChkBtn.SetCheck(1);

		m_CursorCombo.SetCurSel(1);
		m_FpsCombo.SetCurSel(5);
		m_QualityCombo.SetCurSel(0);
	}
}

bool COption_RECDlg::WriteOption()
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
	if(m_ToolRecChkBtn.GetCheck())
		strTemp = "TRUE";
	RegSetValueEx(hKey, "TOOLREC", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
		
	Num = m_CursorCombo.GetCurSel();
	strTemp.Format("%d", Num);
	RegSetValueEx(hKey, "CURSOR", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	Num = m_FpsCombo.GetCurSel();
	strTemp.Format("%d", Num);
	RegSetValueEx(hKey, "FPS", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	Num = m_QualityCombo.GetCurSel();
	strTemp.Format("%d", Num);
	RegSetValueEx(hKey, "QUALITY", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}

BOOL COption_RECDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_RECDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText(m_strInfo, CRect(10, 205, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject();	
	// Do not call CDialog::OnPaint() for painting messages
}

void COption_RECDlg::SetModeChange(bool bAcceleration)
{
	HKEY hk;
	BYTE buff[1024];
	DWORD buff_siz=1024;
	LONG ret;

	int	level;
	if(bAcceleration)
		level = 0;
	else
		level = 5;

	// Find out the position of the current video device, information on this may be found
	// in the registry, here: "\HKLM\HARDWARE\DEVICEMAP\VIDEO\Device\Video0"
	// There you will find a LPSTR type of entry, pointing to a registry key for the current
	// video device, e.g. "\Registry\Machine\System\ControlSet001\Services\G400\Device0"
	//
	buff_siz = sizeof(buff);
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\VIDEO"), 0, KEY_ALL_ACCESS, &hk);
	ret = RegQueryValueEx(hk, _T("\\Device\\Video0"), 0, 0, buff, &buff_siz) != ERROR_SUCCESS;
	RegCloseKey(hk);
	if (ret != ERROR_SUCCESS || buff_siz == 0)
		return;
	else
	{
		buff[buff_siz] = 0;
		buff[1023] = 0;
	}

	// Parse that string, replacing "\Registry\Machine" with HKLM (HKEY_LOCAL_MACHINE)
	// and open the corresponding registry key
	//
	CString videoKey = buff;
	

	if(videoKey.Replace(_T("\\Registry\\Machine\\"), _T("")) == 0)
		videoKey.Replace(_T("\\REGISTRY\\Machine\\"), _T(""));
	
	// Look for a DWORD value "Acceleration.Level".
	// If the value does not exists, hardware acceleration is set to "Full".
	// Otherwise, "Acceleration.Level" may be one of 0x1 to 0x5, 0x5 meaning "No Acceleration",
	// and 0x1 meaning "All but cursor acceleration" (see "Extended Display Settings").
	//
	// Set the desired acceleration level, or delete the entry to set "Full" acceleration mode.
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR) videoKey, 0, KEY_ALL_ACCESS, &hk);
	ret = RegSetValueEx(hk, _T("Acceleration.Level"), 0, REG_DWORD, (CONST BYTE *) &level, 4) != ERROR_SUCCESS;
	RegCloseKey(hk);
	if (ret != ERROR_SUCCESS || buff_siz == 0)
		return;
	
	// Let Windows reload the display settings.
	//
	DEVMODE devMode;
	BOOL success = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	if (success == TRUE)// && g_LatestDevMode != devMode)
	{
	  LONG result = ::ChangeDisplaySettings(&devMode, CDS_RESET);
	  //check(result == DISP_CHANGE_SUCCESSFUL);
	}
}
