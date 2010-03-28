// WmvConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "WmvConvertDlg.h"
#include "AppSettings.h"

#include <Vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		COMPLETE_RENDERING_TIMER	1
#define		ERROR_RENDERING_TIMER		2
#define		USERABORT_RENDERING_TIMER	3
/////////////////////////////////////////////////////////////////////////////
// CWmvConvertDlg dialog


CWmvConvertDlg::CWmvConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWmvConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWmvConvertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_rcProgress.left = 25;
	m_rcProgress.right = 185;
	m_rcProgress.top = 150;
	m_rcProgress.bottom = 162;

	m_nCurPos = 0;
	m_nMaxRange = 0;
	m_AviDuration = 0.0;

	m_nCurrentStep = 1;

	m_hWmvGen = NULL;
}


void CWmvConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWmvConvertDlg)
	DDX_Control(pDX, IDC_WMVPATHBROWSER, m_WMVPathBrowseBtn);
	DDX_Control(pDX, IDC_WMVCOVPROGRESSBAR, m_RenderingProgressbar);
	DDX_Control(pDX, IDC_WMVCLOSEBTN, m_CloseBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDC_WMVCONV_NEXTBTN, m_NextBtn);
	DDX_Control(pDX, IDC_PREVIEWWMVBTN, m_PreviewMovieBtn);	
	DDX_Control(pDX, IDC_AVIPATHBROWSER, m_AVIPathBrowseBtn);
	DDX_Control(pDX, IDC_WMVPATH_EDIT, m_WMVPathEditor);
	DDX_Control(pDX, IDC_AVIPATH_EDIT, m_AVIPathEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWmvConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CWmvConvertDlg)
	ON_BN_CLICKED(IDC_WMVCONV_NEXTBTN, OnWmvconvNextbtn)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_WMVCLOSEBTN, OnWmvclosebtn)
	ON_BN_CLICKED(IDC_AVIPATHBROWSER, OnAvipathbrowser)
	ON_BN_CLICKED(IDC_WMVPATHBROWSER, OnWmvpathbrowser)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWmvConvertDlg message handlers

BOOL CWmvConvertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_RenderingProgressbar.ShowWindow(SW_HIDE);	
	m_OKBtn.ShowWindow(SW_HIDE);	
	m_PreviewMovieBtn.ShowWindow(SW_HIDE);

	m_strProfilePath.Format(_T("%s\\profile.prx"), CAppSettings::GetTempPath());
	m_AVIPathEditor.SetWindowText(m_strAviPath);

	m_CloseBtn.DrawBorder(FALSE);
	m_CloseBtn.SetBitmaps(IDB_WMVCLOSEBTN, RGB(255,0,255));	
	m_CloseBtn.DrawTransparent();

	m_CancelBtn.DrawBorder(FALSE);
	m_CancelBtn.SetBitmaps(IDB_WMVCANCELBTN, RGB(255,0,255));	
	m_CancelBtn.DrawTransparent();
	
	m_OKBtn.DrawBorder(FALSE);
	m_OKBtn.SetBitmaps(IDB_WMVFINISHBTN, RGB(255,0,255));	
	m_OKBtn.DrawTransparent();
	
	m_NextBtn.DrawBorder(FALSE);
	m_NextBtn.SetBitmaps(IDB_WMVNEXTBTN, RGB(255,0,255));	
	m_NextBtn.DrawTransparent();

	m_PreviewMovieBtn.DrawBorder(FALSE);
	m_PreviewMovieBtn.SetCheckButton();
	m_PreviewMovieBtn.SetBitmaps(IDB_WMVPREVIEWBTN, RGB(255,0,255));	
	m_PreviewMovieBtn.DrawTransparent();

	m_AVIPathBrowseBtn.DrawBorder(FALSE);
	m_AVIPathBrowseBtn.SetBitmaps(IDB_WMVBROWSERBTN, RGB(255,0,255));	
	m_AVIPathBrowseBtn.DrawTransparent();

	m_WMVPathBrowseBtn.DrawBorder(FALSE);
	m_WMVPathBrowseBtn.SetBitmaps(IDB_WMVBROWSERBTN, RGB(255,0,255));	
	m_WMVPathBrowseBtn.DrawTransparent();
	
	ArrangeControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWmvConvertDlg::ArrangeControl()
{
	m_CloseBtn.MoveWindow(CRect(531, 4, 546, 19));
	m_CancelBtn.MoveWindow(CRect(433, 246, 530, 270));
	m_OKBtn.MoveWindow(CRect(333, 246, 430, 270));
	m_NextBtn.MoveWindow(CRect(333, 246, 430, 270));
	m_PreviewMovieBtn.MoveWindow(CRect(40, 115, 290, 140));
	m_AVIPathBrowseBtn.MoveWindow(CRect(433, 118, 530, 143));
	m_WMVPathBrowseBtn.MoveWindow(CRect(433, 178, 530, 203));

	CRect rc;
	GetWindowRect(&rc);
	int nWidth, nHeight, cx, cy;
	nWidth = rc.Width();
	nHeight = rc.Height();
	cx = ::GetSystemMetrics(SM_CXSCREEN);
	cy = ::GetSystemMetrics(SM_CYSCREEN);

	rc.left = (cx-nWidth)/2;
	rc.right = rc.left + nWidth;
	rc.top = (cy - nHeight -160)/2;
	rc.bottom = rc.top + nHeight;

	MoveWindow(&rc);

	m_CloseBtn.MakeRgn();
	m_CancelBtn.MakeRgn();
	m_OKBtn.MakeRgn();
	m_NextBtn.MakeRgn();	
	m_PreviewMovieBtn.MakeRgn();
	m_AVIPathBrowseBtn.MakeRgn();	
	m_WMVPathBrowseBtn.MakeRgn();
}



void CWmvConvertDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_WMVCONVBG);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();

	CFont fontMain, fontSub, *oldFont;
	fontMain.CreateFont(14,0,0,0,700,FALSE,FALSE,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));

	fontSub.CreateFont(14,0,0,0,FW_NORMAL,FALSE,FALSE,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));

	oldFont = dc.SelectObject(&fontMain);
	
	CRect rcTitle, rcTitle1;
	rcTitle = CRect(36, 36, 250, 50);
	rcTitle1 = CRect(55, 56, 350, 70);

	CString strTemp;

	dc.SetBkMode(TRANSPARENT);
	switch(m_nCurrentStep)
	{
	case	1:		
		strTemp = "동영상 정보";		
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);
		
		dc.SelectObject(oldFont);
		oldFont = dc.SelectObject(&fontSub);
		strTemp = "동영상 파일의 정보를 입력하세요.";
		dc.DrawText(strTemp, &rcTitle1, DT_LEFT);

		rcTitle.left = 25;
		rcTitle.top = 98;
		rcTitle.right = 500;
		rcTitle.bottom = 112;
		strTemp = "1. AVI동영상 파일의 위치";
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);

		rcTitle.top = 162;
		rcTitle.bottom = 176;
		strTemp = "2. WMV동영상의 저장 위치를 선택합니다.";
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);
		break;
	case	2:
		strTemp = "동영상 변환";		
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);
	
		dc.SelectObject(oldFont);
		oldFont = dc.SelectObject(&fontSub);
		strTemp = "AVI동영상을 WMV동영상으로 변환 중 입니다.";
		dc.DrawText(strTemp, &rcTitle1, DT_LEFT);

		rcTitle.left = 25;
		rcTitle.top = 98;
		rcTitle.right = 500;
		rcTitle.bottom = 112;
		strTemp = "동영상 변환 중 ...";
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);

		if(m_nCurPos == 0)
			strTemp = "남은 시간 계산중 ...";
		else
			strTemp.Format("동영상 변환 진행률 %d%s...", m_nCurPos, "%");

		dc.DrawText(strTemp, &m_rcProgress, DT_LEFT);

		rcTitle.top = 175;
		rcTitle.bottom = 189;
		strTemp.Format("원본 파일 : %s", m_strAviPath);
		dc.DrawText(strTemp, &rcTitle, DT_LEFT|DT_PATH_ELLIPSIS);

		rcTitle.top = 200;
		rcTitle.bottom = 214;
		strTemp.Format("출력 파일 : %s", m_strWmvPath);
		dc.DrawText(strTemp, &rcTitle, DT_LEFT|DT_PATH_ELLIPSIS);
		break;
	case	3:
		strTemp = "동영상 변환 완료";		
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);
	
		dc.SelectObject(oldFont);
		oldFont = dc.SelectObject(&fontSub);
		strTemp = "동영상 변환을 성공적으로 완료하였습니다.";
		dc.DrawText(strTemp, &rcTitle1, DT_LEFT);
	
		rcTitle.left = 25;		
		rcTitle.right = 500;		
		rcTitle.top = 98;
		rcTitle.bottom = 112;
		strTemp = "- 옵션";
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);

		rcTitle.left = 25;		
		rcTitle.right = 500;		
		rcTitle.top = 200;
		rcTitle.bottom = 214;
		strTemp = "- 동영상 변환 마법사를 끝내려면 [마침]을 클릭하십시오.";
		dc.DrawText(strTemp, &rcTitle, DT_LEFT);
		break;
	}

	fontMain.DeleteObject();
	fontSub.DeleteObject();

	
	// Do not call CDialog::OnPaint() for painting messages
}



BOOL CWmvConvertDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}


/////////////////////////////////////////////////////////////////////////////
//
void CWmvConvertDlg::OnWmvconvNextbtn() 
{
	// TODO: Add your control notification handler code here
	
	CString strTemp;

	m_AVIPathEditor.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
	{
		AfxMessageBox("AVI동영상의 파일 위치를 입력하십시오.");
		m_AVIPathEditor.SetSel(0,-1);
		return;
	}
	else
	{	
		if(!IsFile(strTemp))
		{
			AfxMessageBox("존재하지 않는 AVI파일이거나 현재 다른 프로그램에서 사용 중입니다.");
			return;
		}
	}

	m_WMVPathEditor.GetWindowText(strTemp);	
	if(strTemp.IsEmpty())
	{
		AfxMessageBox("WMV동영상의 저장 위치를 선택하십시오.");
		m_WMVPathEditor.SetSel(0,-1);
		return;
	}

	if(!GetAVILength(m_strAviPath))
		return;

	m_nCurrentStep++;
	
	m_CloseBtn.EnableWindow(FALSE);
	m_AVIPathEditor.ShowWindow(SW_HIDE);	
	m_WMVPathEditor.ShowWindow(SW_HIDE);	
	m_AVIPathBrowseBtn.ShowWindow(SW_HIDE);	
	m_WMVPathBrowseBtn.ShowWindow(SW_HIDE);	
	m_RenderingProgressbar.ShowWindow(SW_SHOW);	
	
	m_NextBtn.ShowWindow(SW_HIDE);	
	m_OKBtn.ShowWindow(SW_HIDE);	

	Invalidate();
	UpdateWindow();	

	m_nMaxRange = (int)m_AviDuration;
	m_RenderingProgressbar.SetRange(0,m_nMaxRange);

	int nWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nHeight = ::GetSystemMetrics(SM_CYSCREEN);		

	if(!m_ProfileWriter.WriteVBRProfile(m_strProfilePath, nWidth, nHeight))
	{
		AfxMessageBox("에러: WMV프로파일 생성에 실패하였습니다.", MB_OK|MB_ICONERROR);
		return;
	}

	if(SetReadyRendering())
	{
		if(!wgenStartRender(m_hWmvGen))
			AfxMessageBox("Rendering Start Error");
	}	
}


void CWmvConvertDlg::OnAvipathbrowser() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = "AVI Files (.avi)|*.avi||";
	CFileDialog OpenFileDialog (TRUE, "avi", "",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter);

	OpenFileDialog.m_ofn.lpstrTitle = "Open AVI File";

	if(OpenFileDialog.DoModal() == IDOK)
	{
		m_strAviPath = OpenFileDialog.GetPathName();		
		m_AVIPathEditor.SetWindowText(m_strAviPath);
			
		if(m_strWmvPath.IsEmpty())
		{
			m_strWmvPath = m_strAviPath;
			int nCount = m_strWmvPath.ReverseFind('.');
			m_strWmvPath.Delete(nCount+1, 3);
			m_strWmvPath += "wmv";

			m_WMVPathEditor.SetWindowText(m_strWmvPath);
		}
	}
}


void CWmvConvertDlg::OnWmvpathbrowser() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = "WMV Files (.wmv)|*.wmv||";
	CFileDialog SaveFileDialog (FALSE, "wmv", "",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter);

	SaveFileDialog.m_ofn.lpstrTitle = "Save WMV File";
	if(!m_strWmvPath.IsEmpty())
		SaveFileDialog.m_ofn.lpstrInitialDir = m_strWmvPath;

	if(SaveFileDialog.DoModal() == IDOK)
	{
		m_strWmvPath = SaveFileDialog.GetPathName();		
		m_WMVPathEditor.SetWindowText(m_strWmvPath);	
	}
}

void CWmvConvertDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_PreviewMovieBtn.IsSelected())
		ShellExecute(NULL, "open", m_strWmvPath, NULL, NULL, SW_SHOWNORMAL);


	CDialog::OnOK();
}

void CWmvConvertDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_nCurrentStep == 2)
	{
		if(MessageBox("동영상 변환이 완료되지 않았습니다. 동영상 변환을 중지 하시겠습니까?",
			"동영상 변환 마법사", MB_YESNO| MB_ICONQUESTION) == IDNO)
			return;

		wgenStopRender(m_hWmvGen);
		SetTimer(USERABORT_RENDERING_TIMER, 1000, NULL);

		return;
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
bool CWmvConvertDlg::IsFile(CString strPath)
{
	bool bFile = false;
	DWORD dwFileSize = 0;
	HANDLE hFile = ::CreateFile(strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = ::GetFileSize(hFile, NULL);
		if(dwFileSize > 0)
			bFile = true;
	}

	::CloseHandle(hFile);

	return bFile;
}


bool CWmvConvertDlg::GetAVILength(CString strPath)
{
	PAVIFILE	pfile;
	AVIFILEINFO aviFileInfo;
	DWORD size = sizeof(aviFileInfo);
	HRESULT hr;

	::AVIFileInit();
	hr = ::AVIFileOpen(&pfile, strPath, OF_READ, NULL);
	if(hr != AVIERR_OK)
	{
		AfxMessageBox("AVI Open Error!");
		return false;
	}

	hr = ::AVIFileInfo(pfile, &aviFileInfo, size);
	if(hr != AVIERR_OK)
	{
		AfxMessageBox("Get AVI Info  Error!");
		return false;
	}

	m_AviRate = aviFileInfo.dwRate;
	m_AviScale = aviFileInfo.dwScale;
	m_AviLength = aviFileInfo.dwLength;
	m_AviFrame = (int)((double)m_AviRate/m_AviScale + 0.1);
	
	::AVIFileExit();

	m_AviDuration = (double)m_AviLength/(double)m_AviFrame;

	return true;
}


void CWmvConvertDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{	
	case	COMPLETE_RENDERING_TIMER:
		KillTimer(COMPLETE_RENDERING_TIMER);
		CloseRendering();		
		break;
	case	ERROR_RENDERING_TIMER:
		KillTimer(ERROR_RENDERING_TIMER);
		CloseRendering();
		AfxMessageBox("에러: 시스템 이상으로 동영상 변환작업이 취소되었습니다.", MB_OK|MB_ICONERROR);
		break;
	case	USERABORT_RENDERING_TIMER:	
		KillTimer(USERABORT_RENDERING_TIMER);
		if(m_hWmvGen)
		{
			CloseRendering();
			AfxMessageBox("정보 : 사용자에 의해 동영상 변환작업을 중단합니다.", MB_OK);
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


/////////////////////////////////////////////////////////////////////////////
//

BOOL CALLBACK RenderingProgress(double rtCurrnet, VOID * pData )
{	
	CWmvConvertDlg* pDlg = (CWmvConvertDlg*)pData;	

	if(rtCurrnet == -1.0)
		pDlg->SetTimer(COMPLETE_RENDERING_TIMER, 1000, NULL);
	else if(rtCurrnet == -3.0)
		pDlg->SetTimer(ERROR_RENDERING_TIMER, 1000, NULL);
	else if(rtCurrnet == -2.0)
		pDlg->SetTimer(USERABORT_RENDERING_TIMER, 1000, NULL);		
	else
		pDlg->SetRenderingPosition((int)rtCurrnet);

	return TRUE;
}


BOOL CALLBACK RenderingError( wgenError Error, VOID * pData )
{		
	CString strError;
	switch(Error)
	{
	case	1:			//	wgenInvalidState,
		strError = "Error : Invalid state";
		break;
	case	2:			//	wgenInvalidParam,
		strError = "Error : Invalid param(Uninitialize)";
		break;
	case	3:			//	wgenInvalidTempFolder,
		strError = "Error : Invalid TempFolder";
		break;
	case	4:			//	wgenInvalidInputFile,
		strError = "Error : Invalid AVI file path";
		break;
	case	5:			//	wgenInvalidOutputFile,
		strError = "Error : Invalid WMV file path";
		break;
	case	6:			//	wgenInvalidWmvProfileName,
		strError = "Error : Invalid Profile path";
		break;
	case	7:			//	wgenAviFileOpenFailed,
		strError = "Error : Open Avi file failed";
		break;
	case	8:			//	wgenWmvRenderFailed,
		strError = "Error : Rendering failed";
		break;
	case	9:			//	wgenCreateThreadFailed
		strError = "Error : Rendering failed";
		break;
	default:
		strError = "Unknown Error";
		break;
	}

	::MessageBox(::GetActiveWindow(), strError, "Error", MB_OK);

	return TRUE;
}

bool CWmvConvertDlg::SetReadyRendering()
{
	if(m_hWmvGen)
	{
		wgenDestroyInstance( m_hWmvGen );
		m_hWmvGen = NULL;
	}

	m_hWmvGen = wgenCreateInstance("WGENSDK00001-5638D00FCB74-96B64B349A86-CD9680064CA1");
	if(m_hWmvGen == NULL)
	{
		AfxMessageBox("Failed create Gen instance");
		return false;
	}

	BOOL hr;	
	hr = wgenSetAviFileName(m_hWmvGen, m_strAviPath);
	if(!hr)
	{
		AfxMessageBox("Error : Invalid AVI file path");
		return false;
	}
		
	hr = wgenSetWmvFileName(m_hWmvGen, m_strWmvPath);
	if(!hr)
	{
		AfxMessageBox("Error : Invalid WMV file path");
		return false;
	}	
	
	hr = wgenSetWmvProfileName(m_hWmvGen, m_strProfilePath);
	if(!hr)
	{
		AfxMessageBox("Error : Invalid Profile path");
		return false;
	}
	
	hr = wgenSetRenderRange(m_hWmvGen, 0.0, m_AviDuration);
	if(!hr)
	{
		AfxMessageBox("Error : Invalid rendering range");
		return false;
	}

	hr = wgenSetProgressCallback( m_hWmvGen, &RenderingProgress, (DWORD)this);
	if(!hr)
	{
		AfxMessageBox("Error : Setting progress callback failed");
		return false;
	}

	hr = wgenSetErrorCallback( m_hWmvGen, &RenderingError, (DWORD)this);
	if(!hr)
	{
		AfxMessageBox("Error : Setting error callback failed");
		return false;
	}

	Sleep(500);
	return true;
}

void CWmvConvertDlg::SetRenderingPosition(int nPos)
{
	m_RenderingProgressbar.SetPos(nPos);
	if(nPos == 0)
		m_nCurPos = 0;
	else
		m_nCurPos = nPos*100/m_nMaxRange;

	InvalidateRect(&m_rcProgress);
}

void CWmvConvertDlg::CloseRendering()
{
	if(m_hWmvGen)
	{
		wgenDestroyInstance( m_hWmvGen );
		m_hWmvGen = NULL;

		m_RenderingProgressbar.SetPos(0);
		m_nCurPos = 0;	

		m_nCurrentStep++;

		m_RenderingProgressbar.ShowWindow(SW_HIDE);	
		m_PreviewMovieBtn.ShowWindow(SW_SHOW);	
		m_OKBtn.ShowWindow(SW_SHOW);

		m_CloseBtn.EnableWindow();
		Invalidate();
	}	
}


void CWmvConvertDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_hWmvGen)
	{
		wgenDestroyInstance( m_hWmvGen );
		m_hWmvGen = NULL;
	}	
}


BOOL CWmvConvertDlg::PreTranslateMessage(MSG* pMsg) 
{
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


void CWmvConvertDlg::OnWmvclosebtn() 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}


void CWmvConvertDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = 22;
	if(rc.PtInRect(point))
	{
		this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;
	}

	CDialog::OnLButtonDown(nFlags, point);
}
