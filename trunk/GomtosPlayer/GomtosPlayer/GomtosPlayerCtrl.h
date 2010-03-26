#pragma once

// GomtosPlayerCtrl.h : CGomtosPlayerCtrl ActiveX 컨트롤 클래스의 선언입니다.


// CGomtosPlayerCtrl : 구현을 보려면 GomtosPlayerCtrl.cpp을(를) 참조하십시오.
#include "HMPlayer.h"

class CGomtosPlayerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CGomtosPlayerCtrl)

// 생성자입니다.
public:
	CGomtosPlayerCtrl();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 구현입니다.
protected:
	~CGomtosPlayerCtrl();

	DECLARE_OLECREATE_EX(CGomtosPlayerCtrl)    // 클래스 팩터리와 GUID입니다.
	DECLARE_OLETYPELIB(CGomtosPlayerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CGomtosPlayerCtrl)     // 속성 페이지 ID입니다.
	DECLARE_OLECTLTYPE(CGomtosPlayerCtrl)		// 형식 이름과 기타 상태입니다.

// 메시지 맵입니다.
	DECLARE_MESSAGE_MAP()

// 디스패치 맵입니다.
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 이벤트 맵입니다.
	DECLARE_EVENT_MAP()

// 디스패치와 이벤트 ID입니다.
public:
	enum {
		dispidPlayerState = 9,
		dispidVolume = 8,
		dispidCurPosition = 7,
		eventidOnStop = 1L,
		dispidDuration = 6L,
		dispidClose = 5L,
		dispidStop = 4L,
		dispidPause = 3L,
		dispidPlay = 2L,
		dispidOpen = 1L
	};
protected:
	VARIANT_BOOL fxOpen(BSTR pszFilePath);
	void fxPlay(void);
	void fxPause(void);
	void fxStop(void);
	void fxClose(void);
	DOUBLE fxDuration(void);

	CHMPlayer m_Player;

	void FireOnStop(void)
	{
		FireEvent(eventidOnStop, EVENT_PARAM(VTS_NONE));
	}
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);
protected:
	DOUBLE GetCurPosition(void);
	void SetCurPosition(DOUBLE newVal);
	DOUBLE GetVolume(void);
	void SetVolume(DOUBLE newVal);
	ULONG GetPlayerState(void);
	void SetPlayerState(ULONG newVal);
};

