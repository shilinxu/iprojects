#if !defined(AFX_OPTION_SOUNDDLG_H__56BD533B_9D96_4C97_A17A_17B3061C5EE8__INCLUDED_)
#define AFX_OPTION_SOUNDDLG_H__56BD533B_9D96_4C97_A17A_17B3061C5EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_SoundDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_SoundDlg dialog

class COption_SoundDlg : public CDialog
{
// Construction
public:
	COption_SoundDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_SoundDlg)
	enum { IDD = IDD_OPT_SOUNDDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_SoundDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_SoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOptMikerec();
	afx_msg void OnOptMixrec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool IsValidMixer();
	CBrush		m_brush;

	bool		m_bEnableMixer;
	bool		m_bFindStereoMix;
	bool		m_bFindMike;	

	int			m_nCheckIndex;		// 0: other Control	1: Stereo mix	2: Mike
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_SOUNDDLG_H__56BD533B_9D96_4C97_A17A_17B3061C5EE8__INCLUDED_)
