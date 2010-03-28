#if !defined(AFX_SLIDEMENUDLG_H__691F6706_7C44_4492_8585_999006183A04__INCLUDED_)
#define AFX_SLIDEMENUDLG_H__691F6706_7C44_4492_8585_999006183A04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlideMenuDlg.h : header file
//

#include "SlideListWnd.h"
#include "BtnST.h"
/////////////////////////////////////////////////////////////////////////////
// CSlideMenuDlg dialog

class CSlideMenuDlg : public CDialog
{
// Construction
public:
	void ReplaceSildeThumbnail(CString strPath);
	CString GetSelectSlide();
	void AddSlide(CString strSlide);
	void SetEnableTransparent(bool bTrans = true);
	CSlideMenuDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSlideMenuDlg)
	enum { IDD = IDD_SLIDEMENUDLG };
	CButtonST	m_SelBtn;
	CButtonST	m_OpenBtn;
	CButtonST	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlideMenuDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSlideListWnd	m_SlideListWnd;

	// Generated message map functions
	//{{AFX_MSG(CSlideMenuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSlideSelectbtn();
	afx_msg void OnSlideCancelbtn();
	afx_msg void OnSlideOpenbtn();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	CString		m_strSelectSlide;	

	void		ArrangeControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDEMENUDLG_H__691F6706_7C44_4492_8585_999006183A04__INCLUDED_)
