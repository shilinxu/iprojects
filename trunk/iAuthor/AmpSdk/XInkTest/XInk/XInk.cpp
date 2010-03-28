////////////////////////////////////////////////////////////////////////////////////////////////
// XInk.cpp : implementation of the XInk Engine
//

#include "stdafx.h"
#include "XInk.h"

///////////////////////////////////////////////////////////////////////////////
// Constant


namespace XInk
{

// global functions
//PAINTSTRUCT  m_ps;


// TODO: 바탕화면을 서브클래싱해서 마우스 이벤트를 가로채기
//
WNDPROC fnOldProc = NULL;
LRESULT CALLBACK NewWinProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    // 여기서 원하는 작업을 합니다.
    switch( nMsg )
    {
	case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		Beep(1000,5);

    }

    // call original WinProc
    return CallWindowProc(fnOldProc, hWnd, nMsg, wParam, lParam);
}

//2개의 좌표 거리를 Pixel로 반환한다.
int GetDist(xPoint a, xPoint b)
{
    return (int) sqrt( (double) (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) );
}

///////////////////////////////////////////////////////////////////////////////
// Implement

xCanvas::xCanvas(HWND hwnd, xRect rc)
: m_hwnd(hwnd)
, m_hdc(0)
, m_bStrokeStarted(false)
, m_bAutoDrawing(true)
, m_bNoiseFilter(true)
, m_rcCanvas(rc)
, m_dtFactory(0)
, m_pGrpBottom(0)
, m_pGrpFront(0)
, m_pBmpFront(0)
, m_pBmpBackup(0)
{
	// disable screen saver (lost screen DC after screen saver closed by user,
	// so makes it disable.)
	SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &m_bEnableScrSaver, 0);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, 0, 0);

	// TODO: It'll be declare those custom messages in Hooks.dll in the future
	//
	WM_XINK_STARTSTROKE	= RegisterWindowMessage(_T("WM_XINK_STARTSTROKE"));
	WM_XINK_ADDPOINT	= RegisterWindowMessage(_T("WM_XINK_ADDPOINT"));
	WM_XINK_STOPSTROKE	= RegisterWindowMessage(_T("WM_XINK_STOPSTROKE"));

	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	// if (hwnd == 0) means desktop window's dc

	// Desktop(0x00010014)
	// Program Manager(0x000100A2) <- SHELLDLL(0x000100AC) <- Folder View(0x000100AE)
	m_hwnd = GetDesktopWindow();

	// test for desktop dc without folder icons
	//RECT rc = {0,0,500,500};
	//FillRect( m_hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	//fnOldProc = (WNDPROC)GetWindowLong(m_hwnd, GWL_WNDPROC);
	//SetLastError(0);
	//if (SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG)NewWinProc) == 0) {
	//	//Beep(2100,9);
	//}

	// init the tablet
	m_Tablet.Initialize(m_hwnd);

	m_dtFactory = new DrawingToolFactory(this);

	///
	m_pBmpFront = new Bitmap(m_rcCanvas.Width(), m_rcCanvas.Height(), PixelFormat32bppARGB);
	m_pGrpFront = new Graphics(m_pBmpFront);
	m_pGrpFront->SetSmoothingMode(SmoothingModeAntiAlias);// SmoothingModeHighQuality, SmoothingModeHighSpeed
	m_pGrpFront->SetCompositingQuality(CompositingQualityHighQuality);

	m_pBmpBackup = m_pBmpFront->Clone(0, 0, m_pBmpFront->GetWidth(), m_pBmpFront->GetHeight(), PixelFormat32bppARGB);

}

xCanvas::~xCanvas()
{
	/* test for capture desktop window
	Gdiplus::Bitmap* pBmpDesktop = new Bitmap(1024,768,m_pGrpBottom);
	CLSID Clsid;
	XInkGetEncoderClsid(L"image/png", &Clsid);
	pBmpDesktop->Save(L"c:\\myImage.png", &Clsid);
	delete pBmpDesktop;
	*/

	ClearStrokes();

	delete m_dtFactory;

	::ReleaseDC(m_hwnd, m_hdc);

	SAFE_DELETE( m_pGrpFront );
	SAFE_DELETE( m_pBmpFront );
	SAFE_DELETE( m_pBmpBackup );

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	// restore screen saver's activity
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, m_bEnableScrSaver, 0, 0);

	//SetWindowLong(m_hwnd, GWL_WNDPROC, (LONG)fnOldProc);
}


//////////////////////////////////////////////////////////////////////////////////
// 

int xCanvas::AddPoint(const xPoint& pt)
{
	if (!m_bStrokeStarted)
		return -1;

	xPoint rpt = pt;
    xPoint rpt_old = m_stroke.points.size() > 0 ? *(m_stroke.points.end()-1) : pt;
    int dist = GetDist(rpt, rpt_old);
    if (m_stroke.points.size() > 0 && dist < 2)     // 천천히 움직일경우 무시
        return -1;

	// process tablet messages ...
	uint32 mcnt = m_Tablet.PeekMsgs();	// get latest message
	rpt.pressure = -1.0f;	// init pressure for mouse
	if (mcnt)
	{
		WCMSG msg;
		m_Tablet.ProcessMsg(mcnt-1, msg);

		//rpt.x = msg.pos.x;
		//rpt.y = msg.pos.y;
		rpt.pressure = (msg.btn_pressure >= 1023) ? 1.0f : float(msg.btn_pressure)/1023.0f;;

#ifdef _DEBUG
			TRACE("##xCanvas::AddPoint: btn_pressure = %d\n", msg.btn_pressure);
			//if (btn_pressure >= 1024)
			//Beep(1000 + msg.btn_pressure,5);
#endif
	}

	//rpt.timeStamp = timeGetTime();

	// add new point
	if (0 == m_stroke.points.size())	// for start point of the curve line
	{
		m_stroke.points.push_back(rpt);
		m_stroke.points.push_back(rpt);
		m_stroke.points.push_back(rpt);
		//m_stroke.points.push_back(rpt);
	}
	else
	{
		m_stroke.points.push_back(rpt);
		if (m_bAutoDrawing) {
			Draw();
		}
	}


	m_stroke.selection = m_stroke.points.size() - 1;
	return m_stroke.selection;
}

void xCanvas::StartStroke()
{
	ResetGraphics();

	m_stroke.clear();
	m_stroke.bStopped = false;
	m_bStrokeStarted = true;
}

void xCanvas::StopStroke()
{
	if (m_bStrokeStarted)
	{
		// auto drawing..
		if (m_bAutoDrawing)
		{
			m_stroke.bStopped = true;
			Draw();
		}

		// add new stroke
		m_stroke.bStopped = false;		// for redrawing
		m_strokes.push_back(m_stroke);

		m_bStrokeStarted = false;
	}
}

// draw stroke on canvas in realtime
void xCanvas::Draw()
{
	xIDrawingTool* itool = m_dtFactory->GetDrawingTool(m_stroke.pen.dtool);
	if (itool)
	{
		int sz = m_stroke.points.size();

		itool->Draw(m_pGrpBottom, m_stroke, sz-4);
		itool->Draw(m_pGrpFront,  m_stroke, sz-4);

		//delete itool;
	}
}

void xCanvas::RedrawAll()
{
	m_pGrpFront->Clear(Gdiplus::Color(0, 0, 0, 0));

	int cnt = m_strokes.size();
	for (int i=0; i<cnt; i++)
	{
		xIDrawingTool* itool = m_dtFactory->GetDrawingTool(m_strokes[i].pen.dtool);
		if (itool)
		{
			//int sz = m_strokes[i].points.size();
			itool->Draw(m_pGrpFront, m_strokes[i], 0, true);
		}
	}

	Refresh();
}

void xCanvas::Refresh()
{
	ResetGraphics();

	::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	// message pumping
	MSG msg;
	while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
		Sleep(0);	// yeild for redraw message
	}
	Sleep(100);	// yeild for redraw message

	
	CompositingMode    oldCompMode = m_pGrpBottom->GetCompositingMode();

	m_pGrpBottom->SetCompositingMode(CompositingModeSourceOver);

	m_pGrpBottom->DrawImage(m_pBmpFront, 0, 0);
	
	m_pGrpBottom->SetCompositingMode(oldCompMode);
}

void xCanvas::EraseAll()
{
	ClearStrokes();
	RedrawAll();
}

void xCanvas::Undo()
{
	int sz = m_strokes.size();
	if (sz > 0)
	{
		m_strokes.pop_back();
		sz = m_strokes.size();
		if (sz > 0)
		{
			//m_strokes[sz-1].selection -= 1;
			m_stroke = m_strokes[sz-1];
		}
		else
		{
			m_stroke.clear();
		}
	}

	RedrawAll();
}

void xCanvas::ClearStrokes()
{
	int sz = m_strokes.size();
	for (int i=0; i<sz; i++) {
		//m_strokes[i].stroke.clear();
	}
	m_strokes.clear();
	m_stroke.clear();

	m_bStrokeStarted = false;
}

void xCanvas::ProcessMsg(UINT msg, int x, int y)
{
	if (msg == WM_XINK_STARTSTROKE)
		StartStroke();
	if (msg == WM_XINK_ADDPOINT)
		AddPoint(x, y);
	if (msg == WM_XINK_STOPSTROKE)
		StopStroke();
}


} //XInk
