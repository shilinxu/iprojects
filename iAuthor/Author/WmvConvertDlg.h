#if !defined(AFX_WMVCONVERTDLG_H__1E9E32DB_5F64_46C0_A38A_24BD9028B969__INCLUDED_)
#define AFX_WMVCONVERTDLG_H__1E9E32DB_5F64_46C0_A38A_24BD9028B969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WmvConvertDlg.h : header file
//

#include "BtnST.h"
#include "../AmpSdk/include/wmvgen.h"
#include "ProfilesFileWrite.h"

/////////////////////////////////////////////////////////////////////////////
// CWmvConvertDlg dialog

class CWmvConvertDlg : public CDialog
{
// Construction
public:
	CWmvConvertDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWmvConvertDlg)
	enum { IDD = IDD_WMVCONVERTDLG };
	CButtonST	m_WMVPathBrowseBtn;
	CProgressCtrl	m_RenderingProgressbar;
	CButtonST	m_CloseBtn;
	CButtonST	m_CancelBtn;
	CButtonST	m_OKBtn;
	CButtonST	m_NextBtn;
	CButtonST	m_PreviewMovieBtn;	
	CButtonST	m_AVIPathBrowseBtn;
	CEdit	m_WMVPathEditor;
	CEdit	m_AVIPathEditor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWmvConvertDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWmvConvertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWmvconvNextbtn();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWmvclosebtn();
	afx_msg void OnAvipathbrowser();
	afx_msg void OnWmvpathbrowser();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetRenderingPosition(int nPos);
	int		m_nMaxRange;

private:
	void ArrangeControl();	
	bool GetAVILength(CString strPath);
	bool IsFile(CString strPath);

	bool SetReadyRendering();
	void CloseRendering();	

private:
	WGENHANDLE	m_hWmvGen;
	CProfilesFileWrite	m_ProfileWriter;

	int m_nCurrentStep;
	CString m_strAviPath;
	CString m_strWmvPath;
	CString m_strProfilePath;

	CRect	m_rcProgress;
	int		m_nCurPos;

	DWORD	m_AviRate;
	DWORD	m_AviScale;
	DWORD	m_AviLength;
	DWORD	m_AviFrame;
	double	m_AviDuration;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMVCONVERTDLG_H__1E9E32DB_5F64_46C0_A38A_24BD9028B969__INCLUDED_)
