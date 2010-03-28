// Option_SnapshotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_SnapshotDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_SnapshotDlg dialog


COption_SnapshotDlg::COption_SnapshotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_SnapshotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_SnapshotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_SnapshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_SnapshotDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_SnapshotDlg, CDialog)
	//{{AFX_MSG_MAP(COption_SnapshotDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_SNAPSHOTFULL, OnOptSnapshotfull)
	ON_BN_CLICKED(IDC_OPT_SNAPSHOTRANGE, OnOptSnapshotrange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_SnapshotDlg message handlers

BOOL COption_SnapshotDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadOption();
	m_bModifyFlag = false;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COption_SnapshotDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("툴바에서 스냅샷 기능을 사용하실 때, 전체화면 스냅샷으로 설정할"
			    "\r\n것인지 또는 선택영역 스냅샷으로 설정하여 사용할 것인지를 선택할"
				"\r\n수 있습니다."
				"\r\n\r\n선택영역 스냅샷을 적용하시면 화면상에서 원하는 부분을 마우스로"
				"\r\n선택하여 캡쳐할 수 있습니다.",
				CRect(10, 130, 406, 260), DT_LEFT);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();
}


HBRUSH COption_SnapshotDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_SnapshotDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_SnapshotDlg::OnOptSnapshotfull() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_SnapshotDlg::OnOptSnapshotrange() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}


///////////////////////////////////////////////////////////////////////////////
//
void COption_SnapshotDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionSnapshotPath();
	CString strTemp;	

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SNAPSHOTMODE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_SNAPSHOTFULL);
			pButton->SetCheck(1);
		}
		else
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_SNAPSHOTRANGE);
			pButton->SetCheck(1);
		}
	
		RegCloseKey( hKey );
	}
	else
	{		
		CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_SNAPSHOTFULL);
		pButton->SetCheck(1);
	}
}

bool COption_SnapshotDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionSnapshotPath();
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

	strTemp = "FALSE";
	CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_SNAPSHOTFULL);
	if(pButton->GetCheck())
		strTemp = "TRUE";

	RegSetValueEx(hKey, "SNAPSHOTMODE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);		

	RegCloseKey( hKey );

	return true;
}
