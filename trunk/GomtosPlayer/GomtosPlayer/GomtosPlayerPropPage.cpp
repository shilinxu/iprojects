// GomtosPlayerPropPage.cpp : CGomtosPlayerPropPage 속성 페이지 클래스의 구현입니다.

#include "stdafx.h"
#include "GomtosPlayer.h"
#include "GomtosPlayerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CGomtosPlayerPropPage, COlePropertyPage)



// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CGomtosPlayerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CGomtosPlayerPropPage, "GOMTOSPLAYER.GomtosPlayerPropPage.1",
	0x50b29abb, 0x7822, 0x49a3, 0xa6, 0x4e, 0x2d, 0x2b, 0xb1, 0x47, 0x86, 0x77)



// CGomtosPlayerPropPage::CGomtosPlayerPropPageFactory::UpdateRegistry -
// CGomtosPlayerPropPage에서 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CGomtosPlayerPropPage::CGomtosPlayerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GOMTOSPLAYER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CGomtosPlayerPropPage::CGomtosPlayerPropPage - 생성자입니다.

CGomtosPlayerPropPage::CGomtosPlayerPropPage() :
	COlePropertyPage(IDD, IDS_GOMTOSPLAYER_PPG_CAPTION)
{
}



// CGomtosPlayerPropPage::DoDataExchange - 페이지와 속성 사이에서 데이터를 이동시킵니다.

void CGomtosPlayerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CGomtosPlayerPropPage 메시지 처리기입니다.
