// Option_PenFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_PenFolderDlg.h"
#include "AppSettings.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_PenFolderDlg dialog


COption_PenFolderDlg::COption_PenFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_PenFolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_PenFolderDlg)
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_PenFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_PenFolderDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_PenFolderDlg, CDialog)
	//{{AFX_MSG_MAP(COption_PenFolderDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_PATTERN1BTN, OnOptPattern1btn)
	ON_BN_CLICKED(IDC_OPT_PATTERN2BTN, OnOptPattern2btn)
	ON_BN_CLICKED(IDC_OPT_PATTERN3BTN, OnOptPattern3btn)
	ON_BN_CLICKED(IDC_OPT_PATTERN4BTN, OnOptPattern4btn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_PenFolderDlg message handlers

HBRUSH COption_PenFolderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_PenFolderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	ReadOption();
	m_bModifyFlag = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_PenFolderDlg::OnOptPattern1btn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_PenFolderDlg::OnOptPattern2btn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_PenFolderDlg::OnOptPattern3btn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_PenFolderDlg::OnOptPattern4btn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

/////////////////////////////////////////////////////////////////////////////
//
void COption_PenFolderDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionPenPath();
	CString strTemp;
	int Num = 0;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "PENPATTERN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);		
		if(Num == 0)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN1BTN);
			pButton->SetCheck(1);
		}
		else
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN2BTN);
			pButton->SetCheck(1);
		}

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "LINEPATTERN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		if(Num == 0)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN3BTN);
			pButton->SetCheck(1);
		}
		else
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN4BTN);
			pButton->SetCheck(1);
		}		

		RegCloseKey( hKey );
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN1BTN);
		pButton->SetCheck(1);

		pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN3BTN);
		pButton->SetCheck(1);		
	}
}

bool COption_PenFolderDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionPenPath();
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

	CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN2BTN);
	if(pButton->GetCheck())
		Num = 1;
	strTemp.Format("%d", Num);
	RegSetValueEx(hKey, "PENPATTERN", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	Num = 0;
	pButton = (CButton*)GetDlgItem(IDC_OPT_PATTERN4BTN);
	if(pButton->GetCheck())
		Num = 1;
	strTemp.Format("%d", Num);
	RegSetValueEx(hKey, "LINEPATTERN", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}

BOOL COption_PenFolderDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_PenFolderDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("형광펜 패턴 설정에서 [ I ] 패턴을 선택하시면 실제와 비슷한 형광펜을"
				"\r\n사용하실 수 있습니다."
				"\r\n\r\n형광펜 패턴 설정에서 [O] 패턴을 선택하시면 마우스에 적합한 형광펜을"
				"\r\n사용하실 수 있습니다."
				"\r\n\r\n선그리기 설정에서 화살표를 선택하시면 도형 그리기 메뉴의 직선을"
				"\r\n화살표로 사용하실 수 있습니다.",
				CRect(10, 140, 406, 265), DT_LEFT);
	
	dc.SelectObject(pOldFont);
	Font.DeleteObject();	

	// Do not call CDialog::OnPaint() for painting messages
}
