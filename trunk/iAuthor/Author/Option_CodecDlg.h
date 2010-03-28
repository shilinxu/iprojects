#if !defined(AFX_Option_CodecDlg_H__093BE0B6_99A4_4828_B462_50FFC46BDA6A__INCLUDED_)
#define AFX_Option_CodecDlg_H__093BE0B6_99A4_4828_B462_50FFC46BDA6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_CodecDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_CodecDlg dialog

class COption_CodecDlg : public CDialog
{
// Construction
public:
	bool WriteOption();
	void ReadOption();
	COption_CodecDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_CodecDlg)
	enum { IDD = IDD_OPT_CODECDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_CodecDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_CodecDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptVideocodecCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;	
	bool		m_bModifyFlag;
	bool		m_bSelect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Option_CodecDlg_H__093BE0B6_99A4_4828_B462_50FFC46BDA6A__INCLUDED_)
