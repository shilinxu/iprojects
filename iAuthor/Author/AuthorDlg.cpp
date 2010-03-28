// AuthorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Author.h"
#include "AuthorDlg.h"
#include "AppSettings.h"

#include "PPTUtil.h"

#include <atlbase.h>
#include <atlconv.h>
#include "GdiUtils.h"

#include "Volume/IVolume.h"
#include "Volume/VolumeInXXX.h"
#include "Volume/VolumeOutMaster.h"
#include "Volume/VolumeOutWave.h"

#include "WmvConvertDlg.h"
#include "RegistrationDlg.h"
#include "OptionDlg.h"

#include "RecordSelectRange.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		RECORDING_TIMER					1
#define		REGSTRATION_TIMER				2
#define		RECORD_SELECTWINDOW_TIMER		3
#define		RECORD_SELECTRANGE_TIMER		4
#define		RECORD_MARKINGRANGE_TIMER		5
#define		REDRAW_TIMER					6

#define		RECORD_FULLSCREEN				1
#define		RECORD_WINDOW					2
#define		RECORD_AREA						3

UINT WM_XINK_STARTSTROKE;
UINT WM_XINK_ADDPOINT;
UINT WM_XINK_STOPSTROKE;

/////////////////////////////////////////////////////////////////////////////
// CAuthorDlg dialog

CAuthorDlg::CAuthorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthorDlg::IDD, pParent), m_pen(XInk::DT_Pen, XInk::xColor(RGB(0,0,0), 255), 5.0)
{
	//{{AFX_DATA_INIT(CAuthorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_bRecToolbar = false;
	m_bMoveTop = true;

	m_nSubMenuPopup = NONE_POPUP;
	m_nSelMouseOverCtrlID = -1;
	m_rcEnable = CRect(-1,-1,-1,-1);
	m_rcEnableMenu = CRect(-1,-1,-1,-1);
	
	m_bFold = false;
	m_nFoldWidth = 42;

	m_nHLPenPattern = 0;		//0: "I", 1: "O"
	m_nLinePattern = 0;			//0: "-" , 1: "->"
	m_bSnapshotFullMode = true;
	m_nCamSize = 0;
	m_bSnapshotRange = false;
	m_bTextRange = false;

	m_nPenToolType = PENTOOL;
	m_nPenType = PENCIL;
	m_nEraserType = ERASER;	
	m_Color = RGB(0,0,0);
	m_PenWidth = 5;
	m_nDiagramType = DIAGRAM_RECT;

	m_bBoardOpen = false;
	m_nSlideIndex = 0;
	m_strSlidePath = "";

	m_bZoomMode = false;
	m_rcZoom = CRect(0,0,0,0);

	m_bEnableVolume = false;
	m_uMicLineIndex = (UINT)-1;
	m_dwMicCurVol = -1;

	m_hCursor = NULL;
  
	m_strRecState = "READY";
	m_strRecTime = "00:00:00";	
	
	m_nRecProgress = 0;

	m_hCapture = NULL;
	m_bRecordFlag = false;
	m_bInitRecord = false;
	m_bRecCursor = TRUE;
	m_dwFps = 20;
	m_dwQuality = 8000;
	m_dwKeyFrameRate = 0;
	m_bRecLogo = false;
	m_hBitmapLogo = NULL;

	m_WidthScreen = ::GetSystemMetrics(SM_CXSCREEN);
	m_HeightScreen = ::GetSystemMetrics(SM_CYSCREEN);

	m_pCanvas = 0;
	// 캔버스 객체를 바탕화면이 아니라 슬라이드(복사된 바탕화면, 배경 칠판)에 만든다.
	// 판서 모드 버튼을 누를때 마다 바탕화면을 새로운 슬라이드에 복사하고 여기에 캔버스를 만든다.
	CreateCanvas(0, XInk::xRect(0,0, m_WidthScreen,m_HeightScreen));

	m_bMakeIndex = false;
	m_nIndexStartTime = 0;
	m_nSchemeIndex = 0;

	m_bPPTSlideShowMinize = false;	

	m_nControlWidth = 161;
	m_nSlideBGWidth = 114;
	m_nShowControlCount = 0;

	m_nHideControlStart = 0;
	m_nHideSlideStart = 0;

	m_nRecordRangeState = RECORD_FULLSCREEN;
	m_rcPrevRecordRangeWindow = CRect(-1,-1,-1,-1);
	m_rcCurRecordRangeWindow = CRect(-1,-1,-1,-1);
	m_bSelectRecordWindow = false;
	m_bSelectRecordArea = false;

	m_LogBrush.lbStyle = BS_HOLLOW;
	m_LogBrush.lbColor = DIB_RGB_COLORS;
	m_LogBrush.lbHatch = HS_CROSS;

	m_bSmallMode = false;
	m_bReName = false;
	m_bMarkingRecArea = false;

	m_bgTextColor = RGB(255,255,255);
	m_bSelCodec = false;
}

void CAuthorDlg::CreateCanvas(HWND hwnd, XInk::xRect rc)
{
	ReleaseCanvas();

	m_pCanvas = new XInk::xCanvas(hwnd, rc);
	m_pCanvas->SetPen(m_pen);
	m_pCanvas->EnableAutoDrawing(true);
	m_bDrawCanvas = false;
}

void CAuthorDlg::ReleaseCanvas()
{
	SAFE_DELETE(m_pCanvas);
}

void CAuthorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuthorDlg)	
	DDX_Control(pDX, IDC_CAMERABTN, m_CameraBtn);
	DDX_Control(pDX, IDC_OPTIONBTN, m_OptionBtn);
	DDX_Control(pDX, IDOK, m_ExitBtn);
	DDX_Control(pDX, IDC_SLIDEMENUBTN, m_SlideMenuBtn);
	DDX_Control(pDX, IDC_PRINTBTN, m_PrintBtn);
	DDX_Control(pDX, IDC_WMVBTN, m_WmvBtn);
	DDX_Control(pDX, IDC_SNAPSHOTBTN, m_SnapshotBtn);
	DDX_Control(pDX, IDC_ZOOMBTN, m_ZoomBtn);	
	DDX_Control(pDX, IDC_BOARDOPENBTN, m_BoardOpenBtn);
	DDX_Control(pDX, IDC_HELPBTN, m_HelpBtn);
	DDX_Control(pDX, IDC_MICBTN, m_MicBtn);	
	DDX_Control(pDX, IDC_PPTNEXTBTN, m_PPTNextBtn);
	DDX_Control(pDX, IDC_PPTPREVBTN, m_PPTPrevBtn);
	DDX_Control(pDX, IDC_RECORDSTOPBTN, m_RecordStopBtn);
	DDX_Control(pDX, IDC_RECORDPAUSEBTN, m_RecordPauseBtn);
	DDX_Control(pDX, IDC_RECORDBTN, m_RecordBtn);
	DDX_Control(pDX, IDC_DIAGRAMBTN, m_DiagramBtn);
	DDX_Control(pDX, IDC_LINEWEIGHTCHANGEBTN, m_LineWeightBtn);
	DDX_Control(pDX, IDC_COLORCHANGEBTN, m_ColorChangeBtn);
	DDX_Control(pDX, IDC_ERASERBTN, m_EraserBtn);
	DDX_Control(pDX, IDC_TEXT, m_TextBtn);
	DDX_Control(pDX, IDC_PENBTN, m_PenBtn);
	DDX_Control(pDX, IDC_MODEBTN, m_ModeBtn);
	DDX_Control(pDX, IDC_MOVEBOTTOMBTN, m_MoveBottomBtn);
	DDX_Control(pDX, IDC_MOVETOPBTN, m_MoveTopBtn);
	DDX_Control(pDX, IDC_SHOWHIDETOOLBTN, m_ShowhideBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAuthorDlg, CDialog)
	//{{AFX_MSG_MAP(CAuthorDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SHOWHIDETOOLBTN, OnShowhidetoolbtn)
	ON_BN_CLICKED(IDC_MODEBTN, OnModebtn)
	ON_BN_CLICKED(IDC_TEXT, OnTextBtn)
	ON_BN_CLICKED(IDC_ERASERBTN, OnEraserbtn)
	ON_BN_CLICKED(IDC_COLORCHANGEBTN, OnColorchangebtn)
	ON_BN_CLICKED(IDC_LINEWEIGHTCHANGEBTN, OnLineweightchangebtn)
	ON_BN_CLICKED(IDC_PPTPREVBTN, OnPptprevbtn)
	ON_BN_CLICKED(IDC_PPTNEXTBTN, OnPptnextbtn)
	ON_BN_CLICKED(IDC_RECORDBTN, OnRecordbtn)
	ON_BN_CLICKED(IDC_RECORDPAUSEBTN, OnRecordpausebtn)
	ON_BN_CLICKED(IDC_RECORDSTOPBTN, OnRecordstopbtn)
	ON_BN_CLICKED(IDC_WMVBTN, OnWMVbtn)
	ON_BN_CLICKED(IDC_ZOOMBTN, OnZoombtn)
	ON_BN_CLICKED(IDC_BOARDOPENBTN, OnBoardOpenbtn)
	ON_BN_CLICKED(IDC_DIAGRAMBTN, OnDiagrambtn)
	ON_BN_CLICKED(IDC_MICBTN, OnMicbtn)
	ON_BN_CLICKED(IDC_MOVETOPBTN, OnMovetopbtn)
	ON_BN_CLICKED(IDC_PENBTN, OnPenbtn)
	ON_BN_CLICKED(IDC_PRINTBTN, OnPrintbtn)
	ON_BN_CLICKED(IDC_SLIDEMENUBTN, OnSlidemenubtn)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDC_MOVEBOTTOMBTN, OnMovebottombtn)
	ON_BN_CLICKED(IDC_HELPBTN, OnHelp)
	ON_BN_CLICKED(IDC_OPTIONBTN, OnOptionbtn)
	ON_BN_CLICKED(IDC_CAMERABTN, OnCameraOnOff)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSE_OVER, OnCtrlMouseOver)
	ON_MESSAGE(WM_MOUSE_OUT, OnCtrlMouseOut)
	ON_MESSAGE(WM_SNAPSHOT, OnSnapshot)	
	ON_MESSAGE(WM_PENSELCT, OnNotifyPenSelect)
	ON_MESSAGE(WM_ERASERSELCT, OnNotifyEraserSelect)
	ON_MESSAGE(WM_COLORSELCT, OnNotifyColorSelect)
	ON_MESSAGE(WM_LINEWEIGHTSELCT, OnNotifyLineWeightSelect)
	ON_MESSAGE(WM_DIAGRAMSELCT, OnNotifyDiagramSelect)
	ON_MESSAGE(WM_BOARDSELCT, OnNotifyBoardSelect)
	ON_MESSAGE(WM_SLIDEMENUSELCT, OnNotifySlideMenuSelect)
	ON_MESSAGE(WM_MESSAGE_TEXT_DELETE, OnTextDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuthorDlg message handlers

BOOL CAuthorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	WM_XINK_STARTSTROKE = RegisterWindowMessage("WM_XINK_STARTSTROKE");
	WM_XINK_ADDPOINT	= RegisterWindowMessage("WM_XINK_ADDPOINT");
	WM_XINK_STOPSTROKE	= RegisterWindowMessage("WM_XINK_STOPSTROKE");

	CFileFind finder;	
	CString strPath = CAppSettings::GetSlidePath();
	BOOL bWorking = finder.FindFile(strPath);
	if(!bWorking)
		::CreateDirectory(strPath, NULL);

	strPath = CAppSettings::GetWorkPath();
	bWorking = finder.FindFile(strPath);
	if(!bWorking)
		::CreateDirectory(strPath, NULL);

	strPath = CAppSettings::GetTempPath();
	bWorking = finder.FindFile(strPath);
	if(!bWorking)
		::CreateDirectory(strPath, NULL);
	
	strPath = CAppSettings::GetContentsPath();
	bWorking = finder.FindFile(strPath);
	if(!bWorking)
		::CreateDirectory(strPath, NULL);

	finder.Close();	

	VERIFY(m_BoardSelectDlg.Create( IDD_BOARDSELECTDLG, this));
	VERIFY(m_ColorSelctDlg.Create( IDD_COLORSELECTDLG, this));
	VERIFY(m_DiagramSelectDlg.Create( IDD_DIAGRAMSELECTDLG, this));
	VERIFY(m_EraserSelectDlg.Create( IDD_ERASERSELECTDLG, this));
	VERIFY(m_LineWeightSelectDlg.Create( IDD_LINEWEIGHTSELECTDLG, this));
	VERIFY(m_PenSelectDlg.Create( IDD_PENSELECTDLG, this));
	VERIFY(m_SlideMenuDlg.Create( IDD_SLIDEMENUDLG, this));
	
	// 부모윈도우가 CWnd::GetDesktopWindow() 이면 윈도 Z-Order가 컨트롤패널 아래로
	VERIFY(m_BoardDlg.Create( IDD_BOARDDLG, CWnd::GetDesktopWindow()));
	
	// 부모윈도우가 NULL 이면 윈도 Z-Order가 컨트롤패널 위에
	VERIFY(m_MagnifierDlg.Create( IDD_MAGNIFIERDLG, NULL));
	m_MagnifierDlg.MoveWindow(CRect(0,0,m_WidthScreen, m_HeightScreen));
	m_MagnifierDlg.SetParentWnd(this);
	m_MagnifierDlg.ShowWindow(SW_HIDE);	

	VERIFY(m_RecSizeDlg.Create( IDD_RECSIZEDLG, CWnd::GetDesktopWindow()));
	m_RecSizeDlg.MoveWindow(CRect(0,0,75,15));
	m_RecSizeDlg.ShowWindow(SW_HIDE);
	
	ArrangeControl();

	m_ShowhideBtn.DrawBorder(FALSE);
	m_ShowhideBtn.SetCheckButton();
	m_ShowhideBtn.SetBitmaps(IDB_SHOWHIDETOOLBTN, RGB(255,0,255));	
	m_ShowhideBtn.DrawTransparent();

	m_MoveTopBtn.DrawBorder(FALSE);
	m_MoveTopBtn.SetBitmaps(IDB_MOVETOPBTN, RGB(255,0,255));	
	m_MoveTopBtn.DrawTransparent();

	m_MoveBottomBtn.DrawBorder(FALSE);
	m_MoveBottomBtn.SetBitmaps(IDB_MOVEBOTTOMBTN, RGB(255,0,255));	
	m_MoveBottomBtn.DrawTransparent();

	m_ModeBtn.DrawBorder(FALSE);
	m_ModeBtn.SetCheckButton();
	m_ModeBtn.SetBitmaps(IDB_MODEBTN, RGB(255,0,255));	
	m_ModeBtn.DrawTransparent();

	m_PenBtn.DrawBorder(FALSE);
	m_PenBtn.Set3StateButton();
	m_PenBtn.SetBitmaps(IDB_PENCIL, RGB(255,0,255));
	m_PenBtn.SetSelect();
	m_PenBtn.DrawTransparent();	

	m_TextBtn.DrawBorder(FALSE);
	m_TextBtn.Set3StateButton();
	m_TextBtn.SetBitmaps(IDB_TEXT, RGB(255,0,255));	
	m_TextBtn.DrawTransparent();

	m_EraserBtn.DrawBorder(FALSE);
	m_EraserBtn.Set3StateButton();
	m_EraserBtn.SetBitmaps(IDB_ERASER, RGB(255,0,255));	
	m_EraserBtn.DrawTransparent();

	m_ColorChangeBtn.DrawBorder(FALSE);
	m_ColorChangeBtn.SetColorButton();
	m_ColorChangeBtn.SetBitmaps(IDB_COLORBTN, RGB(255,0,255));	
	m_ColorChangeBtn.DrawTransparent();

	m_LineWeightBtn.DrawBorder(FALSE);
	m_LineWeightBtn.SetWidthButton();
	m_LineWeightBtn.SetBitmaps(IDB_LINEWEIGHTBTN, RGB(255,0,255));	
	m_LineWeightBtn.DrawTransparent();

	m_DiagramBtn.DrawBorder(FALSE);
	m_DiagramBtn.Set3StateButton();
	m_DiagramBtn.SetBitmaps(IDB_RECT, RGB(255,0,255));	
	m_DiagramBtn.DrawTransparent();

	m_OptionBtn.DrawBorder(FALSE);	
	m_OptionBtn.SetBitmaps(IDB_OPTIONBTN, RGB(255,0,255));	
	m_OptionBtn.DrawTransparent();

	m_RecordBtn.DrawBorder(FALSE);
	m_RecordBtn.SetCheckButton();
	m_RecordBtn.SetBitmaps(IDB_RECORDBTN, RGB(255,0,255));	
	m_RecordBtn.DrawTransparent();

	m_RecordPauseBtn.DrawBorder(FALSE);
	m_RecordPauseBtn.SetBitmaps(IDB_RECORDPAUSEBTN, RGB(255,0,255));	
	m_RecordPauseBtn.DrawTransparent();
	m_RecordPauseBtn.EnableWindow(FALSE);

	m_RecordStopBtn.DrawBorder(FALSE);
	m_RecordStopBtn.SetBitmaps(IDB_RECORDSTOPBTN, RGB(255,0,255));	
	m_RecordStopBtn.DrawTransparent();
	m_RecordStopBtn.EnableWindow(FALSE);
	
	m_MicBtn.DrawBorder(FALSE);
	m_MicBtn.SetCheckButton();
	m_MicBtn.SetBitmaps(IDB_MICBTN, RGB(255,0,255));	
	m_MicBtn.DrawTransparent();

	m_WmvBtn.DrawBorder(FALSE);
	m_WmvBtn.SetBitmaps(IDB_WMVBTN, RGB(255,0,255));	
	m_WmvBtn.DrawTransparent();

	m_HelpBtn.DrawBorder(FALSE);
	m_HelpBtn.SetBitmaps(IDB_HELPBTN, RGB(255,0,255));	
	m_HelpBtn.DrawTransparent();

	m_PPTPrevBtn.DrawBorder(FALSE);
	m_PPTPrevBtn.SetRgnBtn(TRUE);
	m_PPTPrevBtn.SetBitmaps(IDB_PPTPREV, RGB(255,0,255));	
	m_PPTPrevBtn.DrawTransparent();

	m_PPTNextBtn.DrawBorder(FALSE);
	m_PPTNextBtn.SetRgnBtn(TRUE);
	m_PPTNextBtn.SetBitmaps(IDB_PPTNEXT, RGB(255,0,255));	
	m_PPTNextBtn.DrawTransparent();

	m_BoardOpenBtn.DrawBorder(FALSE);
	m_BoardOpenBtn.SetBitmaps(IDB_BLACKBOARD, RGB(255,0,255));	
	m_BoardOpenBtn.DrawTransparent();

	m_ZoomBtn.DrawBorder(FALSE);
	m_ZoomBtn.SetBitmaps(IDB_ZOOMBTN, RGB(255,0,255));	
	m_ZoomBtn.DrawTransparent();

	m_SnapshotBtn.DrawBorder(FALSE);
	m_SnapshotBtn.SetActionButton();
	m_SnapshotBtn.SetBitmaps(IDB_SNAPSHOT, RGB(255,0,255));	
	m_SnapshotBtn.DrawTransparent();
	
	m_SlideMenuBtn.DrawBorder(FALSE);
	m_SlideMenuBtn.SetBitmaps(IDB_SLIDEMENUBTN, RGB(255,0,255));	
	m_SlideMenuBtn.DrawTransparent();

	m_CameraBtn.DrawBorder(FALSE);
	m_CameraBtn.SetBitmaps(IDB_CAMERA, RGB(255,0,255));	
	m_CameraBtn.DrawTransparent();

	m_PrintBtn.DrawBorder(FALSE);
	m_PrintBtn.SetBitmaps(IDB_PRINT, RGB(255,0,255));	
	m_PrintBtn.DrawTransparent();
	
	m_ExitBtn.DrawBorder(FALSE);
	m_ExitBtn.SetBitmaps(IDB_EXIT, RGB(255,0,255));	
	m_ExitBtn.DrawTransparent();

	m_SlideMenuDlg.MoveWindow(CRect(0,0, 220, 192));

	SetImageCursor();
	InitScheme();
	InitVolume();

	m_bLayeredWndEnable = CAppSettings::IsLayeredWndEnable();	
	SetEnableLayeredWnd();	

	ReadToolbarOption();
	ReadRecordOption();
	ReadPenFolderOption();
	ReadSnapshotOption();
	ReadTextOption();
	ReadCameraOption();
	ReadCodecOption();

	if(!CAppSettings::IsRegistration())
	{
		m_bRegistration = false;
		m_bRecLogo = true;
		m_hBitmapLogo = (HBITMAP)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_COMPANY),
						IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

		SetTimer(REGSTRATION_TIMER, 500, NULL);
	}
	else
		m_bRegistration = true;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CAuthorDlg::OnRegstrationDlg()
{
	if(!m_bRegistration)
	{
		CRegistrationDlg RegDlg;

		if(RegDlg.DoModal() == IDOK)
			OnOK();	
	}
}


void CAuthorDlg::ReadToolbarOption()
{
	CString REG_KEY  = CAppSettings::GetOptionToolbarPath();
	CString strTemp;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	for(int i = 0; i < 13; i++)
	{
		m_bShowControl[i] = false;
	}
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "MODE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[0] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PEN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[1] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "HLPEN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[2] = true;			

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "ERASER", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[3] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "COLOR", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[4] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "WIDTH", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[5] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "DIAGRAM", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[6] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "CONTROL", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[7] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PPT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[8] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "BOARD", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[9] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "ZOOM", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[10] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SNAPSHOT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[11] = true;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PRINT", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp.Format("%s", szCheck);
		if(strTemp == "TRUE")
			m_bShowControl[12] = true;

		RegCloseKey( hKey );

		CalFoldControl();
	}
}

void CAuthorDlg::ReadRecordOption()
{	
	CString REG_KEY  = CAppSettings::GetOptionRecordPath();
	CString strTemp;
	int Num = 0;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{
		if(m_bRegistration)
		{
			RegQueryValueEx( hKey, "LOGOENABLE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
			strTemp.Format("%s", szCheck);
			if(strTemp == "TRUE")
			{						
				memset(szCheck, 0, MAX_PATH );
				dwBufLen = MAX_PATH;
				RegQueryValueEx( hKey, "LOGOPATH", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
				strTemp.Format("%s", szCheck);			

				if(m_hBitmapLogo != NULL)
				{
					::DeleteObject(m_hBitmapLogo);
					m_hBitmapLogo = NULL;
				}

				m_hBitmapLogo = (HBITMAP)LoadImage(NULL, strTemp, IMAGE_BITMAP, 0, 0,
										LR_LOADFROMFILE | LR_CREATEDIBSECTION);

				if(m_hBitmapLogo != NULL)
					m_bRecLogo = true;
				else
					m_bRecLogo = false;
			}
			else
			{
				if(m_hBitmapLogo != NULL)
				{
					::DeleteObject(m_hBitmapLogo);
					m_hBitmapLogo = NULL;
				}

				m_bRecLogo = false;
			}

			bool bRecToolbar = m_bRecToolbar;
			memset(szCheck, 0, MAX_PATH );
			dwBufLen = MAX_PATH;
			RegQueryValueEx( hKey, "TOOLREC", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
			strTemp.Format("%s", szCheck);
			if(strTemp == "TRUE")
				m_bRecToolbar = true;
			else
				m_bRecToolbar = false;

			if(bRecToolbar != m_bRecToolbar)
				SetEnableRecLayeredWnd();
		}

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "CURSOR", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		if(Num == 1)
			m_bRecCursor = TRUE;
		else
			m_bRecCursor = FALSE;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "FPS", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		switch(Num)
		{		
		case	1:
			m_dwFps = 6;			
			break;
		case	2:
			m_dwFps = 8;			
			break;
		case	3:
			m_dwFps = 10;			
			break;
		case	4:
			m_dwFps = 15;			
			break;
		case	5:
			m_dwFps = 20;			
			break;
		default:
			m_dwFps = 2;		
			break;
		}	

		m_dwKeyFrameRate = 0;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "QUALITY", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		switch(Num)
		{		
		case	0:
			m_dwQuality = 8000;
			break;
		case	2:
			m_dwQuality = 7000;
			break;
		default:
			m_dwQuality = 7500;
			break;
		}		

		RegCloseKey( hKey );
	}
}

void CAuthorDlg::ReadPenFolderOption()
{
	CString REG_KEY  = CAppSettings::GetOptionPenPath();
	CString strTemp;
	int Num;

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "PENPATTERN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		if(m_nHLPenPattern != Num)
		{
			m_nHLPenPattern = Num;
			if(m_nPenToolType == PENTOOL && m_nPenType == HIGHLIGHTPEN)
				SetPenType(m_nPenType);
		}	
		else
			m_nHLPenPattern = Num;

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "LINEPATTERN", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		Num = atoi(szCheck);
		if(m_nLinePattern != Num)
		{
			m_nLinePattern = Num;
			if((m_nPenToolType == DIAGRAMTOOL) && (m_nDiagramType == DIAGRAM_LINE))
				SetPenType(m_nDiagramType);
		}
		else
			m_nLinePattern = Num;
		
		RegCloseKey( hKey );
	}
}



void CAuthorDlg::ReadSnapshotOption()
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
			m_bSnapshotFullMode = true;
		else
			m_bSnapshotFullMode = false;
	
		RegCloseKey( hKey );
	}
}


void CAuthorDlg::ReadTextOption()
{
	CString REG_KEY  = CAppSettings::GetOptionTextPath();
	CString strTemp;	

	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		int CR, CG, CB;

		RegQueryValueEx( hKey, "SELECTCOLOR_R", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		CR = atoi(szCheck);		

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SELECTCOLOR_G", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		CG = atoi(szCheck);		

		memset(szCheck, 0, MAX_PATH );
		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "SELECTCOLOR_B", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		CB = atoi(szCheck);	
		
		m_bgTextColor = RGB(CR, CG, CB);
		
		RegCloseKey( hKey );
	}
}



void CAuthorDlg::ReadCameraOption()
{
	CString REG_KEY = CAppSettings::GetOptionCameraPath();
	
	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "SIZE", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		m_nCamSize = atoi(szCheck);

		RegCloseKey( hKey );
	}
}


void CAuthorDlg::ReadCodecOption()
{
	CString REG_KEY = CAppSettings::GetOptionCodecPath();
	
	HKEY hKey;
	char szCheck[MAX_PATH];
	DWORD dwBufLen = MAX_PATH;
	CString strTemp;
	
	if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
	{	
		RegQueryValueEx( hKey, "CHECK", NULL, NULL, (LPBYTE)szCheck, &dwBufLen);
		strTemp = szCheck;

		if(strTemp == "TRUE")
			m_bSelCodec = true;
		else
			m_bSelCodec = false;

		RegCloseKey( hKey );
	}

}



void CAuthorDlg::InitScheme()
{
	m_bMakeIndex = false;
	m_nIndexStartTime = 0;
	m_nSchemeIndex = 0;

	CString strValue;
	CString strSchemePath = CAppSettings::GetSchemePath();
	WritePrivateProfileString("AUTHOR_INFO", "VERSION", "1.0", strSchemePath);
	WritePrivateProfileString("AUTHOR_INFO", "TITLE", "NoTitle", strSchemePath);
	WritePrivateProfileString("AUTHOR_INFO", "AUTHOR", CAppSettings::GetUserName(), strSchemePath);

	CTime t = CTime::GetCurrentTime();
	strValue.Format("%s", t.Format( " %Y, %B %d" ));
	WritePrivateProfileString("AUTHOR_INFO", "DATE", strValue, strSchemePath);

	strValue.Format("%d", m_WidthScreen);	
	WritePrivateProfileString("VIDEO_INFO", "WIDTH", strValue, strSchemePath);

	strValue.Format("%d", m_HeightScreen);	
	WritePrivateProfileString("VIDEO_INFO", "HEIGHT", strValue, strSchemePath);

	strValue.Format("%d", m_dwFps);	
	WritePrivateProfileString("VIDEO_INFO", "FPS", strValue, strSchemePath);

	strValue.Format("%d", 1);	
	WritePrivateProfileString("VIDEO_INFO", "KEY_FRAME", strValue, strSchemePath);

	strValue.Format("%d", m_dwQuality);	
	WritePrivateProfileString("VIDEO_INFO", "QUALITY", strValue, strSchemePath);

	WritePrivateProfileString("VIDEO_INFO", "DURATION", "0", strSchemePath);	
	WritePrivateProfileString("VIDEO_INFO", "INDEX_COUNT", "0", strSchemePath);
	WritePrivateProfileString("VIDEO_INFO", "VIDEO_PATH", "Notitle.avi", strSchemePath);
}


void CAuthorDlg::WriteSchemeIndex(DWORD dwEndTime)
{
	CString strSchemePath, strSection, strValue;
	strSchemePath = CAppSettings::GetSchemePath();
	strSection.Format("INDEX_%d", m_nSchemeIndex);

	strValue.Format("Index_%d", m_nSchemeIndex);
	WritePrivateProfileString(strSection, "NAME", strValue, strSchemePath);	

	strValue.Format("%d", m_nIndexStartTime);
	WritePrivateProfileString(strSection, "START_TIME", strValue, strSchemePath);	

	strValue.Format("%d", dwEndTime);
	WritePrivateProfileString(strSection, "END_TIME", strValue, strSchemePath);	
	WritePrivateProfileString(strSection, "DESCRIPTION", "", strSchemePath);	
	WritePrivateProfileString(strSection, "SELECTION", "", strSchemePath);
	
	m_nSchemeIndex++;
}


void CAuthorDlg::SetSchemeTerminate(CString strFilename, DWORD dwDuration)
{
	CString strValue;
	CString strSchemePath = CAppSettings::GetSchemePath();

	strValue.Format("%d", dwDuration);
	WritePrivateProfileString("VIDEO_INFO", "DURATION", strValue, strSchemePath);

	strValue.Format("%d", m_nSchemeIndex);
	WritePrivateProfileString("VIDEO_INFO", "INDEX_COUNT", strValue, strSchemePath);

	WritePrivateProfileString("VIDEO_INFO", "VIDEO_PATH", strFilename, strSchemePath);
}


/////////////////////////////////////////////////////////////////////////////
// Volume initialize

void CAuthorDlg::InitVolume()
{
	if(::waveInGetNumDevs() < 1)
		return;

	m_bEnableVolume = true;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAuthorDlg::OnPaint() 
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);
	
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_SHOWHIDEBG);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();

	int i = bm.bmWidth;		
	LoadBitmap.LoadBitmap(IDB_BGIMAGE);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
	dc.BitBlt(i, 0, i+bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();
	

	i = i+bm.bmWidth; 
	LoadBitmap.LoadBitmap(IDB_BGIMAGE1);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
	for(; i <= rc.Width(); i+= bm.bmWidth)
	{
		dc.BitBlt(i, 0, i+bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	}

	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();
	

	if(m_bSmallMode)
	{
		if(m_ShowhideBtn.IsSelected())
		{
			LoadBitmap.LoadBitmap(IDB_CONTROLBG);
			LoadBitmap.GetObject(sizeof(BITMAP), &bm);
			pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);		
			dc.BitBlt(m_nControlStart, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(pBitmap);
			LoadBitmap.DeleteObject();

			CFont	Font, *pOldFont;		
			Font.CreatePointFont(80,"Arial", &dc);
			pOldFont = (CFont*)dc.SelectObject(&Font);

			dc.SetBkMode(TRANSPARENT);	
			dc.SetTextColor(RGB(255,114,0));
			dc.DrawText(m_strRecState, &m_rcRecState, DT_LEFT);
			dc.SetTextColor(RGB(121,89,21));
			dc.DrawText(m_strRecTime, &m_rcRecTime, DT_LEFT);

			dc.SelectObject(pOldFont);
			Font.DeleteObject(); 
			
			if(m_nRecProgress > 0)
			{
				LoadBitmap.LoadBitmap(IDB_RECORDPROGRESS);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);

				i = 4*m_nRecProgress;
				dc.BitBlt(m_nControlStart+105, 34, i, 9, &memDC, 0, 0, SRCCOPY);

				
				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();
			}	
		}
	
		int nX = m_WidthScreen - 114;
		if(m_bMoveTop)
		{	
			LoadBitmap.LoadBitmap(IDB_SLIDEMENU_TOPBG);
			LoadBitmap.GetObject(sizeof(BITMAP), &bm);
			pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
			dc.BitBlt(nX, 29, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
			
			memDC.SelectObject(pBitmap);
			LoadBitmap.DeleteObject();			
		}
		else
		{
			LoadBitmap.LoadBitmap(IDB_SLIDEMENU_BOTTOMBG);
			LoadBitmap.GetObject(sizeof(BITMAP), &bm);
			pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
			dc.BitBlt(nX, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
			
			memDC.SelectObject(pBitmap);
			LoadBitmap.DeleteObject();			
		}

		LoadBitmap.LoadBitmap(IDB_BGIMAGE2);
		LoadBitmap.GetObject(sizeof(BITMAP), &bm);
		pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
		dc.BitBlt(rc.Width()-2, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pBitmap);
		LoadBitmap.DeleteObject();		
	}
	else
	{
		if(!m_ShowhideBtn.IsSelected())
		{
			LoadBitmap.LoadBitmap(IDB_CONTROLBG);
			LoadBitmap.GetObject(sizeof(BITMAP), &bm);
			pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);		
			dc.BitBlt(m_nControlStart, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
			
			memDC.SelectObject(pBitmap);
			LoadBitmap.DeleteObject();			

			CFont	Font, *pOldFont;		
			Font.CreatePointFont(80,"Arial", &dc);
			pOldFont = (CFont*)dc.SelectObject(&Font);

			dc.SetBkMode(TRANSPARENT);	
			dc.SetTextColor(RGB(255,114,0));
			dc.DrawText(m_strRecState, &m_rcRecState, DT_LEFT);
			dc.SetTextColor(RGB(121,89,21));
			dc.DrawText(m_strRecTime, &m_rcRecTime, DT_LEFT);

			dc.SelectObject(pOldFont);
			Font.DeleteObject(); 
			
			if(m_nRecProgress > 0)
			{
				LoadBitmap.LoadBitmap(IDB_RECORDPROGRESS);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);

				i = 4*m_nRecProgress;
				dc.BitBlt(m_nControlStart+105, 34, i, 9, &memDC, 0, 0, SRCCOPY);
				
				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();				
			}	
			
		
			int nX = m_WidthScreen - 114;
			if(m_bMoveTop)
			{	
				LoadBitmap.LoadBitmap(IDB_SLIDEMENU_TOPBG);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
				dc.BitBlt(nX, 29, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
				
				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();
				
			}
			else
			{
				LoadBitmap.LoadBitmap(IDB_SLIDEMENU_BOTTOMBG);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
				dc.BitBlt(nX, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
				
				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();				
			}

			LoadBitmap.LoadBitmap(IDB_BGIMAGE2);
			LoadBitmap.GetObject(sizeof(BITMAP), &bm);
			pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
			dc.BitBlt(rc.Width()-2, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
			
			memDC.SelectObject(pBitmap);
			LoadBitmap.DeleteObject();			
		}
		else 
		{
			if(m_bShowControl[7])
			{
				LoadBitmap.LoadBitmap(IDB_CONTROLBG);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);		
				dc.BitBlt(m_nHideControlStart, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();				

				CFont	Font, *pOldFont;		
				Font.CreatePointFont(80,"Arial", &dc);
				pOldFont = (CFont*)dc.SelectObject(&Font);

				dc.SetBkMode(TRANSPARENT);	
				dc.SetTextColor(RGB(255,114,0));
				dc.DrawText(m_strRecState, &m_rcHideRecState, DT_LEFT);
				dc.SetTextColor(RGB(121,89,21));
				dc.DrawText(m_strRecTime, &m_rcHideRecTime, DT_LEFT);

				dc.SelectObject(pOldFont);
				Font.DeleteObject(); 

				if(m_nRecProgress > 0)
				{
					LoadBitmap.LoadBitmap(IDB_RECORDPROGRESS);
					LoadBitmap.GetObject(sizeof(BITMAP), &bm);
					pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);

					i = 4*m_nRecProgress;
					dc.BitBlt(m_nHideControlStart+105, 34, i, 9, &memDC, 0, 0, SRCCOPY);

					LoadBitmap.DeleteObject();
					memDC.SelectObject(pBitmap);
				}	
			}

			if(m_bShowControl[12])
			{		
				if(m_bMoveTop)
				{	
					LoadBitmap.LoadBitmap(IDB_SLIDEMENU_TOPBG);
					LoadBitmap.GetObject(sizeof(BITMAP), &bm);
					pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
					dc.BitBlt(m_nHideSlideStart, 29, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
					
					memDC.SelectObject(pBitmap);
					LoadBitmap.DeleteObject();					
				}
				else
				{
					LoadBitmap.LoadBitmap(IDB_SLIDEMENU_BOTTOMBG);
					LoadBitmap.GetObject(sizeof(BITMAP), &bm);
					pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
					dc.BitBlt(m_nHideSlideStart, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);

					memDC.SelectObject(pBitmap);
					LoadBitmap.DeleteObject();					
				}
			}

			if(m_nShowControlCount > 0)
			{
				LoadBitmap.LoadBitmap(IDB_BGIMAGE2);
				LoadBitmap.GetObject(sizeof(BITMAP), &bm);
				pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);	
				dc.BitBlt(rc.Width()-2, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
				
				memDC.SelectObject(pBitmap);
				LoadBitmap.DeleteObject();				
			}
		}
	}
	
	CDialog::OnPaint();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAuthorDlg::SetEnableLayeredWnd()
{
	if(m_bLayeredWndEnable)
	{			
		typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
		SLWA pSetLayeredWindowAttributes = NULL;
		HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
		pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
		HWND hwnd = this->m_hWnd; 
		SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		pSetLayeredWindowAttributes(hwnd, RGB(255,0,255), 230, LWA_ALPHA);
		
		m_BoardSelectDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_ColorSelctDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_DiagramSelectDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_EraserSelectDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_LineWeightSelectDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_PenSelectDlg.SetEnableTransparent(m_bLayeredWndEnable);
		m_SlideMenuDlg.SetEnableTransparent(m_bLayeredWndEnable);		
	}
}


void CAuthorDlg::SetEnableRecLayeredWnd()
{
	if(m_bLayeredWndEnable)
	{
		if(m_hCapture != NULL)
		{
			BOOL hr = ::srecSetRecordLayeredWnd( m_hCapture, m_bRecToolbar);
			if(!hr)
				AfxMessageBox("Error : Failed! setting recoding cursor");
		}
	}
}


void CAuthorDlg::ArrangeControl()
{
	if(m_WidthScreen < 1024)
	{
		ArrangeControlSmallMode();
		m_bSmallMode = true;

		return;
	}

	CRect	rc, rc1;	
	rc.left = -1;
	rc.right = m_WidthScreen;	
	rc.top = 0;	
	rc.bottom = 63;

	MoveWindow(&rc);

	int nPPTStart;	
	if(rc.right < 1280)
	{		
		m_nMargine = 4;
		m_nControlStart = 493;
		m_nControlMargine = 6;
		nPPTStart = 660;

		m_rcHideRecState = m_rcRecState = CRect(m_nControlStart+28, 30, m_nControlStart+78, 40);
		m_rcHideRecTime = m_rcRecTime = CRect(m_nControlStart+28, 20, m_nControlStart+74, 30);
		m_rcHideRecProgress = m_rcRecProgress = CRect(m_nControlStart + 80, 30, m_nControlStart + 98, 38);
	}
	else
	{
		m_nMargine = 10;
		m_nControlStart = 540;
		m_nControlMargine = 17;		
		nPPTStart = 718;

		m_rcHideRecState = m_rcRecState = CRect(m_nControlStart+28, 30, m_nControlStart+78, 40);
		m_rcHideRecTime = m_rcRecTime = CRect(m_nControlStart+28, 20, m_nControlStart+74, 30);
		m_rcHideRecProgress = m_rcRecProgress = CRect(m_nControlStart + 80, 30, m_nControlStart + 98, 38);
	}

	rc1.top = 15;
	rc1.left = 1;
	rc1.bottom = rc1.top + 33;
	rc1.right = rc1.left + 33;
	m_ShowhideBtn.MoveWindow(&rc1);

	rc1.top = 4;	
	rc1.bottom = rc1.top + 9;
	rc1.left = 12;
	rc1.right = rc1.left + 10;
	m_MoveTopBtn.MoveWindow(&rc1);

	rc1.top = 50;	
	rc1.bottom = rc1.top + 9;	
	m_MoveBottomBtn.MoveWindow(&rc1);
	
	m_rcShow[0].top = 3;	
	m_rcShow[0].bottom = 63;
	m_rcShow[0].left = 43;
	m_rcShow[0].right = m_rcShow[0].left + 60;
	m_ModeBtn.MoveWindow(&m_rcShow[0]);

	m_rcShow[1].top = 3;	
	m_rcShow[1].bottom = 63;
	m_rcShow[1].left = m_rcShow[0].right + m_nMargine;
	m_rcShow[1].right = m_rcShow[1].left + 60;	
	m_PenBtn.MoveWindow(&m_rcShow[1]);
	
	m_rcShow[2].top = 3;	
	m_rcShow[2].bottom = 63;
	m_rcShow[2].left = m_rcShow[1].right + m_nMargine;
	m_rcShow[2].right = m_rcShow[2].left + 60;
	m_TextBtn.MoveWindow(&m_rcShow[2]);
		
	m_rcShow[3].top = 3;	
	m_rcShow[3].bottom = 63;
	m_rcShow[3].left = m_rcShow[2].right + m_nMargine;
	m_rcShow[3].right = m_rcShow[3].left + 60;
	m_EraserBtn.MoveWindow(&m_rcShow[3]);

	m_rcShow[4].top = 3;	
	m_rcShow[4].bottom = 63;
	m_rcShow[4].left = m_rcShow[3].right + m_nMargine;
	m_rcShow[4].right = m_rcShow[4].left + 60;
	m_ColorChangeBtn.MoveWindow(&m_rcShow[4]);

	m_rcShow[5].top = 3;	
	m_rcShow[5].bottom = 63;
	m_rcShow[5].left = m_rcShow[4].right + m_nMargine;
	m_rcShow[5].right = m_rcShow[5].left + 60;
	m_LineWeightBtn.MoveWindow(&m_rcShow[5]);

	m_rcShow[6].top = 3;	
	m_rcShow[6].bottom = 63;
	m_rcShow[6].left = m_rcShow[5].right + m_nMargine;
	m_rcShow[6].right = m_rcShow[6].left + 60;
	m_DiagramBtn.MoveWindow(&m_rcShow[6]); 

	m_rcShow[7].top = 28;
	m_rcShow[7].bottom = m_rcShow[7].top + 14;
	m_rcShow[7].left = m_nControlStart + 12;
	m_rcShow[7].right = m_rcShow[7].left + 16;
	m_MicBtn.MoveWindow(&m_rcShow[7]);

	m_rcShow[8].top = 6;
	m_rcShow[8].bottom = m_rcShow[8].top + 15;
	m_rcShow[8].left = m_nControlStart + 140;
	m_rcShow[8].right = m_rcShow[8].left + 13;
	m_HelpBtn.MoveWindow(&m_rcShow[8]);

	m_rcShow[9].top = 48;
	m_rcShow[9].bottom = m_rcShow[9].top + 14;
	m_rcShow[9].left = m_nControlStart + 6;
	m_rcShow[9].right = m_rcShow[9].left + 28;
	m_OptionBtn.MoveWindow(&m_rcShow[9]);

	m_rcShow[10].top = 42;
	m_rcShow[10].bottom = m_rcShow[10].top + 14;
	m_rcShow[10].left = m_nControlStart + 46;
	m_rcShow[10].right = m_rcShow[10].left + 28;
	m_RecordBtn.MoveWindow(&m_rcShow[10]);

	m_rcShow[11].top = 42;
	m_rcShow[11].bottom = m_rcShow[11].top + 14;
	m_rcShow[11].left = m_nControlStart + 75;
	m_rcShow[11].right = m_rcShow[11].left + 28;
	m_RecordPauseBtn.MoveWindow(&m_rcShow[11]);	

	m_rcShow[12].top = 42;
	m_rcShow[12].bottom = m_rcShow[12].top + 14;
	m_rcShow[12].left = m_nControlStart + 104;
	m_rcShow[12].right = m_rcShow[12].left + 28;
	m_RecordStopBtn.MoveWindow(&m_rcShow[12]);

	m_rcShow[13].top = 26;
	m_rcShow[13].bottom = m_rcShow[13].top + 14;
	m_rcShow[13].left = m_nControlStart + 109;
	m_rcShow[13].right = m_rcShow[13].left + 28;
	m_WmvBtn.MoveWindow(&m_rcShow[13]);	

	m_rcShow[14].top = 7;	
	m_rcShow[14].bottom = m_rcShow[14].top + 25;
	m_rcShow[14].left = nPPTStart;
	m_rcShow[14].right = m_rcShow[14].left + 50;
	m_PPTNextBtn.MoveWindow(&m_rcShow[14]);
	
	m_rcShow[15].top = m_rcShow[14].bottom;
	m_rcShow[15].bottom = m_rcShow[15].top +25;
	m_rcShow[15].left = nPPTStart;
	m_rcShow[15].right = m_rcShow[15].left + 50;
	m_PPTPrevBtn.MoveWindow(&m_rcShow[15]);

	m_rcShow[16].top = 3;
	m_rcShow[16].bottom = 63;
	m_rcShow[16].left = m_rcShow[15].right + m_nMargine;
	m_rcShow[16].right = m_rcShow[16].left + 60;
	m_BoardOpenBtn.MoveWindow(&m_rcShow[16]);

	m_rcShow[17].top = 3;
	m_rcShow[17].bottom = 63;
	m_rcShow[17].left = m_rcShow[16].right + m_nMargine;
	m_rcShow[17].right = m_rcShow[17].left + 60;
	m_ZoomBtn.MoveWindow(&m_rcShow[17]);
	
	m_rcShow[18].top = 3;
	m_rcShow[18].bottom = 63;
	m_rcShow[18].left = m_rcShow[17].right + m_nMargine;
	m_rcShow[18].right = m_rcShow[18].left + 60;
	m_SnapshotBtn.MoveWindow(&m_rcShow[18]);
	
	m_rcShow[19].top = 44;	
	m_rcShow[19].bottom = 57;
	m_rcShow[19].left = m_WidthScreen - 90;
	m_rcShow[19].right = m_rcShow[19].left + 78;
	m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);

	m_rcShow[20].top = 4;
	m_rcShow[20].bottom = m_rcShow[20].top + 29;	
	m_rcShow[20].left = m_WidthScreen - 110;
	m_rcShow[20].right = m_rcShow[20].left + 28;
	m_CameraBtn.MoveWindow(&m_rcShow[20]);	

	m_rcShow[21].top = 4;
	m_rcShow[21].bottom = m_rcShow[21].top + 28;	
	m_rcShow[21].left = m_rcShow[20].right + 12;
	m_rcShow[21].right = m_rcShow[21].left + 28;
	m_PrintBtn.MoveWindow(&m_rcShow[21]);
	
	m_rcShow[22].top = 8;	
	m_rcShow[22].bottom = m_rcShow[22].top + 23;
	m_rcShow[22].left = m_rcShow[21].right + 10;
	m_rcShow[22].right = m_rcShow[22].left + 23;
	m_ExitBtn.MoveWindow(&m_rcShow[22]);

	for(int i=0; i<23; i++)
	{
		m_rcHide[i] = m_rcShow[i];
	}
	
	ArrangePopupMenu();
}

void CAuthorDlg::ArrangePopupMenu()
{
	CRect	rc, rc1;
	int nWidth, nHeight;

	if(m_bMoveTop)
	{
		m_PenSelectDlg.GetWindowRect(&rc1);
		m_PenBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_PenSelectDlg.MoveWindow(&rc1);

		m_EraserSelectDlg.GetWindowRect(&rc1);
		m_EraserBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_EraserSelectDlg.MoveWindow(&rc1);

		m_ColorSelctDlg.GetWindowRect(&rc1);
		m_ColorChangeBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_ColorSelctDlg.MoveWindow(&rc1);

		m_LineWeightSelectDlg.GetWindowRect(&rc1);
		m_LineWeightBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_LineWeightSelectDlg.MoveWindow(&rc1);

		m_DiagramSelectDlg.GetWindowRect(&rc1);
		m_DiagramBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_DiagramSelectDlg.MoveWindow(&rc1);

		m_BoardSelectDlg.GetWindowRect(&rc1);
		m_BoardOpenBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;	
		rc1.top = 63;
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;
		m_BoardSelectDlg.MoveWindow(&rc1);		
	}
	else
	{		
		int top = m_HeightScreen-64;

		m_PenSelectDlg.GetWindowRect(&rc1);
		m_PenBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_PenSelectDlg.MoveWindow(&rc1);

		m_EraserSelectDlg.GetWindowRect(&rc1);
		m_EraserBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_EraserSelectDlg.MoveWindow(&rc1);

		m_ColorSelctDlg.GetWindowRect(&rc1);
		m_ColorChangeBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_ColorSelctDlg.MoveWindow(&rc1);

		m_LineWeightSelectDlg.GetWindowRect(&rc1);
		m_LineWeightBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_LineWeightSelectDlg.MoveWindow(&rc1);

		m_DiagramSelectDlg.GetWindowRect(&rc1);
		m_DiagramBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_DiagramSelectDlg.MoveWindow(&rc1);

		m_BoardSelectDlg.GetWindowRect(&rc1);
		m_BoardOpenBtn.GetWindowRect(&rc);
		nWidth = rc1.Width();
		nHeight = rc1.Height();
		rc1.left = rc.left;
		rc1.right = rc1.left + nWidth;		
		rc1.bottom = top +1;
		rc1.top = rc1.bottom - nHeight;
		m_BoardSelectDlg.MoveWindow(&rc1);	
	}
}


void CAuthorDlg::ArrangeControlSmallMode()
{	
	CRect	rc, rc1;	
	rc.left = -1;
	rc.right = m_WidthScreen;	
	rc.top = 0;	
	rc.bottom = 63;

	MoveWindow(&rc);

	rc1.top = 15;
	rc1.left = 1;
	rc1.bottom = rc1.top + 33;
	rc1.right = rc1.left + 33;
	m_ShowhideBtn.MoveWindow(&rc1);

	rc1.top = 4;	
	rc1.bottom = rc1.top + 9;
	rc1.left = 12;
	rc1.right = rc1.left + 10;
	m_MoveTopBtn.MoveWindow(&rc1);

	rc1.top = 50;	
	rc1.bottom = rc1.top + 9;	
	m_MoveBottomBtn.MoveWindow(&rc1);

	m_nMargine = 25;
	m_nControlStart = 90;	
	
	int nPPTStart = 295;

	m_rcRecState = CRect(m_nControlStart+28, 30, m_nControlStart+78, 40);
	m_rcRecTime = CRect(m_nControlStart+28, 20, m_nControlStart+74, 30);
	m_rcRecProgress = CRect(m_nControlStart + 80, 30, m_nControlStart + 98, 38);

	m_rcSmallModeShow[0].top = 3;	
	m_rcSmallModeShow[0].bottom = 63;
	m_rcSmallModeShow[0].left = 53;
	m_rcSmallModeShow[0].right = m_rcSmallModeShow[0].left + 60;
	m_ModeBtn.MoveWindow(&m_rcSmallModeShow[0]);

	m_rcSmallModeShow[1].top = 3;	
	m_rcSmallModeShow[1].bottom = 63;
	m_rcSmallModeShow[1].left = m_rcSmallModeShow[0].right + m_nMargine;
	m_rcSmallModeShow[1].right = m_rcSmallModeShow[1].left + 60;	
	m_PenBtn.MoveWindow(&m_rcSmallModeShow[1]);
	
	m_rcSmallModeShow[2].top = 3;	
	m_rcSmallModeShow[2].bottom = 63;
	m_rcSmallModeShow[2].left = m_rcSmallModeShow[1].right + m_nMargine;
	m_rcSmallModeShow[2].right = m_rcSmallModeShow[2].left + 60;
	m_TextBtn.MoveWindow(&m_rcSmallModeShow[2]);
		
	m_rcSmallModeShow[3].top = 3;	
	m_rcSmallModeShow[3].bottom = 63;
	m_rcSmallModeShow[3].left = m_rcSmallModeShow[2].right + m_nMargine;
	m_rcSmallModeShow[3].right = m_rcSmallModeShow[3].left + 60;
	m_EraserBtn.MoveWindow(&m_rcSmallModeShow[3]);

	m_rcSmallModeShow[4].top = 3;	
	m_rcSmallModeShow[4].bottom = 63;
	m_rcSmallModeShow[4].left = m_rcSmallModeShow[3].right + m_nMargine;
	m_rcSmallModeShow[4].right = m_rcSmallModeShow[4].left + 60;
	m_ColorChangeBtn.MoveWindow(&m_rcSmallModeShow[4]);

	m_rcSmallModeShow[5].top = 3;	
	m_rcSmallModeShow[5].bottom = 63;
	m_rcSmallModeShow[5].left = m_rcSmallModeShow[4].right + m_nMargine;
	m_rcSmallModeShow[5].right = m_rcSmallModeShow[5].left + 60;
	m_LineWeightBtn.MoveWindow(&m_rcSmallModeShow[5]);

	m_rcSmallModeShow[6].top = 3;	
	m_rcSmallModeShow[6].bottom = 63;
	m_rcSmallModeShow[6].left = m_rcSmallModeShow[5].right + m_nMargine;
	m_rcSmallModeShow[6].right = m_rcSmallModeShow[6].left + 60;
	m_DiagramBtn.MoveWindow(&m_rcSmallModeShow[6]); 


	/////////////////////////////////////////////////////////////////////////
	m_rcSmallModeHide[0].top = 28;
	m_rcSmallModeHide[0].bottom = m_rcSmallModeHide[0].top + 14;
	m_rcSmallModeHide[0].left = m_nControlStart + 12;
	m_rcSmallModeHide[0].right = m_rcSmallModeHide[0].left + 16;
	m_MicBtn.MoveWindow(&m_rcSmallModeHide[0]);
	m_MicBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[1].top = 6;
	m_rcSmallModeHide[1].bottom = m_rcSmallModeHide[1].top + 15;
	m_rcSmallModeHide[1].left = m_nControlStart + 140;
	m_rcSmallModeHide[1].right = m_rcSmallModeHide[1].left + 13;
	m_HelpBtn.MoveWindow(&m_rcSmallModeHide[1]);
	m_HelpBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[2].top = 48;
	m_rcSmallModeHide[2].bottom = m_rcSmallModeHide[2].top + 14;
	m_rcSmallModeHide[2].left = m_nControlStart + 6;
	m_rcSmallModeHide[2].right = m_rcSmallModeHide[2].left + 28;
	m_OptionBtn.MoveWindow(&m_rcSmallModeHide[2]);
	m_OptionBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[3].top = 42;
	m_rcSmallModeHide[3].bottom = m_rcSmallModeHide[3].top + 14;
	m_rcSmallModeHide[3].left = m_nControlStart + 46;
	m_rcSmallModeHide[3].right = m_rcSmallModeHide[3].left + 28;
	m_RecordBtn.MoveWindow(&m_rcSmallModeHide[3]);
	m_RecordBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[4].top = 42;
	m_rcSmallModeHide[4].bottom = m_rcSmallModeHide[4].top + 14;
	m_rcSmallModeHide[4].left = m_nControlStart+75;
	m_rcSmallModeHide[4].right = m_rcSmallModeHide[4].left + 28;
	m_RecordPauseBtn.MoveWindow(&m_rcSmallModeHide[4]);	
	m_RecordPauseBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[5].top = 42;
	m_rcSmallModeHide[5].bottom = m_rcSmallModeHide[5].top + 14;
	m_rcSmallModeHide[5].left = m_nControlStart + 104;
	m_rcSmallModeHide[5].right = m_rcSmallModeHide[5].left + 28;
	m_RecordStopBtn.MoveWindow(&m_rcSmallModeHide[5]);
	m_RecordStopBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[6].top = 26;
	m_rcSmallModeHide[6].bottom = m_rcSmallModeHide[6].top + 14;
	m_rcSmallModeHide[6].left = m_nControlStart + 109;
	m_rcSmallModeHide[6].right = m_rcSmallModeHide[6].left + 28;
	m_WmvBtn.MoveWindow(&m_rcSmallModeHide[6]);	
	m_WmvBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[7].top = 7;	
	m_rcSmallModeHide[7].bottom = m_rcSmallModeHide[7].top + 25;
	m_rcSmallModeHide[7].left = nPPTStart;
	m_rcSmallModeHide[7].right = m_rcSmallModeHide[7].left + 50;
	m_PPTNextBtn.MoveWindow(&m_rcSmallModeHide[7]);
	m_PPTNextBtn.ShowWindow(SW_HIDE);
	
	m_rcSmallModeHide[8].top = m_rcSmallModeHide[7].bottom;
	m_rcSmallModeHide[8].bottom = m_rcSmallModeHide[8].top +25;
	m_rcSmallModeHide[8].left = nPPTStart;
	m_rcSmallModeHide[8].right = m_rcSmallModeHide[8].left + 50;
	m_PPTPrevBtn.MoveWindow(&m_rcSmallModeHide[8]);
	m_PPTPrevBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[9].top = 3;
	m_rcSmallModeHide[9].bottom = 63;
	m_rcSmallModeHide[9].left = m_rcSmallModeHide[8].right + m_nMargine+8;
	m_rcSmallModeHide[9].right = m_rcSmallModeHide[9].left + 60;
	m_BoardOpenBtn.MoveWindow(&m_rcSmallModeHide[9]);
	m_BoardOpenBtn.ShowWindow(SW_HIDE);

	m_rcSmallModeHide[10].top = 3;
	m_rcSmallModeHide[10].bottom = 63;
	m_rcSmallModeHide[10].left = m_rcSmallModeHide[9].right + m_nMargine+2;
	m_rcSmallModeHide[10].right = m_rcSmallModeHide[10].left + 60;
	m_ZoomBtn.MoveWindow(&m_rcSmallModeHide[10]);
	m_ZoomBtn.ShowWindow(SW_HIDE);
	
	m_rcSmallModeHide[11].top = 3;
	m_rcSmallModeHide[11].bottom = 63;
	m_rcSmallModeHide[11].left = m_rcSmallModeHide[10].right + m_nMargine+2;
	m_rcSmallModeHide[11].right = m_rcSmallModeHide[11].left + 60;
	m_SnapshotBtn.MoveWindow(&m_rcSmallModeHide[11]);
	m_SnapshotBtn.ShowWindow(SW_HIDE);
	
	/////////////////////////////////////////////////////////////////////////
	m_rcSmallModeSlide.top = 44;	
	m_rcSmallModeSlide.bottom = 57;
	m_rcSmallModeSlide.left = m_WidthScreen - 90;
	m_rcSmallModeSlide.right = m_rcSmallModeSlide.left + 78;
	m_SlideMenuBtn.MoveWindow(&m_rcSmallModeSlide);

	m_rcSmallModeCamera.top = 4;
	m_rcSmallModeCamera.bottom = m_rcSmallModeCamera.top + 29;	
	m_rcSmallModeCamera.left = m_WidthScreen - 110;
	m_rcSmallModeCamera.right = m_rcSmallModeCamera.left + 28;
	m_CameraBtn.MoveWindow(&m_rcSmallModeCamera);	

	m_rcSmallModePrint.top = 4;
	m_rcSmallModePrint.bottom = m_rcSmallModePrint.top + 28;	
	m_rcSmallModePrint.left = m_rcSmallModeCamera.right + 12;
	m_rcSmallModePrint.right = m_rcSmallModePrint.left + 28;
	m_PrintBtn.MoveWindow(&m_rcSmallModePrint);
	
	m_rcSmallModeExit.top = 8;	
	m_rcSmallModeExit.bottom = m_rcSmallModeExit.top + 23;
	m_rcSmallModeExit.left = m_rcSmallModePrint.right + 10;
	m_rcSmallModeExit.right = m_rcSmallModeExit.left + 23;
	m_ExitBtn.MoveWindow(&m_rcSmallModeExit);

	ArrangePopupMenu();
}

void CAuthorDlg::CalFoldControl()
{	
	m_nShowControlCount = 0;
	for(int i=0; i<13; i++)
	{
		if(m_bShowControl[i])
			m_nShowControlCount++;
	}

	int nCount, nControlStart, nNextStart;
	nCount = 0;	

	nNextStart = 43;
	if(m_bShowControl[0])
	{
		nNextStart = m_rcHide[0].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[1])
	{
		m_rcHide[1].left = nNextStart;
		m_rcHide[1].right = m_rcHide[1].left + 60;

		nNextStart = m_rcHide[1].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[2])
	{
		m_rcHide[2].left = nNextStart;
		m_rcHide[2].right = m_rcHide[2].left + 60;
		
		nNextStart = m_rcHide[2].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[3])
	{
		m_rcHide[3].left = nNextStart;
		m_rcHide[3].right = m_rcHide[3].left + 60;

		nNextStart = m_rcHide[3].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[4])
	{
		m_rcHide[4].left = nNextStart;
		m_rcHide[4].right = m_rcHide[4].left + 60;

		nNextStart = m_rcHide[4].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[5])
	{
		m_rcHide[5].left = nNextStart;
		m_rcHide[5].right = m_rcHide[5].left + 60;

		nNextStart = m_rcHide[5].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[6])
	{
		m_rcHide[6].left = nNextStart;
		m_rcHide[6].right = m_rcHide[6].left + 60;

		nNextStart = m_rcHide[6].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[7])
	{	
		if(nCount > 0)
			m_nHideControlStart = nControlStart = 43 + (60+m_nMargine)*nCount - m_nMargine + m_nControlMargine;
		else
			m_nHideControlStart = nControlStart = 43;

		m_rcHide[7].left = nControlStart + 12;
		m_rcHide[7].right = m_rcHide[7].left + 16;

		m_rcHide[8].left = nControlStart + 140;
		m_rcHide[8].right = m_rcHide[8].left + 13;

		m_rcHide[9].left = nControlStart + 6;
		m_rcHide[9].right = m_rcHide[9].left + 28;

		m_rcHide[10].left = nControlStart + 46;
		m_rcHide[10].right = m_rcHide[10].left + 28;
		
		m_rcHide[11].left = nControlStart + 75;
		m_rcHide[11].right = m_rcHide[11].left + 28;
		
		m_rcHide[12].left = nControlStart + 104;
		m_rcHide[12].right = m_rcHide[12].left + 28;
		
		m_rcHide[13].left = nControlStart + 109;
		m_rcHide[13].right = m_rcHide[13].left + 28;

		m_rcHideRecState.left = m_nHideControlStart + 28;
		m_rcHideRecState.right = m_rcHideRecState.left + 50;

		m_rcHideRecTime.left = m_nHideControlStart + 28;
		m_rcHideRecTime.right = m_rcHideRecTime.left + 46;

		m_rcHideRecProgress.left = m_nHideControlStart + 80;
		m_rcHideRecProgress.right = m_rcHideRecProgress.left + 18;

		nNextStart = nControlStart + m_nControlWidth + m_nControlMargine;
	}

	
	if(m_bShowControl[8])
	{	
		m_rcHide[15].left = m_rcHide[14].left = nNextStart;		
		m_rcHide[15].right = m_rcHide[14].right = m_rcHide[14].left + 50;

		nNextStart = m_rcHide[15].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[9])
	{		
		m_rcHide[16].left = nNextStart;
		m_rcHide[16].right = m_rcHide[16].left + 60;

		nNextStart = m_rcHide[16].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[10])
	{		
		m_rcHide[17].left = nNextStart;		
		m_rcHide[17].right = m_rcHide[17].left + 60;		

		nNextStart = m_rcHide[17].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[11])
	{
		m_rcHide[18].left = nNextStart;
		m_rcHide[18].right = m_rcHide[18].left + 60;

		nNextStart = m_rcHide[18].right + m_nMargine;
		nCount++;
	}

	if(m_bShowControl[12])
	{		
		if(m_nShowControlCount == 1)
		{
			m_nFoldWidth = 43 + m_nSlideBGWidth;
			m_nHideSlideStart = 43;
		}
		else if(m_nShowControlCount == 13)
		{
			m_nFoldWidth = m_WidthScreen;
			m_nHideSlideStart = m_nFoldWidth - 114;
		}
		else			
		{
			m_nFoldWidth = nNextStart + m_nSlideBGWidth;
			m_nHideSlideStart = nNextStart;
		}	

		m_rcHide[19].left = m_nFoldWidth - 90;
		m_rcHide[19].right = m_rcHide[19].left + 78;

		m_rcHide[20].left = m_nFoldWidth - 110;
		m_rcHide[20].right = m_rcHide[20].left + 28;

		m_rcHide[21].left = m_rcHide[20].right +12;
		m_rcHide[21].right = m_rcHide[21].left + 28;

		m_rcHide[22].left = m_rcHide[21].right + 10;
		m_rcHide[22].right = m_rcHide[22].left + 23;
	}
	else
	{		
		if(m_nShowControlCount == 0)
			m_nFoldWidth = 42;
		else
			m_nFoldWidth = nNextStart;
	}
}


void CAuthorDlg::FoldDialog()
{	
	if(m_nShowControlCount == 0)
	{
		CRect rc;
		GetWindowRect(&rc);
		rc.right = m_nFoldWidth;

		MoveWindow(&rc);
		m_bFold = true;
		return;
	}

	if(m_bShowControl[0])
	{
		if(m_rcShow[0] != m_rcHide[0])
			m_ModeBtn.MoveWindow(&m_rcHide[0]);
	}
	else
		m_ModeBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[1])
	{
		if(m_rcShow[1] != m_rcHide[1])
			m_PenBtn.MoveWindow(&m_rcHide[1]);
	}
	else
		m_PenBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[2])
	{
		if(m_rcShow[2] != m_rcHide[2])
			m_TextBtn.MoveWindow(&m_rcHide[2]);
	}
	else
		m_TextBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[3])
	{
		if(m_rcShow[3] != m_rcHide[3])
			m_EraserBtn.MoveWindow(&m_rcHide[3]);
	}
	else
		m_EraserBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[4])
	{
		if(m_rcShow[4] != m_rcHide[4])
			m_ColorChangeBtn.MoveWindow(&m_rcHide[4]);
	}
	else
		m_ColorChangeBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[5])
	{
		if(m_rcShow[5] != m_rcHide[5])
			m_LineWeightBtn.MoveWindow(&m_rcHide[5]);
	}
	else
		m_LineWeightBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[6])
	{
		if(m_rcShow[6] != m_rcHide[6])
			m_DiagramBtn.MoveWindow(&m_rcHide[6]);
	}
	else
		m_DiagramBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[7])
	{	
		if(m_rcShow[7] != m_rcHide[7])
		{
			m_MicBtn.MoveWindow(&m_rcHide[7]);
			m_HelpBtn.MoveWindow(&m_rcHide[8]);
			m_OptionBtn.MoveWindow(&m_rcHide[9]);
			m_RecordBtn.MoveWindow(&m_rcHide[10]);
			m_RecordPauseBtn.MoveWindow(&m_rcHide[11]);
			m_RecordStopBtn.MoveWindow(&m_rcHide[12]);
			m_WmvBtn.MoveWindow(&m_rcHide[13]);
		}
	}
	else
	{	
		m_MicBtn.ShowWindow(SW_HIDE);
		m_HelpBtn.ShowWindow(SW_HIDE);
		m_OptionBtn.ShowWindow(SW_HIDE);
		m_RecordBtn.ShowWindow(SW_HIDE);
		m_RecordPauseBtn.ShowWindow(SW_HIDE);
		m_RecordStopBtn.ShowWindow(SW_HIDE);
		m_WmvBtn.ShowWindow(SW_HIDE);
	}

	if(m_bShowControl[8])
	{
		if(m_rcShow[14] != m_rcHide[14])
		{
			m_PPTNextBtn.MoveWindow(&m_rcHide[14]);
			m_PPTPrevBtn.MoveWindow(&m_rcHide[15]);

			m_PPTNextBtn.MakeRgn();
			m_PPTPrevBtn.MakeRgn();
		}
	}
	else
	{
		m_PPTNextBtn.ShowWindow(SW_HIDE);
		m_PPTPrevBtn.ShowWindow(SW_HIDE);
	}

	if(m_bShowControl[9])
	{
		if(m_rcShow[16] != m_rcHide[16])
			m_BoardOpenBtn.MoveWindow(&m_rcHide[16]);
	}
	else
		m_BoardOpenBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[10])
	{
		if(m_rcShow[17] != m_rcHide[17])
			m_ZoomBtn.MoveWindow(&m_rcHide[17]);
	}
	else
		m_ZoomBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[11])
	{
		if(m_rcShow[18] != m_rcHide[18])
			m_SnapshotBtn.MoveWindow(&m_rcHide[18]);
	}
	else
		m_SnapshotBtn.ShowWindow(SW_HIDE);

	if(m_bShowControl[12])
	{
		if(m_rcShow[19] != m_rcHide[19])
		{
			m_SlideMenuBtn.MoveWindow(&m_rcHide[19]);
			m_CameraBtn.MoveWindow(&m_rcHide[20]);
			m_PrintBtn.MoveWindow(&m_rcHide[21]);
			m_ExitBtn.MoveWindow(&m_rcHide[22]);
		}
	}
	else
	{
		m_SlideMenuBtn.ShowWindow(SW_HIDE);
		m_CameraBtn.ShowWindow(SW_HIDE);
		m_PrintBtn.ShowWindow(SW_HIDE);
		m_ExitBtn.ShowWindow(SW_HIDE);
	}		
	
	ArrangePopupMenu();

	CRect rc;
	GetWindowRect(&rc);
	rc.right = m_nFoldWidth;

	MoveWindow(&rc);
	// vista 에서 폴딩시 보드화면 리프레쉬가 안됨
	if (m_bBoardOpen)
		m_BoardDlg.UpdateWindow();
	m_bFold = true;
}


void CAuthorDlg::UnFoldDialog()
{	
	if(m_nShowControlCount == 0)
	{		
		CRect rc;
		GetWindowRect(&rc);
		rc.right = m_WidthScreen;

		MoveWindow(&rc);
		m_bFold = false;
		return;
	}

	if(m_bShowControl[0])
	{
		if(m_rcShow[0] != m_rcHide[0])
			m_ModeBtn.MoveWindow(&m_rcShow[0]);
	}
	else
		m_ModeBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[1])
	{
		if(m_rcShow[1] != m_rcHide[1])
			m_PenBtn.MoveWindow(&m_rcShow[1]);
	}
	else
		m_PenBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[2])
	{
		if(m_rcShow[2] != m_rcHide[2])
			m_TextBtn.MoveWindow(&m_rcShow[2]);
	}
	else
		m_TextBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[3])
	{
		if(m_rcShow[3] != m_rcHide[3])
			m_EraserBtn.MoveWindow(&m_rcShow[3]);
	}
	else
		m_EraserBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[4])
	{
		if(m_rcShow[4] != m_rcHide[4])
			m_ColorChangeBtn.MoveWindow(&m_rcShow[4]);
	}
	else
		m_ColorChangeBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[5])
	{
		if(m_rcShow[5] != m_rcHide[5])
			m_LineWeightBtn.MoveWindow(&m_rcShow[5]);
	}
	else
		m_LineWeightBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[6])
	{
		if(m_rcShow[6] != m_rcHide[6])
			m_DiagramBtn.MoveWindow(&m_rcShow[6]);
	}
	else
		m_DiagramBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[7])
	{	
		if(m_rcShow[7] != m_rcHide[7])
		{
			m_MicBtn.MoveWindow(&m_rcShow[7]);
			m_HelpBtn.MoveWindow(&m_rcShow[8]);
			m_OptionBtn.MoveWindow(&m_rcShow[9]);
			m_RecordBtn.MoveWindow(&m_rcShow[10]);
			m_RecordPauseBtn.MoveWindow(&m_rcShow[11]);
			m_RecordStopBtn.MoveWindow(&m_rcShow[12]);
			m_WmvBtn.MoveWindow(&m_rcShow[13]);
		}
	}
	else
	{	
		m_MicBtn.ShowWindow(SW_SHOW);
		m_HelpBtn.ShowWindow(SW_SHOW);
		m_OptionBtn.ShowWindow(SW_SHOW);
		m_RecordBtn.ShowWindow(SW_SHOW);
		m_RecordPauseBtn.ShowWindow(SW_SHOW);
		m_RecordStopBtn.ShowWindow(SW_SHOW);
		m_WmvBtn.ShowWindow(SW_SHOW);
	}

	if(m_bShowControl[8])
	{
		if(m_rcShow[14] != m_rcHide[14])
		{
			m_PPTNextBtn.MoveWindow(&m_rcShow[14]);
			m_PPTPrevBtn.MoveWindow(&m_rcShow[15]);

			m_PPTNextBtn.MakeRgn();
			m_PPTPrevBtn.MakeRgn();
		}
	}
	else
	{
		m_PPTNextBtn.ShowWindow(SW_SHOW);
		m_PPTPrevBtn.ShowWindow(SW_SHOW);
	}

	if(m_bShowControl[9])
	{
		if(m_rcShow[16] != m_rcHide[16])
			m_BoardOpenBtn.MoveWindow(&m_rcShow[16]);
	}
	else
		m_BoardOpenBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[10])
	{
		if(m_rcShow[17] != m_rcHide[17])
			m_ZoomBtn.MoveWindow(&m_rcShow[17]);
	}
	else
		m_ZoomBtn.ShowWindow(SW_SHOW);

	if(m_bShowControl[11])
	{
		if(m_rcShow[18] != m_rcHide[18])
			m_SnapshotBtn.MoveWindow(&m_rcShow[18]);
	}
	else
		m_SnapshotBtn.ShowWindow(SW_SHOW);
	
	m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);
	m_CameraBtn.MoveWindow(&m_rcShow[20]);
	m_PrintBtn.MoveWindow(&m_rcShow[21]);
	m_ExitBtn.MoveWindow(&m_rcShow[22]);
		
	if(!m_bShowControl[12])
	{
		m_SlideMenuBtn.ShowWindow(SW_SHOW);
		m_CameraBtn.ShowWindow(SW_SHOW);
		m_PrintBtn.ShowWindow(SW_SHOW);
		m_ExitBtn.ShowWindow(SW_SHOW);
	}

	ArrangePopupMenu();
	UpdateWindow();
	
	CRect rc;
	GetWindowRect(&rc);
	rc.right = m_WidthScreen;

	MoveWindow(&rc);
	m_bFold = false;
}


void CAuthorDlg::SmallModeFold()
{
	if(m_ShowhideBtn.IsSelected())
	{
		m_WmvBtn.ShowWindow(SW_SHOW);
		m_SnapshotBtn.ShowWindow(SW_SHOW);
		m_ZoomBtn.ShowWindow(SW_SHOW);
		m_BoardOpenBtn.ShowWindow(SW_SHOW);	
		m_HelpBtn.ShowWindow(SW_SHOW);
		m_MicBtn.ShowWindow(SW_SHOW);
		m_OptionBtn.ShowWindow(SW_SHOW);
		m_PPTNextBtn.ShowWindow(SW_SHOW);
		m_PPTPrevBtn.ShowWindow(SW_SHOW);
		m_RecordStopBtn.ShowWindow(SW_SHOW);
		m_RecordPauseBtn.ShowWindow(SW_SHOW);
		m_RecordBtn.ShowWindow(SW_SHOW);
		
		m_DiagramBtn.ShowWindow(SW_HIDE);
		m_LineWeightBtn.ShowWindow(SW_HIDE);
		m_ColorChangeBtn.ShowWindow(SW_HIDE);	
		m_EraserBtn.ShowWindow(SW_HIDE);
		m_TextBtn.ShowWindow(SW_HIDE);	
		m_PenBtn.ShowWindow(SW_HIDE);
		m_ModeBtn.ShowWindow(SW_HIDE);
	}
	else
	{
		m_WmvBtn.ShowWindow(SW_HIDE);
		m_SnapshotBtn.ShowWindow(SW_HIDE);
		m_ZoomBtn.ShowWindow(SW_HIDE);
		m_BoardOpenBtn.ShowWindow(SW_HIDE);	
		m_HelpBtn.ShowWindow(SW_HIDE);
		m_MicBtn.ShowWindow(SW_HIDE);
		m_OptionBtn.ShowWindow(SW_HIDE);
		m_PPTNextBtn.ShowWindow(SW_HIDE);
		m_PPTPrevBtn.ShowWindow(SW_HIDE);
		m_RecordStopBtn.ShowWindow(SW_HIDE);
		m_RecordPauseBtn.ShowWindow(SW_HIDE);
		m_RecordBtn.ShowWindow(SW_HIDE);
		
		m_DiagramBtn.ShowWindow(SW_SHOW);
		m_LineWeightBtn.ShowWindow(SW_SHOW);
		m_ColorChangeBtn.ShowWindow(SW_SHOW);	
		m_EraserBtn.ShowWindow(SW_SHOW);
		m_TextBtn.ShowWindow(SW_SHOW);	
		m_PenBtn.ShowWindow(SW_SHOW);
		m_ModeBtn.ShowWindow(SW_SHOW);
	}

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAuthorDlg::OnShowhidetoolbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_bSmallMode)
	{
		SmallModeFold();
	}
	else
	{
		if(m_ShowhideBtn.IsSelected())
			FoldDialog();
		else
			UnFoldDialog();
	}
}

void CAuthorDlg::OnMovetopbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_bMoveTop)
		return;

	m_bMoveTop = true;

	CRect	rc, rc1;	
	rc.left = 0;

	if(m_bSmallMode)
	{
		rc.top = -1;	
		rc.bottom = 63;
		rc.right = m_WidthScreen;
	
		m_rcSmallModeSlide.top = 44;	m_rcSmallModeSlide.bottom = 57;
		m_rcSmallModeCamera.top = 4;	m_rcSmallModeCamera.bottom = m_rcSmallModeCamera.top + 29;
		m_rcSmallModePrint.top = 4;	m_rcSmallModePrint.bottom = m_rcSmallModePrint.top + 28;
		m_rcSmallModeExit.top = 8;	m_rcSmallModeExit.bottom = m_rcSmallModeExit.top + 23;	

		m_SlideMenuBtn.MoveWindow(&m_rcSmallModeSlide);	
		m_CameraBtn.MoveWindow(&m_rcSmallModeCamera);
		m_PrintBtn.MoveWindow(&m_rcSmallModePrint);			
		m_ExitBtn.MoveWindow(&m_rcSmallModeExit);
		
		rc1.top = 0; rc1.bottom = 64; 
		rc1.left = rc.right - 120;
		rc1.right = rc.right;

		InvalidateRect(&rc1);

		UpdateWindow();
		MoveWindow(&rc);		

		if(m_ShowhideBtn.IsSelected())
		{
			m_PPTNextBtn.MakeRgn();
			m_PPTPrevBtn.MakeRgn();
		}

		ArrangePopupMenu();

		return;
	}

	m_rcShow[19].top = 44;	m_rcShow[19].bottom = 57;
	m_rcShow[20].top = 4;	m_rcShow[20].bottom = m_rcShow[20].top + 29;
	m_rcShow[21].top = 4;	m_rcShow[21].bottom = m_rcShow[21].top + 28;
	m_rcShow[22].top = 8;	m_rcShow[22].bottom = m_rcShow[22].top + 23;

	if(m_bShowControl[12])
	{
		m_rcHide[19].top = 44;	m_rcHide[19].bottom = 57;
		m_rcHide[20].top = 4;	m_rcHide[20].bottom = m_rcShow[20].top + 29;
		m_rcHide[21].top = 4;	m_rcHide[21].bottom = m_rcShow[21].top + 28;
		m_rcHide[22].top = 8;	m_rcHide[22].bottom = m_rcShow[22].top + 23;
	}

	if(m_ShowhideBtn.IsSelected())
	{
		rc.right = m_nFoldWidth;

		if(m_bShowControl[12])
		{
			m_SlideMenuBtn.MoveWindow(&m_rcHide[19]);
			m_CameraBtn.MoveWindow(&m_rcHide[20]);
			m_PrintBtn.MoveWindow(&m_rcHide[21]);			
			m_ExitBtn.MoveWindow(&m_rcHide[22]);

			rc1.top = 0; rc1.bottom = 64; 
			rc1.left = m_nFoldWidth - 120;
			rc1.right = m_nFoldWidth;

			InvalidateRect(&rc1);
		}
		else
		{
			m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);
			m_CameraBtn.MoveWindow(&m_rcShow[20]);
			m_PrintBtn.MoveWindow(&m_rcShow[21]);
			m_ExitBtn.MoveWindow(&m_rcShow[22]);
		}
	}
	else
	{
		rc.right = m_WidthScreen;	
			
		m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);
		m_CameraBtn.MoveWindow(&m_rcShow[20]);
		m_PrintBtn.MoveWindow(&m_rcShow[21]);
		m_ExitBtn.MoveWindow(&m_rcShow[22]);

		rc1.top = 0; rc1.bottom = 64; 
		rc1.left = rc.right - 120;
		rc1.right = rc.right;

		InvalidateRect(&rc1);	
	}
	
	rc.top = -1;	
	rc.bottom = 63;
	
	UpdateWindow();
	MoveWindow(&rc);
	
	m_PPTNextBtn.MakeRgn();
	m_PPTPrevBtn.MakeRgn();

	ArrangePopupMenu();
}


void CAuthorDlg::OnMovebottombtn() 
{
	// TODO: Add your control notification handler code here
	if(!m_bMoveTop)
		return;	

	m_bMoveTop = false;
	
	CRect	rc, rc1;	
	rc.left = 0;

	if(m_bSmallMode)
	{
		rc.top = m_HeightScreen-64;	
		rc.bottom = m_HeightScreen;
		rc.right = m_WidthScreen;

		m_rcSmallModeSlide.top = 6;	m_rcSmallModeSlide.bottom = 19;
		m_rcSmallModeCamera.top = 32;	m_rcSmallModeCamera.bottom = m_rcSmallModeCamera.top + 29;
		m_rcSmallModePrint.top = 32;	m_rcSmallModePrint.bottom = m_rcSmallModePrint.top + 28;
		m_rcSmallModeExit.top = 36;	m_rcSmallModeExit.bottom = m_rcSmallModeExit.top + 23;	

		m_SlideMenuBtn.MoveWindow(&m_rcSmallModeSlide);	
		m_CameraBtn.MoveWindow(&m_rcSmallModeCamera);
		m_PrintBtn.MoveWindow(&m_rcSmallModePrint);
		m_ExitBtn.MoveWindow(&m_rcSmallModeExit);
		
		rc1.top = 0; rc1.bottom = 64; 
		rc1.left = rc.right - 120;
		rc1.right = rc.right;

		InvalidateRect(&rc1);

		UpdateWindow();
		MoveWindow(&rc);		

		if(m_ShowhideBtn.IsSelected())
		{
			m_PPTNextBtn.MakeRgn();
			m_PPTPrevBtn.MakeRgn();
		}

		ArrangePopupMenu();

		return;
	}

	m_rcHide[19].top = m_rcShow[19].top = 6;
	m_rcHide[19].bottom = m_rcShow[19].bottom = 19;
	m_rcHide[20].top = m_rcShow[20].top = 32;
	m_rcHide[20].bottom = m_rcShow[20].bottom = m_rcShow[20].top + 29;
	m_rcHide[21].top = m_rcShow[21].top = 32;
	m_rcHide[21].bottom = m_rcShow[21].bottom = m_rcShow[21].top + 28;
	m_rcHide[22].top = m_rcShow[22].top = 36;
	m_rcHide[22].bottom = m_rcShow[22].bottom = m_rcShow[22].top + 23;
	

	if(m_ShowhideBtn.IsSelected())
	{
		rc.right = m_nFoldWidth;

		if(m_bShowControl[12])
		{
			m_SlideMenuBtn.MoveWindow(&m_rcHide[19]);
			m_CameraBtn.MoveWindow(&m_rcHide[20]);
			m_PrintBtn.MoveWindow(&m_rcHide[21]);
			m_ExitBtn.MoveWindow(&m_rcHide[22]);

			rc1.top = 0; rc1.bottom = 64; 
			rc1.left = m_nFoldWidth - 120;
			rc1.right = m_nFoldWidth;

			InvalidateRect(&rc1);
		}
		else
		{
			m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);
			m_CameraBtn.MoveWindow(&m_rcShow[20]);
			m_PrintBtn.MoveWindow(&m_rcShow[21]);
			m_ExitBtn.MoveWindow(&m_rcShow[22]);
		}
	}
	else
	{
		rc.right = m_WidthScreen;	

		
		m_SlideMenuBtn.MoveWindow(&m_rcShow[19]);
		m_CameraBtn.MoveWindow(&m_rcShow[20]);
		m_PrintBtn.MoveWindow(&m_rcShow[21]);
		m_ExitBtn.MoveWindow(&m_rcShow[22]);

		rc1.top = 0; rc1.bottom = 64; 
		rc1.left = rc.right - 120;
		rc1.right = rc.right;
		InvalidateRect(&rc1);
	}

	rc.top = m_HeightScreen-64;	
	rc.bottom = m_HeightScreen;	
	
	UpdateWindow();
	MoveWindow(&rc);
	
	m_PPTNextBtn.MakeRgn();
	m_PPTPrevBtn.MakeRgn();
	ArrangePopupMenu();
}


void CAuthorDlg::OnModebtn() 
{
	// TODO: Add your control notification handler code here	
	if(m_ModeBtn.IsSelected())
	{
		SetImageCursor();
		OpenBoard(DESKTOP_BOARD);
	}
	else
	{	
		if(m_bBoardOpen)
		{	
			if(m_strSlidePath.IsEmpty())
			{
				if(m_bDrawCanvas)
				{
					CString strPath;
					strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);
					if(CaptureDesktop(strPath))
					{				
					//	strPath.Format("Slide%d", m_nSlideIndex);
						m_SlideMenuDlg.AddSlide(strPath);

						m_nSlideIndex++;
					}
				}
			}
			else if(m_bDrawCanvas)
			{				
				CaptureDesktop(m_strSlidePath);
				m_SlideMenuDlg.ReplaceSildeThumbnail(m_strSlidePath);
				m_strSlidePath = "";
			}

			m_BoardDlg.ShowWindow(SW_HIDE);

			SetPPTSlideShowHide(false);
			m_bBoardOpen = false;
		}
		
		SetImageCursor();
		
		m_bDrawCanvas = false;

		if(m_bZoomMode)
		{
			m_bZoomMode = false;

			switch(m_nPenToolType)
			{
			case	PENTOOL:			
				SetPenType(m_nPenType);		
				break;
			case	ERASERTOOL:
				SetPenType(m_nEraserType);				
				break;		
			case	DIAGRAMTOOL:
				SetPenType(m_nDiagramType);				
				break;
			}
		}
	}
}

void CAuthorDlg::OnPenbtn() 
{
	// TODO: Add your control notification handler code here
	ShowPopupMenu();
}

void CAuthorDlg::OnTextBtn() 
{
	// TODO: Add your control notification handler code here
	if (!m_bBoardOpen || !m_pCanvas)
	{
		Beep(2100,9);
		return;
	}

	EnableWindow(FALSE);

	m_TextBtn.SetSelect(FALSE);
	switch(m_nPenToolType)
	{
	case	PENTOOL:
		m_PenBtn.SetSelect();
		break;
	case	ERASERTOOL:
		m_EraserBtn.SetSelect();
		break;
	case	DIAGRAMTOOL:
		m_DiagramBtn.SetSelect();
		break;
	}

	m_bTextRange = true;
	m_bZoomMode = true;		
	
	m_pen.dtool = XInk::DT_Magnifier;
	m_pCanvas->SetPen(m_pen);
	
	HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_TEXTCURSOR);

	if(m_hCapture != NULL && m_bRecCursor)
		::srecSetCustomCursor(m_hCapture, hCursor);	
}


void CAuthorDlg::EndTextRange()
{		
	CTextDlg *TextDlg = new CTextDlg;
	TextDlg->Create(IDD_TEXTDLG, NULL);
	TextDlg->SetParent(this);
	TextDlg->SetProperty(m_bgTextColor, m_Color, m_PenWidth);
	TextDlg->MoveWindow(m_rcZoom);	
	TextDlg->ShowWindow(SW_SHOW);

	m_TextList.AddTail((CTextDlg*)TextDlg);

	m_rcZoom = CRect(0,0,0,0);
}


void CAuthorDlg::OnNotifyTextInputComplete()
{
	if(m_bTextRange)
	{
		if(m_ModeBtn.IsSelected())
		{
			m_pCanvas->RedrawAll();
		}		

		EnableWindow();
		m_bTextRange = false;
		SetFocus();

		SetPenType(m_nPenType);	
		SetImageCursor();
	}
}

LRESULT CAuthorDlg::OnTextDelete(WPARAM wp, LPARAM lp)
{
	CTextDlg *pDelClip = (CTextDlg*)wp;

	CTextDlg *pFindClip;
	POSITION pos = m_TextList.GetHeadPosition(), old_pos;
	for (int i=0;i < m_TextList.GetCount();i++)
	{
	   old_pos = pos;
	   pFindClip = (CTextDlg*)m_TextList.GetNext(pos);
	   if(pFindClip == pDelClip)
	   {
		   delete	pDelClip;
		   m_TextList.RemoveAt(old_pos);
		   
		   if(m_bTextRange)
			   SetTimer(REDRAW_TIMER, 100, NULL);

		   return 1;
	   }
	}	
	
	return 0;
}


void CAuthorDlg::OnEraserbtn() 
{
	// TODO: Add your control notification handler code here
	ShowPopupMenu();
}


void CAuthorDlg::OnColorchangebtn() 
{
	// TODO: Add your control notification handler code here	
	ShowPopupMenu();
}

void CAuthorDlg::OnLineweightchangebtn() 
{
	// TODO: Add your control notification handler code here
	ShowPopupMenu();
}

void CAuthorDlg::OnDiagrambtn() 
{
	// TODO: Add your control notification handler code here
	ShowPopupMenu();
}


void CAuthorDlg::OnOptionbtn() 
{
	// TODO: Add your control notification handler code here
	OnRecordpausebtn();

	COptionDlg OptionDlg;
	int nID = OptionDlg.DoModal();

	if(nID == IDOK)
	{
		if(OptionDlg.m_bModifyRegistration)
			OnOK();

		if(OptionDlg.m_bModifyPenFolder)
			ReadPenFolderOption();			

		if(OptionDlg.m_bModifyRecord || OptionDlg.m_bModifyLogo)
			ReadRecordOption();

		if(OptionDlg.m_bModifyToolbar)
		{
			bool bFold = m_bFold;
			if(bFold)
				UnFoldDialog();

			ReadToolbarOption();

			if(bFold)
				FoldDialog();
		}

		if(OptionDlg.m_bModifySnapshot)
			ReadSnapshotOption();

		if(OptionDlg.m_bModifyText)
			ReadTextOption();

		if(OptionDlg.m_bModifyCamera)
			ReadCameraOption();

		if(OptionDlg.m_bModifyCodec)
			ReadCodecOption();
	}
	else if(nID == IDCANCEL)
	{
		if(OptionDlg.m_bModifyRegistration)
			OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Record initialize
BOOL CAuthorDlg::OnReadyRecord()
{
	if(m_hCapture == NULL)
	{
		m_hCapture = ::srecCreateCapture( "SRECSDK00001-147AA447D127-86AE49659587-924D3BEFF365" );

		if ( m_hCapture == NULL )
			return FALSE;
	}

	BOOL	hr;
	hr = ::srecSetAudioCaptureDevice( m_hCapture, DEFAULT_AUDIO_DEVICE );	// from first wavein device
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting audio capture device");
		return FALSE;
	}

	hr = ::srecSetVideoCaptureDevice( m_hCapture, DEFAULT_VIDEO_DEVICE );	// from screen
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting Video capture device");
		return FALSE;
	}

	switch(m_nRecordRangeState)
	{
	case	RECORD_WINDOW:		
	case	RECORD_AREA:
		hr = ::srecSetScreenInput( m_hCapture, cinRegion, NULL, (LPRECT)m_rcCurRecordRangeWindow);
		break;
	default:	
		hr = ::srecSetScreenInput( m_hCapture, cinScreen, NULL, NULL );
		break;
	}
		
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting video input window");
		return FALSE;
	}

	hr = ::srecSetAviOptions( m_hCapture, m_dwFps, m_dwQuality, m_dwKeyFrameRate );
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting AVI option");
		return FALSE;
	}

	hr = ::srecSetAviVideoCodec( m_hCapture, vcmMSVC, m_bSelCodec);
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting video codec");
		return FALSE;
	}

	hr = ::srecSetAviAudioCodec( m_hCapture, acmGSM610 );	
	if(!hr)
	{
		hr = ::srecSetAviAudioCodec( m_hCapture, acmPCM );

		if(!hr)
		{
			if(m_hCapture)
			{
				::srecDestroyCapture( m_hCapture );
				m_hCapture = NULL;
			}

			AfxMessageBox("Error : Failed! setting audio codec");
			return FALSE;
		}
	}

	hr = ::srecSetRecordCursor( m_hCapture, m_bRecCursor);
	if(!hr)
	{
		if(m_hCapture)
		{
			::srecDestroyCapture( m_hCapture );
			m_hCapture = NULL;
		}

		AfxMessageBox("Error : Failed! setting recoding cursor");
		return FALSE;
	}

	SetEnableRecLayeredWnd();
	
	if(m_bRecLogo)
	{
		hr = ::srecSetRecordLogo( m_hCapture, TRUE);
		if(!hr)
		{
			if(m_hCapture)
			{
				::srecDestroyCapture( m_hCapture );
				m_hCapture = NULL;
			}

			AfxMessageBox("Error : Failed! setting recoding cursor");
			return FALSE;
		}

		hr = ::srecSetCustomLogo( m_hCapture, m_hBitmapLogo);
		if(!hr)
		{
			if(m_hCapture)
			{
				::srecDestroyCapture( m_hCapture );
				m_hCapture = NULL;
			}

			AfxMessageBox("Error : Failed! setting recoding cursor");
			return FALSE;
		}
	}
	
	CString strFilePath;
	strFilePath.Format("%s\\Notitle.avi", CAppSettings::GetWorkPath());
	hr = ::srecSetFileName( m_hCapture, strFilePath);
	if(!hr)
	{
		AfxMessageBox("Error : Failed! setting save file name");
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Select Record Range
void CAuthorDlg::DisplaySelectWindow(bool bSeletFlag)
{	
	if(bSeletFlag)
	{
		m_bSelectRecordWindow = false;
		KillTimer(RECORD_SELECTWINDOW_TIMER);
		m_rcPrevRecordRangeWindow = CRect(-1,-1,-1,-1);

		SetImageCursor();

		POINT pt;
		::GetCursorPos(&pt);
		
		HWND hWnd = ::WindowFromPoint(pt);
		if (!hWnd)
			return;
		else
		{
			while(::GetParent(hWnd))
			{
				hWnd = ::GetParent(hWnd);
			}
		}

		::GetWindowRect(hWnd, &m_rcCurRecordRangeWindow);

		HDC hdcScreen;
 		hdcScreen = ::GetDC(NULL);

		::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
		Sleep(100);
		
		HBRUSH hPen = (HBRUSH)::CreatePen(PS_SOLID, 5, RGB(255,0,255));
		HBRUSH hOldPen = (HBRUSH)::SelectObject(hdcScreen, hPen);
		HBRUSH hBrush = (HBRUSH)::CreateBrushIndirect(&m_LogBrush);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcScreen, hBrush);
			
		CRect rc = m_rcCurRecordRangeWindow;		
		rc.DeflateRect(2,2,2,2);
			
		::Rectangle(hdcScreen, rc.left, rc.top,	rc.right, rc.bottom);
		Sleep(1000);
		
		::SelectObject(hdcScreen, hOldPen);
		::DeleteObject(hPen);	
		::ReleaseDC(NULL, hdcScreen);		

		::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);	

		ShowWindow(SW_SHOW);
		UpdateWindow();

		Sleep(1000);
		InitRecord();

		m_bMarkingRecArea = true;
		MarkingRecArea();
		SetTimer(RECORD_MARKINGRANGE_TIMER, 2000, NULL);
	}
	else
	{
		POINT pt;
		::GetCursorPos(&pt);
		
		HWND hWnd = ::WindowFromPoint(pt);
		if (!hWnd)
			return;
		else
		{
			while(::GetParent(hWnd))
			{
				hWnd = ::GetParent(hWnd);
			}
		}

		::GetWindowRect(hWnd, &m_rcCurRecordRangeWindow);

		HDC hdcScreen;
 		hdcScreen = ::GetDC(NULL);

		int maxxScreen = GetDeviceCaps(hdcScreen, HORZRES);
		int maxyScreen = GetDeviceCaps(hdcScreen, VERTRES);

		if(m_rcCurRecordRangeWindow.left < 0) m_rcCurRecordRangeWindow.left = 0;
		if(m_rcCurRecordRangeWindow.right > maxxScreen) m_rcCurRecordRangeWindow.right = maxxScreen;
		if(m_rcCurRecordRangeWindow.top < 0) m_rcCurRecordRangeWindow.top = 0;
		if(m_rcCurRecordRangeWindow.bottom > maxyScreen) m_rcCurRecordRangeWindow.bottom = maxyScreen;
				
		if(::RedrawWindow(NULL, (LPCRECT)m_rcPrevRecordRangeWindow, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN))
		{			
			Sleep(200);
			HBRUSH hPen = (HBRUSH)::CreatePen(PS_SOLID, 5, RGB(0,255,0));
			HBRUSH hOldPen = (HBRUSH)::SelectObject(hdcScreen, hPen);
			HBRUSH hBrush = (HBRUSH)::CreateBrushIndirect(&m_LogBrush);
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcScreen, hBrush);
			
			CRect rc = m_rcCurRecordRangeWindow;		
			rc.DeflateRect(2,2,2,2);
			
			::Rectangle(hdcScreen, rc.left, rc.top,	rc.right, rc.bottom);	
			
			::SelectObject(hdcScreen, hOldPen);
			::DeleteObject(hPen);	
			::ReleaseDC(NULL, hdcScreen);

			m_rcPrevRecordRangeWindow = m_rcCurRecordRangeWindow;
		}
	}
}


void CAuthorDlg::EndSelectArea()
{
	m_rcCurRecordRangeWindow.NormalizeRect();

	if(m_rcCurRecordRangeWindow.Width() < 64 && m_rcCurRecordRangeWindow.Height() < 64)
	{
		CString strTemp = "녹화 할 영역이 너무 좁습니다.";
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		return;	
	}

	m_bSelectRecordArea = false;
	SetPenType(m_nPenType);
	SetImageCursor();

	HDC hdcScreen;
 	hdcScreen = ::GetDC(NULL);
	
	HBRUSH hPen = (HBRUSH)::CreatePen(PS_SOLID, 5, RGB(255,0,255));
	HBRUSH hOldPen = (HBRUSH)::SelectObject(hdcScreen, hPen);
	HBRUSH hBrush = (HBRUSH)::CreateBrushIndirect(&m_LogBrush);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcScreen, hBrush);
		
	CRect rc = m_rcCurRecordRangeWindow;		
	rc.DeflateRect(2,2,2,2);
		
	::Rectangle(hdcScreen, rc.left, rc.top,	rc.right, rc.bottom);
	Sleep(1000);
	
	::SelectObject(hdcScreen, hOldPen);
	::DeleteObject(hPen);	
	::ReleaseDC(NULL, hdcScreen);		

	::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	SetTimer(RECORD_SELECTRANGE_TIMER, 1000, NULL);	
	ShowWindow(SW_SHOW);
	UpdateWindow();
	
	m_bMarkingRecArea = true;
	SetTimer(RECORD_MARKINGRANGE_TIMER, 2000, NULL);

	return;
}



void CAuthorDlg::SelectRecordRange()
{
	CRecordSelectRange RecordRangeDlg;
	RecordRangeDlg.SetRecordRangeState(m_nRecordRangeState);
	if(RecordRangeDlg.DoModal() == IDOK)
	{				
		m_nRecordRangeState = RecordRangeDlg.m_nRangeState;
		HCURSOR hCursor;

		switch(m_nRecordRangeState)
		{
		case	RECORD_WINDOW:
			m_bSelectRecordWindow = true;	
			hCursor  = ::LoadCursor(NULL, IDC_ARROW);

			SetTimer(RECORD_SELECTWINDOW_TIMER, 400, NULL);
			ShowWindow(SW_HIDE);			
			return;
		case	RECORD_AREA:	
			m_bSelectRecordArea = true;	
			hCursor  = ::LoadCursor(NULL, IDC_CROSS);

			m_pen.dtool = XInk::DT_Magnifier;
			m_pCanvas->SetPen(m_pen);

			ShowWindow(SW_HIDE);
			return;	

		default:
			if(m_bDrawCanvas)
				m_pCanvas->Refresh();


			SetTimer(RECORD_SELECTRANGE_TIMER, 500, NULL);
			return;
		}
	}
	else
	{
		m_RecordBtn.SetSelect(FALSE);
		return;	
	}
}


void CAuthorDlg::MarkingRecArea()
{
	HDC hdcScreen;
 	hdcScreen = ::GetDC(NULL);
	
	HBRUSH hPen = (HBRUSH)::CreatePen(PS_SOLID, 5, RGB(255,0,255));
	HBRUSH hOldPen = (HBRUSH)::SelectObject(hdcScreen, hPen);	
		
	CRect rc = m_rcCurRecordRangeWindow;		
	rc.InflateRect(5,5,5,5);

	::MoveToEx(hdcScreen, rc.left, rc.top, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.left + 20, rc.top); 
	
	::MoveToEx(hdcScreen, rc.left, rc.top, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.left, rc.top+20); 
	
	::MoveToEx(hdcScreen, rc.left, rc.bottom, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.left + 20, rc.bottom); 
	
	::MoveToEx(hdcScreen, rc.left, rc.bottom, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.left, rc.bottom-20); 
	
	::MoveToEx(hdcScreen, rc.right, rc.top, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.right - 20, rc.top); 
	
	::MoveToEx(hdcScreen, rc.right, rc.top, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.right, rc.top+20); 
	
	::MoveToEx(hdcScreen, rc.right, rc.bottom, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.right - 20, rc.bottom); 
	
	::MoveToEx(hdcScreen, rc.right, rc.bottom, (LPPOINT) NULL); 
    ::LineTo(hdcScreen, rc.right, rc.bottom-20); 
}
/////////////////////////////////////////////////////////////////////////////
//
void CAuthorDlg::InitRecord()
{
	CWaitCursor wait;

	if(!OnReadyRecord())
	{
		m_RecordBtn.SetSelect(FALSE);
		m_RecordBtn.UpdateWindow();
		return;
	}

	m_bInitRecord = true;

	BOOL	hr;
	hr = ::srecStartCapture( m_hCapture );
	if(!hr)
	{
		AfxMessageBox("Error : Failed! AVI capture start");			
		SetRecordTerminate();
		m_RecordBtn.SetSelect(FALSE);
		m_RecordBtn.UpdateWindow();
		return;
	}

	if(!m_bRecordFlag)
	{
		m_bRecordFlag = true;
		m_RecordStopBtn.EnableWindow(TRUE);
		m_RecordStopBtn.UpdateWindow();
	}

	m_RecordBtn.EnableWindow(FALSE);
	m_RecordPauseBtn.EnableWindow(TRUE);
	m_RecordBtn.UpdateWindow();
	m_RecordPauseBtn.UpdateWindow();

	m_strRecState = "RECORD";
	
	if(m_bSmallMode)
	{
		if(m_ShowhideBtn.IsSelected())
			InvalidateRect(&m_rcRecState);
	}
	else
	{
		if(!m_ShowhideBtn.IsSelected())
			InvalidateRect(&m_rcRecState);
		else if(m_bShowControl[7])
			InvalidateRect(&m_rcHideRecState);
	}

	m_bMakeIndex = true;

	SetTimer(RECORDING_TIMER, 200, NULL);

	m_WmvBtn.EnableWindow(FALSE);
}

void CAuthorDlg::OnRecordbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_RecordBtn.IsSelected())
	{
		if(!m_bInitRecord)
		{
			SelectRecordRange();
			return;
		}
			
		BOOL	hr;
		hr = ::srecResumeCapture( m_hCapture );
		if(!hr)
		{
			AfxMessageBox("Error : Failed! AVI capture resume");			
			SetRecordTerminate();
			m_RecordBtn.SetSelect(FALSE);
			m_RecordBtn.UpdateWindow();
			return;		
		}

		if(!m_bRecordFlag)
		{
			m_bRecordFlag = true;
			m_RecordStopBtn.EnableWindow(TRUE);
			m_RecordStopBtn.UpdateWindow();
		}

		m_RecordBtn.EnableWindow(FALSE);
		m_RecordPauseBtn.EnableWindow(TRUE);
		m_RecordBtn.UpdateWindow();
		m_RecordPauseBtn.UpdateWindow();

		m_strRecState = "RECORD";
		
		if(m_bSmallMode)
		{
			if(m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecState);
		}
		else
		{
			if(!m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecState);
			else if(m_bShowControl[7])
				InvalidateRect(&m_rcHideRecState);
		}

		m_bMakeIndex = true;

		SetTimer(RECORDING_TIMER, 200, NULL);
		
		if(m_bMarkingRecArea)
		{
			MarkingRecArea();
			SetTimer(RECORD_MARKINGRANGE_TIMER, 2000, NULL);
		}
	}
}


void CAuthorDlg::OnRecordpausebtn() 
{
	// TODO: Add your control notification handler code here
	if(m_bRecordFlag && m_bMakeIndex)
	{
		m_RecordBtn.EnableWindow(TRUE);	
		m_RecordBtn.SetSelect(FALSE);
		m_RecordPauseBtn.EnableWindow(FALSE);
		m_RecordBtn.UpdateWindow();
		m_RecordPauseBtn.UpdateWindow();

		BOOL	hr;
		hr = ::srecPauseCapture( m_hCapture );
		if(!hr)
		{
			AfxMessageBox("Error : Failed! AVI capture Pause");	
			SetRecordTerminate();

			return;
		}

		m_strRecState = "PAUSE";

		if(m_bSmallMode)
		{
			if(m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecState);
		}
		else
		{
			if(!m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecState);
			else if(m_bShowControl[7])
				InvalidateRect(&m_rcHideRecState);
		}
		
		m_bMakeIndex = false;
		DWORD dwEnd = ::srecGetDuration( m_hCapture )*m_dwFps;
		WriteSchemeIndex(dwEnd);	
		
		m_nIndexStartTime = dwEnd+1;

		KillTimer(RECORDING_TIMER);

		if(m_bMarkingRecArea)
		{			
			KillTimer(RECORD_MARKINGRANGE_TIMER);
			::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);	
		}
	}
}


void CAuthorDlg::OnRecordstopbtn() 
{
	if(m_bRecordFlag)
	{
		if(m_bMakeIndex)
			OnRecordpausebtn();

		DWORD dwEnd;
		dwEnd = ::srecGetDuration( m_hCapture )*m_dwFps;

		if(!SetRecordTerminate())
			return;
		
		EnableWindow(FALSE);
		CString strFilter = "Movie file (.avi)|*.avi||";
		CFileDialog SaveFileDialog (FALSE, "Save file", "",
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter, GetDesktopWindow());
		
		SaveFileDialog.m_ofn.lpstrInitialDir = CAppSettings::GetContentsPath();
		SaveFileDialog.m_ofn.lpstrTitle = "Save Avi File";			
		if(SaveFileDialog.DoModal() == IDOK)
		{
			CWaitCursor w;
			CString strTemp, strAviPath, strContentFolder, strTitle;

			strAviPath.Format("%s\\Notitle.avi", CAppSettings::GetWorkPath());	
			strTitle = SaveFileDialog.GetFileTitle();
			strContentFolder = SaveFileDialog.GetPathName();

			int nLength = strContentFolder.GetLength();
			int nCount = strContentFolder.ReverseFind('\\');
			strContentFolder.Delete(nCount, nLength-nCount);

			strContentFolder += "\\" + strTitle;

			CFileFind finder;			
			BOOL bWorking = finder.FindFile(strContentFolder);
			if(!bWorking)
				::CreateDirectory(strContentFolder, NULL);

			finder.Close();

			strTemp = strContentFolder + "\\" + strTitle + ".avi";
			SetSchemeTerminate(strTitle, dwEnd);

			if(strAviPath != strTemp)
			{
				if(::MoveFileEx(strAviPath, strTemp, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED))
				{
					strTemp = strContentFolder + "\\" + strTitle + ".ini";
					::MoveFileEx(CAppSettings::GetSchemePath(), strTemp, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
				}
				else
				{
					AfxMessageBox("Save AVI Error");
					return;
				}
			}
			else
			{
				strTemp = strContentFolder + "\\" + strTitle + ".ini";
				::MoveFileEx(CAppSettings::GetSchemePath(), strTemp, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
			}

			m_bRecordFlag = false;
			InitScheme();			
		}
		else
		{
			m_bRecordFlag = false;
			if(::DeleteFile(CAppSettings::GetSchemePath()))
				InitScheme();
		}		
	}	

	EnableWindow();
	m_WmvBtn.EnableWindow();
}

void CAuthorDlg::OnWMVbtn() 
{
	// TODO: Add your control notification handler code here
	CWmvConvertDlg dlg;
	dlg.DoModal();
}

void CAuthorDlg::OnMicbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_MicBtn.IsSelected())
		m_MicBtn.SetSelect(FALSE);

	if(!m_bEnableVolume)
		return;

	OnRecordpausebtn();

	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL, // No module name (use command line). 
        "sndvol32.exe", // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    )
		return;
 
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}


void CAuthorDlg::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CString strHelpFile;
	strHelpFile = CAppSettings::GetHelpPath();

	ShellExecute(NULL, "open", strHelpFile, NULL, NULL, SW_SHOWNORMAL);
}


void CAuthorDlg::OnPptprevbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_bBoardOpen)
		return;

	if(CPPTUtil::GoPrevSlide())
	{
		if(m_ModeBtn.IsSelected() && m_bDrawCanvas)
		{
			CWaitCursor w;
			m_bDrawCanvas = false;

	/*		CWnd* pWnd = FindWindow("screenClass", NULL);
			if(pWnd != NULL)
				pWnd->RedrawWindow();
	*/	}
	}	
}

void CAuthorDlg::OnPptnextbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_bBoardOpen)
		return;

	if(CPPTUtil::GoNextSlide())
	{
		if(m_ModeBtn.IsSelected() && m_bDrawCanvas)
		{
			CWaitCursor w;		
			m_bDrawCanvas = false;
	/*		CWnd* pWnd = FindWindow("screenClass", NULL);
			if(pWnd != NULL)
				pWnd->RedrawWindow();
	*/	}
	}		
}

void CAuthorDlg::OnBoardOpenbtn() 
{
	// TODO: Add your control notification handler code here
	ShowPopupMenu();
}


void CAuthorDlg::OpenBoard(int nBoardType)
{	
	CWaitCursor w;	

	if(m_bBoardOpen)
	{
		if(m_strSlidePath.IsEmpty())
		{
			if(m_bDrawCanvas)
			{
				CString strPath;
				strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);
				if(CaptureDesktop(strPath))
				{				
				//	strPath.Format("Slide%d", m_nSlideIndex);
					m_SlideMenuDlg.AddSlide(strPath);

					m_nSlideIndex++;
				}
			}
		}
		else if(m_bDrawCanvas)
		{
			CaptureDesktop(m_strSlidePath);
			m_SlideMenuDlg.ReplaceSildeThumbnail(m_strSlidePath);
			m_strSlidePath = "";
		}
	}

	if(!m_ModeBtn.IsSelected())
	{
		m_ModeBtn.SetSelect();
		OnModebtn();
	}
	else if(m_bDrawCanvas)
	{
		m_bDrawCanvas = false;
	}
	
	SetPPTSlideShowHide();
	m_BoardDlg.SetBoardStyle(nBoardType);
	m_BoardDlg.MoveWindow(CRect(0,0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN)));
	m_BoardDlg.ShowWindow(SW_SHOW);

	m_bBoardOpen = true;
	CreateCanvas(m_BoardDlg, XInk::xRect(0,0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CXSCREEN)));
}


void CAuthorDlg::OnZoombtn() 
{
	// TODO: Add your control notification handler code here
	if(!m_bZoomMode)
	{
		CString strPath;
		strPath.Format("%s\\Magnifier.bmp", CAppSettings::GetTempPath());
		if(CaptureDesktop(strPath))
		{			
			HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);

			if(m_hCapture != NULL && m_bRecCursor)
				::srecSetCustomCursor(m_hCapture, hCursor);

			m_MagnifierDlg.SetEnbleMagnifier(true);
			m_MagnifierDlg.ShowWindow(SW_SHOW);
			EnableWindow(FALSE);

			if(m_bBoardOpen)
				m_BoardDlg.ShowWindow(SW_HIDE);

			return;
		}
		else
		{
			AfxMessageBox("지금은 돋보기 기능을 사용할 수 없습니다!", MB_ICONERROR);
			return;
		}
	}
}

void CAuthorDlg::OnNotifyZoomEnd(bool bZoom)
{
	if(bZoom)
	{				
		m_bZoomMode = true;			

		m_pen.dtool = XInk::DT_Magnifier;
		m_pCanvas->SetPen(m_pen);

		HCURSOR hCursor = ::LoadCursor(NULL, IDC_CROSS);

		if(m_hCapture != NULL && m_bRecCursor)
			::srecSetCustomCursor(m_hCapture, hCursor);
	}
	else
	{
		m_MagnifierDlg.SetEnbleMagnifier(false);
		m_MagnifierDlg.ShowWindow(SW_HIDE);
		EnableWindow(TRUE);

		if(m_bBoardOpen)
			m_BoardDlg.ShowWindow(SW_SHOW);
	}		
}


void CAuthorDlg::OnZoomShot()
{
	m_rcZoom.NormalizeRect();
	
	if(m_bTextRange)
	{
		if(m_rcZoom.Width() < 80 || m_rcZoom.Height() < 50)
		{
			CString strTemp = "텍스트 쓰기 영역이 너무 좁습니다.";
			AfxMessageBox(strTemp, MB_ICONINFORMATION);
		
			return;
		}
		
		m_bZoomMode = false;
		EndTextRange();

		return;
	}

	if(m_rcZoom.Width() < 10 || m_rcZoom.Height() < 10)
		return;

	CWaitCursor w;	
	m_bZoomMode = false;	

	EnableWindow(TRUE);

	if(m_bSnapshotRange)
	{
		m_bSnapshotRange = false;
		EndSnapshotRange();

		return;
	}

	CString strPath, strPath1, strPath2;		
	if(m_bReName)
	{
		strPath.Format("%s\\Screen.bmp", CAppSettings::GetTempPath());
		m_bReName = false;
	}
	else
	{
		strPath.Format("%s\\Screen1.bmp", CAppSettings::GetTempPath());
		m_bReName = true;
	}

	if(CaptureDesktop(strPath, true))
	{		
		m_BoardDlg.SetBoardImage(strPath);
	
		if(m_bBoardOpen)
		{
			if(m_strSlidePath.IsEmpty())
			{		
				if(m_bDrawCanvas)
				{
					strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);
					strPath1.Format("%s\\Magnifier.bmp", CAppSettings::GetTempPath());
					
					if(::CopyFile(strPath1, strPath, FALSE))
					{
					//	strPath2.Format("Slide%d", m_nSlideIndex);
						m_SlideMenuDlg.AddSlide(strPath2);
						m_nSlideIndex++;
					}
				}				
			}
			else if(m_bDrawCanvas)
			{
				strPath1.Format("%s\\Magnifier.bmp", CAppSettings::GetTempPath());
				::CopyFile(strPath1, m_strSlidePath, FALSE);
				m_SlideMenuDlg.ReplaceSildeThumbnail(m_strSlidePath);
			}
		}
		else
		{		
			m_bBoardOpen = true;
		}
		
		if(m_bDrawCanvas)
		{
			m_bDrawCanvas = false;
		}

		SetPPTSlideShowHide();
		m_BoardDlg.ShowWindow(SW_SHOW);
	}				
	else
	{
		m_BoardDlg.ShowWindow(SW_HIDE);
		SetPPTSlideShowHide(false);
		m_bBoardOpen = false;
	
		if(m_bDrawCanvas)
		{
			m_bDrawCanvas = false;
		}
	}	

	m_MagnifierDlg.SetEnbleMagnifier(false);
	m_MagnifierDlg.ShowWindow(SW_HIDE);

	SetPenType(m_nPenType);

	m_strSlidePath = "";
	SetImageCursor();

	ShowWindow(SW_SHOW);
}


void CAuthorDlg::OnSlidemenubtn() 
{
	// TODO: Add your control notification handler code here
	CRect rc, rc1;
	int cy, w, h;

	m_SlideMenuDlg.GetWindowRect(&rc1);	
	GetWindowRect(&rc);
	w = rc1.Width();
	h = rc1.Height();
	cy= rc.Height();

	rc1.left = ::GetSystemMetrics(SM_CXSCREEN) - w;
	rc1.right = rc1.left + w;

	if(m_bMoveTop)
	{
		rc1.top = cy;
		rc1.bottom = rc1.top + h;
	}
	else
	{
		rc1.top = ::GetSystemMetrics(SM_CYSCREEN) - cy - h;
		rc1.bottom = rc1.top + h;
	}

	m_SlideMenuDlg.MoveWindow(&rc1);
	m_SlideMenuDlg.ShowWindow(SW_SHOW);	
	EnableWindow(FALSE);	
}



void CAuthorDlg::OnCameraOnOff() 
{
	// TODO: Add your control notification handler code here
	HWND hWnd = NULL;
	hWnd = ::FindWindow("{ED7AFEB4-AB6A-43ed-B4C2-84F5367336BC}", 0);
	
	if(hWnd == NULL)
	{
		CString strPath, strCommand;
		strPath = CAppSettings::GetAppPath();
		strPath += "\\camview.exe";
		int nSize;

		switch(m_nCamSize)
		{
		case	1:
			nSize = 325;
			break;
		case	2:
			nSize = 645;
			break;
		case	3:
			nSize = 1029;
			break;
		default:
			nSize = 165;
			break;
		}

		int nLeft = ::GetSystemMetrics(SM_CXSCREEN)-nSize;
		int nTop;
		if(m_bMoveTop)
			nTop = 64;
		else
			nTop = 0;

		strCommand.Format("%d %d %d %d denjang", 0,m_nCamSize,nLeft,nTop);
		ShellExecute(NULL, "open", strPath, strCommand, NULL, SW_SHOWNORMAL );
	}
	else
	{
		::PostMessage(hWnd, WM_QUIT,0,0);
	}
}


void CAuthorDlg::OnPrintbtn() 
{
	// TODO: Add your control notification handler code here
	CWaitCursor w;

	if(m_ModeBtn.IsSelected())
	{
		CString strPath;
		if(m_bReName)
		{
			strPath.Format("%s\\Screen.bmp", CAppSettings::GetTempPath());
			m_bReName = false;
		}
		else
		{
			strPath.Format("%s\\Screen1.bmp", CAppSettings::GetTempPath());
			m_bReName = true;
		}

		if(CaptureDesktop(strPath))
		{
			HANDLE  hDevMode = NULL;
			HANDLE  hDevNames = NULL;
			HANDLE  hSaveDevMode = NULL;
			HANDLE  hSaveDevNames = NULL;
			DWORD   dwLen;

			CPrintDialog *pDlg = new CPrintDialog(FALSE);
			DOCINFO docinfo = {sizeof(DOCINFO), "iAuthor", NULL};

			if(pDlg->DoModal() == IDCANCEL)
			{
				delete pDlg;
				return;
			}
			
			hSaveDevNames = pDlg->m_pd.hDevNames;
			hSaveDevMode = pDlg->m_pd.hDevMode;

			if(pDlg->m_pd.hDevNames)
			{
				dwLen = GlobalSize(pDlg->m_pd.hDevNames);
				hDevNames = GlobalAlloc(GHND, dwLen);
				if (hDevNames) 
				{
					BYTE *lpCopy = (BYTE*)GlobalLock(hDevNames);
					BYTE *lp = (BYTE*)GlobalLock(pDlg->m_pd.hDevNames);
					CopyMemory(lpCopy, lp, dwLen);
					GlobalUnlock(hDevNames);
					GlobalUnlock(pDlg->m_pd.hDevNames);
				}
			} 

			if (pDlg->m_pd.hDevMode)
			{
				dwLen = GlobalSize(pDlg->m_pd.hDevMode);
				hDevMode = GlobalAlloc(GHND, dwLen);
				if (hDevMode)
				{
					BYTE *lpCopy = (BYTE*)GlobalLock(hDevMode);
					BYTE *lp = (BYTE*)GlobalLock(pDlg->m_pd.hDevMode);
					CopyMemory(lpCopy, lp, dwLen);
					GlobalUnlock(hDevMode);
					GlobalUnlock(pDlg->m_pd.hDevMode);
				}
			}

			 if (hDevNames && hDevMode)
			{
				DEVMODE *pDevMode = (DEVMODE *)GlobalLock(hDevMode);
				// Change the orientation : 수직인쇄=DMORIENT_PORTRAIT, 수평인쇄=DMORIENT_LANDSCAPE
				pDevMode->dmOrientation = DMORIENT_LANDSCAPE;  
				pDevMode->dmFields = pDevMode->dmFields | DM_ORIENTATION;
				GlobalUnlock(hDevMode);
				AfxGetApp()->SelectPrinter(hDevNames, hDevMode, FALSE);
			}

			CDC PrintDC;
			AfxGetApp()->CreatePrinterDC(PrintDC);

			PrintDC.StartDoc(&docinfo);
			PrintDC.StartPage();
			
			Graphics graphics(PrintDC.m_hDC);

			WCHAR wszPath[MAX_PATH];
			MultiByteToWideChar( CP_ACP, 0, strPath, strlen(strPath)+1, wszPath,   
								 sizeof(wszPath)/sizeof(wszPath[0]) );

			Bitmap bitmap(wszPath,false);
			graphics.DrawImage(&bitmap, Rect(0, 0, 1024, 768));


			PrintDC.EndPage();
			PrintDC.EndDoc();		

			delete pDlg;

			if (hSaveDevNames && hSaveDevMode)
			{
				AfxGetApp()->SelectPrinter(hSaveDevNames, hSaveDevMode, FALSE);
				hSaveDevNames = NULL;
				hSaveDevMode = NULL;
			} 

			if (hDevMode)  GlobalFree(hDevMode);
			if (hDevNames) GlobalFree(hDevNames);
		}

		m_ModeBtn.SetSelect(FALSE);	
	}
}

void CAuthorDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bRecordFlag)
	{
		if(m_bMakeIndex)
			OnRecordpausebtn();

		EnableWindow(FALSE);
		CString strTemp;
		strTemp = "저장되지 않은 동영상파일이 있습니다.\n\r저장하시겠습니까?";
		if(::MessageBox(::GetDesktopWindow(), strTemp, "Save AVI file", MB_YESNO|MB_ICONQUESTION) == IDYES)
			OnRecordstopbtn();

		EnableWindow();
	}

	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// Util
void CAuthorDlg::SetImageCursor()
{
	CWaitCursor w;

	if(m_ModeBtn.IsSelected())
	{
		bool	bLoadCursor = false;
		switch(m_nPenToolType)
		{
		case	PENTOOL:	
			if(m_nPenType == PENCIL)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_PENCURSOR);
			else if(m_nPenType == BRUSH)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_BRUSHCURSOR);
			else if(m_nPenType == HIGHLIGHTPEN)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_HGPENCURSOR);
			break;
		case	ERASERTOOL:
			if(m_nEraserType ==  ERASER)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_ERASERCURSOR);
			else if(m_nEraserType ==  ERASER_RECT)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_ERASER_RECTCURSOR);
			break;
		case	DIAGRAMTOOL:
			if(m_nDiagramType == DIAGRAM_ELLIPSE)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_ELLIPSECURSOR);
			else if(m_nDiagramType == DIAGRAM_RECT)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_RECTCURSOR);
			else if(m_nDiagramType == DIAGRAM_LINE)
				m_hCursor = AfxGetApp()->LoadCursor(IDC_LINECURSOR);
			break;
		}

		m_BoardDlg.SetCursor(m_hCursor);
		if(m_hCapture != NULL && m_bRecCursor)
			::srecSetCustomCursor(m_hCapture, m_hCursor);
	}
	else
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
		if(m_hCapture != NULL && m_bRecCursor)
			::srecSetCustomCursor(m_hCapture, hCursor);
	}

	CPoint pt;
	::GetCursorPos(&pt);
	::SetCursorPos(pt.x+1, pt.y);
}


// Capture
BOOL CAuthorDlg::CaptureDesktop(CString strPath, bool bStrech)
{
	CWaitCursor wait;

	USES_CONVERSION;
 
 	// Create a normal DC and a memory DC for the entire screen. The 
 	// normal DC provides a "snapshot" of the screen contents. The 
 	// memory DC keeps a copy of this "snapshot" in the associated 
 	// bitmap. 
	HDC hScreenDC = ::GetDC(0); 

 	// Create a compatible bitmap for hScreenDC.
 	int w = GetDeviceCaps(hScreenDC, HORZRES);
 	int h = GetDeviceCaps(hScreenDC, VERTRES);	

	HBITMAP	hBitmap = ::CreateCompatibleBitmap(hScreenDC, w, h);
	if (hBitmap == 0)
	{
		::ReleaseDC(0, hScreenDC);
 		return FALSE;
	}

	// Select the bitmaps into the compatible DC. 
 	HDC hMemDC = ::CreateCompatibleDC(hScreenDC);
	if (hMemDC && !::SelectObject(hMemDC, hBitmap)) 
	{
		::DeleteDC(hMemDC);
 		return FALSE;
	}
 	 
 	// Hide the application window. 
 	//m_pParentWnd->ShowWindow(SW_HIDE); 
 
 	//Copy color data for the entire display into a 
 	//bitmap that is selected into a compatible DC.

	int ok;
	if(bStrech)
		ok = ::StretchBlt(hMemDC, 0,0, w,h, hScreenDC, 
			 m_rcZoom.left, m_rcZoom.top, m_rcZoom.Width(), m_rcZoom.Height(), SRCCOPY);
	else
		ok = ::BitBlt(hMemDC, 0,0, w,h, hScreenDC, 0,0, SRCCOPY);

	if (ok)
	{
 		// Save image to file
 		CLSID   encoderClsid;
		GetEncoderClsid(L"image/bmp", &encoderClsid);

 		Status  stat;
 		Bitmap* image = new Bitmap(hBitmap, NULL);
		stat = image->Save(T2OLE(strPath), &encoderClsid, NULL);
 		delete image;
	}

	// Release
 	::DeleteObject(hBitmap);
 	::DeleteDC(hMemDC);
	::ReleaseDC(0, hScreenDC);

	return TRUE;
}


void CAuthorDlg::DisplayRecTime()
{	
	DWORD nLength = ::srecGetDuration( m_hCapture );
	DWORD nTotalSec = nLength;

	int nHour, nMin, nSec;

	nHour = nTotalSec/3600;
	nMin = (nTotalSec-nHour*3600)/60;
	nSec = nTotalSec - nHour*3600 - nMin*60;

	CString strTemp;
	strTemp.Format("%.2d:%.2d:%.2d", nHour, nMin, nSec);

	if(strTemp != m_strRecTime)
	{
		m_strRecTime = strTemp;

		if(m_bSmallMode)
		{
			if(m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecTime);
		}
		else
		{
			if(!m_ShowhideBtn.IsSelected())
				InvalidateRect(&m_rcRecTime);
			else if(m_bShowControl[7])
				InvalidateRect(&m_rcHideRecTime);
		}
	}

	if(!m_bRegistration)
	{
		if(nTotalSec > 600)
		{
			KillTimer(RECORDING_TIMER);
			CString strTemp = "평가판은 녹화시간이 10분으로 제한되어 있습니다.\n\r녹화를 중지 하겠습니다.";
			AfxMessageBox(strTemp, MB_OK|MB_ICONINFORMATION);

			OnRecordstopbtn();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	
LRESULT CAuthorDlg::OnSnapshot(WPARAM wParam, LPARAM lParam)
{	
	if(m_nSlideIndex > 30)
		return 0;

	CAppSettings::SetSlideFolderLock();
	CWaitCursor w;

	CString strPath;
	strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);

	if(m_bSnapshotFullMode)
	{			
		if(CaptureDesktop(strPath))
		{				
		//	strPath.Format("Slide%d", m_nSlideIndex);
			m_SlideMenuDlg.AddSlide(strPath);

			m_nSlideIndex++;
		}
	}
	else 
	{
		m_bSnapshotRange = true;	

		m_bZoomMode = true;			
		if(!m_ModeBtn.IsSelected())
		{
			m_ModeBtn.SetSelect();
			OnModebtn();
		}

		m_pen.dtool = XInk::DT_Magnifier;
		m_pCanvas->SetPen(m_pen);

		HCURSOR hCursor = ::LoadCursor(NULL, IDC_CROSS);
		if(m_hCapture != NULL && m_bRecCursor)
			::srecSetCustomCursor(m_hCapture, hCursor);
	}	

	return 0;
}


void CAuthorDlg::EndSnapshotRange()
{
	USES_CONVERSION;

 	// Create a normal DC and a memory DC for the entire screen. The 
 	// normal DC provides a "snapshot" of the screen contents. The 
 	// memory DC keeps a copy of this "snapshot" in the associated 
 	// bitmap. 
	HDC hdcScreen, hdcCompatible;
	HBITMAP hbmScreen;

 	hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 
 	hdcCompatible = CreateCompatibleDC(hdcScreen); 
		 
 	// Create a compatible bitmap for hdcScreen.
 	int width  = GetDeviceCaps(hdcScreen, HORZRES);
 	int height = GetDeviceCaps(hdcScreen, VERTRES);	

 	hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
	if (hbmScreen == 0) 
 		return;

	// Select the bitmaps into the compatible DC. 
	if (!SelectObject(hdcCompatible, hbmScreen)) 
 		return ;

	int nX, nY;
	nX = (width-m_rcZoom.Width())/2;
	nY = (height-m_rcZoom.Height())/2;

	if(!BitBlt(hdcCompatible, 0,0, width,height,hdcScreen, 0,0, WHITENESS))
		return;
	
	if (!BitBlt(hdcCompatible, nX,nY, m_rcZoom.Width(),m_rcZoom.Height(),
		hdcScreen, m_rcZoom.left,m_rcZoom.top, SRCCOPY)) 
			return;

	HBRUSH hBrush = (HBRUSH)::CreateBrushIndirect(&m_LogBrush);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdcCompatible, hBrush);

	Rectangle(hdcCompatible, nX, nY, nX + m_rcZoom.Width(),nY + m_rcZoom.Height());

	// Save image to file
 	CLSID   encoderClsid;
	GetEncoderClsid(L"image/bmp", &encoderClsid);

	CString strPath;
	strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);
	
 	Status  stat;
 	Bitmap* image = new Bitmap(hbmScreen, NULL);
	stat = image->Save(T2OLE(strPath), &encoderClsid, NULL);
 	
	delete image;
 
 	// Release	
	::SelectObject(hdcCompatible, hOldBrush);	
	::DeleteObject(hBrush);

 	DeleteObject(hbmScreen);
 	DeleteDC(hdcCompatible);
 	DeleteDC(hdcScreen);	

	m_strSlidePath = strPath;

	if(m_bDrawCanvas)
	{
		m_bDrawCanvas = false;
	}

	if(m_bBoardOpen)
	{
		m_BoardDlg.ShowWindow(SW_HIDE);
		m_BoardDlg.SetBoardImage(strPath);
		SetPPTSlideShowHide();
		m_BoardDlg.ShowWindow(SW_SHOW);
		
	//	strPath.Format("Slide%d", m_nSlideIndex);
		m_SlideMenuDlg.AddSlide(strPath);
		m_nSlideIndex++;
	}
	else
	{
		m_BoardDlg.ShowWindow(SW_HIDE);
		m_BoardDlg.SetBoardImage(strPath);
		SetPPTSlideShowHide();
		m_BoardDlg.ShowWindow(SW_SHOW);

		m_bBoardOpen = true;

//		strPath.Format("Slide%d", m_nSlideIndex);
		m_SlideMenuDlg.AddSlide(strPath);
		m_nSlideIndex++;
	}	

	ShowWindow(SW_SHOW);
	SetPenType(m_nPenType);	
	SetImageCursor();

	m_BoardDlg.UpdateWindow();	

	m_rcZoom = CRect(0,0,0,0);
}

// 메뉴 처리는 각 버튼 이벤트로 이동
LRESULT CAuthorDlg::OnCtrlMouseOver(WPARAM wParam, LPARAM lParam)
{
	m_nSelMouseOverCtrlID = (int)wParam;
	return 0;

	/*
	int nCtrlID = (int)wParam;
	if(m_nSelMouseOverCtrlID == nCtrlID)
		return 0;

	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	switch (nCtrlID)
	{		
	case	IDC_PENBTN:		
	case	IDC_ERASERBTN:
	case	IDC_COLORCHANGEBTN:
	case	IDC_LINEWEIGHTCHANGEBTN:
	case	IDC_DIAGRAMBTN:
	case	IDC_BOARDOPENBTN:		
		m_nSelMouseOverCtrlID = nCtrlID;
		ShowPopupMenu();
		break;
	}

	return 0;
	*/
}


LRESULT CAuthorDlg::OnCtrlMouseOut(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
	{
		CPoint pt;
		GetCursorPos(&pt);
		if(!m_rcEnableMenu.PtInRect(pt))
		{
			ScreenToClient(&pt);
			if(!m_rcEnable.PtInRect(pt))
				HidePopupMenu();
		}
	}

	return 0;
}


void CAuthorDlg::ShowPopupMenu()
{
	if (!m_bBoardOpen || !m_pCanvas)
	{
		Beep(2100,9);
		return;
	}

	switch(m_nSelMouseOverCtrlID)
	{
	case	IDC_PENBTN:
		m_PenBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_PenSelectDlg.ShowWindow(SW_SHOW);
		m_PenSelectDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = PENSELECT_POPUP;

		if(m_nPenToolType != PENTOOL)
		{
			switch(m_nPenToolType)
			{		
			case	ERASERTOOL:
				m_EraserBtn.SetSelect(FALSE);
				break;	
			case	DIAGRAMTOOL:
				m_DiagramBtn.SetSelect(FALSE);
				break;
			}

			m_nPenToolType = PENTOOL;
			SetPenType(m_nPenType);
			SetImageCursor();
		}		
		break;
	case	IDC_ERASERBTN:
		m_EraserBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_EraserSelectDlg.ShowWindow(SW_SHOW);
		m_EraserSelectDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = ERASERSELECT_POPUP;

		if(m_nEraserType == ERASER || m_nEraserType == ERASER_RECT)
		{
			if(m_nPenToolType != ERASERTOOL)
			{
				switch(m_nPenToolType)
				{
				case	PENTOOL:
					m_PenBtn.SetSelect(FALSE);
					break;
				case	DIAGRAMTOOL:
					m_DiagramBtn.SetSelect(FALSE);
					break;
				}

				m_nPenToolType = ERASERTOOL;
				SetPenType(m_nEraserType);
				SetImageCursor();		
			}		
		}
		else
		{
			SetPenType(m_nEraserType);
			m_EraserBtn.SetSelect(FALSE);
		}
		break;
	case	IDC_COLORCHANGEBTN:
		m_ColorChangeBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_ColorSelctDlg.ShowWindow(SW_SHOW);
		m_ColorSelctDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = COLORSELECT_POPUP;
		break;
	case	IDC_LINEWEIGHTCHANGEBTN:
		m_LineWeightBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_LineWeightSelectDlg.ShowWindow(SW_SHOW);
		m_LineWeightSelectDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = LINEWEIGHTSELECT_POPUP;
		break;
	case	IDC_DIAGRAMBTN:
		m_DiagramBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_DiagramSelectDlg.ShowWindow(SW_SHOW);
		m_DiagramSelectDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = DIAGRAMSELECT_POPUP;

		if(m_nPenToolType != DIAGRAMTOOL)
		{
			switch(m_nPenToolType)
			{
			case	PENTOOL:
				m_PenBtn.SetSelect(FALSE);
				break;
			case	ERASERTOOL:
				m_EraserBtn.SetSelect(FALSE);
				break;
			}

			m_nPenToolType = DIAGRAMTOOL;
			SetPenType(m_nDiagramType);
			SetImageCursor();
		}
		break;
	case	IDC_BOARDOPENBTN:
		m_BoardOpenBtn.GetWindowRect(&m_rcEnable);
		ScreenToClient(&m_rcEnable);
		m_rcEnable.InflateRect(0,3,0,3);
		m_BoardSelectDlg.ShowWindow(SW_SHOW);
		m_BoardSelectDlg.GetWindowRect(&m_rcEnableMenu);
		m_nSubMenuPopup = BOARDSELECT_POPUP;
		break;
	}

}

void CAuthorDlg::HidePopupMenu()
{	
	switch(m_nSubMenuPopup)
	{
	case	PENSELECT_POPUP:
		m_PenSelectDlg.ShowWindow(SW_HIDE);
		break;
	case	ERASERSELECT_POPUP:
		m_EraserSelectDlg.ShowWindow(SW_HIDE);
		break;
	case	COLORSELECT_POPUP:
		m_ColorSelctDlg.ShowWindow(SW_HIDE);
		break;
	case	LINEWEIGHTSELECT_POPUP:
		m_LineWeightSelectDlg.ShowWindow(SW_HIDE);
		break;
	case	DIAGRAMSELECT_POPUP:
		m_DiagramSelectDlg.ShowWindow(SW_HIDE);
		break;
	case	BOARDSELECT_POPUP:
		m_BoardSelectDlg.ShowWindow(SW_HIDE);
		break;
	}

	m_nSubMenuPopup = NONE_POPUP;

	m_nSelMouseOverCtrlID = -1;
	m_rcEnable = CRect(-1,-1,-1,-1);
	m_rcEnableMenu = CRect(-1,-1,-1,-1);
}

void CAuthorDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nSubMenuPopup != NONE_POPUP)
	{	
		if(!m_rcEnable.PtInRect(point))
		{
			ClientToScreen(&point);
			if(!m_rcEnableMenu.PtInRect(point))
				HidePopupMenu();
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}



/////////////////////////////////////////////////////////////////////////////
//
LRESULT CAuthorDlg::OnNotifyPenSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	if(m_nPenToolType != PENTOOL)
	{
		m_PenBtn.SetSelect();

		switch(m_nPenToolType)
		{		
		case	ERASERTOOL:
			m_EraserBtn.SetSelect(FALSE);
			m_EraserBtn.UpdateWindow();
			break;
		case	DIAGRAMTOOL:
			m_DiagramBtn.SetSelect(FALSE);
			m_DiagramBtn.UpdateWindow();
			break;
		}

		m_nPenToolType = PENTOOL;
		
		if(m_nPenType != (int)wParam)
		{
			m_nPenType = (int)wParam;

			switch(m_nPenType)
			{
			case	PENCIL:
				m_PenBtn.SetBitmaps(IDB_PENCIL, RGB(255,0,255));				
				break;
			case	BRUSH:
				m_PenBtn.SetBitmaps(IDB_BRUSH, RGB(255,0,255));				
				break;
			case	HIGHLIGHTPEN:
				m_PenBtn.SetBitmaps(IDB_HIGHLIGHTPEN, RGB(255,0,255));				
				break;
			}
			
			m_PenBtn.UpdateWindow();
		}		

		SetPenType(m_nPenType);
		SetImageCursor();
	}
	else
	{
		if(m_nPenType != (int)wParam)
		{
			m_nPenType = (int)wParam;

			switch(m_nPenType)
			{
			case	PENCIL:
				m_PenBtn.SetBitmaps(IDB_PENCIL, RGB(255,0,255));
				break;
			case	BRUSH:
				m_PenBtn.SetBitmaps(IDB_BRUSH, RGB(255,0,255));
				break;
			case	HIGHLIGHTPEN:
				m_PenBtn.SetBitmaps(IDB_HIGHLIGHTPEN, RGB(255,0,255));				
				break;
			}

			m_PenBtn.UpdateWindow();

			SetPenType(m_nPenType);
			SetImageCursor();
		}
	}	

	return 0;
}


LRESULT CAuthorDlg::OnNotifyEraserSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();
	
	if((int)wParam == ERASER || (int)wParam == ERASER_RECT)
	{
		if(m_nPenToolType != ERASERTOOL)
		{
			m_EraserBtn.SetSelect();

			switch(m_nPenToolType)
			{
			case	PENTOOL:
				m_PenBtn.SetSelect(FALSE);
				m_PenBtn.UpdateWindow();
				break;
			case	DIAGRAMTOOL:
				m_DiagramBtn.SetSelect(FALSE);
				m_DiagramBtn.UpdateWindow();
				break;
			}

			m_nPenToolType = ERASERTOOL;
		}

		if(m_nEraserType != (int)wParam)
		{
			m_nEraserType = (int)wParam;

			switch(m_nEraserType)
			{
			case	ERASER:
				m_EraserBtn.SetBitmaps(IDB_ERASER, RGB(255,0,255));
				break;
			case	ERASER_RECT:
				m_EraserBtn.SetBitmaps(IDB_ERASERRECT, RGB(255,0,255));
				break;
			}
			
			m_EraserBtn.UpdateWindow();
		}
		
		SetPenType(m_nEraserType);
		SetImageCursor();
	}
	else
	{
		if(m_nPenToolType != ERASERTOOL)
		{
			m_EraserBtn.SetSelect(FALSE);

			if(m_nEraserType != (int)wParam)
			{			
				m_nEraserType = (int)wParam;

				switch(m_nEraserType)
				{
				case	ERASER_ALL:
					m_EraserBtn.SetBitmaps(IDB_ERASERALL, RGB(255,0,255));
					break;
				case	UNDO:
					m_EraserBtn.SetBitmaps(IDB_UNDO, RGB(255,0,255));
					break;
				}

				m_EraserBtn.UpdateWindow();
			}
		}
		else
		{
			if(m_nEraserType != (int)wParam)
			{			
				m_nEraserType = (int)wParam;
				switch(m_nEraserType)
				{
				case	ERASER_ALL:
					m_EraserBtn.SetBitmaps(IDB_ERASERALL, RGB(255,0,255));
					break;
				case	UNDO:
					m_EraserBtn.SetBitmaps(IDB_UNDO, RGB(255,0,255));
					break;
				}

				m_EraserBtn.UpdateWindow();
			}			
		}
		
		SetPenType(m_nEraserType);
		SetImageCursor();
	}

	return 0;
}


LRESULT CAuthorDlg::OnNotifyColorSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	m_Color = (COLORREF)wParam;
	m_ColorChangeBtn.SetButtonColor(m_Color);

	m_pen.color = XInk::xColor(m_Color, 255);
	m_pCanvas->SetPen(m_pen);

	return 0;
}


LRESULT CAuthorDlg::OnNotifyLineWeightSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	m_PenWidth = (int)wParam;
	m_LineWeightBtn.SetButtonWidth(m_PenWidth);

	m_pen.width = (float)m_PenWidth;
	m_pCanvas->SetPen(m_pen);

	return 0;
}


LRESULT CAuthorDlg::OnNotifyDiagramSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	if(m_nPenToolType != DIAGRAMTOOL)
	{
		m_DiagramBtn.SetSelect();

		switch(m_nPenToolType)
		{
		case	PENTOOL:
			m_PenBtn.SetSelect(FALSE);
			m_PenBtn.UpdateWindow();
			break;
		case	ERASERTOOL:
			m_EraserBtn.SetSelect(FALSE);
			m_EraserBtn.UpdateWindow();
			break;
		}

		m_nPenToolType = DIAGRAMTOOL;

		if(m_nDiagramType != (int)wParam)
		{
			m_nDiagramType = (int)wParam;

			switch(m_nDiagramType)
			{
			case	DIAGRAM_ELLIPSE:
				m_DiagramBtn.SetBitmaps(IDB_ELLIPSE, RGB(255,0,255));
				break;
			case	DIAGRAM_RECT:
				m_DiagramBtn.SetBitmaps(IDB_RECT, RGB(255,0,255));
				break;
			case	DIAGRAM_LINE:
				m_DiagramBtn.SetBitmaps(IDB_LINE, RGB(255,0,255));
				break;	
			}	
			
			m_DiagramBtn.UpdateWindow();
		}	
		
		SetPenType(m_nDiagramType);		
		SetImageCursor();
	}
	else
	{
		if(m_nDiagramType != (int)wParam)
		{
			m_nDiagramType = (int)wParam;

			switch(m_nDiagramType)
			{
			case	7:
				m_DiagramBtn.SetBitmaps(IDB_ELLIPSE, RGB(255,0,255));
				break;
			case	8:
				m_DiagramBtn.SetBitmaps(IDB_RECT, RGB(255,0,255));
				break;
			case	9:
				m_DiagramBtn.SetBitmaps(IDB_LINE, RGB(255,0,255));
				break;	
			}

			m_DiagramBtn.UpdateWindow();

			SetPenType(m_nDiagramType);
			SetImageCursor();
		}
	}

	return 0;
}


LRESULT CAuthorDlg::OnNotifyBoardSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_nSubMenuPopup != NONE_POPUP)
		HidePopupMenu();

	int nBoardType = (int)wParam;

	switch (nBoardType)
	{
	case	WHITE_BOARD:
		m_BoardOpenBtn.SetBitmaps(IDB_WHITEBOARD, RGB(255,0,255));
		break;
	case	BLACK_BOARD:
		m_BoardOpenBtn.SetBitmaps(IDB_BLACKBOARD, RGB(255,0,255));
		break;
	case	MUSIC_BOARD:
		m_BoardOpenBtn.SetBitmaps(IDB_MUSICBOARD, RGB(255,0,255));
		break;
	case	WRITE_BOARD:
		m_BoardOpenBtn.SetBitmaps(IDB_WRITEBOARD, RGB(255,0,255));
		break;
	}

	OpenBoard(nBoardType);

	return 0;
}



LRESULT CAuthorDlg::OnNotifySlideMenuSelect(WPARAM wParam, LPARAM lParam)
{	
	if(wParam)
	{		
		CString strSlide = m_SlideMenuDlg.GetSelectSlide();

		if(m_bBoardOpen)
		{
			if(m_strSlidePath.IsEmpty())
			{
				if(m_bDrawCanvas)
				{
					CString strPath;
					strPath.Format("%s\\Slide%d.bmp", CAppSettings::GetSlidePath(), m_nSlideIndex);
					if(CaptureDesktop(strPath))
					{				
					//	strPath.Format("Slide%d", m_nSlideIndex);
						m_SlideMenuDlg.AddSlide(strPath);

						m_nSlideIndex++;
					}
				}				
			}
			else if(m_bDrawCanvas)
			{
				CaptureDesktop(m_strSlidePath);
				m_SlideMenuDlg.ReplaceSildeThumbnail(m_strSlidePath);
				m_strSlidePath = "";
			}

			if(m_bDrawCanvas)
			{
				m_bDrawCanvas = false;
			}
			
			m_strSlidePath = strSlide;
			m_BoardDlg.SetBoardImage(m_strSlidePath);
			SetPPTSlideShowHide();
			m_BoardDlg.ShowWindow(SW_SHOW);
		}
		else
		{	
			if(!m_ModeBtn.IsSelected())
			{
				m_ModeBtn.SetSelect();
				OnModebtn();
			}
			else
			{	
				if(m_bDrawCanvas)
				{
					m_bDrawCanvas = false;
				}
			}

			m_strSlidePath = strSlide;

			m_BoardDlg.SetBoardImage(m_strSlidePath);
			SetPPTSlideShowHide();
			m_BoardDlg.ShowWindow(SW_SHOW);
			m_bBoardOpen = true;
		}
	}
	
	EnableWindow();

	return 0;
}



void CAuthorDlg::SetPenType(int nType)
{
	switch(nType)
	{
	case	PENCIL:		
		m_pen.dtool = XInk::DT_Pen;
		m_pCanvas->SetPen(m_pen);		
		break;
	case	BRUSH:
		m_pen.dtool = XInk::DT_ChinesePen;
		m_pCanvas->SetPen(m_pen);		
		break;
	case	ERASER:
		m_pen.dtool = XInk::DT_Eraser;
		m_pCanvas->SetPen(m_pen);
		break;
	case	ERASER_RECT:
		m_pen.dtool = XInk::DT_EraserRect;
		m_pCanvas->SetPen(m_pen);
		break;
	case	ERASER_ALL:	
		if(m_ModeBtn.IsSelected() && m_bDrawCanvas)
		{			
			CWaitCursor w;
			if (m_pCanvas)
				m_pCanvas->EraseAll();
			m_bDrawCanvas = false;
		}
		break;
	case	UNDO:
		if(m_ModeBtn.IsSelected())
		{
			CWaitCursor w;
			m_pCanvas->Undo();
		}
		break;
	case	DIAGRAM_ELLIPSE:
		m_pen.dtool = XInk::DT_ShapeEllipse;
		m_pCanvas->SetPen(m_pen);
		break;
	case	DIAGRAM_RECT:
		m_pen.dtool = XInk::DT_ShapeRect;
		m_pCanvas->SetPen(m_pen);
		break;
	case	DIAGRAM_LINE:		
		if(m_nLinePattern == 0)
			m_pen.dtool = XInk::DT_ShapeLine;
		else
			m_pen.dtool = XInk::DT_ShapeLineArrow;

		m_pCanvas->SetPen(m_pen);
		break;
	case	HIGHLIGHTPEN:
		if(m_nHLPenPattern == 0)
			m_pen.dtool = XInk::DT_HighLighter;
		else
			m_pen.dtool = XInk::DT_HighLighterEllipse;

		m_pCanvas->SetPen(m_pen);		
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CAuthorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case RECORDING_TIMER:
		{
			m_nRecProgress++;
			if(m_nRecProgress == 7)
				m_nRecProgress = 0;

			DisplayRecTime();
			
			if(m_bSmallMode)
			{
				if(m_ShowhideBtn.IsSelected())
					InvalidateRect(&m_rcRecProgress);
			}
			else
			{
				if(!m_ShowhideBtn.IsSelected())
					InvalidateRect(&m_rcRecProgress);
				else if(m_bShowControl[7])
					InvalidateRect(&m_rcHideRecProgress);
			}
		}
		break;
	case	REGSTRATION_TIMER:
		KillTimer(REGSTRATION_TIMER);
		OnRegstrationDlg();
		break;	
	case	RECORD_SELECTWINDOW_TIMER:
		DisplaySelectWindow();
		break;
	case	RECORD_SELECTRANGE_TIMER:
		KillTimer(RECORD_SELECTRANGE_TIMER);		
		InitRecord();
		break;
	case	RECORD_MARKINGRANGE_TIMER:
		MarkingRecArea();
		break;
	case	REDRAW_TIMER:
		KillTimer(REDRAW_TIMER);
		OnNotifyTextInputComplete();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CAuthorDlg::SetRecordTerminate()
{
	if(m_hCapture != NULL)
	{
		if(m_bMarkingRecArea)
		{
			m_bMarkingRecArea = false;		
			KillTimer(RECORD_MARKINGRANGE_TIMER);
			::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);	
		}

		//	rstStarted = 1,	rstRecording = 2, rstPaused = 3, rstStopped = 4
		int nState = ::srecGetCaptureState( m_hCapture );
		if(nState != rstStopped)
		{
			BOOL	hr;
			hr = ::srecStopCapture( m_hCapture );
			if(!hr)
			{
				AfxMessageBox("Error : Failed! stop AVI capture");
				return FALSE;
			}
		}

		::srecDestroyCapture( m_hCapture );

		m_hCapture = NULL;
		m_bInitRecord = false;

		m_RecordBtn.EnableWindow(TRUE);	
		m_RecordBtn.SetSelect(FALSE);
		m_RecordPauseBtn.EnableWindow(FALSE);
		m_RecordStopBtn.EnableWindow(FALSE);

		KillTimer(RECORDING_TIMER);		
		m_strRecState = "READY";
		m_strRecTime = "00:00:00";
		m_nRecProgress = 0;	

		CRect rc;
		if(m_bSmallMode)
		{
			if(m_ShowhideBtn.IsSelected())
			{
				rc.left = m_rcRecState.left;
				rc.top = m_rcRecTime.top;
				rc.right = m_rcRecProgress.right;
				rc.bottom = m_rcRecProgress.bottom;

				InvalidateRect(&rc);
			}
		}
		else
		{
			if(!m_ShowhideBtn.IsSelected())
			{
				rc.left = m_rcRecState.left;
				rc.top = m_rcRecTime.top;
				rc.right = m_rcRecProgress.right;
				rc.bottom = m_rcRecProgress.bottom;

				InvalidateRect(&rc);
			}
			else if(m_bShowControl[7])
			{			
				rc.left = m_rcHideRecState.left;
				rc.top = m_rcHideRecTime.top;
				rc.right = m_rcHideRecProgress.right;
				rc.bottom = m_rcHideRecProgress.bottom;

				InvalidateRect(&rc);
			}
		}
	}

	return TRUE;
}



void CAuthorDlg::SetPPTSlideShowHide(bool bHide)
{
	if(bHide)
	{
		if(CPPTUtil::IsPPTActive() && !m_bPPTSlideShowMinize)
		{
			CWnd* pWnd = FindWindow("screenClass", NULL);
			if(pWnd != NULL)
				pWnd->ShowWindow(SW_HIDE);//>SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);

			m_bPPTSlideShowMinize = true;
		}
	}
	else
	{
		if(CPPTUtil::IsPPTActive() && m_bPPTSlideShowMinize)
		{
			CWnd* pWnd = FindWindow("screenClass", NULL);
			if(pWnd != NULL)
				pWnd->ShowWindow(SW_SHOW);//>SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

			m_bPPTSlideShowMinize = false;			
		}		
	}
}


void CAuthorDlg::OnDestroy() 
{
	SetRecordTerminate();

	SetPPTSlideShowHide(false);
	m_BoardDlg.DestroyWindow();
	m_MagnifierDlg.DestroyWindow();

	ReMoveAllTextClp();

	if(m_hCursor != NULL)
		::DestroyIcon(m_hCursor);

	ReleaseCanvas();

	CString szFilename;	
	szFilename.Format("%s\\*.*", CAppSettings::GetTempPath());	

	CFileFind finder;
	BOOL bWorking = finder.FindFile(szFilename);	
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		// skip
		if( finder.IsDots() )
			continue;
		// if it's a directory
		if( finder.IsDirectory() )
			continue;

		szFilename = finder.GetFilePath();
		SetFileAttributes(szFilename, FILE_ATTRIBUTE_NORMAL);
		::DeleteFile(szFilename);
	}

	szFilename.Format("%s\\*.*", CAppSettings::GetWorkPath());	
	bWorking = finder.FindFile(szFilename);	
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		// skip
		if( finder.IsDots() )
			continue;
		// if it's a directory
		if( finder.IsDirectory() )
			continue;

		szFilename = finder.GetFilePath();
		SetFileAttributes(szFilename, FILE_ATTRIBUTE_NORMAL);
		::DeleteFile(szFilename);
	}
	finder.Close();

	if(m_hBitmapLogo != NULL)
		::DeleteObject(m_hBitmapLogo);

	HWND hWnd = NULL;
	hWnd = ::FindWindow("{ED7AFEB4-AB6A-43ed-B4C2-84F5367336BC}", 0);
	if(hWnd != NULL)
		::PostMessage(hWnd, WM_QUIT,0,0);

	CDialog::OnDestroy();
}

void CAuthorDlg::ReMoveAllTextClp()
{
	POSITION   pos;
    CTextDlg *TextDlg;

	for( pos = m_TextList.GetHeadPosition(); pos != NULL; ) {
        TextDlg = (CTextDlg*)m_TextList.GetNext(pos);
		TextDlg->DestroyWindow();
        delete TextDlg;                    // Deletion avoids memory leak.        
    }  
	
    m_TextList.RemoveAll();
}


LRESULT CAuthorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (::IsWindow(GetSafeHwnd()))
	{			
		if(message == WM_XINK_STARTSTROKE)
		{
			if(m_bSelectRecordWindow)
			{
				DisplaySelectWindow(true);
				return TRUE;
			}

			if(m_hCapture != NULL)
				::srecSetDrawing(m_hCapture, TRUE);

			m_pCanvas->StartStroke();

			int ptX, ptY;
			ptX = LOWORD(lParam);
			ptY = HIWORD(lParam);

			if(m_bSelectRecordArea)
			{
				m_rcCurRecordRangeWindow.left = ptX;
				m_rcCurRecordRangeWindow.top = ptY;
				
				m_RecSizeDlg.MoveWindow(CRect(ptX+5, ptY+5, ptX + 75, ptY + 20));
				m_RecSizeDlg.SetStartPosition(ptX, ptY);
				m_RecSizeDlg.ShowWindow(SW_SHOW);
				return TRUE;
			}

			if(m_bZoomMode)
			{
				m_rcZoom.left = ptX;
				m_rcZoom.top = ptY;
			}
			else
				m_bDrawCanvas = true;

			return TRUE;
		}
		else if(message == WM_XINK_STOPSTROKE)
		{	
			int ptX, ptY;
			ptX = LOWORD(lParam);
			ptY = HIWORD(lParam);	

			m_pCanvas->AddPoint(ptX, ptY);
			m_pCanvas->StopStroke();

			if(m_hCapture != NULL)
				::srecSetDrawing(m_hCapture, FALSE);

			if(m_bSelectRecordArea)
			{	
				m_rcCurRecordRangeWindow.right = ptX;
				m_rcCurRecordRangeWindow.bottom = ptY;

				EndSelectArea();
				
				m_RecSizeDlg.ShowWindow(SW_HIDE);
				m_RecSizeDlg.SetStartPosition(ptX, ptY);

				return TRUE;
			}

			if(m_bZoomMode)
			{
				m_rcZoom.right = ptX;
				m_rcZoom.bottom = ptY;

				OnZoomShot();					
			}
			return TRUE;
		}
		else if(message == WM_XINK_ADDPOINT)
		{
			if (m_pCanvas->IsStrokeStarted())
			{
				int ptX, ptY;

				ptX = LOWORD(lParam);
				ptY = HIWORD(lParam);

				m_pCanvas->AddPoint(ptX, ptY);

				if(m_bSelectRecordArea)
					m_RecSizeDlg.SetPosition(ptX, ptY);
					
			}
		
			return TRUE;
		}	
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CAuthorDlg::PreTranslateMessage(MSG* pMsg) 
{		

	if(pMsg->message == WM_SYSKEYDOWN )
	{
		UINT WPARAM = pMsg->wParam;

		switch(WPARAM)
		{
		case	VK_F4:
			return TRUE;
		case	77:				// "M"	
			if(!m_bZoomMode)		
			{
				if(m_ModeBtn.IsSelected())
					m_ModeBtn.SetSelect(FALSE);
				else
					m_ModeBtn.SetSelect();
				
				OnModebtn();
			}
			break;
		case	80:				// "P"
			if(!m_bZoomMode)		
			{
				m_PenBtn.SetSelect();
				OnPenbtn();
				m_PenBtn.Invalidate();
			}
			break;
		case	84:				// "T"
			if(!m_bZoomMode)		
			{
				m_TextBtn.SetSelect();
				OnTextBtn();
				m_TextBtn.Invalidate();
			}
			break;
		case	69:				// "E"	
			if(!m_bZoomMode)		
			{
				m_EraserBtn.SetSelect();
				OnEraserbtn();
				m_EraserBtn.Invalidate();
			}
			break;
		case	68:				// "D"	
			if(!m_bZoomMode)		
			{
				m_DiagramBtn.SetSelect();
				OnDiagrambtn();
				m_DiagramBtn.Invalidate();
			}
			break;
		case	79:				// "O"
			if(!m_bZoomMode)		
				OnOptionbtn();
			break;
		case	82:				// "R"
			if(!m_RecordBtn.IsSelected() && !m_bZoomMode)			
			{
				m_RecordBtn.SetSelect();
				OnRecordbtn();
			}
			break;		
		case	66:				// "B"
			if(!m_bZoomMode)		
				OnBoardOpenbtn();
			break;
		case	90:				// "Z"
			if(!m_bZoomMode)		
				OnZoombtn();
			break;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{	
		UINT WPARAM = pMsg->wParam;
		switch(WPARAM)
		{
		case	VK_ESCAPE:		// Esc, Enter key down
			if(m_bTextRange && !m_pCanvas->IsStrokeStarted())
			{
				m_bZoomMode = false;

				SetPenType(m_nPenType);	
				SetImageCursor();

				EnableWindow();
				m_bTextRange = false;
			}
		case	VK_RETURN:			
			return	TRUE;
		case	VK_LEFT:
			if(!m_bZoomMode)		
				OnPptprevbtn();
			break;
		case	VK_RIGHT:
			if(!m_bZoomMode)		
				OnPptnextbtn();
			break;
		case	VK_SPACE:
			if(!m_bZoomMode)		
				OnRecordpausebtn();
			return TRUE;
		case	VK_F1:
			OnHelp();
			break;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAuthorDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);
}
