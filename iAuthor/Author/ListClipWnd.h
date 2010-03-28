/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//	CCListClipWnd Class	- Display window of List Item					   //
//  Include CListItem													   //
//                                                                         //
//   by samgo 2002.7													   //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTCLIPWND_H__AB10750E_D141_47DB_A07F_197A7D654213__INCLUDED_)
#define AFX_LISTCLIPWND_H__AB10750E_D141_47DB_A07F_197A7D654213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListClipWnd.h : header file
//

#include "ListItem.h"

/////////////////////////////////////////////////////////////////////////////
// CListClipWnd window

class CListClipWnd : public CWnd
{
// Construction
public:
	CListClipWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListClipWnd)
	//}}AFX_VIRTUAL

	virtual ~CListClipWnd();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd);

	enum _Style{
		TREE = 0,
		CLIP,
		SLIDE
	};

	// Generated message map functions
protected:
	//{{AFX_MSG(CListClipWnd)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// Item width & height
	int		m_ItemWidth;
	int		m_ItemHeight;

	// Draw Image rect
	CRect	m_rcImageRect;

protected:
	// Item List
	CObList m_ItemList;

	int     m_CurColumn;
	int		m_CurRow;
	int		m_nColumn;	
	int		m_WSpace_Item;
	int		m_HSpace_Item;
	CPoint	m_ptStart_Item;	

	// Selected item index
	int		m_IDSelectItem;
	CString	m_strSelItem;


	CRect	m_rcSelectItem;
protected:
	// Rearrang item
	void LayOut(void);
	// get window size
	int GetSize(void);	

public:	
	void ReplaceSildeThumbnail(CString strPath);
	BOOL AddSlideItem(CString strPath, int nIndex);
	void OnSelectChangeItem(int nIndex);
	
	// Return Selected Item Path
	CString GetSelteditemPath();
	// Delete Item
	// Initialize
	void Init(void);
	// Return Index
private:
	bool IsValidSlide(CString strPath);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCLIPWND_H__AB10750E_D141_47DB_A07F_197A7D654213__INCLUDED_)
