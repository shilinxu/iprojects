/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//	CListWnd Class - Enable Dispaly Animation item						   //
//  Include CCListClipWnd												   //
//                                                                         //
//   by samgo 2002.7													   //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTWND_H__653A55D0_5C15_4892_A00C_9A40C8AED1DA__INCLUDED_)
#define AFX_LISTWND_H__653A55D0_5C15_4892_A00C_9A40C8AED1DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListWnd.h : header file
//

#include "ListClipWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CListWnd window

class CListWnd : public CWnd
{
// Construction
public:
	CListWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListWnd)
	//}}AFX_VIRTUAL

	virtual ~CListWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	// Generated message map functions
protected:
	//{{AFX_MSG(CListWnd)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	// Scroll Bar
	CScrollBar	m_ListScrollbar;
	// Item arrange window
	CListClipWnd m_ListClipWnd;

	SCROLLINFO m_ScrollInfo;

	int		m_ScrollPosition;
	int		m_ShiftWndValue;
	int		m_nPageHeight;
	int		m_nScrollRange;	
	UINT	m_nID;

	CString m_strSelectItemPath;
	
protected:	
	// Scroll function
	void MoveListClipWnd();

public:
	void ReplaceSildeThumbnail(CString strPath);
	BOOL AddSlideItem(CString strPath, int nIndex);
	// insert item Url, Resource ID
	// Delete item

	// Rearrange item
	void ReLayOut(int nHeight, int nPage);	
	// Init position
	void MoveListWnd_Init(void);

	// Return Selected Item Path
	CString GetSelteditemPath();
	
	// Initialize 
	void Init(void);
	// Return Index 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTWND_H__653A55D0_5C15_4892_A00C_9A40C8AED1DA__INCLUDED_)
