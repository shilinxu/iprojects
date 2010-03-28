#if !defined(AFX_OPTION_HOTKEYDLG_H__E08947C4_FD38_4E1A_B294_6C63641C1530__INCLUDED_)
#define AFX_OPTION_HOTKEYDLG_H__E08947C4_FD38_4E1A_B294_6C63641C1530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_HotkeyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_HotkeyDlg dialog

class COption_HotkeyDlg : public CDialog
{
// Construction
public:
	COption_HotkeyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_HotkeyDlg)
	enum { IDD = IDD_OPT_HOTKEYDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_HotkeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_HotkeyDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_HOTKEYDLG_H__E08947C4_FD38_4E1A_B294_6C63641C1530__INCLUDED_)
