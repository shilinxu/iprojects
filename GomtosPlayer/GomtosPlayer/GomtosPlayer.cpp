// GomtosPlayer.cpp : CGomtosPlayerApp 및 DLL 등록의 구현입니다.

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



// CGomtosPlayerApp::InitInstance - DLL 초기화입니다.

BOOL CGomtosPlayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 여기에 직접 작성한 모듈 초기화 코드를 추가합니다.
	}

	return bInit;
}



// CGomtosPlayerApp::ExitInstance - DLL 종료입니다.

int CGomtosPlayerApp::ExitInstance()
{
	// TODO: 여기에 직접 작성한 모듈 종료 코드를 추가합니다.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 시스템 레지스트리에 항목을 추가합니다.

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 시스템 레지스트리에서 항목을 제거합니다.

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
