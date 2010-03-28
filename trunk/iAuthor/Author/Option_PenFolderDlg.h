#if !defined(AFX_OPTION_PENFOLDERDLG_H__2917D8D6_D725_4475_8BEE_B94D7B3DF1C0__INCLUDED_)
#define AFX_OPTION_PENFOLDERDLG_H__2917D8D6_D725_4475_8BEE_B94D7B3DF1C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_PenFolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_PenFolderDlg dialog

class COption_PenFolderDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_PenFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_PenFolderDlg)
	enum { IDD = IDD_OPT_PENFOLDERDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_PenFolderDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_PenFolderDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnOptPattern1btn();
	afx_msg void OnOptPattern2btn();
	afx_msg void OnOptPattern3btn();
	afx_msg void OnOptPattern4btn();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_PENFOLDERDLG_H__2917D8D6_D725_4475_8BEE_B94D7B3DF1C0__INCLUDED_)
