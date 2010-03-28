// XInkTest.h : main header file for the XINKTEST application
//

#if !defined(AFX_XINKTEST_H__AC33766C_D7C5_4400_8EAC_123B5A0280E3__INCLUDED_)
#define AFX_XINKTEST_H__AC33766C_D7C5_4400_8EAC_123B5A0280E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXInkTestApp:
// See XInkTest.cpp for the implementation of this class
//

class CXInkTestApp : public CWinApp
{
public:
	CXInkTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXInkTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CXInkTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XINKTEST_H__AC33766C_D7C5_4400_8EAC_123B5A0280E3__INCLUDED_)
