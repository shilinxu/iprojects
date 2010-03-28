#if !defined(AFX_OPTIONDLG_H__E198B83C_E9F1_4562_9F13_976EC9F869F8__INCLUDED_)
#define AFX_OPTIONDLG_H__E198B83C_E9F1_4562_9F13_976EC9F869F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//

#include "Option_AboutDlg.h"
#include "Option_HotkeyDlg.h"
#include "Option_PenFolderDlg.h"
#include "Option_RECDlg.h"
#include "Option_ToolbarDlg.h"
#include "Option_LogoDlg.h"
#include "Option_FolderDlg.h"
#include "Option_TextDlg.h"
#include "Option_SoundDlg.h"
#include "Option_SnapshotDlg.h"
#include "Option_CameraDlg.h"
#include "Option_CodecDlg.h"

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

class COptionDlg : public CDialog
{
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTIONDLG };
	CButtonST	m_UpBtn;
	CButtonST	m_DownBtn;
	CButtonST	m_CodecBtn;
	CButtonST	m_CameraBtn;
	CButtonST	m_TextBtn;
	CButtonST	m_SoundBtn;
	CButtonST	m_SnapshotBtn;
	CButtonST	m_LogoBtn;
	CButtonST	m_FolderBtn;
	CButtonST	m_OkBtn;
	CButtonST	m_CancelBtn;
	CButtonST	m_CloseBtn;
	CButtonST	m_ToolbarBtn;
	CButtonST	m_RecordBtn;
	CButtonST	m_PenBtn;
	CButtonST	m_HotkeyBtn;
	CButtonST	m_AboutBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnOptionToolbarbtn();
	afx_msg void OnOptionRecordbtn();
	afx_msg void OnOptionPenbtn();
	afx_msg void OnOptionHotkeybtn();
	afx_msg void OnOptionAboutbtn();
	afx_msg void OnOptionclosebtn();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnOptionFolderbtn();
	afx_msg void OnOptionLogobtn();
	afx_msg void OnOptionSnapshotbtn();
	afx_msg void OnOptionSoundbtn();
	afx_msg void OnOptionTextbtn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOptionCamera();
	afx_msg void OnOptionCodec();
	afx_msg void OnOptionUpbtn();
	afx_msg void OnOptionDownbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetClose();
	bool	m_bModifyToolbar;
	bool	m_bModifyRecord;
	bool	m_bModifyLogo;
	bool	m_bModifyPenFolder;
	bool	m_bModifySnapshot;
	bool	m_bModifyText;
	bool	m_bModifyCamera;
	bool	m_bModifyCodec;
	bool	m_bModifyRegistration;

private:
	COption_AboutDlg		m_AboutDlg;
	COption_HotkeyDlg		m_HotkeyDlg;
	COption_PenFolderDlg	m_PenFolderDlg;
	COption_RECDlg			m_RECDlg;
	COption_ToolbarDlg		m_ToolbarDlg;
	COption_LogoDlg			m_LogoDlg;
	COption_FolderDlg		m_FolderDlg;
	COption_TextDlg			m_TextDlg;
	COption_SoundDlg		m_SoundDlg;
	COption_SnapshotDlg		m_SnapshotDlg;
	COption_CameraDlg		m_CameraDlg;
	COption_CodecDlg		m_CodecDlg;

	int			m_State;

private:
	void State3();
	void State2();
	void State1();
	void State0();
	void ArrangeControl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__E198B83C_E9F1_4562_9F13_976EC9F869F8__INCLUDED_)
