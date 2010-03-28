////////////////////////////////////////////////////////////////////////////////////////////////
// XInk.h : interface of the XInk Engine
//
// http://www.cse.unsw.edu.au/~lambert/splines/source.html
// http://www.experts-exchange.com/Programming/Game_Development/Game_Graphics/Q_20084536.html
// http://courses.ece.uiuc.edu/ece291/archive/archive-f2000/mp/mp4/anti.html
//
// The XInk Engine provide for vector free drawing environment to a multimedia apllication
// developers using microsoft gdi+
//
//
// Scenario:
// 1) create Canvas
// 2) create pen(pen,brush,marker,eraser,...)
// 3) select pen to Canvas
// 4) start drawing (stroke)
// 5) recur to 4)
// 6) delete current pen and create another pen
// 7) recur to 3)
// 8) delete Canvas
// 9) recur to 1)

#ifndef _XINK_H_
#define _XINK_H_

#include <gdiplus.h>
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

#include "dtypes.h"
#include "XInkTypes.h"
#include "XInkInterfaces.h"
#include "XInkImpl.h"
#include "xink.tablet.h"

#include "math.h"

#define SAFE_DELETE(x)	{ if(x) {delete x; x=0;} }

#define cDefSmoothingMode   SmoothingModeAntiAlias				// SmoothingModeAntiAlias SmoothingModeHighQuality SmoothingModeHighSpeed
#define cDefCompositQuality CompositingQualityHighQuality		// CompositingQualityHighSpeed CompositingQualityHighQuality

namespace XInk
{

class DrawingToolFactory
{
private:
	xIDrawingTool* m_tools[DT_NumberOfDrawingTools];

public:
	DrawingToolFactory(xICanvas* pCanvas)
	{
		for (int i=0; i<DT_NumberOfDrawingTools; i++)
		{
			switch (i)
			{
			case DT_Pen:
				m_tools[i] = new DTPen(pCanvas);	//DTLine();
				break;
			case DT_ChinesePen:
				m_tools[i] = new DTChinesePen(pCanvas);
				break;
			case DT_HighLighter:
				m_tools[i] = new DTHighLighter(pCanvas);
				break;
			case DT_HighLighterEllipse:
				m_tools[i] = new DTHighLighterEllipse(pCanvas);
				break;
			case DT_Eraser:
				m_tools[i] = new DTEraser(pCanvas);
				break;
			case DT_EraserRect:
				m_tools[i] = new DTEraserRect(pCanvas);
				break;
			case DT_Magnifier:
				m_tools[i] = new DTMagnifier(pCanvas);
				break;

			case DT_ShapeLine:
				m_tools[i] = new DTShapeLine(pCanvas);
				break;
			case DT_ShapeLineArrow:
				m_tools[i] = new DTShapeLineArrow(pCanvas);
				break;
			case DT_ShapeRect:
				m_tools[i] = new DTShapeRect(pCanvas);
				break;
			case DT_ShapeEllipse:
				m_tools[i] = new DTShapeEllipse(pCanvas);
				break;
			default:
				m_tools[i] = 0;
			}
		}
	}

	~DrawingToolFactory()
	{
		for (int i=0; i<DT_NumberOfDrawingTools; i++)
		{
			if (m_tools[i])
				delete m_tools[i];
		}
	}

	inline xIDrawingTool* GetDrawingTool(xDrawingTool dtool)
	{
		return m_tools[dtool];
	}
};


class xCanvas : public xICanvas
{
protected:
    HWND			m_hwnd;
	HDC				m_hdc;

	xStrokes		m_strokes;
	xStroke			m_stroke;
	bool			m_bStrokeStarted;
	bool			m_bAutoDrawing;
	bool			m_bNoiseFilter;
	
	DrawingToolFactory*		m_dtFactory;

	Gdiplus::Graphics*		m_pGrpBottom;		// a: Bottom layer
	Gdiplus::Graphics*		m_pGrpFront;		// b: Topmost layer (drawing here)
	Gdiplus::Graphics*		m_pGrpMixed;		// a + b: 

	Gdiplus::Bitmap*		m_pBmpFront;
	Gdiplus::Bitmap*		m_pBmpBackup;
	xRect					m_rcCanvas;

	ULONG_PTR						m_gdiplusToken;
	Gdiplus::GdiplusStartupInput	m_gdiplusStartupInput;

	xITablet	m_Tablet;

	BOOL		m_bEnableScrSaver;

	UINT WM_XINK_STARTSTROKE;
	UINT WM_XINK_ADDPOINT;
	UINT WM_XINK_STOPSTROKE;

public:
	xCanvas()
	{
		xCanvas(0, xRect(0,0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)));
	}
	xCanvas(HWND hwnd, xRect rc);

	~xCanvas();

public:

	// xICanvas Interface
	HDC GetDC()
	{
		return m_hdc;
	}


	//
	//

	inline void SetPen(const xPen& pen)
	{
		m_stroke.pen = pen;
	}
	inline const xPen& GetPen()
	{
		return m_stroke.pen;
	}

	void StartStroke();
	void StopStroke();

	// add a control point, return index of new control point
	inline int AddPoint(const int x, const int y)
	{
		return AddPoint(xPoint(x, y));
	}

	int AddPoint(const xPoint& pt);

	void RedrawAll();
	void Refresh();

	void EraseAll();
	void Undo();


	inline bool EnableAutoDrawing(bool bEnable)
	{
		bool bRet = m_bAutoDrawing;
		m_bAutoDrawing = bEnable;
		return bRet;
	}

	inline bool EnableNoiseFilter(bool bEnable)
	{
		bool bRet = m_bNoiseFilter;
		m_bNoiseFilter = bEnable;
		return bRet;
	}

	inline bool IsStrokeStarted() { return m_bStrokeStarted; }

	// return index of control point near to (x,y) or -1 if nothing near */
	int SelectPoint(int x, int y);
	
	void ClearStrokes();
	void ProcessMsg(UINT msg, int x, int y);

	//
	inline void ResetGraphics()
	{
		m_hdc  = ::GetWindowDC(m_hwnd);

		SAFE_DELETE( m_pGrpBottom );
		m_pGrpBottom = new Gdiplus::Graphics(m_hdc);
		m_pGrpFront->SetSmoothingMode(cDefSmoothingMode);
		m_pGrpFront->SetCompositingQuality(cDefCompositQuality);
	}

	// not yet implements
	//
	void Fill() {}
	void Print(const HDC hdc) {}
	void SaveFile(LPCTSTR pszFileName) {}

private:
	// draw stroke on canvas in realtime
	inline void Draw();



};


// global functions
//




#ifdef _DEBUG


static void XInk_utest();


// for unit test code
static void XInk_utest()
{
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	XInk::xCanvas * pCanvas = new XInk::xCanvas(0, XInk::xRect(0,0, cx,cy));

	pCanvas->SetPen(xPen(DT_Pen, xColor(RGB(0,200,0)), 2.0));
	pCanvas->EnableAutoDrawing(true);

	// 1st stroke
//	pCanvas->SetSteps(5);
	pCanvas->StartStroke();

	pCanvas->AddPoint(100, 400);
	pCanvas->AddPoint(200, 100);
	pCanvas->AddPoint(300, 400);
	pCanvas->AddPoint(400, 150);
	pCanvas->AddPoint(500, 450);
	pCanvas->AddPoint(600, 100);

	pCanvas->StopStroke();
	
	// 2nd stroke
//	pCanvas->SetSteps(20);
	pCanvas->SetPen(xPen(DT_Pen, xColor(RGB(0,0,250), 55), 1.0));

	pCanvas->StartStroke();

	pCanvas->AddPoint(100, 500);
	pCanvas->AddPoint(200, 200);
	pCanvas->AddPoint(300, 500);
	pCanvas->AddPoint(400, 250);

	pCanvas->StopStroke();

	delete pCanvas;
}

#else // !_DEBUG

static void XInk_utest() {}

#endif // _DEBUG


} //XInk

#endif // _XINK_H_

