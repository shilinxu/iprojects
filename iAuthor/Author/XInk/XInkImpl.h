////////////////////////////////////////////////////////////////////////////////////////////////
// XInk.h : Iinterfaces of the XInk Engine
//

#ifndef _XINK_IMPL_H_
#define _XINK_IMPL_H_

namespace XInk
{

int XInkGetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void XInkLine(Gdiplus::Graphics* gr, const Gdiplus::Pen *pen, xPoint a, xPoint b);
void XInkLineByEllipse(Gdiplus::Graphics* gr, const Gdiplus::Brush *brush, float width, xPoint a, xPoint b);


// XInk Drawing Tool Classes
//

class DTLine : public xIDrawingTool
{
protected:
	xICanvas* m_pCanvas;

public:
	DTLine(xICanvas* pCanvas)
	: m_pCanvas(pCanvas)
	{}
	
	virtual ~DTLine()
	{}

	inline void Clear()	{}

	// return index of control point near to (x,y) or -1 if nothing near */
	int SelectPoint(int x, int y);
	

	// IPen Interface
	//

	// draw stroke into DC in realtime
	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0)
	{
		int sz = stroke.points.size();
		if (sz < 4)
			return;

		Gdiplus::Color color((uint32)stroke.pen.color);
		Gdiplus::Pen pen(color, stroke.pen.width);

		pen.SetLineJoin(Gdiplus::LineJoinRound);
		pen.SetStartCap(Gdiplus::LineCapRound); //LineCapRound);
		pen.SetEndCap(Gdiplus::LineCapRound);

		for (int i = start; i < sz-1; i++)
		{
			xPoint a = stroke.points[i];
			xPoint b = stroke.points[i+1];
			gr->DrawLine(&pen, a.x, a.y, b.x, b.y);
		}
	}
};


// This class represents a curve defined by a sequence of control points
// using bspline method
//
class DTBspline : public DTLine
{
public:

	// draw stroke into DC in realtime
	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);

protected:
	
	// evaluate a point on the B spline
	virtual XInk::xPoint EvalPoint(const xStroke& stroke, int i, float t);

	// the basis function for a cubic B spline
	virtual float basis(int i, float t);
};


// This class represents a curve defined by a sequence of control points
// using catmull rom method
//
class DTCatmullRom : public DTBspline
{
protected:

	// Catmull-Rom spline is just like a B spline, only with a different basis
	float basis(int i, float t);
};


// Implement Drawing Tool Classes
//

class DTPen : public DTLine
{
public:
	DTPen(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);
};


class DTChinesePen : public DTLine
{
public:
	DTChinesePen(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);
};

class DTHighLighter : public DTLine
{
public:
	DTHighLighter(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);
};

class DTHighLighterEllipse : public DTLine
{
public:
	DTHighLighterEllipse(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);
};

class DTEraser : public DTLine
{
public:
	DTEraser(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw=false);

protected:
	inline void RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped=false);

};

class DTEraserRect : public DTEraser
{
public:
	DTEraserRect(xICanvas* pCanvas)
	: DTEraser(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);

protected:
	xRect m_rcEraserOld;
};

class DTMagnifier : public DTEraser
{
public:
	DTMagnifier(xICanvas* pCanvas)
	: DTEraser(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start=0, bool bRedraw=false);

protected:
	xRect m_rcMagnifier;
};

class DTShapeLine : public DTLine
{
public:
	DTShapeLine(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw=false);

protected:
	inline void RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped=false);

};

class DTShapeLineArrow : public DTLine
{
public:
	DTShapeLineArrow(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw=false);

protected:
	inline void RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped=false);
};

class DTShapeRect : public DTLine
{
public:
	DTShapeRect(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw=false);

protected:
	inline void RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped=false);
};

class DTShapeEllipse : public DTLine
{
public:
	DTShapeEllipse(xICanvas* pCanvas)
	: DTLine(pCanvas)
	{}

	void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw=false);

protected:
	inline void RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped=false);
};

}; // XInk

#endif //_XINK_IMPL_H_