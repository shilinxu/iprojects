#if !defined(AFX_RECORDSELECTRANGE_H__BD9D2F8D_C038_46D7_BE8F_B63E33EBD39D__INCLUDED_)
#define AFX_RECORDSELECTRANGE_H__BD9D2F8D_C038_46D7_BE8F_B63E33EBD39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordSelectRange.h : header file
//
#include "BtnST.h"
/////////////////////////////////////////////////////////////////////////////
// CRecordSelectRange dialog

class CRecordSelectRange : public CDialog
{
// Construction
public:
	CRecordSelectRange(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordSelectRange)
	enum { IDD = IDD_RECORD_SELECTRANGE };
	CButtonST	m_OkBtn;
	CButtonST	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordSelectRange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordSelectRange)
	afx_msg void OnRecordRangeArea();
	afx_msg void OnRecordRangeFull();
	afx_msg void OnRecordRangeWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRecordRangeState(int nRecordRangeState);
	int			m_nRangeState;

private:
	CBrush		m_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSELECTRANGE_H__BD9D2F8D_C038_46D7_BE8F_B63E33EBD39D__INCLUDED_)
