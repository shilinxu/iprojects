#if !defined(AFX_OPTION_TOOLBARDLG_H__FA8AA3FF_6B09_46F1_8F0D_4D2A9506D71C__INCLUDED_)
#define AFX_OPTION_TOOLBARDLG_H__FA8AA3FF_6B09_46F1_8F0D_4D2A9506D71C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option_ToolbarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COption_ToolbarDlg dialog

class COption_ToolbarDlg : public CDialog
{
// Construction
public:
	void ReadOption();
	bool WriteOption();
	COption_ToolbarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COption_ToolbarDlg)
	enum { IDD = IDD_OPT_TOOLBARDLG };
	CButton	m_ZoomCheckBtn;
	CButton	m_ModeCheckBtn;
	CButton	m_WidthCheckBtn;
	CButton	m_SnapshotCheckBtn;
	CButton	m_PPTCheckBtn;
	CButton	m_PenCheckBtn;
	CButton	m_HLPenCheckBtn;
	CButton	m_EraserCheckBtn;
	CButton	m_DiagramCheckBtn;
	CButton	m_ControlCheckBtn;
	CButton	m_ColorCheckBtn;
	CButton	m_BoardCheckBtn;
	CButton	m_PrintCheckBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COption_ToolbarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COption_ToolbarDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnOptModecheckbtn();
	afx_msg void OnOptPencheckbtn();
	afx_msg void OnOptHlpencheckbtn();
	afx_msg void OnOptErasercheckbtn();
	afx_msg void OnOptColorcheckbtn();
	afx_msg void OnOptWidthcheckbtn();
	afx_msg void OnOptDiagramcheckbtn();
	afx_msg void OnOptControlcheckbtn();
	afx_msg void OnOptPptcheckbtn();
	afx_msg void OnOptBoardcheckbtn();
	afx_msg void OnOptSnapshotcheckbtn();
	afx_msg void OnOptPrintcheckbtn();
	afx_msg void OnOptZoomcheckbtn();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush		m_brush;
	bool		m_bModifyFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_TOOLBARDLG_H__FA8AA3FF_6B09_46F1_8F0D_4D2A9506D71C__INCLUDED_)
