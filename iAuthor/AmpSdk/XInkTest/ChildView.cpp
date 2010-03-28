/*===========================================================================
| Project     : SrecSDK Tester
| Module      : ChildView
|               C:\Projects\raguru\AmpSdk040528\XInk\ChildView.cpp
| Author      : raguru
| Description : 
|
+----------------------------------------------------------------------------
| 2004/05/09 (raguru)	Add a menu of the DT_HighLighterEllipse
| 2004/03/05 (raguru)	Create
| 
+============================================================================
*/


#include "stdafx.h"
#include "XInkTest.h"
#include "ChildView.h"

#include "../include/bclscrrec.h"


SRECHANDLE  hCapture=NULL;

/////////////////////////////////////////////////////////////////////////////
// CChildView
DEVMODE g_LatestDevMode;

CChildView::CChildView() :
m_pen(XInk::DT_Pen, XInk::xColor(RGB(0,0,0), 255), 1.0)
, m_bDisableHardwareAcceleration(false)
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	m_pCanvas = new XInk::xCanvas(0, XInk::xRect(0,0, cx,cy));

	m_pCanvas->SetPen(m_pen);

	m_pCanvas->EnableAutoDrawing(true);

	BOOL success = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &g_LatestDevMode);
}

CChildView::~CChildView()
{
	OnRecordStop();
	delete m_pCanvas;
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_COLOR_BLACK, OnColorBlack)
	ON_COMMAND(ID_COLOR_GREEN, OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, OnColorBlue)
	ON_COMMAND(ID_COLOR_RED, OnColorRed)
	ON_COMMAND(ID_COLOR_WHITE, OnColorWhite)
	ON_COMMAND(ID_WIDTH_01, OnWidth01)
	ON_COMMAND(ID_WIDTH_02, OnWidth02)
	ON_COMMAND(ID_WIDTH_03, OnWidth03)
	ON_COMMAND(ID_WIDTH_05, OnWidth05)
	ON_COMMAND(ID_WIDTH_07, OnWidth07)
	ON_COMMAND(ID_WIDTH_09, OnWidth09)
	ON_COMMAND(ID_WIDTH_20, OnWidth20)
	ON_COMMAND(ID_WIDTH_30, OnWidth30)
	ON_COMMAND(ID_DTOOL_CHPEN, OnDtoolChpen)
	ON_COMMAND(ID_DTOOL_ERASER, OnDtoolEraser)
	ON_COMMAND(ID_DTOOL_HLIGHTER, OnDtoolHlighter)
	ON_COMMAND(ID_DTOOL_PEN, OnDtoolPen)
	ON_COMMAND(ID_DTOOL_ERASER_ALL, OnDtoolEraserAll)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_DTOOL_ERASER_RECT, OnDtoolEraserRect)
	ON_COMMAND(ID_DTOOL_SHAPE_LINE, OnDtoolShapeLine)
	ON_COMMAND(ID_DTOOL_SHAPE_LINE_ARROW, OnDtoolShapeLineArrow)
	ON_COMMAND(ID_DTOOL_SHAPE_RECT, OnDtoolShapeRect)
	ON_COMMAND(ID_DTOOL_SHAPE_ELLIPSE, OnDtoolShapeEllipse)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DTOOL_HLIGHTER_EL, OnDtoolHlighterEl)
	ON_COMMAND(ID_RECORD_START, OnRecordStart)
	ON_COMMAND(ID_RECORD_STOP, OnRecordStop)
	ON_COMMAND(ID_WIDTH_15, OnWidth15)
	ON_UPDATE_COMMAND_UI(ID_APP_HACCEL, OnUpdateAppHaccel)
	ON_COMMAND(ID_APP_HACCEL, OnAppHaccel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	srecSetDrawing(hCapture, TRUE);
	m_pCanvas->StartStroke();
	CWnd ::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// start stoke?
	if ((nFlags & MK_LBUTTON) && m_pCanvas->IsStrokeStarted())
	{
		this->ClientToScreen(&point);
		m_pCanvas->AddPoint(point.x, point.y);
	}
	//else if (!(nFlags & MK_LBUTTON) && m_pCanvas->IsStrokeStarted())
	//{
	//	m_pCanvas->StopStroke();
	//}

	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pCanvas->IsStrokeStarted())
	{
		this->ClientToScreen(&point);
		m_pCanvas->StopStroke();
		srecSetDrawing(hCapture, FALSE);
	}
	
	CWnd ::OnLButtonUp(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//this->Invalidate();	// temporary screen clear

	CWnd ::OnRButtonDown(nFlags, point);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}


void CChildView::OnViewRefresh() 
{
	CWaitCursor w;
	m_pCanvas->Refresh();
}

void CChildView::OnEditUndo() 
{
	CWaitCursor w;
	m_pCanvas->Undo();
}

void CChildView::OnColorBlack() 
{
	m_pen.color = XInk::xColor(RGB(0,0,0));
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnColorGreen() 
{
	m_pen.color = XInk::xColor(RGB(0,255,0));
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnColorBlue() 
{
	m_pen.color = XInk::xColor(RGB(0,0,255));
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnColorRed() 
{
	m_pen.color = XInk::xColor(RGB(255,0,0));
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnColorWhite() 
{
	m_pen.color = XInk::xColor(RGB(255,255,255));
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth01()
{
	m_pen.width = 1.0f;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth02()
{
	m_pen.width = 2.0f;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth03()
{
	m_pen.width = 3.0f;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth05()
{
	m_pen.width = 5.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth07()
{
	m_pen.width = 7.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth09() 
{
	m_pen.width = 9.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth15() 
{
	m_pen.width = 15.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth20() 
{
	m_pen.width = 20.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnWidth30() 
{
	m_pen.width = 30.0;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolPen() 
{
	m_pen.dtool = XInk::DT_Pen;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolChpen() 
{
	m_pen.dtool = XInk::DT_ChinesePen;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolHlighter() 
{
	m_pen.dtool = XInk::DT_HighLighter;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolHlighterEl() 
{
	m_pen.dtool = XInk::DT_HighLighterEllipse;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolEraser() 
{
	m_pen.dtool = XInk::DT_Eraser;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolEraserRect() 
{
	m_pen.dtool = XInk::DT_EraserRect;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolEraserAll() 
{
	CWaitCursor w;
	m_pCanvas->EraseAll();
}


void CChildView::OnDtoolShapeLine() 
{
	m_pen.dtool = XInk::DT_ShapeLine;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolShapeLineArrow() 
{
	m_pen.dtool = XInk::DT_ShapeLineArrow;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolShapeRect() 
{
	m_pen.dtool = XInk::DT_ShapeRect;
	m_pCanvas->SetPen(m_pen);
}

void CChildView::OnDtoolShapeEllipse() 
{
	m_pen.dtool = XInk::DT_ShapeEllipse;
	m_pCanvas->SetPen(m_pen);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	return CWnd ::OnEraseBkgnd(pDC);
}


void CChildView::OnRecordStart() 
{
	int fps = 10;
	DWORD vcodec = vcmMSVC;//vcmTSCC;//vcmMSVC;

	CString strPathName;
	CString strFilter = "Avi Files (.avi)|*.avi|All Files (*.*)|*.*||";

	CFileDialog fd(TRUE, "", "", OFN_OVERWRITEPROMPT, (LPCTSTR)strFilter, this);
	if(fd.DoModal() != IDOK)
		return;

	strPathName = fd.GetPathName();
	if (strPathName.IsEmpty())
		return;
	
	SendMessage(WM_PAINT);
	
	hCapture = srecCreateCapture( "SRECSDK00001-147AA447D127-86AE49659587-924D3BEFF365" );
	if ( hCapture == NULL )
	{
		AfxMessageBox("Capture create failed!");
		return;
	}

	// Configure to capture video and audio device identifier
	srecSetAudioCaptureDevice( hCapture, DEFAULT_AUDIO_DEVICE );	// from first wavein device
	srecSetVideoCaptureDevice( hCapture, DEFAULT_VIDEO_DEVICE );	// from screen

	// if video device is screen the configure to capture entire screen or the window
	// and save to an AVI file
	int sx = ::GetSystemMetrics(SM_CXSCREEN);
	int sy = ::GetSystemMetrics(SM_CYSCREEN);
	srecSetScreenInput( hCapture, cinScreen, NULL, NULL );
/*	if (sx < 1024)
	{
		srecSetScreenInput( hCapture, cinScreen, NULL, NULL );
	}
	else
	{
		RECT rc = {0,0, 1024, 768};
		srecSetScreenInput( hCapture, cinScreen, NULL, &rc );
	}
*/

	// Configure to capture AVI Options
	DWORD dwFps = fps;
	DWORD dwQuality = 8000; // 0 ~ 10000
	DWORD dwKeyFramesEvery = 0;
	srecSetAviOptions( hCapture, dwFps, dwQuality, dwKeyFramesEvery );

	// Configure to capture Audio and Video Codecs
	srecSetAviVideoCodec( hCapture, vcodec, true );	// 3rd param(new) is option to use system compressor dialog.
	srecSetAviAudioCodec( hCapture, acmPCM );	//acmGSM610 );	// some audio card doesn't support GSM610 44.1KHz.

	// Toggle enable audio
	srecSetAudioEnable( hCapture, TRUE);//TRUE FALSE );

	// Recording cursor shape ?
	srecSetRecordCursor( hCapture, TRUE );

	// An output file name MUST be specified to create an output file
	//char* pszFile = "ScrTest.avi";
	srecSetFileName( hCapture, (LPCTSTR) strPathName);


	// Step 3 - Controlling the Capture Session
	// Once the capture object is configured correctly, start the capture session.
	// You have complete control to Pause, Resume, Stop or Abort using the capture methods.
	// See the programming reference for a complete list.

	if ( srecStartCapture( hCapture ) != SCRREC_SUCCESS )
	{
		CString msg;
		msg.Format("Error: %s", srecGetLastErrorMsg ( srecGetLastError( hCapture ) ));
		AfxMessageBox(msg);

		OnRecordStop();//srecDestroyCapture( hCapture ); 
		return;
	}
	
}

void CChildView::OnRecordStop() 
{
	CWaitCursor wait;

	if (!hCapture)
		return;

	// Finish the capture session
	srecStopCapture( hCapture );


	// Note that the srecSetStateChangeCallback can be used to automatically obtain
	// the capture state in a state change event callback function.

   	// Display capture statistics¡¦
	// The capture statistics will be reset when a new capture session is started
	// (  i.e. the statistics will persist until  srecStartCapture is called again ).
	/*
	CString msg;
   	printf("\n\n\n");
   	printf("==============================================================\n");
   	printf("**File('%s') Capture statistics...\n", pszFile);
   	printf("--------------------------------------------------------------\n");
	printf("## %d Frames Captured, \t duration is %d secs\n", srecGetCaptureFrames( hCapture ), srecGetDuration( hCapture ));
	printf("## %d Frames Dropped\n", srecGetDroppedFrames( hCapture ));
	printf("## Actual frame rate = %5.2f fps\n", srecGetActualFrameRate( hCapture ));
	printf("\n## Last Error Message: %s\n\n\n", srecGetLastErrorMsg( srecGetLastError( hCapture ) ));
	msg.Format("Error: %s", srecGetLastErrorMsg ( srecGetLastError( hCapture ) ));
	AfxMessageBox(msg);
	*/


	// Step 5 - Destroying the Capture Object
	// After using srecStopCapture to finish a capture session to discard a capture session,
	// you may continue to use the capture handle to reconfigure and begin a new capture session.
	// If you not longer need to capture the screen, be sure to destroy the capture object
	// by calling srecDestroyCapture.

	srecDestroyCapture( hCapture );

	hCapture = NULL;
}


void CChildView::OnUpdateAppHaccel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetCheck((int) m_bDisableHardwareAcceleration);
}

void CChildView::OnAppHaccel() 
{
	m_bDisableHardwareAcceleration = !m_bDisableHardwareAcceleration;
	SetVideoHWAccelerationLevel(m_bDisableHardwareAcceleration ? 5 : 0);
}

//
// The video hardware acceleration mode have 6 levels (0 ~ 5)
// 0: Full acceleration mode (most right position of the slider)
// 5: Disable acceleration mode (most left position of the slider)
//
int CChildView::GetVideoHWAccelerationLevel()
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
	videoKey.Replace(_T("\\Registry\\Machine\\"), _T(""));


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

//
// The video hardware acceleration mode have 6 levels (0 ~ 5)
// 0: Full acceleration mode (most right position of the slider)
// 5: Disable acceleration mode (most left position of the slider)
//
bool CChildView::SetVideoHWAccelerationLevel(const int level, const bool bForever)
{
	HKEY hk;
	BYTE buff[1024];
	DWORD buff_siz=1024;
	LONG ret;

	int orgLevel = GetVideoHWAccelerationLevel();
	if (orgLevel == level)
		return true;


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
		return false;
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
	videoKey.Replace(_T("\\Registry\\Machine\\"), _T(""));
	
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
	{
		return false;
	}
	
	// Let Windows reload the display settings.
	//
	DEVMODE devMode;
	BOOL success = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	if (success == TRUE)// && g_LatestDevMode != devMode)
	{
	  LONG result = ::ChangeDisplaySettings(&devMode, CDS_RESET);
	  //check(result == DISP_CHANGE_SUCCESSFUL);
	}

	// restore original value
	if (!bForever && orgLevel != level)
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR) videoKey, 0, KEY_ALL_ACCESS, &hk);
		ret = RegSetValueEx(hk, _T("Acceleration.Level"), 0, REG_DWORD, (CONST BYTE *) &orgLevel, 4) != ERROR_SUCCESS;
		RegCloseKey(hk);
	}

	return true;
}

