#if !defined(AFX_OPTION_CAMERADLG_H__207407B2_7DD3_4A60_98EE_2EFBC1409042__INCLUDED_)
#define AFX_OPTION_CAMERADLG_H__207407B2_7DD3_4A60_98EE_2EFBC1409042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_CameraDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_CameraDlg dialog

class COption_CameraDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_CameraDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_CameraDlg)
	enum { IDD = IDD_OPT_CAMERADLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_CameraDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_CameraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptCameraSize0();
	afx_msg void OnOptCameraSize1();
	afx_msg void OnOptCameraSize2();
	afx_msg void OnOptCameraSize3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;	
	bool		m_bModifyFlag;
	int			m_nCurSize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_CAMERADLG_H__207407B2_7DD3_4A60_98EE_2EFBC1409042__INCLUDED_)
