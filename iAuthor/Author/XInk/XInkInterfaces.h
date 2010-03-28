////////////////////////////////////////////////////////////////////////////////////////////////
// XInkInterfaces.h : Interfaces of the XInk Engine
//
//

#ifndef _XINK_INTERF_H_
#define _XINK_INTERF_H_

namespace XInk
{

// User-defined callback for volume change notification
//typedef void (CALLBACK *PONMICVOULUMECHANGE)( DWORD dwCurrentVolume, DWORD dwUserValue );

struct xICanvas
{
	virtual HDC GetDC() = 0;
};


////////////////////////////////////////////////////////////////////////
// IPen interface
struct xIDrawingTool
{
	virtual void Draw(Gdiplus::Graphics* const gr, const xStroke& stroke,
							int start=0, bool bRedraw=false) = 0;
};

} //XInk


#endif // _XINK_INTERF_H_
