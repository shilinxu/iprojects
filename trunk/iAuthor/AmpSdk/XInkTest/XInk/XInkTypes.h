////////////////////////////////////////////////////////////////////////////////////////////////
// XInkTypes.h : types for the XInk Engine
//
//

#ifndef _XINK_TYPES_H_
#define _XINK_TYPES_H_

#include "dtypes.h"

//#include <wingdi.h>
#include <vector>

namespace XInk
{

//#include <pack.h>
#pragma pack(push,8)

enum xDrawingTool
{
	DT_Pen = 0,					// always start 0
	DT_ChinesePen,
	DT_HighLighter,				// pattern is |
	DT_HighLighterEllipse,		// pattern is O
	DT_Eraser,
	DT_EraserRect,
	DT_EraserAll,
	DT_Magnifier,
	DT_ShapeLine,
	DT_ShapeLineArrow,
	DT_ShapeRect,
	DT_ShapeEllipse,
	DT_NumberOfDrawingTools		// number of drawing tools
};


struct xPoint
{
    float x;
    float y;
	float pressure;		// from tablet
	DWORD timeStamp;	// = timeGetTime();

	xPoint() : x(-1.0), y(-1.0), pressure(-1.0)
	{}
	xPoint(int ix, int iy, float p=-1.0) : x((float)ix), y((float)iy), pressure(p)
	{}
	xPoint(float fx, float fy, float p=-1.0) : x(fx), y(fy), pressure(p)
	{}
	xPoint(double dx, double dy, float p=-1.0) : x((float)dx), y((float)dy), pressure(p)
	{}

	inline bool operator== (const xPoint &p) const
	{
		return ((x==p.x) && (y==p.y));
	}

};

struct xRect
{
    float l;
    float t;
    float r;
    float b;
	RECT rect;


	xRect() : l(.0), t(.0), r(.0), b(.0)
	{}
	xRect(int x, int y, int x2, int y2)
	{
		l = x; t = y;
		r = x2; b = y2;
	}
	xRect(float x, float y, float x2, float y2)
	{
		l = x; t = y;
		r = x2; b = y2;
	}
	xRect(const xPoint& p1, const xPoint& p2)
	{
		SetRect(p1, p2);
	}

	float Width()  { return r - l; }
	float Height() { return b - t; }

	operator RECT() //const
	{
		rect.left   = l;
		rect.top    = t;
		rect.right  = r;
		rect.bottom = b;
		return rect;
	}
	
	operator LPRECT() //const
	{
		rect.left   = l;
		rect.top    = t;
		rect.right  = r;
		rect.bottom = b;
		return &rect;
	}
	
	inline bool operator== (const xRect &rect) const
	{
		return ((l==rect.l) && (t==rect.t) && (r==rect.r) && (b==rect.b));
	}

	inline bool IsEmpty()
	{
		return ((r-l)==0 || (b-t)==0);
	}

	inline void Empty()
	{
		l = .0;
		t = .0;
		r = .0;
		b = .0;
	}


	inline void SetRect(const xPoint& p1, const xPoint& p2)
	{
		l = p1.x; t = p1.y;
		r = p2.x; b = p2.y;
	}

	inline void NormalizeRect()
	{
		float min_x = min(l, r);
		float max_x = max(l, r);
		float min_y = min(t, b);
		float max_y = max(t, b);

		l = min_x;
		r = max_x;
		t = min_y;
		b = max_y;
	}

	inline void InflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		l -= left;
		t -= top;
		r += (left+right);
		b += (top+bottom);
	}

	inline void InflateRect(uint32 x_offs, uint32 y_offs)
	{
		InflateRect(x_offs,y_offs,x_offs,y_offs);
	}

	inline void InflateRect(uint32 offs)
	{
		InflateRect(offs,offs,offs,offs);
	}

	inline void DeflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		l += left;
		t += top;
		r -= (left+right);
		b -= (top+bottom);
	}

	inline void DeflateRect(uint32 x_offs, uint32 y_offs)
	{
		DeflateRect(x_offs,y_offs,x_offs,y_offs);
	}

	inline void DeflateRect(uint32 offs)
	{
		DeflateRect(offs,offs,offs,offs);
	}
};

struct xColor
{
	union 
	{
		uint32 argb;
		struct 
		{
			uint8 b;
			uint8 g;
			uint8 r;
			uint8 a;
		};
	};

	xColor() : r(0), g(0), b(0), a(0)
	{}

	xColor(COLORREF cr, uint8 alpha=255)
	{
		r = GetRValue(cr);
		g = GetGValue(cr);
		b = GetBValue(cr);
		a = alpha;
	}

	xColor(uint8 red, uint8 green, uint8 blue, uint8 alpha=255)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	inline operator COLORREF() const
	{
		return RGB(r,g,b);
	}

	inline operator uint32 () const
	{
		return (argb);
	}
};

struct xPen
{
	xDrawingTool	dtool;
 
	xColor	color;
	float	width;

	xPen() : dtool(DT_Pen), color(RGB(0,0,0), 255), width(1.0)
	{}
	xPen(xDrawingTool dt, xColor c, float w) : dtool(dt), color(c), width(w)
	{}
};

typedef std::vector<xPoint> xPoints;

struct xStroke
{
	xPen	pen;
	xPoints	points;
	int		selection;
	int		steps;		// for curve methods
	DWORD	startTime;	// = timeGetTime();
	bool	bStopped;		// whether stop ?

	xStroke() : selection(-1), steps(16), startTime(0), bStopped(false)
	{}

	void clear()
	{
		selection = -1;
		startTime = 0;
		points.clear();
	}

	xStroke& operator=(const xStroke& x)
	{
		if (this != &x)
		{
			pen = x.pen;
			points = x.points;
			selection = x.selection;
			steps = x.steps;
			startTime = x.startTime;
			bStopped = x.bStopped;
		}
		return (*this);
	}
};

//#include <unpack.h>
#pragma pack(pop)


typedef std::vector<xStroke> xStrokes;

} //XInk


#endif // _XINK_TYPES_H_
