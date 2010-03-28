#if !defined(AFX_OPTION_RECDLG_H__A7DF4EB0_096B_4405_857F_0E14C10E0E7A__INCLUDED_)
#define AFX_OPTION_RECDLG_H__A7DF4EB0_096B_4405_857F_0E14C10E0E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_RECDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_RECDlg dialog

class COption_RECDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_RECDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_RECDlg)
	enum { IDD = IDD_OPT_RECDLG };
	CButton	m_ToolRecChkBtn;
	CComboBox	m_QualityCombo;
	CComboBox	m_FpsCombo;
	CComboBox	m_CursorCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_RECDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_RECDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOptCursorcombo();
	afx_msg void OnSelchangeOptFpscombo();
	afx_msg void OnSelchangeOptQualitycombo();
	afx_msg void OnOptToolreccheckbtn();
	afx_msg void OnExclusiveRecmode();
	afx_msg void OnNormalRecmode();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetModeChange(bool bAcceleration);
	int GetAccelerationLevel();
	CBrush		m_brush;
	bool		m_bReg;
	bool		m_bModifyFlag;
	CString		m_strInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_RECDLG_H__A7DF4EB0_096B_4405_857F_0E14C10E0E7A__INCLUDED_)
