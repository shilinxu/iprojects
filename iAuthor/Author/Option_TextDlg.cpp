// Option_TextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_TextDlg.h"
#include "AppSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_TextDlg dialog


COption_TextDlg::COption_TextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_TextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_TextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brush.CreateSolidBrush(RGB(203,202,206));
	m_bModifyFlag = false;
}


void COption_TextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_TextDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_TextDlg, CDialog)
	//{{AFX_MSG_MAP(COption_TextDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_TextDlg message handlers

BOOL COption_TextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_rcColor[0].SetRect(40, 78, 73, 106);
	m_rcColor[1].SetRect(83, 78, 116, 106);
	m_rcColor[2].SetRect(126, 78, 159, 106);
	m_rcColor[3].SetRect(169, 78, 202, 106);
	m_rcColor[4].SetRect(212, 78, 245, 106);
	m_rcColor[5].SetRect(255, 78, 288, 106);
	m_rcColor[6].SetRect(297, 78, 331, 106);
	m_rcColor[7].SetRect(341, 78, 374, 106);
	m_rcColor[8].SetRect(40, 118, 73, 146);
	m_rcColor[9].SetRect(83, 118, 116, 146);
	m_rcColor[10].SetRect(126, 118, 159, 146);
	m_rcColor[11].SetRect(169, 118, 202, 146);
	m_rcColor[12].SetRect(212, 118, 245, 146);
	m_rcColor[13].SetRect(255, 118, 288, 146);
	m_rcColor[14].SetRect(297, 118, 331, 146);
	m_rcColor[15].SetRect(341, 118, 374, 146);

	m_EnumColor[0] = RGB(0xFF, 0xFF, 0xFF);
	m_EnumColor[1] = RGB(0x80, 0x80, 0x80);
	m_EnumColor[2] = RGB(0x00, 0x00, 0x00);
	m_EnumColor[3] = RGB(0xFF, 0x00, 0x00);
	m_EnumColor[4] = RGB(0xFF, 0x7D, 0x00);
	m_EnumColor[5] = RGB(0xFF, 0x80, 0x80);
	m_EnumColor[6] = RGB(0xFF, 0xFF, 0x00);
	m_EnumColor[7] = RGB(0x80, 0xFF, 0x80);
	m_EnumColor[8] = RGB(0x00, 0xFF, 0x00);
	m_EnumColor[9] = RGB(0x00, 0xFF, 0xFF);
	m_EnumColor[10] = RGB(0x00, 0x00, 0xFF);
	m_EnumColor[11] = RGB(0x80, 0x00, 0x80);
	m_EnumColor[12] = RGB(0xFF, 0x00, 0xFF);
	m_EnumColor[13] = RGB(0x80, 0x80, 0xFF);
	m_EnumColor[14] = RGB(0x80, 0x80, 0x00);
	m_EnumColor[15] = RGB(0x00, 0x80, 0x80);

	ReadOption();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_TextDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	

	CRect rc;
	for(int i = 0; i < 16; i++)
	{
		dc.FillSolidRect(m_rcColor[i], m_EnumColor[i]);
		rc = m_rcColor[i];

		if(i==m_nSelNum)
		{	
			rc.InflateRect(1,1,1,1);
			dc.Draw3dRect(&rc, RGB(255,255,255), RGB(50,50,50));
			rc.DeflateRect(1,1,1,1);
			dc.Draw3dRect(&rc, RGB(50,50,50), RGB(255,255,255));	
			rc.DeflateRect(1,1,1,1);
			dc.Draw3dRect(&rc, RGB(255,255,255), RGB(50,50,50));
			rc.DeflateRect(1,1,1,1);
			dc.Draw3dRect(&rc, RGB(255,255,255), RGB(50,50,50));
		}
		else
			dc.Draw3dRect(&rc, RGB(200,200,200), RGB(50,50,50));
	}

	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("텍스트 배경색을 선택하시면 새로 만드는 텍스트 창에 적용됩니다."
				"\r\n텍스트 색상은 현재 저작도구에서 선택된 펜 색상이며 글씨 크기는"
				"\r\n저작도구에서 선택된 크기로 작성됩니다.",
				CRect(10, 200, 406, 260), DT_LEFT);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH COption_TextDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

BOOL COption_TextDlg::PreTranslateMessage(MSG* pMsg) 
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

void COption_TextDlg::ReadOption()
{
	CString REG_KEY  = CAppSettings::GetOptionTextPath();
	CString strTemp;	

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SELECTNUM", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		m_nSelNum = atoi(strTemp);
		InvalidateRect(m_rcColor[m_nSelNum]);
			
		RegCloseKey( hKey );
	}
	else
	{		
		m_nSelNum = 0;
		InvalidateRect(m_rcColor[m_nSelNum]);
	}
}

bool COption_TextDlg::WriteOption()
{
	if(!m_bModifyFlag)
		return false;

	CString REG_KEY  = CAppSettings::GetOptionTextPath();
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

	strTemp.Format("%d", m_nSelNum);
	RegSetValueEx(hKey, "SELECTNUM", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);
	
	int CR, CG, CB;
	CR = GetRValue(m_EnumColor[m_nSelNum]);
	CG = GetGValue(m_EnumColor[m_nSelNum]);
	CB = GetBValue(m_EnumColor[m_nSelNum]);

	strTemp.Format("%d", CR);
	RegSetValueEx(hKey, "SELECTCOLOR_R", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp.Format("%d", CG);
	RegSetValueEx(hKey, "SELECTCOLOR_G", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	strTemp.Format("%d", CB);
	RegSetValueEx(hKey, "SELECTCOLOR_B", 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strTemp, strTemp.GetLength() + 1);

	RegCloseKey( hKey );

	return true;
}

void COption_TextDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	for(int i=0; i<16; i++)
	{
		if(m_rcColor[i].PtInRect(point))
		{
			if(m_nSelNum != i)
			{
				CRect rc = m_rcColor[m_nSelNum];
				rc.InflateRect(2,2,2,2);
				InvalidateRect(rc);
				m_nSelNum = i;
				rc = m_rcColor[m_nSelNum];
				rc.InflateRect(2,2,2,2);
				InvalidateRect(rc);

				m_bModifyFlag = true;
			}
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}
