#if !defined(AFX_MAGNIFIERDLG_H__F52C9248_7E9A_447E_AB12_BBCE3B96AC79__INCLUDED_)
#define AFX_MAGNIFIERDLG_H__F52C9248_7E9A_447E_AB12_BBCE3B96AC79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MagnifierDlg.h : header file
//

#include "MagnifierPrevDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMagnifierDlg dialog

class CMagnifierDlg : public CDialog
{
// Construction
public:
	void SetParentWnd(CWnd* pWnd);
	void SetEnbleMagnifier(bool bEnable);
	void OnEscape();
	CMagnifierDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMagnifierDlg)
	enum { IDD = IDD_MAGNIFIERDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagnifierDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagnifierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:	

private:
	bool				m_bEnable;
	CMagnifierPrevDlg	MagniPrevDlg;	
	Bitmap*		m_pMagnifierBitmap;
	CSize		m_nWindowSize;
	CPoint		m_ptPrev;
	CWnd*		m_pParentWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGNIFIERDLG_H__F52C9248_7E9A_447E_AB12_BBCE3B96AC79__INCLUDED_)
