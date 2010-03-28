#if !defined(AFX_COLORSELCTDLG_H__CD11AD47_8DBF_445C_AED1_C7C8E82B6790__INCLUDED_)
#define AFX_COLORSELCTDLG_H__CD11AD47_8DBF_445C_AED1_C7C8E82B6790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSelctDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorSelctDlg dialog

class CColorSelctDlg : public CDialog
{
// Construction
public:
	void SetEnableTransparent(bool bTrans = true);
	CColorSelctDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorSelctDlg)
	enum { IDD = IDD_COLORSELECTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSelctDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorSelctDlg)
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

	COLORREF	m_SelectColor;

	CRect	m_rcSelColor;
	CRect	m_rcTempColor;
	COLORREF	m_EnumColor[16];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSELCTDLG_H__CD11AD47_8DBF_445C_AED1_C7C8E82B6790__INCLUDED_)
