#if !defined(AFX_LINEWEIGHTSELECTDLG_H__4EC19F32_0CD2_4113_89C3_CDB3F5AF5EE2__INCLUDED_)
#define AFX_LINEWEIGHTSELECTDLG_H__4EC19F32_0CD2_4113_89C3_CDB3F5AF5EE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineWeightSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineWeightSelectDlg dialog

class CLineWeightSelectDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CLineWeightSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineWeightSelectDlg)
	enum { IDD = IDD_LINEWEIGHTSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineWeightSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineWeightSelectDlg)
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

	int		m_SelectWidth;	
	CRect	m_rcSelWidth;
	CRect	m_rcTempWidth;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEWEIGHTSELECTDLG_H__4EC19F32_0CD2_4113_89C3_CDB3F5AF5EE2__INCLUDED_)
