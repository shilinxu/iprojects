#if !defined(AFX_OPTION_LOGODLG_H__FBD03647_A7A7_4C0B_9045_E8E387DC5331__INCLUDED_)
#define AFX_OPTION_LOGODLG_H__FBD03647_A7A7_4C0B_9045_E8E387DC5331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_LogoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_LogoDlg dialog

class COption_LogoDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_LogoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_LogoDlg)
	enum { IDD = IDD_OPT_LOGODLG };
	CEdit	m_LogoEditor;
	CButton	m_LogoBrowBtn;
	CButton	m_LogoChkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_LogoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_LogoDlg)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptLogocheckbtn();
	afx_msg void OnOptLogobrowbtn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
	bool		m_bReg;
	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_LOGODLG_H__FBD03647_A7A7_4C0B_9045_E8E387DC5331__INCLUDED_)
