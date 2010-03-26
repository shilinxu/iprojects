// GomtosPlayerPropPage.cpp : CGomtosPlayerPropPage �Ӽ� ������ Ŭ������ �����Դϴ�.

#include "stdafx.h"
#include "GomtosPlayer.h"
#include "GomtosPlayerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CGomtosPlayerPropPage, COlePropertyPage)



// �޽��� ���Դϴ�.

BEGIN_MESSAGE_MAP(CGomtosPlayerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Ŭ���� ���͸��� GUID�� �ʱ�ȭ�մϴ�.

IMPLEMENT_OLECREATE_EX(CGomtosPlayerPropPage, "GOMTOSPLAYER.GomtosPlayerPropPage.1",
	0x50b29abb, 0x7822, 0x49a3, 0xa6, 0x4e, 0x2d, 0x2b, 0xb1, 0x47, 0x86, 0x77)



// CGomtosPlayerPropPage::CGomtosPlayerPropPageFactory::UpdateRegistry -
// CGomtosPlayerPropPage���� �ý��� ������Ʈ�� �׸��� �߰��ϰų� �����մϴ�.

BOOL CGomtosPlayerPropPage::CGomtosPlayerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GOMTOSPLAYER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CGomtosPlayerPropPage::CGomtosPlayerPropPage - �������Դϴ�.

CGomtosPlayerPropPage::CGomtosPlayerPropPage() :
	COlePropertyPage(IDD, IDS_GOMTOSPLAYER_PPG_CAPTION)
{
}



// CGomtosPlayerPropPage::DoDataExchange - �������� �Ӽ� ���̿��� �����͸� �̵���ŵ�ϴ�.

void CGomtosPlayerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CGomtosPlayerPropPage �޽��� ó�����Դϴ�.
