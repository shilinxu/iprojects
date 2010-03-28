// VidCapTestDlg.h : header file
//

#if !defined(AFX_VIDCAPTESTDLG_H__6EFFC94C_9452_4AC2_ACDB_CFAA36189BD5__INCLUDED_)
#define AFX_VIDCAPTESTDLG_H__6EFFC94C_9452_4AC2_ACDB_CFAA36189BD5__INCLUDED_

//#include <mmsystem.h>
#include <vfw.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVidCapTestDlg dialog

class CVidCapTestDlg : public CDialog
{
// Construction
public:
	CVidCapTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVidCapTestDlg)
	enum { IDD = IDD_VIDCAPTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVidCapTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVidCapTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioHideCursor();
	afx_msg void OnRadioShowCursor();
	afx_msg void OnAbout();
	afx_msg void OnRecStart();
	afx_msg void OnRecStop();
	afx_msg void OnRecPause();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitVideoCompressorList();
	void RefreshCompressorButtons();
	
	
	bool m_bRecordCursor;

	int m_ScreenBits;
	DWORD m_CompFCCHandler;
	ICINFO * m_pCompressorInfo;
	int m_nCompressorNumber;
	int m_nSelectedCompressor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDCAPTESTDLG_H__6EFFC94C_9452_4AC2_ACDB_CFAA36189BD5__INCLUDED_)
