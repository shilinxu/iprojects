// Option_ToolbarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_ToolbarDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_ToolbarDlg dialog


COption_ToolbarDlg::COption_ToolbarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_ToolbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_ToolbarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
}


void COption_ToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_ToolbarDlg)
	DDX_Control(pDX, IDC_OPT_ZOOMCHECKBTN, m_ZoomCheckBtn);
	DDX_Control(pDX, IDC_OPT_MODECHECKBTN, m_ModeCheckBtn);
	DDX_Control(pDX, IDC_OPT_WIDTHCHECKBTN, m_WidthCheckBtn);
	DDX_Control(pDX, IDC_OPT_SNAPSHOTCHECKBTN, m_SnapshotCheckBtn);
	DDX_Control(pDX, IDC_OPT_PPTCHECKBTN, m_PPTCheckBtn);
	DDX_Control(pDX, IDC_OPT_PENCHECKBTN, m_PenCheckBtn);
	DDX_Control(pDX, IDC_OPT_HLPENCHECKBTN, m_HLPenCheckBtn);
	DDX_Control(pDX, IDC_OPT_ERASERCHECKBTN, m_EraserCheckBtn);
	DDX_Control(pDX, IDC_OPT_DIAGRAMCHECKBTN, m_DiagramCheckBtn);
	DDX_Control(pDX, IDC_OPT_CONTROLCHECKBTN, m_ControlCheckBtn);
	DDX_Control(pDX, IDC_OPT_COLORCHECKBTN, m_ColorCheckBtn);
	DDX_Control(pDX, IDC_OPT_BOARDCHECKBTN, m_BoardCheckBtn);
	DDX_Control(pDX, IDC_OPT_PRINTCHECKBTN, m_PrintCheckBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_ToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(COption_ToolbarDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_MODECHECKBTN, OnOptModecheckbtn)
	ON_BN_CLICKED(IDC_OPT_PENCHECKBTN, OnOptPencheckbtn)
	ON_BN_CLICKED(IDC_OPT_HLPENCHECKBTN, OnOptHlpencheckbtn)
	ON_BN_CLICKED(IDC_OPT_ERASERCHECKBTN, OnOptErasercheckbtn)
	ON_BN_CLICKED(IDC_OPT_COLORCHECKBTN, OnOptColorcheckbtn)
	ON_BN_CLICKED(IDC_OPT_WIDTHCHECKBTN, OnOptWidthcheckbtn)
	ON_BN_CLICKED(IDC_OPT_DIAGRAMCHECKBTN, OnOptDiagramcheckbtn)
	ON_BN_CLICKED(IDC_OPT_CONTROLCHECKBTN, OnOptControlcheckbtn)
	ON_BN_CLICKED(IDC_OPT_PPTCHECKBTN, OnOptPptcheckbtn)
	ON_BN_CLICKED(IDC_OPT_BOARDCHECKBTN, OnOptBoardcheckbtn)
	ON_BN_CLICKED(IDC_OPT_SNAPSHOTCHECKBTN, OnOptSnapshotcheckbtn)
	ON_BN_CLICKED(IDC_OPT_PRINTCHECKBTN, OnOptPrintcheckbtn)
	ON_BN_CLICKED(IDC_OPT_ZOOMCHECKBTN, OnOptZoomcheckbtn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_ToolbarDlg message handlers

HBRUSH COption_ToolbarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_ToolbarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadOption();
	m_bModifyFlag = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
void COption_ToolbarDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionToolbarPath();
	CString strTemp;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "MODE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_ModeCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PEN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_PenCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "HLPEN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_HLPenCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "ERASER", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_EraserCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "COLOR", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_ColorCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "WIDTH", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_WidthCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "DIAGRAM", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_DiagramCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "CONTROL", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_ControlCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PPT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_PPTCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "BOARD", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_BoardCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SNAPSHOT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_SnapshotCheckBtn.SetCheck(1);

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "ZOOM", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_ZoomCheckBtn.SetCheck(1);
		
		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PRINT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_PrintCheckBtn.SetCheck(1);

		RegCloseKey( hKey );
	}
}


bool COption_ToolbarDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionToolbarPath();
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
	if(m_ModeCheckBtn.GetCheck())
		strTemp = "TRUE";
	RegSetValueEx(hKey, "MODE", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	
	strTemp = "FALSE";
	if(m_PenCheckBtn.GetCheck())
		strTemp = "TRUE";
	RegSetValueEx(hKey, "PEN", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_HLPenCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "HLPEN", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_EraserCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "ERASER", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_ColorCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "COLOR", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_WidthCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "WIDTH", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_DiagramCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "DIAGRAM", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_ControlCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "CONTROL", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_PPTCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "PPT", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_BoardCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "BOARD", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_SnapshotCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "SNAPSHOT", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp = "FALSE";
	if(m_ZoomCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "ZOOM", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	
	strTemp = "FALSE";
	if(m_PrintCheckBtn.GetCheck())
		strTemp = "TRUE";	
	RegSetValueEx(hKey, "PRINT", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}

void COption_ToolbarDlg::OnOptModecheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptPencheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptHlpencheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptErasercheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptColorcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptWidthcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptDiagramcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptControlcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptPptcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptBoardcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptSnapshotcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptPrintcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

void COption_ToolbarDlg::OnOptZoomcheckbtn() 
{
	// TODO: Add your control notification handler code here
	m_bModifyFlag = true;
}

BOOL COption_ToolbarDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_ToolbarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);

	dc.DrawText("축소모드 시 툴바에서 사용할 메뉴(자주 사용하는 메뉴)를 선택 하십시오."
				"\r\n선택하신 메뉴는 툴바의 축소모드 버튼을 누르시면 적용되어 나타납니다.",
				CRect(10, 220, 406, 260), DT_LEFT);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}
