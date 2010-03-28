#if !defined(AFX_TEXTDLG_H__5C9B06F8_807A_4BF5_9CA8_F0A0F45C281B__INCLUDED_)
#define AFX_TEXTDLG_H__5C9B06F8_807A_4BF5_9CA8_F0A0F45C281B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog

class CTextDlg : public CDialog
{
// Construction
public:
	CTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextDlg)
	enum { IDD = IDD_TEXTDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetParent(CWnd* pParent);
	void SetProperty(COLORREF bgColor, COLORREF TextColor, int nFont);


private:
	CRichEditCtrl	m_Edit;

	CWnd*			m_pParent;
	COLORREF		m_bgColor;
	COLORREF		m_TextColor;
	int				m_nFontSize;
	CHARFORMAT		m_EditFont;

	CRect			m_rcClient;

	bool			m_bInputComplete;
	int				m_nEdgeState;
	HCURSOR			m_hCursor;

	CRect			m_rcInputComplete;
	CRect			m_rcClose;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDLG_H__5C9B06F8_807A_4BF5_9CA8_F0A0F45C281B__INCLUDED_)
