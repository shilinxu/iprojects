// VidCapTest.h : main header file for the VIDCAPTEST application
//

#if !defined(AFX_VIDCAPTEST_H__18AAF4FB_F2A8_4276_8A28_B0B49DF0B74E__INCLUDED_)
#define AFX_VIDCAPTEST_H__18AAF4FB_F2A8_4276_8A28_B0B49DF0B74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVidCapTestApp:
// See VidCapTest.cpp for the implementation of this class
//

class CVidCapTestApp : public CWinApp
{
public:
	CVidCapTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVidCapTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVidCapTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDCAPTEST_H__18AAF4FB_F2A8_4276_8A28_B0B49DF0B74E__INCLUDED_)
