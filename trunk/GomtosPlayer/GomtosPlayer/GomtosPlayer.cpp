// GomtosPlayer.cpp : CGomtosPlayerApp �� DLL ����� �����Դϴ�.

#include "stdafx.h"
#include "GomtosPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGomtosPlayerApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x54E0B22E, 0x4217, 0x4EE1, { 0x8B, 0xB4, 0x52, 0x8F, 0x3B, 0x2D, 0xBB, 0x1D } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CGomtosPlayerApp::InitInstance - DLL �ʱ�ȭ�Դϴ�.

BOOL CGomtosPlayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: ���⿡ ���� �ۼ��� ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	}

	return bInit;
}



// CGomtosPlayerApp::ExitInstance - DLL �����Դϴ�.

int CGomtosPlayerApp::ExitInstance()
{
	// TODO: ���⿡ ���� �ۼ��� ��� ���� �ڵ带 �߰��մϴ�.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - �ý��� ������Ʈ���� �׸��� �߰��մϴ�.

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �ý��� ������Ʈ������ �׸��� �����մϴ�.

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
