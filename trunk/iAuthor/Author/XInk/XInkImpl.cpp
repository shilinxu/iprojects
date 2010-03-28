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
inline void GenCurve(const int start, const XInk::xPoints& ptsSrc, XInk::xPoints& ptsDest, const int nMinSteps=7)
{
	static float  ax, ay, bx, b_y, steps, stepsize, stepsize2, stepsize3;
	static float  x, dx, d2x, d3x, y, dy, d2y, d3y, len;


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
	d2x = (stepsize3*3.0f) * ax + stepsize2 * bx;
	d2y = (stepsize3*3.0f) * ay + stepsize2 * b_y;
	d3x = (stepsize3*3.0f) * ax;
	d3y = (stepsize3*3.0f) * ay;

	// calculate the points for drawing (this piece of) the curve

	for (int j=0; j<steps; j++)
	{
		x   += dx;
		y   += dy;
		dx  += d2x;
		dy  += d2y;
		d2x += d3x;
		d2y += d3y;
		ptsDest.push_back(xPoint(x,y));
	}
}

/*
inline void GenCurve2(const int start, const XInk::xPoints& ptsSrc, XInk::xPoints& ptsDest)
{
	static xPoint pt[4];

	int sz = ptsSrc.size();
	if (sz == 0) return;
	else if (sz <= 4)
	{
		pt[0] = ptsSrc[sz-4];
		pt[1] = ptsSrc[sz-3];
		pt[2] = ptsSrc[sz-2];
		pt[3] = ptsSrc[sz-1];
	}

	if (pt[1] == pt[2]) return;

	//typedef agg::pixfmtQ pixfmt;
	agg::rendering_buffer rbuf((unsigned char *)m_PaintDib.GetBuffer(),m_PaintDib.GetWidth(),m_PaintDib.GetHeight(),m_PaintDib.GetWidth());
	agg::renderer<agg::span_mono8> ren(rbuf);
	agg::rasterizer ras;
	ras.gamma(1.5);
	//ras.filling_rule(agg::fill_non_zero);
	//ren.clear(agg::rgba8(0,0,0,0));

	//
	// Drawing Spline
	//
	{
		static iSplineVertex m_vertices[12];

		iSplineVertex	m_pntCur;
		iSplineVertex	m_pntDrv[3];
		iSplineVertex	m_pntSvDrv[3];
		int		m_num_steps;

		float ax = pt[3].p.x - 3 * pt[2].p.x + 3 * pt[1].p.x - pt[0].p.x;
		float ay = pt[3].p.y - 3 * pt[2].p.y + 3 * pt[1].p.y - pt[0].p.y;
		float bx = 2 * pt[0].p.x - 5 * pt[1].p.x + 4 * pt[2].p.x - pt[3].p.x;
		float by = 2 * pt[0].p.y - 5 * pt[1].p.y + 4 * pt[2].p.y - pt[3].p.y;

		float dx = (float)(pt[0].p.x-pt[1].p.x);
		float dy = (float)(pt[0].p.y-pt[1].p.y);

		float len = sqrtf(dx * dx + dy * dy);
		//m_num_steps = iMIN<int>(10,int(len * 0.5)+1);
		m_num_steps = iMIN<int>(10,int(len * 0.5)+1);
//		if (int(len * 0.5)+1 > 10)
//		{
//			m_num_steps = 7;
//		}
		float step_size = 1.0f / m_num_steps;
		float step_size2 = step_size * step_size;
		float step_size3 = step_size2 * step_size;

		m_pntSvDrv[0].p.x = (step_size3*0.5f)*ax + (step_size2*0.5f)*bx + (step_size*0.5f)*(pt[2].p.x-pt[0].p.x);
		m_pntSvDrv[0].p.y = (step_size3*0.5f)*ay + (step_size2*0.5f)*by + (step_size*0.5f)*(pt[2].p.y-pt[0].p.y);
		m_pntSvDrv[1].p.x = (step_size3*3) * ax + step_size2 * bx;
		m_pntSvDrv[1].p.y = (step_size3*3) * ay + step_size2 * by;
		m_pntSvDrv[2].p.x = (step_size3*3) * ax;
		m_pntSvDrv[2].p.y = (step_size3*3) * ay;
		memcpy(m_pntDrv, m_pntSvDrv, sizeof(m_pntSvDrv));
		//CopyBlock8((uint8 *)m_pntDrv, (const uint8 *)m_pntSvDrv, sizeof(m_pntSvDrv));

		// calculate the weight of the new vertexs according to pressure
		//
		m_pntCur = pt[1];
		m_vertices[0] = m_pntCur;
		for (int xx=0; xx<m_num_steps; ++xx) {
			m_pntCur += m_pntDrv[0];
			m_vertices[xx+1] = m_pntCur;
			m_pntDrv[0] += m_pntDrv[1];
			m_pntDrv[1] += m_pntDrv[2];
			m_vertices[xx+1].w = pt[1].w + (pt[2].w-pt[1].w) * (xx+1) / (float)m_num_steps;
		}

		// 
		static i2DVector drwVtces[26];
		i2DVector nrm;
		i2DVector* pVecBegin = drwVtces;
		i2DVector* pVecEnd = drwVtces + ((m_num_steps+1)*2-1);
		for (int zz=0; zz<m_num_steps; zz++){
			nrm = m_vertices[zz+1] - m_vertices[zz];
			nrm = nrm.GetApproxNormalized();
			iSwap(nrm.x,nrm.y);
			nrm.x = -nrm.x;
			*pVecBegin = m_vertices[zz].p + nrm * m_vertices[zz].w;
			*pVecEnd = m_vertices[zz].p - nrm * m_vertices[zz].w;
			pVecBegin++;
			pVecEnd--;
		}
		*pVecBegin = m_vertices[m_num_steps].p + nrm * m_vertices[m_num_steps].w;
		*pVecEnd = m_vertices[m_num_steps].p - nrm * m_vertices[m_num_steps].w;

		//
		ras.move_to_d(drwVtces[0].x,drwVtces[0].y);
		for (uint32 qq=1; qq<(m_num_steps+1)*2; ++qq) {
			ras.line_to_d(drwVtces[qq].x,drwVtces[qq].y);
		}

		if (m_vertices[0].w > 0.5) {
			//
			float w = m_vertices[0].w-0.5f;
			ras.move_to_d(m_vertices[0].p.x + (SIN_TABLE[252]*w),m_vertices[0].p.y + (COS_TABLE[252]*w));
			for (xx=0; xx<64; ++xx) {
				ras.line_to_d(m_vertices[0].p.x + (SIN_TABLE[xx*4]*w),m_vertices[0].p.y + (COS_TABLE[xx*4]*w));
			}
		}
		//
		if (m_vertices[m_num_steps].w > 0.5) {
			float w = m_vertices[m_num_steps].w-0.5f;
			ras.line_to_d(m_vertices[m_num_steps].p.x + (SIN_TABLE[252]*w),m_vertices[m_num_steps].p.y + (COS_TABLE[252]*w));
			for (xx=0; xx<64; ++xx) {
				ras.line_to_d(m_vertices[m_num_steps].p.x + (SIN_TABLE[xx*4]*w),m_vertices[m_num_steps].p.y + (COS_TABLE[xx*4]*w));
			}
		}
	}


	//
	// Rendering
	//
	ras.render(ren,agg::rgba8(alpha,alpha,alpha));

	update_rect = iRect(ras.min_x(), ras.min_y(), ras.max_x() - ras.min_x()+1, ras.max_y() - ras.min_y()+1 );
	//iClipper::ClipRect(update_rect,m_PaintDib.GetSize());

}
*/

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

	for (int i = start+2; i < sz-1; i++)
	{
		// get spline curve points
		ptsCurve.clear();
		GenCurve(i-2, stroke.points, ptsCurve);

		// draw curve line
		int csz = ptsCurve.size();
		for (int k = 0; k < csz-1; k++)
		{
			gr->DrawLine(&pen, ptsCurve[k].x, ptsCurve[k].y, ptsCurve[k+1].x, ptsCurve[k+1].y);
			//gr->FillEllipse(&brush, ptsCurve[k].x-w/2, ptsCurve[k].y-w/2, w, h);
		}
	}
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
	for (int i = start+2; i < sz-1; i++)
	{
		// get spline curve points
		ptsCurve.clear();
		GenCurve(i-2, stroke.points, ptsCurve, 15);

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
		
		for (int k = 0; k < csz-1; k++)
		{
			xPoint a = ptsCurve[k];

			float w;
			if (a.pressure == -1.0f)
			{
				w = stroke.pen.width;
				pen.SetColor(Gdiplus::Color(255, xc.r, xc.g, xc.b));
			}
			else
			{
				// 붓은 실제크기에 최대 1.5배
				w = stroke.pen.width * a.pressure * 1.5f;
				BYTE alpha = int(255.0f * a.pressure);
				pen.SetColor(Gdiplus::Color(max(alpha, 220), xc.r, xc.g, xc.b));
			}

			pen.SetWidth(w);
			gr->DrawLine(&pen, ptsCurve[k].x, ptsCurve[k].y, ptsCurve[k+1].x, ptsCurve[k+1].y);
			//gr->FillEllipse(&brush, a.x-w/2, a.y-w/2, w, w);
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
