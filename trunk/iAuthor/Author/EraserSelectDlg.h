#if !defined(AFX_ERASERSELECTDLG_H__C6371D77_8E71_44FC_A77D_AB8C767304D9__INCLUDED_)
#define AFX_ERASERSELECTDLG_H__C6371D77_8E71_44FC_A77D_AB8C767304D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EraserSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEraserSelectDlg dialog

class CEraserSelectDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CEraserSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEraserSelectDlg)
	enum { IDD = IDD_ERASERSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEraserSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEraserSelectDlg)
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

	int		m_nSelectEraser;

	CRect	m_rcEraser[4];
	CRect	m_rcSelEraser;
	CRect	m_rcTempEraser;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERASERSELECTDLG_H__C6371D77_8E71_44FC_A77D_AB8C767304D9__INCLUDED_)
