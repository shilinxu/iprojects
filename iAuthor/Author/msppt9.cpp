// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msppt9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _Application::GetPresentations()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetActivePresentation()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetSlideShowWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetPath()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetCaption()
{
	CString result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Application::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Application::GetAssistant()
{
	LPDISPATCH result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetFileSearch()
{
	LPDISPATCH result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetFileFind()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetBuild()
{
	CString result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetVersion()
{
	CString result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetOperatingSystem()
{
	CString result;
	InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetActivePrinter()
{
	CString result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Application::GetCreator()
{
	long result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Application::Help(LPCTSTR HelpFile, long ContextID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 HelpFile, ContextID);
}

void _Application::Quit()
{
	InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

float _Application::GetLeft()
{
	float result;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application::SetLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application::GetTop()
{
	float result;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application::SetTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application::GetWidth()
{
	float result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Application::GetHeight()
{
	float result;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Application::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Application::GetWindowState()
{
	long result;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ed, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application::GetVisible()
{
	long result;
	InvokeHelper(0x7ee, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application::GetActive()
{
	long result;
	InvokeHelper(0x7f0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::Activate()
{
	InvokeHelper(0x7f1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Application::GetAnswerWizard()
{
	LPDISPATCH result;
	InvokeHelper(0x7f2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetCOMAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x7f3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetProductCode()
{
	CString result;
	InvokeHelper(0x7f4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetDefaultWebOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x7f5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetLanguageSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetShowWindowsInTaskbar()
{
	long result;
	InvokeHelper(0x7f8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetShowWindowsInTaskbar(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Application::GetFeatureInstall()
{
	long result;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Application::SetFeatureInstall(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowWindow operations

LPDISPATCH SlideShowWindow::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow::GetView()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowWindow::GetPresentation()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowWindow::GetIsFullScreen()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float SlideShowWindow::GetLeft()
{
	float result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow::SetLeft(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow::GetTop()
{
	float result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow::SetTop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow::GetWidth()
{
	float result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float SlideShowWindow::GetHeight()
{
	float result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long SlideShowWindow::GetActive()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowWindow::Activate()
{
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowView properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowView operations

LPDISPATCH SlideShowView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView::GetZoom()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView::GetSlide()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView::GetPointerType()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView::SetPointerType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowView::GetState()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView::SetState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowView::GetAcceleratorsEnabled()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowView::SetAcceleratorsEnabled(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float SlideShowView::GetPresentationElapsedTime()
{
	float result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

float SlideShowView::GetSlideElapsedTime()
{
	float result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void SlideShowView::SetSlideElapsedTime(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH SlideShowView::GetLastSlideViewed()
{
	LPDISPATCH result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView::GetAdvanceMode()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowView::GetPointerColor()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowView::GetIsNamedShow()
{
	long result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString SlideShowView::GetSlideShowName()
{
	CString result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void SlideShowView::DrawLine(float BeginX, float BeginY, float EndX, float EndY)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 BeginX, BeginY, EndX, EndY);
}

void SlideShowView::EraseDrawing()
{
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::First()
{
	InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::Last()
{
	InvokeHelper(0x7e2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::Next()
{
	InvokeHelper(0x7e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::Previous()
{
	InvokeHelper(0x7e4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::GotoSlide(long index, long ResetSlide)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7e5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 index, ResetSlide);
}

void SlideShowView::GotoNamedShow(LPCTSTR SlideShowName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SlideShowName);
}

void SlideShowView::EndNamedShow()
{
	InvokeHelper(0x7e7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::ResetSlideTime()
{
	InvokeHelper(0x7e8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SlideShowView::Exit()
{
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SlideShowView::GetCurrentShowPosition()
{
	long result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings properties

/////////////////////////////////////////////////////////////////////////////
// SlideShowSettings operations

LPDISPATCH SlideShowSettings::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings::GetPointerColor()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SlideShowSettings::GetNamedSlideShows()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowSettings::GetStartingSlide()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetStartingSlide(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings::GetEndingSlide()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetEndingSlide(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings::GetAdvanceMode()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetAdvanceMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH SlideShowSettings::Run()
{
	LPDISPATCH result;
	InvokeHelper(0x7d8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SlideShowSettings::GetLoopUntilStopped()
{
	long result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetLoopUntilStopped(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings::GetShowType()
{
	long result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetShowType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7da, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings::GetShowWithNarration()
{
	long result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetShowWithNarration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7db, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long SlideShowSettings::GetShowWithAnimation()
{
	long result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetShowWithAnimation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString SlideShowSettings::GetSlideShowName()
{
	CString result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetSlideShowName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long SlideShowSettings::GetRangeType()
{
	long result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void SlideShowSettings::SetRangeType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// Presentations properties

/////////////////////////////////////////////////////////////////////////////
// Presentations operations

long Presentations::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Presentations::Item(const VARIANT& index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&index);
	return result;
}

LPDISPATCH Presentations::Add(long WithWindow)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		WithWindow);
	return result;
}

LPDISPATCH Presentations::Open(LPCTSTR FileName, long ReadOnly, long Untitled, long WithWindow)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7d4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, ReadOnly, Untitled, WithWindow);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// PageSetup properties

/////////////////////////////////////////////////////////////////////////////
// PageSetup operations

LPDISPATCH PageSetup::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PageSetup::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PageSetup::GetFirstSlideNumber()
{
	long result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetFirstSlideNumber(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

float PageSetup::GetSlideHeight()
{
	float result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetSlideHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float PageSetup::GetSlideWidth()
{
	float result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetSlideWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long PageSetup::GetSlideSize()
{
	long result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetSlideSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PageSetup::GetNotesOrientation()
{
	long result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetNotesOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long PageSetup::GetSlideOrientation()
{
	long result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void PageSetup::SetSlideOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// Slides properties

/////////////////////////////////////////////////////////////////////////////
// Slides operations

long Slides::GetCount()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Slides::Item(const VARIANT& index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&index);
	return result;
}

LPDISPATCH Slides::FindBySlideID(long SlideID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		SlideID);
	return result;
}

LPDISPATCH Slides::Add(long index, long Layout)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x7d4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		index, Layout);
	return result;
}

long Slides::InsertFromFile(LPCTSTR FileName, long index, long SlideStart, long SlideEnd)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x7d5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FileName, index, SlideStart, SlideEnd);
	return result;
}

LPDISPATCH Slides::Range(const VARIANT& index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x7d6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&index);
	return result;
}

LPDISPATCH Slides::Paste(long index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7d7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Slide properties

/////////////////////////////////////////////////////////////////////////////
// _Slide operations

LPDISPATCH _Slide::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetHeadersFooters()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetSlideShowTransition()
{
	LPDISPATCH result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetColorScheme()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide::SetColorScheme(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7d6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Slide::GetBackground()
{
	LPDISPATCH result;
	InvokeHelper(0x7d7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Slide::GetName()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Slide::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Slide::GetSlideID()
{
	long result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide::GetPrintSteps()
{
	long result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide::Select()
{
	InvokeHelper(0x7db, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Slide::Cut()
{
	InvokeHelper(0x7dc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Slide::Copy()
{
	InvokeHelper(0x7dd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Slide::GetLayout()
{
	long result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide::SetLayout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7de, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Slide::Duplicate()
{
	LPDISPATCH result;
	InvokeHelper(0x7df, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide::Delete()
{
	InvokeHelper(0x7e0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Slide::GetTags()
{
	LPDISPATCH result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Slide::GetSlideIndex()
{
	long result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide::GetSlideNumber()
{
	long result;
	InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Slide::GetDisplayMasterShapes()
{
	long result;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide::SetDisplayMasterShapes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Slide::GetFollowMasterBackground()
{
	long result;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Slide::SetFollowMasterBackground(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Slide::GetNotesPage()
{
	LPDISPATCH result;
	InvokeHelper(0x7e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Slide::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Slide::Export(LPCTSTR FileName, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7e9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FilterName, ScaleWidth, ScaleHeight);
}

LPDISPATCH _Slide::GetScripts()
{
	LPDISPATCH result;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Presentation properties

/////////////////////////////////////////////////////////////////////////////
// _Presentation operations

LPDISPATCH _Presentation::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x7d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetSlideMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetTitleMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation::GetHasTitleMaster()
{
	long result;
	InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::AddTitleMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation::ApplyTemplate(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7d7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

CString _Presentation::GetTemplateName()
{
	CString result;
	InvokeHelper(0x7d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetNotesMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7d9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetHandoutMaster()
{
	LPDISPATCH result;
	InvokeHelper(0x7da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetSlides()
{
	LPDISPATCH result;
	InvokeHelper(0x7db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x7dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetColorSchemes()
{
	LPDISPATCH result;
	InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetExtraColors()
{
	LPDISPATCH result;
	InvokeHelper(0x7de, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetSlideShowSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetFonts()
{
	LPDISPATCH result;
	InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x7e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetTags()
{
	LPDISPATCH result;
	InvokeHelper(0x7e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetDefaultShape()
{
	LPDISPATCH result;
	InvokeHelper(0x7e3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetBuiltInDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetCustomDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x7e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation::GetReadOnly()
{
	long result;
	InvokeHelper(0x7e7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Presentation::GetFullName()
{
	CString result;
	InvokeHelper(0x7e8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Presentation::GetName()
{
	CString result;
	InvokeHelper(0x7e9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Presentation::GetPath()
{
	CString result;
	InvokeHelper(0x7ea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Presentation::GetSaved()
{
	long result;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetSaved(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation::GetLayoutDirection()
{
	long result;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetLayoutDirection(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Presentation::NewWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7ed, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation::FollowHyperlink(LPCTSTR Address, LPCTSTR SubAddress, BOOL NewWindow, BOOL AddHistory, LPCTSTR ExtraInfo, long Method, LPCTSTR HeaderInfo)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(0x7ee, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Address, SubAddress, NewWindow, AddHistory, ExtraInfo, Method, HeaderInfo);
}

void _Presentation::AddToFavorites()
{
	InvokeHelper(0x7ef, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation::GetPrintOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x7f1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation::PrintOut(long From, long To, LPCTSTR PrintToFile, long Copies, long Collate)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 From, To, PrintToFile, Copies, Collate);
}

void _Presentation::Save()
{
	InvokeHelper(0x7f3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Presentation::SaveAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileFormat, EmbedTrueTypeFonts);
}

void _Presentation::SaveCopyAs(LPCTSTR FileName, long FileFormat, long EmbedTrueTypeFonts)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileFormat, EmbedTrueTypeFonts);
}

void _Presentation::Export(LPCTSTR Path, LPCTSTR FilterName, long ScaleWidth, long ScaleHeight)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x7f6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, FilterName, ScaleWidth, ScaleHeight);
}

void _Presentation::Close()
{
	InvokeHelper(0x7f7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation::GetContainer()
{
	LPDISPATCH result;
	InvokeHelper(0x7f9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation::GetDisplayComments()
{
	long result;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetDisplayComments(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation::GetFarEastLineBreakLevel()
{
	long result;
	InvokeHelper(0x7fb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetFarEastLineBreakLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7fb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Presentation::GetNoLineBreakBefore()
{
	CString result;
	InvokeHelper(0x7fc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation::SetNoLineBreakBefore(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7fc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Presentation::GetNoLineBreakAfter()
{
	CString result;
	InvokeHelper(0x7fd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Presentation::SetNoLineBreakAfter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7fd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Presentation::UpdateLinks()
{
	InvokeHelper(0x7fe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Presentation::GetSlideShowWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x7ff, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Presentation::GetFarEastLineBreakLanguage()
{
	long result;
	InvokeHelper(0x800, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetFarEastLineBreakLanguage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x800, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Presentation::WebPagePreview()
{
	InvokeHelper(0x801, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Presentation::GetDefaultLanguageID()
{
	long result;
	InvokeHelper(0x802, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetDefaultLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Presentation::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x803, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetPublishObjects()
{
	LPDISPATCH result;
	InvokeHelper(0x804, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetWebOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x805, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Presentation::GetHTMLProject()
{
	LPDISPATCH result;
	InvokeHelper(0x806, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Presentation::ReloadAs(long cp)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x807, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 cp);
}

long _Presentation::GetEnvelopeVisible()
{
	long result;
	InvokeHelper(0x809, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Presentation::SetEnvelopeVisible(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x809, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Presentation::GetVBASigned()
{
	long result;
	InvokeHelper(0x80b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
