#if !defined(AFX_OPTION_SNAPSHOTDLG_H__DC55E495_97B6_4075_971C_289A5A98E5C8__INCLUDED_)
#define AFX_OPTION_SNAPSHOTDLG_H__DC55E495_97B6_4075_971C_289A5A98E5C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_SnapshotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_SnapshotDlg dialog

class COption_SnapshotDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_SnapshotDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_SnapshotDlg)
	enum { IDD = IDD_OPT_SNAPSHOTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_SnapshotDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_SnapshotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptSnapshotfull();
	afx_msg void OnOptSnapshotrange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_SNAPSHOTDLG_H__DC55E495_97B6_4075_971C_289A5A98E5C8__INCLUDED_)
