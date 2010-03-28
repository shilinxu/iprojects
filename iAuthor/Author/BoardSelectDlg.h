#if !defined(AFX_BOARDSELECTDLG_H__06C2A809_1CC5_4A20_ACED_C26EDF9111FB__INCLUDED_)
#define AFX_BOARDSELECTDLG_H__06C2A809_1CC5_4A20_ACED_C26EDF9111FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoardSelectDlg dialog

class CBoardSelectDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CBoardSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardSelectDlg)
	enum { IDD = IDD_BOARDSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardSelectDlg)
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
	int		m_nSelectBoard;

	CRect	m_rcBoard[4];
	CRect	m_rcSelBoard;
	CRect	m_rcTempBoard;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDSELECTDLG_H__06C2A809_1CC5_4A20_ACED_C26EDF9111FB__INCLUDED_)
