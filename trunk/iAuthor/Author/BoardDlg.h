#if !defined(AFX_BOARDDLG_H__B5F27942_9798_4462_9F60_2484EE9DF209__INCLUDED_)
#define AFX_BOARDDLG_H__B5F27942_9798_4462_9F60_2484EE9DF209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoardDlg dialog

class CBoardDlg : public CDialog
{
// Construction
public:
	void SetCursor(HCURSOR hCursor);
	void SetBoardImage(CString strPath);
	void SetBoardImageByDesktop();
	void SetBoardStyle(int nBoardStyle);
	CBoardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardDlg)
	enum { IDD = IDD_BOARDDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Bitmap*		m_pBGBitmap;
	CString		m_strImagePath;
	HCURSOR		m_hCursor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDDLG_H__B5F27942_9798_4462_9F60_2484EE9DF209__INCLUDED_)
