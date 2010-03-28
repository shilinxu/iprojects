#if !defined(AFX_SLIDELISTWND_H__EB404B07_8AC7_4FC8_9729_0EF01D4C2D10__INCLUDED_)
#define AFX_SLIDELISTWND_H__EB404B07_8AC7_4FC8_9729_0EF01D4C2D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlideListWnd.h : header file
//

#include "ListWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSlideListWnd window

class CSlideListWnd : public CWnd
{
// Construction
public:
	CSlideListWnd();

// Attributes
public:
	CListWnd	m_CLipListWnd;

	int m_nIndex;
	CString m_strSelectItemPath;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideListWnd)
	public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReplaceSildeThumbnail(CString strPath);
	CString GetSelteditemPath();
	int GetCount();
	void Init();
	void AddItem(CString strPath);	
	virtual ~CSlideListWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSlideListWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDELISTWND_H__EB404B07_8AC7_4FC8_9729_0EF01D4C2D10__INCLUDED_)
