#if !defined(AFX_OPTION_TEXTDLG_H__360B551F_E696_4983_B6EB_84A8BCB90223__INCLUDED_)
#define AFX_OPTION_TEXTDLG_H__360B551F_E696_4983_B6EB_84A8BCB90223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_TextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_TextDlg dialog

class COption_TextDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_TextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_TextDlg)
	enum { IDD = IDD_OPT_TEXTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_TextDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_TextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
	CRect		m_rcColor[16];
	COLORREF	m_EnumColor[16];
	int			m_nSelNum;

	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_TEXTDLG_H__360B551F_E696_4983_B6EB_84A8BCB90223__INCLUDED_)
