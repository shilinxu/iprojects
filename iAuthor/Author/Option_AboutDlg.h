#if !defined(AFX_OPTION_ABOUTDLG_H__C500A988_6D43_4A67_B124_9C6780783EBF__INCLUDED_)
#define AFX_OPTION_ABOUTDLG_H__C500A988_6D43_4A67_B124_9C6780783EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_AboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_AboutDlg dialog

class COption_AboutDlg : public CDialog
{
// Construction
public:
	COption_AboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_AboutDlg)
	enum { IDD = IDD_OPT_ABOUTDLG };
	CButton	m_OptionRegBtn;
	CEdit	m_WarningEditor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_AboutDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_AboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnOptionRegistration();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool	m_bModifyRegistration;

private:
	CBrush		m_brush;
	bool		m_bReg;	
	CString		m_strInfo;
	CRect		m_rcInfo;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_ABOUTDLG_H__C500A988_6D43_4A67_B124_9C6780783EBF__INCLUDED_)
