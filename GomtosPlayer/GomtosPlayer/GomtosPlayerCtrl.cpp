// GomtosPlayerCtrl.cpp : CGomtosPlayerCtrl ActiveX ��Ʈ�� Ŭ������ �����Դϴ�.

#include "stdafx.h"
#include "GomtosPlayer.h"
#include "GomtosPlayerCtrl.h"
#include "GomtosPlayerPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CGomtosPlayerCtrl, COleControl)



// �޽��� ���Դϴ�.

BEGIN_MESSAGE_MAP(CGomtosPlayerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_HM_GRAPHNOTIFY, OnGraphNotify)
END_MESSAGE_MAP()



// ����ġ ���Դϴ�.

BEGIN_DISPATCH_MAP(CGomtosPlayerCtrl, COleControl)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Open", dispidOpen, fxOpen, VT_BOOL, VTS_BSTR)	//VTS_PBSTR)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Play", dispidPlay, fxPlay, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Pause", dispidPause, fxPause, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Stop", dispidStop, fxStop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Close", dispidClose, fxClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGomtosPlayerCtrl, "Duration", dispidDuration, fxDuration, VT_R8, VTS_NONE)
	DISP_PROPERTY_EX_ID(CGomtosPlayerCtrl, "CurPosition", dispidCurPosition, GetCurPosition, SetCurPosition, VT_R8)
	DISP_PROPERTY_EX_ID(CGomtosPlayerCtrl, "Volume", dispidVolume, GetVolume, SetVolume, VT_R8)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_PROPERTY_EX_ID(CGomtosPlayerCtrl, "PlayerState", dispidPlayerState, GetPlayerState, SetPlayerState, VT_UI4)
END_DISPATCH_MAP()



// �̺�Ʈ ���Դϴ�.

BEGIN_EVENT_MAP(CGomtosPlayerCtrl, COleControl)
	EVENT_CUSTOM_ID("OnStop", eventidOnStop, FireOnStop, VTS_NONE)
END_EVENT_MAP()



// �Ӽ� �������Դϴ�.

// TODO: �ʿ信 ���� �Ӽ� �������� �߰��մϴ�. ī��Ʈ�� �̿� ���� �����ؾ� �մϴ�.
BEGIN_PROPPAGEIDS(CGomtosPlayerCtrl, 1)
	PROPPAGEID(CGomtosPlayerPropPage::guid)
END_PROPPAGEIDS(CGomtosPlayerCtrl)



// Ŭ���� ���͸��� GUID�� �ʱ�ȭ�մϴ�.

IMPLEMENT_OLECREATE_EX(CGomtosPlayerCtrl, "GOMTOSPLAYER.GomtosPlayerCtrl.1",
	0x2960fff0, 0x6756, 0x4807, 0x86, 0xa0, 0xd6, 0x68, 0xf2, 0x59, 0xb5, 0xfa)



// ���� ���̺귯�� ID�� �����Դϴ�.

IMPLEMENT_OLETYPELIB(CGomtosPlayerCtrl, _tlid, _wVerMajor, _wVerMinor)



// �������̽� ID�Դϴ�.

const IID BASED_CODE IID_DGomtosPlayer =
		{ 0x8D915DBC, 0x482F, 0x4A7D, { 0xA7, 0xC8, 0xE9, 0x5C, 0xCD, 0xD7, 0xCA, 0xFB } };
const IID BASED_CODE IID_DGomtosPlayerEvents =
		{ 0x49E89DD5, 0xEDFD, 0x4D90, { 0x81, 0x5, 0x58, 0x4C, 0xE, 0xE4, 0x47, 0x1B } };



// ��Ʈ�� ���� �����Դϴ�.

static const DWORD BASED_CODE _dwGomtosPlayerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CGomtosPlayerCtrl, IDS_GOMTOSPLAYER, _dwGomtosPlayerOleMisc)



// CGomtosPlayerCtrl::CGomtosPlayerCtrlFactory::UpdateRegistry -
// CGomtosPlayerCtrl���� �ý��� ������Ʈ�� �׸��� �߰��ϰų� �����մϴ�.

BOOL CGomtosPlayerCtrl::CGomtosPlayerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��Ʈ���� ����Ʈ �� ������ ��Ģ�� �ؼ��ϴ���
	// Ȯ���մϴ�. �ڼ��� ������ MFC Technical Note 64��
	// �����Ͻʽÿ�. ��Ʈ���� ����Ʈ �� ��Ģ��
	// ���� �ʴ� ��� �������� ���� ��° �Ű� ������ �����մϴ�.
	// afxRegInsertable | afxRegApartmentThreading���� afxRegInsertable�� �����մϴ�.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GOMTOSPLAYER,
			IDB_GOMTOSPLAYER,
			afxRegInsertable | afxRegApartmentThreading,
			_dwGomtosPlayerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CGomtosPlayerCtrl::CGomtosPlayerCtrl - �������Դϴ�.

CGomtosPlayerCtrl::CGomtosPlayerCtrl()
{
	InitializeIIDs(&IID_DGomtosPlayer, &IID_DGomtosPlayerEvents);
	// TODO: ���⿡�� ��Ʈ���� �ν��Ͻ� �����͸� �ʱ�ȭ�մϴ�.
}



// CGomtosPlayerCtrl::~CGomtosPlayerCtrl - �Ҹ����Դϴ�.

CGomtosPlayerCtrl::~CGomtosPlayerCtrl()
{
	// TODO: ���⿡�� ��Ʈ���� �ν��Ͻ� �����͸� �����մϴ�.
}



// CGomtosPlayerCtrl::OnDraw - �׸��� �Լ��Դϴ�.

void CGomtosPlayerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	CBrush bkBrush(TranslateColor(GetBackColor()));
	//COLORREF clrFore = TranslateColor(GetForeColor());
	pdc->FillRect(rcBounds, &bkBrush);
	//pdc->SetTextColor(clrFore);
	//pdc->DrawText(InternalGetText(), -1, rcBounds, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(DKGRAY_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CGomtosPlayerCtrl::DoPropExchange - ���Ӽ� �����Դϴ�.

void CGomtosPlayerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: �������� ����� ���� �Ӽ� ��ο� ���� PX_ functions�� ȣ���մϴ�.
}



// CGomtosPlayerCtrl::OnResetState - ��Ʈ���� �⺻ ���·� �ٽ� �����մϴ�.

void CGomtosPlayerCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange�� ��� �ִ� �⺻���� �ٽ� �����մϴ�.

	// TODO: ���⿡�� �ٸ� ��� ��Ʈ���� ���¸� �ٽ� �����մϴ�.
}


int CGomtosPlayerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
    //m_Player.Create(_T("Gomtos Player Window"), WS_CHILD,
				//	CRect(0,0,100,100), this);
    ////m_Player.ShowWindow(SW_SHOW);

	SetBackColor(RGB(0,0,0));
	return 0;
}

void CGomtosPlayerCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CRect cRect;
	GetClientRect(cRect);

	if(m_Player.IsValid())
		m_Player.SetWindowPosition(cRect.left, cRect.top, cRect.Width(), cRect.Height());
}

void CGomtosPlayerCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_Player.Close();
	//m_Player.DestroyWindow();
}

LRESULT CGomtosPlayerCtrl::OnGraphNotify(WPARAM wParam, LPARAM lParam)
{
	// Event Notification Codes (; ref. DirectX help)
	long ev_code = m_Player.HandleGraphEvent();
	 
	if (m_Player.GetState() == CHMPlayer::Stopped)
	{
		FireOnStop();
		//m_Player.Play();
		m_Player.Pause();
		m_Player.SeekTime(m_Player.GetDuration());
	}

	return 0;
}

// CGomtosPlayerCtrl::AboutBox - "����" ��ȭ ���ڸ� ����ڿ��� ���� �ݴϴ�.

void CGomtosPlayerCtrl::AboutBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlgAbout(IDD_ABOUTBOX_GOMTOSPLAYER);
	dlgAbout.DoModal();
}



// CGomtosPlayerCtrl �޽��� ó�����Դϴ�.

VARIANT_BOOL CGomtosPlayerCtrl::fxOpen(BSTR pszFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	//MessageBox(pszFilePath);

	if (m_Player.IsValid())
		m_Player.Close();

	m_Player.SetGraphNotify(m_hWnd);
	bool ok = m_Player.Open(pszFilePath, GetSafeHwnd());
	if (ok)
		fxStop();

	return (ok) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CGomtosPlayerCtrl::fxPlay(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Player.Play();
}

void CGomtosPlayerCtrl::fxPause(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Player.Pause();
}

void CGomtosPlayerCtrl::fxStop(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//m_Player.Stop();
	m_Player.Pause();
	m_Player.SeekTime(0);
}

void CGomtosPlayerCtrl::fxClose(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	//if(m_Player.m_hWnd != NULL)
	//	m_Player.ShowWindow(SW_HIDE);

	m_Player.Close();
}

DOUBLE CGomtosPlayerCtrl::fxDuration(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DOUBLE dur = m_Player.GetDuration();// / 10000;

	return dur;
}

DOUBLE CGomtosPlayerCtrl::GetCurPosition(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DOUBLE curPos = m_Player.GetCurrentPosition();

	return curPos * 1000;
}

void CGomtosPlayerCtrl::SetCurPosition(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DOUBLE curPos = newVal / 1000;
	m_Player.SeekTime(curPos);

	SetModifiedFlag();
}

DOUBLE CGomtosPlayerCtrl::GetVolume(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// -6,000 ... 0 -> 0.0 ... 1.0 Conversion
	LONG vol = m_Player.GetAudioVolume();
	if (vol <= -6000.)
		vol = 0;
	else
		vol = (LONG) ((vol + 6000.0) / 60.);

	return (DOUBLE)vol;
}

void CGomtosPlayerCtrl::SetVolume(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//LONG dbVol = 20 * log (newVal / 100);
	// 0 ~ 100 = -6,000 ~ 0 Conversion
    LONG dbVol = (long) ((newVal / 100.) * 6000. - 6000.);
	if (dbVol <= -6000)
		dbVol = 0;

	m_Player.SetAudioVolume(dbVol);

	SetModifiedFlag();
}

ULONG CGomtosPlayerCtrl::GetPlayerState(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Player.GetState();
}

void CGomtosPlayerCtrl::SetPlayerState(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// �б� �����̶� �ƹ��͵� ����
	//SetModifiedFlag();
}
