#ifndef _XINK_TABLET_H_
#define _XINK_TABLET_H_


//#pragma comment (lib,"wntab32x")
#include "wintab/wintab.h"

namespace XInk
{


typedef UINT (WINAPI *PROC_WTInfo)(UINT,UINT,LPVOID);
typedef BOOL (WINAPI *PROC_WTPacket)(HCTX, UINT, LPVOID);
typedef int (WINAPI *PROC_WTPacketsGet)(HCTX, int, LPVOID);
typedef BOOL (WINAPI *PROC_WTQueueSizeSet)(HCTX, int);

typedef HCTX (WINAPI *PROC_WTOpen)(HWND, LPLOGCONTEXTW, BOOL);
typedef BOOL (WINAPI *PROC_WTClose)(HCTX);
typedef BOOL (WINAPI *PROC_WTEnable)(HCTX, BOOL);

struct WCMSG {
	WCMSG() {}

	WCMSG(const POINT& _pos, uint32 _crs_id, uint32 _btn_state, uint32 _btn_pressure, uint32 _time)
	: pos(_pos)
	, crs_id(_crs_id)
	, btn_state(_btn_state)
	, btn_pressure(_btn_pressure)
	, time(_time) {}

	POINT	pos;
	uint32	crs_id;
	uint32	btn_state;
	sint32	btn_pressure;
	uint32	time;
};

class xITablet
{
public:
	static bool HasWacomTablet();

	xITablet()
	: m_IsInited(false)
	, prcWTInfo(NULL)
	, m_QueuedPackets(0)
	{}

	~xITablet()
	{
		Uninitialize();
	}

	bool Initialize(HWND hwnd);
	void Uninitialize();

	void BeginDrawing();
	void EndDrawing();

	uint32 PeekMsgs();
	bool ProcessMsg(uint32 msgId, WCMSG& msg);

	inline bool IsInited() const
	{ return m_IsInited; }

	inline HCTX GetContext()
	{ return m_hContext; }

private:
	HMODULE			m_hModule;
	HCTX			m_hContext;

	PROC_WTInfo			prcWTInfo;
	PROC_WTPacket		prcWTPacket;
	PROC_WTPacketsGet	prcWTPacketsGet;
	PROC_WTQueueSizeSet	prcWTQueueSizeSet;
	PROC_WTOpen			prcWTOpen;
	PROC_WTClose		prcWTClose;
	PROC_WTEnable		prcWTEnable;

	bool				m_IsInited;
	uint32				m_QueuedPackets;

private:

};

} //XInk


#endif //_XINK_TABLET_H_