// PPTUtil.cpp: implementation of the CPPTUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Author.h"
#include "PPTUtil.h"
#include "msppt9.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPPTUtil::CPPTUtil()
{

}

CPPTUtil::~CPPTUtil()
{

}


bool CPPTUtil::GoFirstSlide()
{
//	CWaitCursor wait;
	_Application app; // app is the PowerPoint _Application object

	// Translate server ProgID into a CLSID. ClsidFromProgID
	// gets this information from the registry.
	CLSID clsid;
	CLSIDFromProgID(L"PowerPoint.Application", &clsid);  

	// Get an interface to the running instance, if any..
	IUnknown *pUnk;
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	if (FAILED(hr)) return false;

	// Get IDispatch interface for Automation...
	IDispatch *pDisp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
	if (FAILED(hr)) return false;

	// Release the no-longer-needed IUnknown...
	pUnk->Release();


	app.AttachDispatch(pDisp);

	//app.SetAutomationSecurity(long newVal);
    _Presentation pr = app.GetActivePresentation();
	SlideShowWindow slsWnd;
	try
	{
		slsWnd = pr.GetSlideShowWindow();
	}
	catch (...)
	{
		return false;
	}

	slsWnd.Activate();	// bring slide window to the front.

    SlideShowView view = slsWnd.GetView();
	long state = view.GetState();
    view.First();

	return true;
}

bool CPPTUtil::GoPrevSlide()
{
//	CWaitCursor wait;
	_Application app; // app is the PowerPoint _Application object

	// Translate server ProgID into a CLSID. ClsidFromProgID
	// gets this information from the registry.
	CLSID clsid;
	CLSIDFromProgID(L"PowerPoint.Application", &clsid);  

	// Get an interface to the running instance, if any..
	IUnknown *pUnk;
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	if (FAILED(hr)) return false;

	// Get IDispatch interface for Automation...
	IDispatch *pDisp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
	if (FAILED(hr)) return false;

	// Release the no-longer-needed IUnknown...
	pUnk->Release();


	app.AttachDispatch(pDisp);

	//app.SetAutomationSecurity(long newVal);
    _Presentation pr = app.GetActivePresentation();
	SlideShowWindow slsWnd;
	try
	{
		slsWnd = pr.GetSlideShowWindow();
	}
	catch (...)
	{
		return false;
	}
	slsWnd.Activate();	// bring slide window to the front.

    SlideShowView view = slsWnd.GetView();
    view.Previous();

	return true;

}

bool CPPTUtil::GoNextSlide()
{
//	CWaitCursor wait;
	_Application app; // app is the PowerPoint _Application object

	// Translate server ProgID into a CLSID. ClsidFromProgID
	// gets this information from the registry.
	CLSID clsid;
	CLSIDFromProgID(L"PowerPoint.Application", &clsid);  

	// Get an interface to the running instance, if any..
	IUnknown *pUnk;
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	if (FAILED(hr)) return false;

	// Get IDispatch interface for Automation...
	IDispatch *pDisp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
	if (FAILED(hr)) return false;

	// Release the no-longer-needed IUnknown...
	pUnk->Release();


	app.AttachDispatch(pDisp);

	//app.SetAutomationSecurity(long newVal);
    _Presentation pr = app.GetActivePresentation();
	SlideShowWindow slsWnd;
	try
	{
		slsWnd = pr.GetSlideShowWindow();
	}
	catch (...)
	{
		return false;
	}
	slsWnd.Activate();	// bring slide window to the front.

    SlideShowView view = slsWnd.GetView();
    view.Next();

	return true;
}

bool CPPTUtil::GoLastSlide()
{
//	CWaitCursor wait;
	_Application app; // app is the PowerPoint _Application object

	// Translate server ProgID into a CLSID. ClsidFromProgID
	// gets this information from the registry.
	CLSID clsid;
	CLSIDFromProgID(L"PowerPoint.Application", &clsid);  

	// Get an interface to the running instance, if any..
	IUnknown *pUnk;
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	if (FAILED(hr)) return false;

	// Get IDispatch interface for Automation...
	IDispatch *pDisp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
	if (FAILED(hr)) return false;

	// Release the no-longer-needed IUnknown...
	pUnk->Release();


	app.AttachDispatch(pDisp);

	//app.SetAutomationSecurity(long newVal);
    _Presentation pr = app.GetActivePresentation();
	SlideShowWindow slsWnd;
	try
	{
		slsWnd = pr.GetSlideShowWindow();
	}
	catch (...)
	{
		return false;
	}
	slsWnd.Activate();	// bring slide window to the front.

    SlideShowView view = slsWnd.GetView();
    view.Last();

	return true;
}

bool CPPTUtil::IsPPTActive()
{
	_Application app; // app is the PowerPoint _Application object

	// Translate server ProgID into a CLSID. ClsidFromProgID
	// gets this information from the registry.
	CLSID clsid;
	CLSIDFromProgID(L"PowerPoint.Application", &clsid);  

	// Get an interface to the running instance, if any..
	IUnknown *pUnk = NULL;
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	if (FAILED(hr) || pUnk == NULL) return false;

	// Get IDispatch interface for Automation...
	IDispatch *pDisp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pDisp);
	if (FAILED(hr)) return false;

	// Release the no-longer-needed IUnknown...
	pUnk->Release();

	app.AttachDispatch(pDisp);

	//app.SetAutomationSecurity(long newVal);
    _Presentation pr = app.GetActivePresentation();
	SlideShowWindow slsWnd;
	try
	{
		slsWnd = pr.GetSlideShowWindow();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

