#pragma once

// GomtosPlayerCtrl.h : CGomtosPlayerCtrl ActiveX ��Ʈ�� Ŭ������ �����Դϴ�.


// CGomtosPlayerCtrl : ������ ������ GomtosPlayerCtrl.cpp��(��) �����Ͻʽÿ�.
#include "HMPlayer.h"

class CGomtosPlayerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CGomtosPlayerCtrl)

// �������Դϴ�.
public:
	CGomtosPlayerCtrl();

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// �����Դϴ�.
protected:
	~CGomtosPlayerCtrl();

	DECLARE_OLECREATE_EX(CGomtosPlayerCtrl)    // Ŭ���� ���͸��� GUID�Դϴ�.
	DECLARE_OLETYPELIB(CGomtosPlayerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CGomtosPlayerCtrl)     // �Ӽ� ������ ID�Դϴ�.
	DECLARE_OLECTLTYPE(CGomtosPlayerCtrl)		// ���� �̸��� ��Ÿ �����Դϴ�.

// �޽��� ���Դϴ�.
	DECLARE_MESSAGE_MAP()

// ����ġ ���Դϴ�.
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �̺�Ʈ ���Դϴ�.
	DECLARE_EVENT_MAP()

// ����ġ�� �̺�Ʈ ID�Դϴ�.
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

