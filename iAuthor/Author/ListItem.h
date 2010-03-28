/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//	CListItem Class	- List control item class							   //
//  Animation display function at Message WM_MOUSE_MOVE					   //
//                                                                         //
//   by samgo 2002.7													   //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTITEM_H__B56A39A4_785E_4190_AEC2_27357CDC6CED__INCLUDED_)
#define AFX_LISTITEM_H__B56A39A4_785E_4190_AEC2_27357CDC6CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListItem window

class CListItem : public CWnd
{
// Construction
public:
	CListItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListItem)
	//}}AFX_VIRTUAL

	virtual ~CListItem();
	BOOL Create(CWnd* pWnd, UINT nID=0, CRect rect=CRect(0,0,0,0));		

	// Generated message map functions
protected:
	//{{AFX_MSG(CListItem)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// Implimentation
public:
	void SetSelection(bool bSelect);

public:	
	bool	m_bSelect;
	Bitmap*  m_pBitmap;
	
	int		m_Index;		
	CString	m_strPath;		
	CString	m_strName;

	CRect	m_rcImageRect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTITEM_H__B56A39A4_785E_4190_AEC2_27357CDC6CED__INCLUDED_)
