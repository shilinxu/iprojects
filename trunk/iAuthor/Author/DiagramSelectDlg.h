#if !defined(AFX_DIAGRAMSELECTDLG_H__F109E2D0_B227_470C_8623_3AE1100E0308__INCLUDED_)
#define AFX_DIAGRAMSELECTDLG_H__F109E2D0_B227_470C_8623_3AE1100E0308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagramSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagramSelectDlg dialog

class CDiagramSelectDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CDiagramSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiagramSelectDlg)
	enum { IDD = IDD_DIAGRAMSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiagramSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiagramSelectDlg)
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
	int		m_nSelectDiagram;

	CRect	m_rcDiagram[3];
	CRect	m_rcSelDiagram;
	CRect	m_rcTempDiagram;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGRAMSELECTDLG_H__F109E2D0_B227_470C_8623_3AE1100E0308__INCLUDED_)
