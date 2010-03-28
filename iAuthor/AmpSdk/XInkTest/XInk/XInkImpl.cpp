////////////////////////////////////////////////////////////////////////////////////////////////
// XInkImpl.cpp : implementation of the XInk Interfaces
//

#include "stdafx.h"
#include "XInkImpl.h"
#include <math.h>

namespace XInk
{

///////////////////////////////////////////////////////////////////////////////
// Constant

#define EPSILON		36  // square of distance for picking
#define POW(x)		(x*x)
#define XROUND(x)	int(x+.5)


///////////////////////////////////////////////////////////////////////////////
// Implement

int XInkGetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

/*
// TODO: 마우스 입력의 샘플링 문제를 Direct Input으로 해결해 보자!!
//

#define PI	(3.141592)
inline void Circle(const int x, const int y, const int r, int color)
{
	//float dx, dy;
    //r = sqrt(dx*dx + dy*dy);

    for (int degree=0 ; degree<=360 ; degree++)
    {
       float radian=degree*PI/180; // convert degree to radian

       xx = x+r*cos(radian);
       yy = y+r*sin(radian);

       put_pixel(xx, yy, color);
    }
}
inline void Ellipse(const int x, const int y, const int x2, const int y2, int color)
{}
*/

//
// A Recursive Evaluation Algorithm for a Class of Catmull-Rom Splines
//
void GenCurve(const int start, const XInk::xPoints& ptsSrc, XInk::xPoints& ptsDest, const int nMinSteps=7)
{
	float  ax, ay, bx, b_y, steps, stepsize, stepsize2, stepsize3;
	float  x, dx, dx2, dx3, y, dy, dy2, dy3, len;


	int i = start+3;
	//
	// build the coefficients ax, ay, bx and b_y, using:
	//                             _              _   _    _
	//   i                 i    1 | -1   3  -3   1 | | Pi-3 |
	//  Q (t) = T * M   * G   = - |  2  -5   4  -1 | | Pi-2 |
	//               CR    Bs   2 | -1   0   1   0 | | Pi-1 |
	//                            |_ 0   2   0   0_| |_Pi  _|
	//

	ax = ptsSrc[i].x - 3 * ptsSrc[i-1].x + 3 * ptsSrc[i-2].x - ptsSrc[i-3].x;
	ay = ptsSrc[i].y - 3 * ptsSrc[i-1].y + 3 * ptsSrc[i-2].y - ptsSrc[i-3].y;
	bx = 2 * ptsSrc[i-3].x - 5 * ptsSrc[i-2].x + 4 * ptsSrc[i-1].x - ptsSrc[i].x;
	b_y = 2 * ptsSrc[i-3].y - 5 * ptsSrc[i-2].y + 4 * ptsSrc[i-1].y - ptsSrc[i].y;

	//
	// calculate the forward differences for the function using
	// intervals of size 0.1
	//
	dx = (float)(ptsSrc[i-1].x-ptsSrc[i-2].x);
	dy = (float)(ptsSrc[i-1].y-ptsSrc[i-2].y);
	len = sqrtf(dx * dx + dy * dy);
	//steps = (len > 0.5) ? len-0.5f : len;	//min(int(len + 0.5), len/ptsSrc[i-1].);
	steps = min(len, (float)nMinSteps);

	stepsize = 1.0f / steps;
	stepsize2 = stepsize * stepsize;
	stepsize3 = stepsize * stepsize2;

	x = ptsSrc[i-2].x;
	y = ptsSrc[i-2].y;
	ptsDest.push_back(xPoint(x,y));

	dx = (stepsize3*0.5f)*ax + (stepsize2*0.5f)*bx + (stepsize*0.5f)*(ptsSrc[i-1].x-ptsSrc[i-3].x);
	dy = (stepsize3*0.5f)*ay + (stepsize2*0.5)*b_y + (stepsize*0.5f)*(ptsSrc[i-1].y-ptsSrc[i-3].y);
	dx2 = (stepsize3*3.0f) * ax + stepsize2 * bx;
	dy2 = (stepsize3*3.0f) * ay + stepsize2 * b_y;
	dx3 = (stepsize3*3.0f) * ax;
	dy3 = (stepsize3*3.0f) * ay;

	// calculate the points for drawing (this piece of) the curve

	for (int j=0; j<steps; j++)
	{
		x   += dx;
		y   += dy;
		dx  += dx2;
		dy  += dy2;
		dx2 += dx3;
		dy2 += dy3;
		ptsDest.push_back(xPoint(x,y));
	}
}

//
// 영목이 소스
//
void GenCurve2(const int start, const XInk::xPoints& ptsSrc, XInk::xPoints& ptsDest, const int nMinSteps=7)
{
    int siz = ptsSrc.size();

	float dx = (float)(ptsSrc[siz-1].x-ptsSrc[siz-2].x);
	float dy = (float)(ptsSrc[siz-1].y-ptsSrc[siz-2].y);
	float dist = sqrtf(dx * dx + dy * dy);
	float steps = min(dist, (float)nMinSteps);
	float dt = 1.0f / steps;
    float x, y;

    // calculate the points for drawing (this piece of) the curve
    float t = dt;
	ptsDest.push_back(xPoint(x,y));
	for (int j=0; j<steps; j++)
	{
        x = (((-ptsSrc[0].x+3*ptsSrc[1].x-3*ptsSrc[2].x+ptsSrc[3].x)*t*t*t+(3*ptsSrc[0].x-6*ptsSrc[1].x+3*ptsSrc[2].x)*t*t+(-3*ptsSrc[0].x+3*ptsSrc[2].x)*t+ptsSrc[0].x+4*ptsSrc[1].x+ptsSrc[2].x)/6); //+0.2; //000000000000008);
        y = (((-ptsSrc[0].y+3*ptsSrc[1].y-3*ptsSrc[2].y+ptsSrc[3].y)*t*t*t+(3*ptsSrc[0].y-6*ptsSrc[1].y+3*ptsSrc[2].y)*t*t+(-3*ptsSrc[0].y+3*ptsSrc[2].y)*t+ptsSrc[0].y+4*ptsSrc[1].y+ptsSrc[2].y)/6);  //+0.2; //000000000000008);
		ptsDest.push_back(xPoint(x,y));

        t += dt;
	}
}

void XInkLine(Gdiplus::Graphics* gr, const Gdiplus::Pen *pen, XInk::xPoint a, XInk::xPoint b)
{
    int step, k;
    float incx, incy, x, y, dx, dy;

    dx = b.x-a.x;
    dy = b.y-a.y;
    if(fabs(dx) > fabs(dy))
        step = abs(dx);
    else
        step = abs(dy);

	if (step > 50) step /= 5;

    incx = dx/(float)step;
    incy = dy/(float)step;
    
    x = a.x;
    y = a.y;

	// DDA Line algorithm
	for (k=0; k<step; k++)
	{
		//gr->DrawLine(pen, x, y, width, width);

		x += incx;
		y += incy;
/*
		a = b;
		b.x = x;
		b.y = y;
*/
	}
}

void XInkLineByEllipse(Gdiplus::Graphics* gr, const Gdiplus::Brush *brush, float width, XInk::xPoint a, XInk::xPoint b)
{
    int step, k;
    float incx, incy, x, y, dx, dy;

    dx = b.x-a.x;
    dy = b.y-a.y;
    if(fabs(dx) > fabs(dy))
        step = abs(dx);
    else
        step = abs(dy);

	//if (step > 50) step /= 5;

    incx = dx/(float)step;
    incy = dy/(float)step;
    
    x = a.x;
    y = a.y;

	// DDA Line algorithm
	for (k=0; k<step; k++)
	{
		gr->FillEllipse(brush, x-width/2, y-width/2, width, width);

		x += incx;
		y += incy;
/*
		a = b;
		b.x = x;
		b.y = y;
*/
	}
}


//////////////////////////////////////////////////////////////////////////////////
// 

//////////////////////////////////////////////////////////////////////////////////
// Class DTBspline

void DTBspline::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	XInk::xPoints ptsCurve;

	int sz = stroke.points.size();
	if (sz < 4)
		return;

	for (int i = start+2; i < sz-1; i++)
	{
		for (int j = 0; j <= stroke.steps; j++)
		{
			XInk::xPoint xp = EvalPoint(stroke, i, j/(float)stroke.steps);
			ptsCurve.push_back(xp);
		}
	}
	
	// draw curve line by DDA and my curve algorithm
	Gdiplus::Color color((uint32)stroke.pen.color);
	Gdiplus::Pen pen(color, stroke.pen.width);

	pen.SetLineJoin(LineJoinRound);
	pen.SetStartCap(LineCapRound); //LineCapRound);
	pen.SetEndCap(LineCapRound);

	sz = ptsCurve.size();
	for (int k = 0; k < sz-1; k++)
	{
		xPoint a = ptsCurve[k];
		xPoint b = ptsCurve[k+1];
		gr->DrawLine(&pen, a.x, a.y, b.x, b.y);
	}

	/*
	// draw curve line by Gdiplus's method and my curve algorithm
	// peformance nice better then DDA method
	Color color(m_Alpha, GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color));
	Pen pen(color, m_Width);
	pen.SetLineJoin(LineJoinRound);
	pen.SetStartCap(LineCapRound); //LineCapRound);
	pen.SetEndCap(LineCapTriangle);

	int sz = ptsCurve.size();
	Point *points = new Point[sz];
	for (int k = 0; k < sz-1; k++)
	{
		points[k].X = ptsCurve.at(k).x;
		points[k].Y = ptsCurve.at(k).y;
	}
	gr->DrawCurve(&pen, points, sz, 0, sz-1, 0.5f);
	*/
}

// evaluate a point on the B spline
XInk::xPoint DTBspline::EvalPoint(const xStroke& stroke, int i, float t)
{
	float px=0;
	float py=0;
	float pp=-1.0;

	for (int j = -2; j<=1; j++)
	{
		px += basis(j, t) * stroke.points[i+j].x;
		py += basis(j, t) * stroke.points[i+j].y;
	}

	return XInk::xPoint(XROUND(px), XROUND(py), stroke.points[i].pressure);
}

// the basis function for a cubic B spline
float DTBspline::basis(int i, float t)
{
	switch (i)
	{
	case -2:
		return (((-t+3)*t-3)*t+1)/6;
	case -1:
		return (((3*t-6)*t)*t+4)/6;
	case 0:
		return (((-3*t+3)*t+3)*t+1)/6;
	case 1:
		return (t*t*t)/6;
	}

	return 0; //we only get here if an invalid i is specified
}


//////////////////////////////////////////////////////////////////////////////////
// Class XInkCatmullRom

// Catmull-Rom spline is just like a B spline, only with a different basis
float DTCatmullRom::basis(int i, float t)
{
	switch (i)
	{
	case -2:
		return ((-t+2)*t-1)*t/2;		//2.05;
	case -1:
		return (((3*t-5)*t)*t+2)/2;
	case 0:
		return ((-3*t+4)*t+1)*t/2;
	case 1:
		return ((t-1)*t*t)/2;
	}

	return 0; // we only get here if an invalid i is specified
}

void DTPen::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	XInk::xPoints ptsCurve;

	int sz = stroke.points.size();
	if (stroke.bStopped || sz < 4)
		return;

	float w = stroke.pen.width;// * .6f;
	XInk::xColor xc = stroke.pen.color;

	//SolidBrush brush(Gdiplus::Color(255, xc.r, xc.g, xc.b));

	Gdiplus::Pen pen(Gdiplus::Color(255, xc.r, xc.g, xc.b), w);
	pen.SetLineJoin(Gdiplus::LineJoinRound);
	pen.SetStartCap(Gdiplus::LineCapRound); //LineCapRound);
	pen.SetEndCap(Gdiplus::LineCapRound);

    //
	ptsCurve.clear();
	GenCurve(start+2-2, stroke.points, ptsCurve, 8);
	//GenCurve2(start, stroke.points, ptsCurve, 8);
	// draw curve line
	int csz = ptsCurve.size();
	for (int k = 0; k < csz-1; k++)
		gr->DrawLine(&pen, ptsCurve[k].x, ptsCurve[k].y, ptsCurve[k+1].x, ptsCurve[k+1].y);
		//gr->FillEllipse(&brush, ptsCurve[k].x-w/2, ptsCurve[k].y-w/2, w, w);
}


//void DTChinesePen::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
//{
//	int i, j, k, m;
//	XInk::xPoints ptsCurve;
//
//	int sz = stroke.points.size();
//	if (stroke.bStopped || sz < 4)
//		return;
//
//	// get spline curve points
//	for (i = start+2; i < sz-1; i++)
//	{
//		ptsCurve.clear();
//		for (j = 0; j <= stroke.steps; j++)
//		{
//			XInk::xPoint xp = EvalPoint(stroke, i, j/(float)stroke.steps);
//			ptsCurve.push_back(xp);
//		}
//
//		// get diffrence of pressure between start and stop point
//		int   csz = ptsCurve.size();
//		float spr = stroke.points[i].pressure;
//		float tpr = stroke.points[i+1].pressure - spr;
//		float dpr = tpr / (float)csz;
//		for (k = 0; k < csz; k++)
//		{
//			ptsCurve[k].pressure = spr + dpr*k;
//		}
//
//		// draw curve line
//		XInk::xColor xc = stroke.pen.color;
//		SolidBrush brush(Gdiplus::Color(90, xc.r, xc.g, xc.b));
//		for (k = 0; k < csz-1; k++)
//		{
//			xPoint a = ptsCurve[k];
//			xPoint b = ptsCurve[k+1];
//
//			float w;
//			int bsAlpha;
//			if (a.pressure == -1.0f)
//			{
//				w = stroke.pen.width;
//				bsAlpha = 255;
//			}
//			else
//			{
//				// 붓은 실제크기에 최대 2.5배
//				w = stroke.pen.width * a.pressure * 2.5f;
//				//bsAlpha = int(255.0f * a.pressure + 100) % 255;
//			}
//
//
//
//			//XInkLineByEllipse(gr, &brush, stroke.pen.width, a, b);
//			{
//				int step;
//				float incx, incy, x, y, dx, dy;
//
//				dx = b.x-a.x;
//				dy = b.y-a.y;
//				if(fabs(dx) > fabs(dy))
//					step = abs(dx);
//				else
//					step = abs(dy);
//
//				incx = dx/(float)step;
//				incy = dy/(float)step;
//    
//				x = a.x;
//				y = a.y;
//
//				// DDA Line algorithm
//				for (m=0; m<step; m++)
//				{
//					gr->FillEllipse(&brush, x-w/2, y-w/2, w, w);
//
//					x += incx;
//					y += incy;
//				}
//			}
//		}
//	}
//}

void DTChinesePen::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	XInk::xPoints ptsCurve;

	int sz = stroke.points.size();
	if (stroke.bStopped || sz < 4)
		return;

	// calculate pen width of each point in curve
	// according to pressure of the poiting device
	//
    const int steps = 15;
	for (int i = start+2; i < sz-1; i++)
	{
		// get spline curve points
		ptsCurve.clear();
		GenCurve(i-2, stroke.points, ptsCurve, steps);

		// get difference of pressure between start and stop point
		int   csz = ptsCurve.size();
		float spr = stroke.points[i].pressure;
		if (spr != -1.0f)	// when using tablet
		{
			float tpr = stroke.points[i+1].pressure - spr;
			float dpr = tpr / (float)csz;
			for (int k = 0; k < csz; k++)
			{
				ptsCurve[k].pressure = spr + dpr*k;
			}
		}


		// draw curve line
		//
		XInk::xColor xc = stroke.pen.color;
		Gdiplus::Pen pen(Gdiplus::Color(255, xc.r, xc.g, xc.b), 1.0f);

		pen.SetLineJoin(Gdiplus::LineJoinRound);
		pen.SetStartCap(Gdiplus::LineCapRound); //LineCapRound);
		pen.SetEndCap(Gdiplus::LineCapRound);
		
		float w;
        BYTE alpha;
		for (int k = 0; k < csz-1; k++)
		{
			xPoint a = ptsCurve[k];

			if (a.pressure == -1.0f)
			{
				w = stroke.pen.width;
				alpha = 255;
				pen.SetColor(Gdiplus::Color(255, xc.r, xc.g, xc.b));
			}
			else
			{
				// 붓은 실제크기에 최대 1.5배
				w = stroke.pen.width * a.pressure * 1.5f;
				alpha = int(255.0f * a.pressure);
				pen.SetColor(Gdiplus::Color(max(alpha, 220), xc.r, xc.g, xc.b));
			}

			pen.SetWidth(w);
       //     if (steps % 3 == 0)
       //     {
       //         SolidBrush brush(Gdiplus::Color(max(alpha, 220), xc.r, xc.g, xc.b));
			    //gr->FillEllipse(&brush, a.x-w/2, a.y-w/2, w, w);
       //     }
       //     else
    			gr->DrawLine(&pen, ptsCurve[k].x, ptsCurve[k].y, ptsCurve[k+1].x, ptsCurve[k+1].y);
        }
	}
}

void DTHighLighter::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	XInk::xPoints ptsCurve;

	int sz = stroke.points.size();
	if (stroke.bStopped || sz < 4)
		return;

	// draw curve line
	float w = stroke.pen.width*2;
	XInk::xColor xc = stroke.pen.color;
	Gdiplus::Pen pen(Gdiplus::Color(160, xc.r, xc.g, xc.b), 1.0f);

	for (int i = start+2; i < sz-1; i++)
	{
/*
		// get spline curve points
		ptsCurve.clear();
		GenCurve(i-2, stroke.points, ptsCurve, 100);

		// draw curve line
		int csz = ptsCurve.size();
		for (int k = 0; k < csz-1; k++)
		{
			xPoint a = ptsCurve[k];
			gr->DrawLine(&pen, a.x, a.y-w/2, a.x, a.y+w/2);
			//gr->FillRectangle(&brush, a.x-2, a.y-w/2, 2.0, w);
		}
*/

		// draw curve line
		xPoint a = stroke.points[i];
		xPoint b = stroke.points[i+1];
		do {
			int step;
			float incx, incy, x, y, dx, dy;

			dx = b.x-a.x;
			dy = b.y-a.y;
			step = abs(sqrt(dx*dx + dy*dy));
/*
			if(fabs(dx) > fabs(dy))
				step = abs(dx);
			else
				step = abs(dy);
*/

			//step = step % int(stroke.pen.width/2);
			incx = dx/(float)step;
			incy = dy/(float)step;

			x = a.x;
			y = a.y;

			// DDA Line algorithm
			for (int m=0; m<step; m++)
			{
				gr->DrawLine(&pen, x, y-w/2, x, y+w/2);

				x += incx;
				y += incy;
			}
		} while (0);
	}
}

void DTHighLighterEllipse::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	XInk::xPoints ptsCurve;

	int sz = stroke.points.size();
	if (stroke.bStopped || sz < 4)
		return;

	// draw curve line
	XInk::xColor xc = stroke.pen.color;
	xc.a = 7;
	Gdiplus::Color color((uint32)xc);
	SolidBrush brush(color);

	float w = stroke.pen.width*2;

	for (int i = start+2; i < sz-1; i++)
	{
		// get spline curve points
		ptsCurve.clear();
		GenCurve(i-2, stroke.points, ptsCurve, 30);

		// draw curve line
		int   csz = ptsCurve.size();
		for (int k = 0; k < csz-1; k++)
		{
			xPoint a = ptsCurve[k];
			gr->FillEllipse(&brush, a.x-w/2, a.y-w/2, w, w);
		}
	}
}

// draw rubber-band
//
void DTEraser::RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped)
{
	static xRect rcEraserOld = xRect(0,0,0,0);
	int oldMixMode = SetROP2(m_pCanvas->GetDC(), R2_NOTXORPEN); 

	// Create a green pen.
	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_INSIDEFRAME, nPenWidth, cr);

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN) SelectObject(m_pCanvas->GetDC(), hpen);

	if (!rcEraserOld.IsEmpty()) {
		Rectangle(m_pCanvas->GetDC(), rcEraserOld.l, rcEraserOld.t, rcEraserOld.r, rcEraserOld.b); 
	}
	if (!bStopped) {
		Rectangle(m_pCanvas->GetDC(), rcBand.l, rcBand.t, rcBand.r, rcBand.b); 
		rcEraserOld = rcBand;
	}
	else {
		rcEraserOld.Empty();
	}

	// clean up gdi resouces
	SelectObject(m_pCanvas->GetDC(), hpenOld);
	DeleteObject(hpen);
	SetROP2(m_pCanvas->GetDC(), oldMixMode); 
}

void DTEraser::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	int w = stroke.pen.width * 2+1;

	// erase stroke
	Bitmap *pEraserBmp = new Bitmap(w*2, w*2, PixelFormat32bppARGB);

	CompositingMode    oldCompMode = gr->GetCompositingMode();
	gr->SetCompositingMode(CompositingModeSourceCopy);

	for (int i = start+2; i < sz-1; i++)
	{
		xPoint a = stroke.points[i];
		xPoint b = stroke.points[i+1];
		do {
			int step;
			float incx, incy, x, y, dx, dy;

			dx = b.x-a.x;
			dy = b.y-a.y;
			if(fabs(dx) > fabs(dy))
				step = abs(dx)/3;
			else
				step = abs(dy)/3;

			//step = step % int(stroke.pen.width/2);
			incx = dx/(float)step;
			incy = dy/(float)step;

			x = a.x;
			y = a.y;

			// DDA Line algorithm
			for (int m=0; m<step; m++)
			{
				xRect rcEraser(x-w, y-w, x+w, y+w);
				if (!rcEraser.IsEmpty())
				{ 
					int nBandWidth = 2;
					//if (!bRedraw)	// 잔상 문제로 제거 (040530)
					//	RubberBand(rcEraser, nBandWidth, RGB(0,255,0), stroke.bStopped);
					
					//rcEraser.DeflateRect(nBandWidth, nBandWidth);

					rcEraser.InflateRect(1);
					gr->DrawImage(pEraserBmp, rcEraser.l, rcEraser.t);
					::RedrawWindow(NULL, rcEraser, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
					
				}

				x += incx;
				y += incy;
			}
		} while (0);
	}

	gr->SetCompositingMode(oldCompMode);
	delete pEraserBmp;
}


void DTEraserRect::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	xRect rcEraser;
	rcEraser.SetRect(stroke.points[0], stroke.points[sz-1]);
	rcEraser.NormalizeRect();
	if (rcEraser.IsEmpty()) 
	{ 
		return;
	} 

	int nBandWidth = 2;
	if (!bRedraw)
		RubberBand(rcEraser, nBandWidth, RGB(0,255,0), stroke.bStopped);

	if (bRedraw || stroke.bStopped)
	{
		// erase stroke
		//EraseStroke(gr, rcEraser);
		Bitmap *pEraserBmp = new Bitmap(rcEraser.Width(), rcEraser.Height(), PixelFormat32bppARGB);

		CompositingMode    oldCompMode = gr->GetCompositingMode();
		gr->SetCompositingMode(CompositingModeSourceCopy);

		gr->DrawImage(pEraserBmp, rcEraser.l, rcEraser.t);
		::RedrawWindow(NULL, rcEraser, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
		
		gr->SetCompositingMode(oldCompMode);
		delete pEraserBmp;
	}
}

void DTMagnifier::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	xRect rcMagnifier;
	rcMagnifier.SetRect(stroke.points[0], stroke.points[sz-1]);
	rcMagnifier.NormalizeRect();
	if (rcMagnifier.IsEmpty()) 
	{ 
		return;
	} 

	int nPenWidth = 2;
	RubberBand(rcMagnifier, nPenWidth, RGB(255,0,255), stroke.bStopped);

	if (stroke.bStopped) {
		m_rcMagnifier = rcMagnifier;
	}
}

// draw rubber-band
//
void DTShapeLine::RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped)
{
	static xRect rcEraserOld = xRect(0,0,0,0);
	int oldMixMode = SetROP2(m_pCanvas->GetDC(), R2_NOTXORPEN); 

	// Create a green pen.
	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_INSIDEFRAME, nPenWidth, cr);

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN) SelectObject(m_pCanvas->GetDC(), hpen);

	if (!rcEraserOld.IsEmpty()) {
		MoveToEx(m_pCanvas->GetDC(), rcEraserOld.l, rcEraserOld.t, (LPPOINT) NULL); 
        LineTo(m_pCanvas->GetDC(), rcEraserOld.r, rcEraserOld.b); 
	}
	if (!bStopped) {
		MoveToEx(m_pCanvas->GetDC(), rcBand.l, rcBand.t, (LPPOINT) NULL); 
        LineTo(m_pCanvas->GetDC(), rcBand.r, rcBand.b); 
		rcEraserOld = rcBand;
	}
	else {
		rcEraserOld.Empty();
	}

	// clean up gdi resouces
	SelectObject(m_pCanvas->GetDC(), hpenOld);
	DeleteObject(hpen);
	SetROP2(m_pCanvas->GetDC(), oldMixMode); 
}

void DTShapeLine::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	int w = stroke.pen.width;
	xPoint a = stroke.points[0];
	xPoint b = stroke.points[sz-1];
	xRect rcEraser(a, b);
	//rcEraser.NormalizeRect();
	if (a == b) {
		return;
	} 

	int nPenWidth = w;
	if (!bRedraw)
		RubberBand(rcEraser, nPenWidth, stroke.pen.color, stroke.bStopped);
	
	if (bRedraw || stroke.bStopped)
	{
		Gdiplus::Color color((uint32)stroke.pen.color);
		Gdiplus::Pen pen(color, stroke.pen.width);

		pen.SetLineJoin(Gdiplus::LineJoinRound);
		pen.SetStartCap(Gdiplus::LineCapRound); //LineCapRound);
		pen.SetEndCap(Gdiplus::LineCapRound);

		gr->DrawLine(&pen, a.x, a.y, b.x, b.y);
	}
}

// draw rubber-band
//
void DTShapeLineArrow::RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped)
{
	static xRect rcEraserOld = xRect(0,0,0,0);
	int oldMixMode = SetROP2(m_pCanvas->GetDC(), R2_NOTXORPEN); 

	// Create a green pen.
	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_INSIDEFRAME, nPenWidth, cr);

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN) SelectObject(m_pCanvas->GetDC(), hpen);

	if (!rcEraserOld.IsEmpty()) {
		MoveToEx(m_pCanvas->GetDC(), rcEraserOld.l, rcEraserOld.t, (LPPOINT) NULL); 
        LineTo(m_pCanvas->GetDC(), rcEraserOld.r, rcEraserOld.b); 
	}
	if (!bStopped) {
		MoveToEx(m_pCanvas->GetDC(), rcBand.l, rcBand.t, (LPPOINT) NULL); 
        LineTo(m_pCanvas->GetDC(), rcBand.r, rcBand.b); 
		rcEraserOld = rcBand;
	}
	else {
		rcEraserOld.Empty();
	}

	// clean up gdi resouces
	SelectObject(m_pCanvas->GetDC(), hpenOld);
	DeleteObject(hpen);
	SetROP2(m_pCanvas->GetDC(), oldMixMode); 
}

void DTShapeLineArrow::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	int w = stroke.pen.width;
	xPoint a = stroke.points[0];
	xPoint b = stroke.points[sz-1];
	xRect rcEraser(a, b);
	//rcEraser.NormalizeRect();
	if (a == b) {
		return;
	} 

	int nPenWidth = w;
	if (!bRedraw)
		RubberBand(rcEraser, nPenWidth, stroke.pen.color, stroke.bStopped);
	
	if (bRedraw || stroke.bStopped)
	{
		Gdiplus::Color color((uint32)stroke.pen.color);
		Gdiplus::Pen pen(color, stroke.pen.width);

		pen.SetLineJoin(Gdiplus::LineJoinRound);
		pen.SetStartCap(Gdiplus::LineCapRound);
		pen.SetEndCap(Gdiplus::LineCapArrowAnchor);

		gr->DrawLine(&pen, a.x, a.y, b.x, b.y);
	}
}

// draw rubber-band
//
void DTShapeRect::RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped)
{
	static xRect rcEraserOld = xRect(0,0,0,0);
	int oldMixMode = SetROP2(m_pCanvas->GetDC(), R2_NOTXORPEN); 

	// Create a green pen.
	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_SOLID/*PS_INSIDEFRAME*/, nPenWidth, cr);

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN) SelectObject(m_pCanvas->GetDC(), hpen);

	if (!rcEraserOld.IsEmpty()) {
		Rectangle(m_pCanvas->GetDC(), rcEraserOld.l, rcEraserOld.t, rcEraserOld.r, rcEraserOld.b); 
	}
	if (!bStopped) {
		Rectangle(m_pCanvas->GetDC(), rcBand.l, rcBand.t, rcBand.r, rcBand.b); 
		rcEraserOld = rcBand;
	}
	else {
		rcEraserOld.Empty();
	}

	// clean up gdi resouces
	SelectObject(m_pCanvas->GetDC(), hpenOld);
	DeleteObject(hpen);
	SetROP2(m_pCanvas->GetDC(), oldMixMode); 
}

void DTShapeRect::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	int w = stroke.pen.width;
	xPoint a = stroke.points[0];
	xPoint b = stroke.points[sz-1];
	xRect rcEraser(a, b);
	rcEraser.NormalizeRect();
	if (a == b) {
		return;
	} 

	int nPenWidth = w;
	if (!bRedraw)
		RubberBand(rcEraser, nPenWidth, stroke.pen.color, stroke.bStopped);
	
	if (bRedraw || stroke.bStopped)
	{
		Gdiplus::Color color((uint32)stroke.pen.color);
		Gdiplus::Pen pen(color, stroke.pen.width);

		//pen.SetLineJoin(Gdiplus::LineJoinRound);
		gr->DrawRectangle(&pen, rcEraser.l, rcEraser.t, rcEraser.Width(), rcEraser.Height());
	}
}

// draw rubber-band
//
void DTShapeEllipse::RubberBand(xRect& rcBand, int nPenWidth, COLORREF cr, bool bStopped)
{
	static xRect rcEraserOld = xRect(0,0,0,0);
	int oldMixMode = SetROP2(m_pCanvas->GetDC(), R2_NOTXORPEN); 

	// Create a green pen.
	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_SOLID/*PS_INSIDEFRAME*/, nPenWidth, cr);

	// Select the new pen and brush, and then draw.
	hpenOld = (HPEN) SelectObject(m_pCanvas->GetDC(), hpen);

	if (!rcEraserOld.IsEmpty()) {
		Ellipse(m_pCanvas->GetDC(), rcEraserOld.l, rcEraserOld.t, rcEraserOld.r, rcEraserOld.b); 
	}
	if (!bStopped) {
		Ellipse(m_pCanvas->GetDC(), rcBand.l, rcBand.t, rcBand.r, rcBand.b); 
		rcEraserOld = rcBand;
	}
	else {
		rcEraserOld.Empty();
	}

	// clean up gdi resouces
	SelectObject(m_pCanvas->GetDC(), hpenOld);
	DeleteObject(hpen);
	SetROP2(m_pCanvas->GetDC(), oldMixMode); 
}

void DTShapeEllipse::Draw(Gdiplus::Graphics* const gr, const xStroke& stroke, int start, bool bRedraw)
{
	int sz = stroke.points.size();
	if (sz < 2)
		return;

	int w = stroke.pen.width;
	xPoint a = stroke.points[0];
	xPoint b = stroke.points[sz-1];
	xRect rcEraser(a, b);
	rcEraser.NormalizeRect();
	if (a == b) {
		return;
	} 

	int nPenWidth = w;
	if (!bRedraw)
		RubberBand(rcEraser, nPenWidth, stroke.pen.color, stroke.bStopped);
	
	if (bRedraw || stroke.bStopped)
	{
		Gdiplus::Color color((uint32)stroke.pen.color);
		Gdiplus::Pen pen(color, stroke.pen.width);

		//pen.SetLineJoin(Gdiplus::LineJoinRound);
		gr->DrawEllipse(&pen, rcEraser.l, rcEraser.t, rcEraser.Width(), rcEraser.Height());
	}
}

} //XInk
