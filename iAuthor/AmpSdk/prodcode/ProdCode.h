// ProdCode.h : main header file for the ProdCode application
//

#if !defined(AFX_ProdCode_H__95FA5412_A387_4571_9F3D_5721D07EFFCF__INCLUDED_)
#define AFX_ProdCode_H__95FA5412_A387_4571_9F3D_5721D07EFFCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProdCodeApp:
// See ProdCode.cpp for the implementation of this class
//

class CProdCodeApp : public CWinApp
{
public:
	CProdCodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdCodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProdCodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ProdCode_H__95FA5412_A387_4571_9F3D_5721D07EFFCF__INCLUDED_)
