#if !defined(AFX_PENSELECTDLG_H__0F394052_A1C9_4F5F_86D9_6B3A50E335A8__INCLUDED_)
#define AFX_PENSELECTDLG_H__0F394052_A1C9_4F5F_86D9_6B3A50E335A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PenSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPenSelectDlg dialog

class CPenSelectDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CPenSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPenSelectDlg)
	enum { IDD = IDD_PENSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPenSelectDlg)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

private:
	BOOL	m_bTracking;

	int		m_nSelectPen;

	CRect	m_rcPen[3];
	CRect	m_rcSelPen;		// º±≈√µ» ∆Ê
	CRect	m_rcCurPen;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PENSELECTDLG_H__0F394052_A1C9_4F5F_86D9_6B3A50E335A8__INCLUDED_)
