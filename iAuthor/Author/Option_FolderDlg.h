#if !defined(AFX_OPTION_FOLDERDLG_H__E83E9923_DB52_4741_AA2E_B3BBF581D26C__INCLUDED_)
#define AFX_OPTION_FOLDERDLG_H__E83E9923_DB52_4741_AA2E_B3BBF581D26C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_FolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_FolderDlg dialog

class COption_FolderDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	COption_FolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_FolderDlg)
	enum { IDD = IDD_OPT_FOLDERDLG };
	CEdit	m_ContetnsEditor;
	CButton	m_SlideBrowBtn;
	CEdit	m_SlideEditor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_FolderDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_FolderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptContentsbrowbtn();
	afx_msg void OnOptSlidebrowbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;	
	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_FOLDERDLG_H__E83E9923_DB52_4741_AA2E_B3BBF581D26C__INCLUDED_)
