// ProdCodeDlg.h : header file
//

#if !defined(AFX_ProdCodeDLG_H__488A0E76_21C3_41FB_AE24_4C39FAC42B2F__INCLUDED_)
#define AFX_ProdCodeDLG_H__488A0E76_21C3_41FB_AE24_4C39FAC42B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"


/////////////////////////////////////////////////////////////////////////////
// CProdCodeDlg dialog

class CProdCodeDlg : public CDialog
{
// Construction
public:
	CProdCodeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProdCodeDlg)
	enum { IDD = IDD_PRODCODE_DIALOG };
	CEdit	m_wndProdEno;
	CEdit	m_wndProdSno;
	CEdit	m_wndProdPrefix;
	CEdit	m_wndListCode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CList<CString,CString&> m_CodeList;

	// Generated message map functions
	//{{AFX_MSG(CProdCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCopy();
	afx_msg void OnCheckCRC();
	afx_msg void OnCheckDuplicate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ProdCodeDLG_H__488A0E76_21C3_41FB_AE24_4C39FAC42B2F__INCLUDED_)
