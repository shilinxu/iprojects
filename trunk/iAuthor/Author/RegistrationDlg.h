#if !defined(AFX_REGISTRATIONDLG_H__1A104177_A9B1_4E71_B4DD_FE36DE1E0C89__INCLUDED_)
#define AFX_REGISTRATIONDLG_H__1A104177_A9B1_4E71_B4DD_FE36DE1E0C89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegistrationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegistrationDlg dialog

class CRegistrationDlg : public CDialog
{
// Construction
public:
	CRegistrationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegistrationDlg)
	enum { IDD = IDD_REGISTRATIONDLG };
	CButton	m_CancelBtn;
	CButton	m_RegBtn;
	CEdit	m_UserNameEditor;
	CEdit	m_RegCodeEditor;
	CEdit	m_ProductCodeEditor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegistrationDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegistrationDlg)
	afx_msg void OnRegistration();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_Brush;

	CString	m_strUserName;
	CString	m_strPCode;
	CString	m_strRCode;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTRATIONDLG_H__1A104177_A9B1_4E71_B4DD_FE36DE1E0C89__INCLUDED_)
