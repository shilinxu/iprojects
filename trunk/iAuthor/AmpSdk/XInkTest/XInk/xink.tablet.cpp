#include "stdafx.h"
#include "xink.tablet.h"

#define PACKETDATA (PK_CURSOR | PK_STATUS | PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_TIME ) 
#define PACKETMODE (0)

#include "wintab/pktdef.h"

namespace XInk
{

const uint32 NPACKETQSIZE = 32;
PACKET g_TblPacketBuff[NPACKETQSIZE];

bool xITablet::HasWacomTablet()
{
	HMODULE hm = LoadLibrary(_T("wintab32.dll"));
	if (!hm) return false;
	FreeLibrary(hm);
	return true;
}

bool xITablet::Initialize(HWND hwnd)
{
	// try to load wintab32.dll
	m_hModule = LoadLibrary(_T("wintab32.dll"));
	if (!m_hModule) {
		return false;
	}
	
	prcWTInfo = (PROC_WTInfo)GetProcAddress(m_hModule,"WTInfoW");
	prcWTPacket = (PROC_WTPacket)GetProcAddress(m_hModule,"WTPacket");
	prcWTPacketsGet = (PROC_WTPacketsGet)GetProcAddress(m_hModule,"WTPacketsGet");
	prcWTQueueSizeSet = (PROC_WTQueueSizeSet)GetProcAddress(m_hModule,"WTQueueSizeSet");
	prcWTOpen = (PROC_WTOpen)GetProcAddress(m_hModule,"WTOpenW");
	prcWTClose = (PROC_WTClose)GetProcAddress(m_hModule,"WTClose");
	prcWTEnable = (PROC_WTEnable)GetProcAddress(m_hModule,"WTEnable");

	if (!prcWTInfo || !prcWTPacket || !prcWTPacketsGet || !prcWTQueueSizeSet || !prcWTOpen || !prcWTClose || !prcWTEnable) {
		FreeLibrary(m_hModule);
		return false;
	}

	LOGCONTEXTW lcMine;
	
	if (!prcWTInfo(WTI_DEFSYSCTX,0,&lcMine)) {
		FreeLibrary(m_hModule);
		return false;
	}
	
	wcscpy(lcMine.lcName, L"XInkTablet");
	lcMine.lcPktData = PACKETDATA;
	lcMine.lcPktMode = PACKETMODE;
	lcMine.lcPktRate = 133;//300;
	lcMine.lcMoveMask = PACKETDATA;
	lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;
	lcMine.lcOutOrgX = 0;
	lcMine.lcOutOrgY = 0;
	lcMine.lcOutExtX = GetSystemMetrics(SM_CXSCREEN);
	lcMine.lcOutExtY = -GetSystemMetrics(SM_CYSCREEN); // must be negative

	m_hContext = prcWTOpen(hwnd,&lcMine,TRUE);
	
	if (!m_hContext) {
		FreeLibrary(m_hModule);
		return false;
	}

	prcWTQueueSizeSet(m_hContext,NPACKETQSIZE);

	m_IsInited = true;

	return true;
}

uint32 xITablet::PeekMsgs()
{
	if (!m_IsInited) return 0;
	m_QueuedPackets = prcWTPacketsGet(m_hContext,NPACKETQSIZE,&g_TblPacketBuff);
	return m_QueuedPackets;
}

bool xITablet::ProcessMsg(uint32 msgId, WCMSG& msg)
{
	if (msgId >= m_QueuedPackets) return false;
	msg.crs_id = g_TblPacketBuff[msgId].pkCursor;
	msg.btn_state = g_TblPacketBuff[msgId].pkButtons;
	msg.btn_pressure = g_TblPacketBuff[msgId].pkNormalPressure;
	msg.pos.x = g_TblPacketBuff[msgId].pkX;
	msg.pos.y = g_TblPacketBuff[msgId].pkY;
	msg.time = g_TblPacketBuff[msgId].pkTime;
	return true;
}

void xITablet::BeginDrawing()
{
	if (m_IsInited) prcWTEnable(m_hContext,TRUE);
}

void xITablet::EndDrawing()
{
	if (m_IsInited) prcWTEnable(m_hContext,FALSE);
}

void xITablet::Uninitialize()
{
	if (m_IsInited) {
		prcWTClose(m_hContext);
		if (m_hModule) FreeLibrary(m_hModule);
		m_IsInited = false;
	}
}



} //XInk
