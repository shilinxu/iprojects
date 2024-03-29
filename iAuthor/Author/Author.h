// Author.h : main header file for the AUTHOR application
//

#if !defined(AFX_AUTHOR_H__3FB4301C_238A_4CB4_921A_5CAA2D7B21F0__INCLUDED_)
#define AFX_AUTHOR_H__3FB4301C_238A_4CB4_921A_5CAA2D7B21F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAuthorApp:
// See Author.cpp for the implementation of this class
//

class CAuthorApp : public CWinApp
{
public:
	CAuthorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuthorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CAuthorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTHOR_H__3FB4301C_238A_4CB4_921A_5CAA2D7B21F0__INCLUDED_)
