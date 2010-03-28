#if !defined(AFX_MAGNIFIERPREVDLG_H__2BB3B1F2_7986_41E8_82E0_AC456A9D175C__INCLUDED_)
#define AFX_MAGNIFIERPREVDLG_H__2BB3B1F2_7986_41E8_82E0_AC456A9D175C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MagnifierPrevDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagnifierPrevDlg dialog

class CMagnifierPrevDlg : public CDialog
{
// Construction
public:
	void SetEnble(bool bEnable);
	CMagnifierPrevDlg(CWnd* pParent = NULL);   // standard constructor

	void ZoomOut();
	void ZoomIn();
// Dialog Data
	//{{AFX_DATA(CMagnifierPrevDlg)
	enum { IDD = IDD_MAGNIFIERPREVIEWDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagnifierPrevDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagnifierPrevDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	Bitmap*		m_pMagnifierBitmap;
	int  m_nZoomRate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGNIFIERPREVDLG_H__2BB3B1F2_7986_41E8_82E0_AC456A9D175C__INCLUDED_)
