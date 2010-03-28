#if !defined(AFX_RECSIZEDLG_H__49FC079E_4AAD_4024_BA07_4D51B88E3D92__INCLUDED_)
#define AFX_RECSIZEDLG_H__49FC079E_4AAD_4024_BA07_4D51B88E3D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecSizeDlg dialog

class CRecSizeDlg : public CDialog
{
// Construction
public:
	void SetStartPosition(int ptX, int ptY);
	void SetPosition(int ptX, int ptY);
	CRecSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecSizeDlg)
	enum { IDD = IDD_RECSIZEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecSizeDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CRect		m_rcRec;
	CString		m_strSize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECSIZEDLG_H__49FC079E_4AAD_4024_BA07_4D51B88E3D92__INCLUDED_)
